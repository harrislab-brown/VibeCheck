/*
 * vibecheck.c
 *
 *  Created on: Sep 12, 2024
 *      Author: jantolik
 */


#include "vibecheck.h"

void VibeCheck_Init(VibeCheck* vc, TIM_HandleTypeDef* htim_strobe)
{
	VibeCheckShell_Init(&vc->shell);

	VibeCheckShell_CMD strobe_cmd = {
			.name = "strobe",
			.execute = VibeCheckStrobeCMD_Execute,
			.obj = &vc->strobe
	};

	VibeCheckShell_RegisterCommand(&vc->shell, strobe_cmd);


	VibeCheckUSB_Init(&vc->usb, &vc->shell);
	VibeCheckStrobe_Init(&vc->strobe, htim_strobe);
}

void VibeCheck_Loop(VibeCheck* vc)
{
	VibeCheckUSB_Update(&vc->usb);
}
