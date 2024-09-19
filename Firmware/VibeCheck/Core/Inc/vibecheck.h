/*
 * vibecheck.h
 *
 *  Created on: Sep 12, 2024
 *      Author: jantolik
 */

#ifndef INC_VIBECHECK_H_
#define INC_VIBECHECK_H_

#include "vibecheck_shell.h"
#include "vibecheck_USB.h"
#include "vibecheck_strobe.h"
#include "vibecheck_strobe_cmd.h"

/* main application code */

/*
 * - explore using DFU for students to update the firmware themselves
 */

typedef struct
{
	VibeCheckShell shell;
	VibeCheckUSB usb;
	VibeCheckStrobe strobe;
} VibeCheck;


void VibeCheck_Init(VibeCheck* vc, TIM_HandleTypeDef* htim_strobe);
void VibeCheck_Loop(VibeCheck* vc);


#endif /* INC_VIBECHECK_H_ */
