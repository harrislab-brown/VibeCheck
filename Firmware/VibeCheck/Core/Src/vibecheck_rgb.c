/*
 * vibecheck_rgb.c
 *
 *  Created on: Sep 30, 2024
 *      Author: johnt
 */

#include "vibecheck_rgb.h"


void VibeCheckRGB_Init(VibeCheckRGB* rgb, TIM_HandleTypeDef* htim)
{
	Sequencer_Init(&rgb->base_sequence);
	Sequencer_Init(&rgb->top_sequence);

	htim->Instance->PSC = VC_RGB_TIM_PSC - 1;
	htim->Instance->ARR = VC_RGB_TIM_ARR - 1;
	rgb->htim = htim;

	for (uint32_t i = 0; i < VC_RGB_NUM_LEDS; i++)
		rgb->update_with_sequence[i] = 1;  /* LEDs follow the sequence by default */

	/* clear the DMA buffer, particularly setting all zeros during the reset time */
	for (uint32_t i = 0; i < VC_RGB_BUF_LEN; i++)
		rgb->bit_stream[i] = 0;

	/* turn off all LEDs */
	VibeCheckRGB_SetAllOff(rgb);
	VibeCheckRGB_SendColors(rgb);
}


void VibeCheckRGB_Update(VibeCheckRGB* rgb)  /* call repeatedly in the main loop */
{
	uint32_t time = HAL_GetTick();
	uint32_t step;
	if (Sequencer_Update(&rgb->top_sequence, time, &step))
	{
		for (uint32_t i = 0; i < VC_RGB_NUM_LEDS; i++)
		{
			VibeCheckRGB_Color color = rgb->top_sequence_colors[VC_RGB_NUM_LEDS * step + i];
			if (rgb->update_with_sequence[i])
				VibeCheckRGB_SetColor(rgb, i, color.r, color.g, color.b);
		}
		VibeCheckRGB_SendColors(rgb);
	}
	else if (!Sequencer_IsRunning(&rgb->top_sequence) && Sequencer_Update(&rgb->base_sequence, time, &step))
	{
		/* top sequence has higher priority, so only update the base sequence if the top sequence is not running */
		for (uint32_t i = 0; i < VC_RGB_NUM_LEDS; i++)
		{
			VibeCheckRGB_Color color = rgb->base_sequence_colors[VC_RGB_NUM_LEDS * step + i];
			if (rgb->update_with_sequence[i])
				VibeCheckRGB_SetColor(rgb, i, color.r, color.g, color.b);
		}
		VibeCheckRGB_SendColors(rgb);
	}
}


void VibeCheckRGB_SetBaseSequence(VibeCheckRGB* rgb, const uint32_t* time, const VibeCheckRGB_Color* color, uint32_t len)
{
	rgb->base_sequence_colors = color;
	Sequencer_SetSequence(&rgb->base_sequence, time, len, 1);
}

void VibeCheckRGB_StartBaseSequence(VibeCheckRGB* rgb)
{
	Sequencer_Start(&rgb->base_sequence, HAL_GetTick());
}

void VibeCheckRGB_StopBaseSequence(VibeCheckRGB* rgb)
{
	Sequencer_Stop(&rgb->base_sequence);
}

void VibeCheckRGB_SetTopSequence(VibeCheckRGB* rgb, const uint32_t* time, const VibeCheckRGB_Color* color, uint32_t len)
{
	rgb->top_sequence_colors = color;
	Sequencer_SetSequence(&rgb->top_sequence, time, len, 0);
}

void VibeCheckRGB_StartTopSequence(VibeCheckRGB* rgb)
{
	Sequencer_Start(&rgb->top_sequence, HAL_GetTick());
}

void VibeCheckRGB_StopTopSequence(VibeCheckRGB* rgb)
{
	Sequencer_Stop(&rgb->top_sequence);
}


void VibeCheckRGB_SetColor(VibeCheckRGB* rgb, uint32_t index, uint8_t r, uint8_t g, uint8_t b)  /* set the color of an individual LED */
{
	if (index > VC_RGB_NUM_LEDS - 1)
		index = VC_RGB_NUM_LEDS - 1;

	rgb->colors[index].r = r;
	rgb->colors[index].g = g;
	rgb->colors[index].b = b;
}


VibeCheckRGB_Color VibeCheckRGB_GetColor(VibeCheckRGB* rgb, uint32_t index)
{
	if (index > VC_RGB_NUM_LEDS - 1)
			index = VC_RGB_NUM_LEDS - 1;

	return rgb->colors[index];
}


void VibeCheckRGB_SetAllOff(VibeCheckRGB* rgb)
{
	for (uint32_t i = 0; i < VC_RGB_NUM_LEDS; i++)
		VibeCheckRGB_SetColor(rgb, i, 0, 0, 0);
}


void VibeCheckRGB_SetUpdateWithSequence(VibeCheckRGB* rgb, uint32_t index, uint32_t update)
{
	if (index > VC_RGB_NUM_LEDS - 1)
			index = VC_RGB_NUM_LEDS - 1;

	rgb->update_with_sequence[index] = update;
}


void VibeCheckRGB_SendColors(VibeCheckRGB* rgb)  /* send the colors to the LEDs */
{
	/* compute the bit stream timings */
	uint32_t i = VC_RGB_RESET_PERIODS;  /* put the reset periods first so we are not affected by spurious pin events at startup and such */
	for (uint32_t j = 0; j < VC_RGB_NUM_LEDS; j++)  /* Bit order for LED is G7, G6, ..., G0, R7, ..., R0, B7, ..., B0 */
	{
		for (uint32_t k = 8; k > 0; k--)  /* green */
			rgb->bit_stream[i++] = ((rgb->colors[j].g >> (k - 1)) & 0x01) ? VC_RGB_TIM_HIGH : VC_RGB_TIM_LOW;

		for (uint32_t k = 8; k > 0; k--)  /* red */
			rgb->bit_stream[i++] = ((rgb->colors[j].r >> (k - 1)) & 0x01) ? VC_RGB_TIM_HIGH : VC_RGB_TIM_LOW;

		for (uint32_t k = 8; k > 0; k--)  /* blue */
			rgb->bit_stream[i++] = ((rgb->colors[j].b >> (k - 1)) & 0x01) ? VC_RGB_TIM_HIGH : VC_RGB_TIM_LOW;
	}

	/* start the DMA transfer */
	(void)HAL_TIM_PWM_Start_DMA(rgb->htim, VC_RGB_TIM_CHANNEL, (uint32_t*)rgb->bit_stream, VC_RGB_BUF_LEN);
}
