/*
 * vibecheck_wavegen.c
 *
 *  Created on: Sep 27, 2024
 *      Author: johnt
 */

#include "vibecheck_wavegen.h"




static void ComputeSineWave(uint32_t* buf, uint32_t len, float amplitude)
{
	for (uint32_t i = 0; i < len; i++)
		*buf++ = 0.5f * VC_WAVE_DAC_FULL_SCALE * (1.0f + amplitude * sin((float)i * 2.0f * 3.14159f / (float)len));
}

static void ComputeSquareWave(uint32_t* buf, uint32_t len, float amplitude)
{
	for (uint32_t i = 0; i < len; i++)
		if (i < len / 2)
			*buf++ = 0.5f * VC_WAVE_DAC_FULL_SCALE * (1 + amplitude);
		else
			*buf++ = 0.5f * VC_WAVE_DAC_FULL_SCALE * (1 - amplitude);
}

static void ComputeSawWave(uint32_t* buf, uint32_t len, float amplitude)
{
	for (uint32_t i = 0; i < len; i++)
		if (i < len / 2)
			*buf++ = 0.5f * VC_WAVE_DAC_FULL_SCALE * (1 + 2 * (float)i * amplitude / (float)len);
		else
			*buf++ = 0.5f * VC_WAVE_DAC_FULL_SCALE * (1 - 2 * amplitude + 2 * (float)i * amplitude / (float)len);
}

static void ComputeTriangleWave(uint32_t* buf, uint32_t len, float amplitude)
{
	for (uint32_t i = 0; i < len; i++)
		if (i < len / 4)
			*buf++ = 0.5f * VC_WAVE_DAC_FULL_SCALE * (1 + 4 * i * amplitude / (float)len);
		else if (i < 3 * len / 4)
			*buf++ = 0.5f * VC_WAVE_DAC_FULL_SCALE * (1 + 2 * amplitude - 4 * i * amplitude / (float)len);
		else
			*buf++ = 0.5f * VC_WAVE_DAC_FULL_SCALE * (1 - 4 * amplitude + 4 * i * amplitude / (float)len);
}




static void VibeCheckWaveGen_RecomputeWave(VibeCheckWaveGen* wavegen)
{
	switch (wavegen->waveform)
	{
	case VC_WAVE_SINE:
		ComputeSineWave(&wavegen->wave[wavegen->wave_buf_index * VC_WAVE_BUF_LEN], VC_WAVE_BUF_LEN, wavegen->amplitude);
		break;
	case VC_WAVE_SQUARE:
		ComputeSquareWave(&wavegen->wave[wavegen->wave_buf_index * VC_WAVE_BUF_LEN], VC_WAVE_BUF_LEN, wavegen->amplitude);
		break;
	case VC_WAVE_SAW:
		ComputeSawWave(&wavegen->wave[wavegen->wave_buf_index * VC_WAVE_BUF_LEN], VC_WAVE_BUF_LEN, wavegen->amplitude);
		break;
	case VC_WAVE_TRIANGLE:
		ComputeTriangleWave(&wavegen->wave[wavegen->wave_buf_index * VC_WAVE_BUF_LEN], VC_WAVE_BUF_LEN, wavegen->amplitude);
		break;
	}
}


void VibeCheckWaveGen_Init(VibeCheckWaveGen* wavegen, DAC_HandleTypeDef *hdac, TIM_HandleTypeDef* htim)
{
	wavegen->hdac = hdac;
	wavegen->htim = htim;
	wavegen->freq_hz = VC_WAVE_MIN_FREQ_HZ;
	wavegen->amplitude = 0.1f;
	wavegen->waveform = VC_WAVE_SINE;
	wavegen->wave_compute_pending = 0;
	wavegen->wave_compute_ready = 0;

	wavegen->is_muted = 0;
	wavegen->time_prev_button_press = 0;

	HAL_GPIO_WritePin(MUTE_SIGNAL_GPIO_Port, MUTE_SIGNAL_Pin, GPIO_PIN_RESET);  /* un-mute the output */
	HAL_GPIO_WritePin(MUTE_INDICATOR_GPIO_Port, MUTE_INDICATOR_Pin, GPIO_PIN_RESET);  /* turn off the mute LED */

	/* set up timer registers */
	wavegen->htim->Instance->PSC = VC_WAVE_TIM_PSC - 1;
	VibeCheckWaveGen_SetFrequency(wavegen, wavegen->freq_hz);

	/* compute the wave */
	wavegen->wave_buf_index = 0;
	VibeCheckWaveGen_RecomputeWave(wavegen);
	wavegen->wave_buf_index = 1;
	VibeCheckWaveGen_RecomputeWave(wavegen);
}

void VibeCheckWaveGen_Update(VibeCheckWaveGen* wavegen)
{
	/* call in the main loop */

	/* check the mute button */
	uint32_t time = HAL_GetTick();
	if (time - wavegen->time_prev_button_press > VC_WAVE_BUTTON_DEBOUNCE_MS && HAL_GPIO_ReadPin(MUTE_BUTTON_GPIO_Port, MUTE_BUTTON_Pin))
	{
		/* TODO: need to alert the shell when we press the mute button */

		if (wavegen->is_muted)
		{
			wavegen->is_muted = 0;
			HAL_GPIO_WritePin(MUTE_SIGNAL_GPIO_Port, MUTE_SIGNAL_Pin, GPIO_PIN_RESET);  /* un-mute the output */
			HAL_GPIO_WritePin(MUTE_INDICATOR_GPIO_Port, MUTE_INDICATOR_Pin, GPIO_PIN_RESET);  /* turn off the LED */
		}
		else
		{
			wavegen->is_muted = 1;
			HAL_GPIO_WritePin(MUTE_SIGNAL_GPIO_Port, MUTE_SIGNAL_Pin, GPIO_PIN_SET);  /* mute the output */
			HAL_GPIO_WritePin(MUTE_INDICATOR_GPIO_Port, MUTE_INDICATOR_Pin, GPIO_PIN_SET);  /* turn on the LED */
		}

		wavegen->time_prev_button_press = time;
	}

	/*
	 * to make a smooth transition between waves when a parameter is changed, first compute the safe half of the double buffer
	 * immediately. Set a pending flag that the other half also needs to be updated when its safe. The pending flag will be turned
	 * into a ready flag when the DMA callback fires. When we see the ready flag in the update function, compute the other half of the
	 * double buffer with the new wave.
	 */
	if (wavegen->wave_compute_ready)
	{
		VibeCheckWaveGen_RecomputeWave(wavegen);
		wavegen->wave_compute_ready = 0;
	}


}

void VibeCheckWaveGen_Start(VibeCheckWaveGen* wavegen)
{
	wavegen->wave_buf_index = 0;
	VibeCheckWaveGen_RecomputeWave(wavegen);
	wavegen->wave_buf_index = 1;
	VibeCheckWaveGen_RecomputeWave(wavegen);

	wavegen->wave_compute_pending = 0;
	wavegen->wave_compute_ready = 0;

	HAL_DAC_Start_DMA(wavegen->hdac, DAC_CHANNEL_1, wavegen->wave, VC_WAVE_BUF_LEN, DAC_ALIGN_12B_R);
	HAL_DAC_Start_DMA(wavegen->hdac, DAC_CHANNEL_2, wavegen->wave, VC_WAVE_BUF_LEN, DAC_ALIGN_12B_R);
	HAL_TIM_Base_Start(wavegen->htim);

}

void VibeCheckWaveGen_Stop(VibeCheckWaveGen* wavegen)
{
	HAL_TIM_Base_Stop(wavegen->htim);
	HAL_DAC_Stop_DMA(wavegen->hdac, DAC_CHANNEL_1);
	HAL_DAC_Stop_DMA(wavegen->hdac, DAC_CHANNEL_2);
}

void VibeCheckWaveGen_SetFrequency(VibeCheckWaveGen* wavegen, float freq_hz)
{
	if (freq_hz < VC_WAVE_MIN_FREQ_HZ)
		freq_hz = VC_WAVE_MIN_FREQ_HZ;
	if (freq_hz > VC_WAVE_MAX_FREQ_HZ)
		freq_hz = VC_WAVE_MAX_FREQ_HZ;

	uint32_t arr = VC_WAVE_TIM_COUNTS_PER_SECOND / (VC_WAVE_BUF_LEN * freq_hz) - 1;
	wavegen->freq_hz = VC_WAVE_TIM_COUNTS_PER_SECOND / ((float)arr + 1.0f) / VC_WAVE_BUF_LEN;
	wavegen->htim->Instance->ARR = arr;
}

float VibeCheckWaveGen_GetFrequency(VibeCheckWaveGen* wavegen)
{
	return wavegen->freq_hz;
}

void VibeCheckWaveGen_SetAmplitude(VibeCheckWaveGen* wavegen, float amplitude)
{
	wavegen->amplitude = amplitude;
	wavegen->wave_compute_pending = 1;
	VibeCheckWaveGen_RecomputeWave(wavegen);
}

float VibeCheckWaveGen_GetAmplitude(VibeCheckWaveGen* wavegen)
{
	return wavegen->amplitude;
}

void VibeCheckWaveGen_SetWaveform(VibeCheckWaveGen* wavegen, VibeCheckWaveGen_Waveform waveform)
{
	wavegen->waveform = waveform;
	wavegen->wave_compute_pending = 1;
	VibeCheckWaveGen_RecomputeWave(wavegen);
}

VibeCheckWaveGen_Waveform VibeCheckWaveGen_GetWaveform(VibeCheckWaveGen* wavegen)
{
	return wavegen->waveform;
}

/* keeps track of which end of the double buffer to compute when updating the wave */
void VibeCheckWaveGen_DMACallback(VibeCheckWaveGen* wavegen)
{
	wavegen->wave_buf_index = !wavegen->wave_buf_index;

	if (wavegen->wave_compute_pending)
	{
		wavegen->wave_compute_pending = 0;
		wavegen->wave_compute_ready = 1;
	}
}
