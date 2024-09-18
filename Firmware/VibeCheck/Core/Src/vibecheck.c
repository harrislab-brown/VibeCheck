/*
 * vibecheck.c
 *
 *  Created on: Sep 12, 2024
 *      Author: jantolik
 */


#include "vibecheck.h"

void VibeCheck_Init(VibeCheck* vc, TIM_HandleTypeDef* htim_strobe)
{
	VibeCheckUSB_Init(&vc->usb);
	VibeCheckStrobe_Init(&vc->strobe, htim_strobe);
}

void VibeCheck_Loop(VibeCheck* vc)
{
	VibeCheckUSB_Update(&vc->usb);
}
