
#include "driver_RGB_LED.h"

static TIM_HandleTypeDef *RGB_TIM;
static RGB_LED_DATA_COLOR RGB_LED_DATA[RGB_NUM_LEDS];
static uint16_t RGB_DMA_BUF[RGB_DMA_BUF_LEN];
static volatile uint8_t RGB_DMA_COMPLETE_FLAG;

void RGB_Init(TIM_HandleTypeDef* htim){
	// Initialize RGB timer for PWM output
	RGB_TIM = htim;
	//HAL_StatusTypeDef halStatus = HAL_TIM_PWM_Init(RGB_TIM);

	// Clear DMA buffer
	for ( uint16_t i = 0; i < RGB_DMA_BUF_LEN; i++){
		RGB_DMA_BUF[i] = 0;
	}

	RGB_DMA_COMPLETE_FLAG = 1;
}


void RGB_SetColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b){

	RGB_LED_DATA[index].color.r = r;
	RGB_LED_DATA[index].color.g = g;
	RGB_LED_DATA[index].color.b = b;

}


HAL_StatusTypeDef RGB_Update(){
	// Return busy if previous DMA transfer is not complete
	if(!RGB_DMA_COMPLETE_FLAG) return HAL_BUSY;

	uint16_t bufIndex = 0;

	for (uint8_t ledIndex = 0; ledIndex < RGB_NUM_LEDS; ledIndex ++){

		for (uint8_t bitIndex = 0; bitIndex < RGB_BITS_PER_LED; bitIndex ++){

			if ( (RGB_LED_DATA[ledIndex].data >> bitIndex) & 0x01){

				RGB_DMA_BUF[bufIndex] = RGB_TIM_HIGH_DUTYCYCLE;

			} else {

			RGB_DMA_BUF[bufIndex] = RGB_TIM_LOW_DUTYCYCLE;

			}

			bufIndex ++;
		}
	}

	// start PWM DMA transfer
	HAL_StatusTypeDef halStatus = HAL_TIM_PWM_Start_DMA(RGB_TIM, RGB_TIM_CHANNEL, (uint32_t *)RGB_DMA_BUF, RGB_DMA_BUF_LEN);

	if (halStatus == HAL_OK){
		// if DMA transfer starts successfully clear DMA complete flag
		RGB_DMA_COMPLETE_FLAG = 0;
	}

	return halStatus;
}

void RGB_Callback(){
	//HAL_TIM_PWM_Stop_DMA(RGB_TIM, RGB_TIM_CHANNEL);
	RGB_DMA_COMPLETE_FLAG = 1;
}
