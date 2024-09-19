/*
 * vibecheck_strobe.h
 *
 *  Created on: Sep 12, 2024
 *      Author: jantolik
 */

#ifndef INC_VIBECHECK_STROBE_H_
#define INC_VIBECHECK_STROBE_H_

#include "stm32h7xx_hal.h"

/* controls the bright ring of strobe lights using timer hardware */

/*
 * - variable frequency, duty cycle and phase of the strobing (directly manipulate CNT register to shift phase or change ARR for one cycle )
 * - potentially synchronize the phase with the wavegen
 */


#define VC_STROBE_TIM_BASE_HZ 240000000
#define VC_STROBE_TIM_PSC 2400
#define VC_STROBE_TIM_COUNTS_PER_SECOND (VC_STROBE_TIM_BASE_HZ / VC_STROBE_TIM_PSC)

#define VC_STROBE_MIN_FREQ_HZ 2.0f
#define VC_STROBE_MAX_FREQ_HZ 1000.0f
#define VC_STROBE_MIN_PHASE_DEG -180.0f
#define VC_STROBE_MAX_PHASE_DEG 180.0f


/*
 * The strobe is on a 16 bit timer. Need to pick a suitable pre-scaler. With PSC 2400 every count will be 10 us, minimum frequency will be about 2 Hz.
 * Rounding errors abound with all this stuff I'm sure but it's just for qualitative observations so no need to be careful
 */


typedef struct
{
	TIM_HandleTypeDef* htim;  /* timer handle for strobe (TIM3_CH1,2,3 -- for now just combine the channels)*/

	float freq_hz;
	float exposure_ms;
	float phase_deg;

	/* for changing the phase */
	uint32_t arr_steady, arr_phase;
	uint32_t phase_change_pending, revert_arr_pending;

} VibeCheckStrobe;

void VibeCheckStrobe_Init(VibeCheckStrobe* strobe, TIM_HandleTypeDef* htim);

void VibeCheckStrobe_Start(VibeCheckStrobe* strobe);
void VibeCheckStrobe_Stop(VibeCheckStrobe* strobe);

void VibeCheckStrobe_SetFrequency(VibeCheckStrobe* strobe, float freq_hz);
float VibeCheckStrobe_GetFrequency(VibeCheckStrobe* strobe);

void VibeCheckStrobe_SetPhase(VibeCheckStrobe* strobe, float phase_deg);
float VibeCheckStrobe_GetPhase(VibeCheckStrobe* strobe);

void VibeCheckStrobe_SetExposure(VibeCheckStrobe* strobe, float exposure_ms);
float VibeCheckStrobe_GetExposure(VibeCheckStrobe* strobe);

/* use an interrupt to make sure this is called each period */
void VibeCheckStrobe_PeriodElapsedUpdate(VibeCheckStrobe* strobe);

#endif /* INC_VIBECHECK_STROBE_H_ */
