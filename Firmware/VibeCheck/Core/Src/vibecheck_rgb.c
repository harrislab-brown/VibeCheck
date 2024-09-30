/*
 * vibecheck_rgb.c
 *
 *  Created on: Sep 30, 2024
 *      Author: johnt
 */

#include "vibecheck_rgb.h"


void VibeCheckRGB_Init(VibeCheckRGB* rgb, TIM_HandleTypeDef* htim)
{
	htim->Instance->PSC = VC_RGB_TIM_PSC - 1;
	htim->Instance->ARR = VC_RGB_TIM_ARR - 1;
	rgb->htim = htim;

	/* clear the DMA buffer, particularly setting all zeros during the reset time */
	for (uint32_t i = 0; i < VC_RGB_BUF_LEN; i++)
		rgb->bit_stream[i] = 0;

	/* turn off all LEDs */
	for (uint32_t i = 0; i < VC_RGB_NUM_LEDS; i++)
		VibeCheckRGB_SetColor(rgb, i, 0, 0, 0);

	VibeCheckRGB_SendColors(rgb);
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
