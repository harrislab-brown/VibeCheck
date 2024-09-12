#ifndef INC_DRIVER_RGB_LED_H_
#define INC_DRIVER_RGB_LED_H_


/*
 * INCLUDES
 */

#include "stm32h7xx_hal.h"
//#include "stm32h7xx_hal_conf.h"
#include "main.h"
#include "bitswap.h"

/*
 * DEFINES
 */
#define RGB_NUM_LEDS 			9
//#define RGB_TIM					htim4
#define RGB_TIM_CHANNEL			TIM_CHANNEL_1

/* assuming 800kHz timer frequency */
#define RGB_TIM_HIGH_DUTYCYCLE 	48
#define RGB_TIM_LOW_DUTYCYCLE 	24

#define RGB_RST_PERIODS			224 // 280us reset time
#define RGB_BITS_PER_LED		24

#define RGB_DMA_BUF_LEN ((RGB_NUM_LEDS * RGB_BITS_PER_LED) + RGB_RST_PERIODS)

typedef union{

		struct{ // Bit order for led is G7,G6,...G0,R7,...R0,B7,...B0
			uint8_t g;
			uint8_t r;
			uint8_t b;
		}color;

		uint32_t data;

}RGB_LED_DATA_COLOR;

/*
 * FUNCTIONS
 */
void			 	RGB_Init(TIM_HandleTypeDef* htim);
void				RGB_SetColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
HAL_StatusTypeDef 	RGB_Update();
void 				RGB_Callback(); // call when PWM pulse is finished


#endif /* INC_DRIVER_RGB_LED_H_ */
