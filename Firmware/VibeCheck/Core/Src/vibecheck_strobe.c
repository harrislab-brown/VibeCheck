/*
 * vibecheck_strobe.c
 *
 *  Created on: Sep 17, 2024
 *      Author: johnt
 */


#include "vibecheck_strobe.h"


void VibeCheckStrobe_Init(VibeCheckStrobe* strobe, TIM_HandleTypeDef* htim)
{
	strobe->htim = htim;
	strobe->freq_hz = VC_STROBE_DEFAULT_FREQ_HZ;
	strobe->exposure_ms = VC_STROBE_DEFAULT_EXPOSURE_MS;
	strobe->phase_deg = 0.0f;
	strobe->is_running = 0;

	/* set up the timer registers */
	strobe->htim->Instance->PSC = VC_STROBE_TIM_PSC - 1;

	VibeCheckStrobe_SetFrequency(strobe, strobe->freq_hz);
	VibeCheckStrobe_SetExposure(strobe, strobe->exposure_ms);

}

void VibeCheckStrobe_Start(VibeCheckStrobe* strobe)
{
	/* start the timers with interrupts when period completes */
	strobe->is_running = 1;
	HAL_TIM_Base_Start_IT(strobe->htim);
	HAL_TIM_PWM_Start(strobe->htim, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(strobe->htim, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(strobe->htim, TIM_CHANNEL_3);
}

void VibeCheckStrobe_Stop(VibeCheckStrobe* strobe)
{
	strobe->is_running = 0;
	HAL_TIM_Base_Start_IT(strobe->htim);
	HAL_TIM_PWM_Stop(strobe->htim, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(strobe->htim, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(strobe->htim, TIM_CHANNEL_3);
}


uint32_t VibeCheckStrobe_IsRunning(VibeCheckStrobe* strobe)
{
	return strobe->is_running;
}

void VibeCheckStrobe_SetFrequency(VibeCheckStrobe* strobe, float freq_hz)
{
	if (freq_hz < VC_STROBE_MIN_FREQ_HZ)
		freq_hz = VC_STROBE_MIN_FREQ_HZ;
	if (freq_hz > VC_STROBE_MAX_FREQ_HZ)
		freq_hz = VC_STROBE_MAX_FREQ_HZ;

	strobe->arr_steady = VC_STROBE_TIM_COUNTS_PER_SECOND / freq_hz - 1;
	strobe->freq_hz = VC_STROBE_TIM_COUNTS_PER_SECOND / ((float)strobe->arr_steady + 1.0f);
	strobe->htim->Instance->ARR = strobe->arr_steady;
}

float VibeCheckStrobe_GetFrequency(VibeCheckStrobe* strobe)
{
	return strobe->freq_hz;
}

void VibeCheckStrobe_SetPhase(VibeCheckStrobe* strobe, float phase_deg)
{
	/*
	 * This is the tricky one.
	 * To shift the phase, update ARR for a single period and then put it back
	 * The most reliable way would probably by with DMA into the ARR register, but this seems more complicated
	 * Instead let's try with an interrupt at the end of each period, and some struct variables to keep track of the phase update sequence
	 */

	if (phase_deg < VC_STROBE_MIN_PHASE_DEG)
		phase_deg = VC_STROBE_MIN_PHASE_DEG;
	if (phase_deg > VC_STROBE_MAX_PHASE_DEG)
		phase_deg = VC_STROBE_MAX_PHASE_DEG;


	float deg_shift = phase_deg - strobe->phase_deg;  /* how many degrees we want to shift the phase */
	int32_t arr_shift = deg_shift / 360.0f * strobe->arr_steady;  /* how much we need to alter the ARR for a single period to achieve the target phase shift */

	if (arr_shift && !strobe->phase_change_pending && !strobe->revert_arr_pending)
	{
		strobe->phase_deg += (float)arr_shift / (float)strobe->arr_steady * 360.0f;  /* calculate the actual phase shift accounting for rounding error */
		strobe->arr_phase = strobe->arr_steady + arr_shift;
		strobe->phase_change_pending = 1;  /* set the flag to enact the phase shift on the ARR interrupt */
	}
}

float VibeCheckStrobe_GetPhase(VibeCheckStrobe* strobe)
{
	return strobe->phase_deg;
}

void VibeCheckStrobe_SetExposure(VibeCheckStrobe* strobe, float exposure_ms)
{

	uint32_t ccr_val = exposure_ms * 0.001f * VC_STROBE_TIM_COUNTS_PER_SECOND;
	if (ccr_val > strobe->htim->Instance->ARR)
		ccr_val = strobe->htim->Instance->ARR;
	strobe->exposure_ms = (float)ccr_val / (float)VC_STROBE_TIM_COUNTS_PER_SECOND * 1000.0f;
	strobe->htim->Instance->CCR1 = ccr_val;
	strobe->htim->Instance->CCR2 = ccr_val;
	strobe->htim->Instance->CCR3 = ccr_val;
}

float VibeCheckStrobe_GetExposure(VibeCheckStrobe* strobe)
{
	return strobe->exposure_ms;
}


/* use an interrupt to make sure this is called each period */
void VibeCheckStrobe_PeriodElapsedUpdate(VibeCheckStrobe* strobe)
{
	if (strobe->revert_arr_pending)
	{
		strobe->htim->Instance->ARR = strobe->arr_steady;
		strobe->revert_arr_pending = 0;
	}

	if (strobe->phase_change_pending)
	{
		strobe->htim->Instance->ARR = strobe->arr_phase;
		strobe->revert_arr_pending = 1;
		strobe->phase_change_pending = 0;
	}
}



