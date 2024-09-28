/*
 * vibecheck_wavegen.c
 *
 *  Created on: Sep 27, 2024
 *      Author: johnt
 */

#include "vibecheck_wavegen.h"


static const uint32_t demo_times0[] = {400, 200, 200, 200,    200,    200,    400,    200, 200, 200, 200,    200,   400,      400, 200, 200,  200,   200,    200,    200,    200,   200,    200,    200,       400,  400};
static const float demo_freqs0[] =    {110, 55,  110, 164.81, 155.56, 146.83, 130.81, 110, 55,  110, 146.83, 73.42, 146.83,    98, 49,  98,  146.83, 138.59, 130.81, 123.47, 61.47, 123.47, 61.74, 123.47,  130.81, 146.83};

static const uint32_t demo_times1[] = {150,    150, 150,    150, 300,    150,    150,    300,    300,    300,    300,           300,   150,    150, 300,    150,       150, 300,    300,    150,   150,    300,        150,   150,    300,    300,    300,    300,        300,    150,   300,    150,   150,    150,    300,    150,    300,    150,    75,     75,   75,   75,  150,    150,    600};
static const float demo_freqs1[] =    {58.27,  185, 233.08, 185, 277.18, 233.08, 277.18, 369.99, 349.23, 277.18, 233.08,        0,     233.08, 185, 277.18, 233.08, 277.18, 369.99, 349.23, 51.91, 277.18, 233.08,     77.78, 311.13, 277.18, 233.08, 207.65, 0,          369.99, 415.3, 369.99, 415.3, 369.99,   0,    369.99, 466.16, 369.99, 466.16, 369.99, 0,  369.99, 0,   369.99,   0,   46.25};

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

static void ComputeNullWave(uint32_t* buf, uint32_t len)
{
	for (uint32_t i = 0; i < len; i++)
		*buf++ = 0.5f * VC_WAVE_DAC_FULL_SCALE;
}




static void VibeCheckWaveGen_RecomputeWave(VibeCheckWaveGen* wavegen, uint32_t* buf)
{
	if (wavegen->is_running)
	{
		switch (wavegen->waveform)
		{
		case VC_WAVE_SINE:
			ComputeSineWave(buf, VC_WAVE_BUF_LEN, wavegen->amplitude);
			break;
		case VC_WAVE_SQUARE:
			ComputeSquareWave(buf, VC_WAVE_BUF_LEN, wavegen->amplitude);
			break;
		case VC_WAVE_SAW:
			ComputeSawWave(buf, VC_WAVE_BUF_LEN, wavegen->amplitude);
			break;
		case VC_WAVE_TRIANGLE:
			ComputeTriangleWave(buf, VC_WAVE_BUF_LEN, wavegen->amplitude);
			break;
		}
	}
	else
	{
		ComputeNullWave(buf, VC_WAVE_BUF_LEN);
	}
}


void VibeCheckWaveGen_Init(VibeCheckWaveGen* wavegen, DAC_HandleTypeDef *hdac, TIM_HandleTypeDef* htim)
{
	wavegen->hdac = hdac;
	wavegen->htim = htim;
	wavegen->freq_hz = 440.0f;
	wavegen->amplitude = 0.001f;
	wavegen->waveform = VC_WAVE_SINE;

	wavegen->is_running = 0;
	wavegen->is_muted = 0;
	wavegen->mute_button_flag = 0;
	wavegen->time_prev_button_press = 0;

	wavegen->wave_ping_compute_pending = 0;
	wavegen->wave_pong_compute_pending = 0;
	wavegen->wave_ping_compute_ready = 0;
	wavegen->wave_pong_compute_ready = 0;

	wavegen->demo_num = 0;

	/* set up timer registers */
	wavegen->htim->Instance->PSC = VC_WAVE_TIM_PSC - 1;
	VibeCheckWaveGen_SetFrequency(wavegen, wavegen->freq_hz);

	/* compute the initial wave (set the DAC to the midpoint to avoid noise) */
	VibeCheckWaveGen_RecomputeWave(wavegen, &wavegen->wave[0]);
	VibeCheckWaveGen_RecomputeWave(wavegen, &wavegen->wave[VC_WAVE_BUF_LEN]);

	/* start the DAC */
	HAL_DAC_Start_DMA(wavegen->hdac, DAC_CHANNEL_1, wavegen->wave, VC_WAVE_BUF_LEN, DAC_ALIGN_12B_R);
	HAL_DAC_Start_DMA(wavegen->hdac, DAC_CHANNEL_2, wavegen->wave, VC_WAVE_BUF_LEN, DAC_ALIGN_12B_R);
	HAL_TIM_Base_Start(wavegen->htim);

	/* un-mute the output and turn off the mute LED*/
	HAL_GPIO_WritePin(MUTE_SIGNAL_GPIO_Port, MUTE_SIGNAL_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUTE_INDICATOR_GPIO_Port, MUTE_INDICATOR_Pin, GPIO_PIN_RESET);
}

void VibeCheckWaveGen_Update(VibeCheckWaveGen* wavegen)
{
	/* call in the main loop */

	/* check the mute button */
	uint32_t time = HAL_GetTick();
	if (time - wavegen->time_prev_button_press > VC_WAVE_BUTTON_DEBOUNCE_MS && HAL_GPIO_ReadPin(MUTE_BUTTON_GPIO_Port, MUTE_BUTTON_Pin))
	{

		/*
		 * the line driver seems to have some kind of slow start built in so no
		 * need to worry about pops when muting via the hardware pin
		 */
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

		wavegen->mute_button_flag = 1;  /* can alert the shell via this flag when we press the mute button */
		wavegen->time_prev_button_press = time;
	}

	/*
	 * to make a smooth transition between waves when a parameter is changed, change the two halves of the double buffer separately
	 */
	if (wavegen->wave_ping_compute_ready)
	{
		VibeCheckWaveGen_RecomputeWave(wavegen, &wavegen->wave[0]);
		wavegen->wave_ping_compute_ready = 0;
	}
	if (wavegen->wave_pong_compute_ready)
	{
		VibeCheckWaveGen_RecomputeWave(wavegen, &wavegen->wave[VC_WAVE_BUF_LEN]);
		wavegen->wave_pong_compute_ready = 0;
	}

	/*
	 * update the sequencer
	 */
	uint32_t seq_index;
	if (Sequencer_Update(&wavegen->sequencer, time, &seq_index))
	{
		VibeCheckWaveGen_SetFrequency(wavegen, wavegen->demo_num ? demo_freqs1[seq_index] : demo_freqs0[seq_index]);
	}

}

void VibeCheckWaveGen_Start(VibeCheckWaveGen* wavegen)
{
	if (!wavegen->is_running)
	{
		wavegen->is_running = 1;
		wavegen->wave_ping_compute_pending = 1;
		wavegen->wave_pong_compute_pending = 1;
	}

}

void VibeCheckWaveGen_Stop(VibeCheckWaveGen* wavegen)
{
	if (wavegen->is_running)
	{
		wavegen->is_running = 0;
		wavegen->wave_ping_compute_pending = 1;
		wavegen->wave_pong_compute_pending = 1;
	}
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
	if (amplitude < 0.0f)
		amplitude = 0.0f;
	if (amplitude > 1.0f)
		amplitude = 1.0f;

	wavegen->amplitude = amplitude;
	wavegen->wave_ping_compute_pending = 1;
	wavegen->wave_pong_compute_pending = 1;
}

float VibeCheckWaveGen_GetAmplitude(VibeCheckWaveGen* wavegen)
{
	return wavegen->amplitude;
}

void VibeCheckWaveGen_SetWaveform(VibeCheckWaveGen* wavegen, VibeCheckWaveGen_Waveform waveform)
{
	wavegen->waveform = waveform;
	wavegen->wave_ping_compute_pending = 1;
	wavegen->wave_pong_compute_pending = 1;
}

VibeCheckWaveGen_Waveform VibeCheckWaveGen_GetWaveform(VibeCheckWaveGen* wavegen)
{
	return wavegen->waveform;
}

uint32_t VibeCheckWaveGen_WasMuteButtonPressed(VibeCheckWaveGen* wavegen, uint32_t* is_muted)
{
	if (wavegen->mute_button_flag)
	{
		wavegen->mute_button_flag = 0;
		*is_muted = wavegen->is_muted;
		return 1;
	}

	return 0;
}


/* keeps track of which end of the double buffer to compute when updating the wave */
void VibeCheckWaveGen_DMAHalfCpltCallback(VibeCheckWaveGen* wavegen)
{
	if (wavegen->wave_ping_compute_pending)
	{
		wavegen->wave_ping_compute_pending = 0;
		wavegen->wave_ping_compute_ready = 1;
	}
}

void VibeCheckWaveGen_DMACpltCallback(VibeCheckWaveGen* wavegen)
{
	if (wavegen->wave_pong_compute_pending)
	{
		wavegen->wave_pong_compute_pending = 0;
		wavegen->wave_pong_compute_ready = 1;
	}
}



void VibeCheckWaveGen_StartDemo(VibeCheckWaveGen* wavegen)
{
	Sequencer_Init(&wavegen->sequencer);
	if (wavegen->demo_num)
		Sequencer_SetSequence(&wavegen->sequencer, demo_times0, sizeof(demo_times0) / sizeof(demo_times0[0]), 1);
	else
		Sequencer_SetSequence(&wavegen->sequencer, demo_times1, sizeof(demo_times1) / sizeof(demo_times1[0]), 1);
	wavegen->demo_num = !wavegen->demo_num;
	Sequencer_Start(&wavegen->sequencer, HAL_GetTick());
	VibeCheckWaveGen_Start(wavegen);
}

void VibeCheckWaveGen_StopDemo(VibeCheckWaveGen* wavegen)
{
	VibeCheckWaveGen_Stop(wavegen);
	Sequencer_Stop(&wavegen->sequencer);
}
