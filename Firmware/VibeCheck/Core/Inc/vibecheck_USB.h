/*
 * vibecheckUSB.h
 *
 *  Created on: Sep 12, 2024
 *      Author: jantolik
 */

#ifndef INC_VIBECHECK_USB_H_
#define INC_VIBECHECK_USB_H_

#include "usbd_cdc_if.h"

/*
 * - sends data to the host PC (COBS encoding)
 * - receives commands in 64 byte packets
 * - controls status of the board (this is a high level object)
 */

typedef struct
{

	/* handles received messages and commands */
	uint8_t* rx_buf;
	uint8_t* rx_ready;

} VibeCheckUSB;

void VibeCheckUSB_Init(VibeCheckUSB* usb);

uint32_t VibeCheckUSB_ProcessCommand(VibeCheckUSB* usb);

#endif /* INC_VIBECHECK_USB_H_ */
