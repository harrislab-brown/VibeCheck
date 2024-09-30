/*
 * vibecheck_rgb.h
 *
 *  Created on: Sep 12, 2024
 *      Author: jantolik
 */

#ifndef INC_VIBECHECK_RGB_H_
#define INC_VIBECHECK_RGB_H_

#include "stm32h7xx_hal.h"

/* controls/sequences the RGB LEDs near the accelerometer connectors */

/*
 * First just need to get the basic functionality of controlling the colors of all LEDs
 *
 * They will be used as status indicators, ideally with little animations (priority handling?)
 *
 * Also to be used for visualization of the accelerometer readings
 *
 */

/*
 * The RGB LEDs are WS2812B-2020, addressable color by varying pulse width of each color bit. TIM4_CH1 is the signal line.
 * Bit order for LED is G7,G6,...G0,R7,...R0,B7,...B0
 */

#define VC_RGB_TIM_BASE_HZ 240000000
#define VC_RGB_TIM_PSC 3
#define VC_RGB_TIM_ARR 100  /* targeting 800 kHz PWM frequency */

#define VC_RGB_TIM_HIGH 48  /* CCR value for a low or high bit */
#define VC_RGB_TIM_LOW 24
#define VC_RGB_RESET_PERIODS 224  /* 280 us reset time, keep low voltage during this time */

#define VC_RGB_TIM_CHANNEL TIM_CHANNEL_1
#define VC_RGB_NUM_LEDS 9
#define VC_RGB_BITS_PER_LED 24

/*
 * NOTE: each time we update the LEDs, we put a reset period before and after which limits the max update rate to 1.2 kHz
 * This ensures there are no glitches from spurious edges on the data line. We could find a more elegant solution but this
 * solution only takes a bit more memory and DMA time, but no additional CPU time
 */
#define VC_RGB_BUF_LEN ((VC_RGB_NUM_LEDS * VC_RGB_BITS_PER_LED) + 2 * VC_RGB_RESET_PERIODS)

typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} VibeCheckRGB_Color;

typedef struct
{
	TIM_HandleTypeDef* htim;  /* timer to signal the LEDs */

	VibeCheckRGB_Color colors[VC_RGB_NUM_LEDS];  /* array of LED colors */
	uint16_t bit_stream[VC_RGB_BUF_LEN];  /* the stream of pulse widths to send to the LED array */

} VibeCheckRGB;

void VibeCheckRGB_Init(VibeCheckRGB* rgb, TIM_HandleTypeDef* htim);

void VibeCheckRGB_SetColor(VibeCheckRGB* rgb, uint32_t index, uint8_t r, uint8_t g, uint8_t b);  /* set the color of an individual LED */
VibeCheckRGB_Color VibeCheckRGB_GetColor(VibeCheckRGB* rgb, uint32_t index);

void VibeCheckRGB_SendColors(VibeCheckRGB* rgb);  /* send the colors to the LEDs */

#endif /* INC_VIBECHECK_RGB_H_ */
