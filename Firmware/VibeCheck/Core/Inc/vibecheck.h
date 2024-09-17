/*
 * vibecheck.h
 *
 *  Created on: Sep 12, 2024
 *      Author: jantolik
 */

#ifndef INC_VIBECHECK_H_
#define INC_VIBECHECK_H_

#include "vibecheck_USB.h"

/* main application code */

/*
 * - explore using DFU for students to update the firmware themselves
 */

typedef struct
{
	VibeCheckUSB usb;
	uint32_t time;
} VibeCheck;


void VibeCheck_Init(VibeCheck* vc);
void VibeCheck_Loop(VibeCheck* vc);


#endif /* INC_VIBECHECK_H_ */
