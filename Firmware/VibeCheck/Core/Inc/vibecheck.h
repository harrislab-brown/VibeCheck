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
	VibeCheckAccel accel;
} VibeCheck;


void VibeCheck_Init(VibeCheck* vc, TIM_HandleTypeDef* htim_strobe);
void VibeCheck_Loop(VibeCheck* vc);


#endif /* INC_VIBECHECK_H_ */
