/*
 * vibecheck_usb.c
 *
 *  Created on: Sep 17, 2024
 *      Author: jantolik
 */


#include "vibecheck_USB.h"

uint8_t usb_rx_buf[64];  /* see USB_DEVICE -> App -> usbd_cdc_if.c for implementation */
uint8_t usb_rx_ready = 0;  /* flag when USB data has been received */

void VibeCheckUSB_Init(VibeCheckUSB* usb)
{
	usb->rx_buf = usb_rx_buf;
	usb->rx_ready = &usb_rx_ready;
}

uint32_t VibeCheckUSB_ProcessCommand(VibeCheckUSB* usb)
{
	/* return true if we have successfully processed a command */

}

