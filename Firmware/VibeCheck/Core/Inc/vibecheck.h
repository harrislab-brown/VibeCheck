/*
 * vibecheck.h
 *
 *  Created on: Sep 12, 2024
 *      Author: jantolik
 */

#ifndef INC_VIBECHECK_H_
#define INC_VIBECHECK_H_

#include "vibecheck_shell.h"
#include "vibecheck_strobe.h"
#include "vibecheck_strobe_handler.h"
#include "vibecheck_wavegen.h"
#include "vibecheck_wavegen_handler.h"
#include "vibecheck_rgb.h"
#include "vibecheck_rgb_handler.h"
#include "vibecheck_accel.h"
#include "vibecheck_accel_handler.h"
#include "usbd_cdc_if.h"

/* main application code */

/*
 * - explore using DFU for students to update the firmware themselves
 */

typedef struct
{
	VibeCheckShell shell;
	VibeCheckStrobe strobe;
	VibeCheckWaveGen wavegen;
	VibeCheckRGB rgb;
	VibeCheckAccel accel;
} VibeCheck;


void VibeCheck_Init(VibeCheck* vc,
		TIM_HandleTypeDef* htim_strobe,
		TIM_HandleTypeDef* htim_wavegen,
		DAC_HandleTypeDef* hdac_wavegen,
		TIM_HandleTypeDef* htim_rgb);

void VibeCheck_Loop(VibeCheck* vc);


#endif /* INC_VIBECHECK_H_ */
