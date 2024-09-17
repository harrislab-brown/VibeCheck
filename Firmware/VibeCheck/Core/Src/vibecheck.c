/*
 * vibecheck.c
 *
 *  Created on: Sep 12, 2024
 *      Author: jantolik
 */


#include "vibecheck.h"

void VibeCheck_Init(VibeCheck* vc)
{
	VibeCheckUSB_Init(&vc->usb);
	vc->time = 0;
}

void VibeCheck_Loop(VibeCheck* vc)
{

	VibeCheckUSB_Update(&vc->usb);

	if (HAL_GetTick() - vc->time > 100)
	{
		VibeCheckUSB_AddData(&vc->usb, 1, vc->time, 1.0f, 2.0f, 3.0f);
		vc->time = HAL_GetTick();
	}
}
