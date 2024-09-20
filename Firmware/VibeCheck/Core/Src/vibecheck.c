/*
 * vibecheck.c
 *
 *  Created on: Sep 12, 2024
 *      Author: jantolik
 */


#include "vibecheck.h"

void VibeCheck_Init(VibeCheck* vc, TIM_HandleTypeDef* htim_strobe)
{
	VibeCheckShell_Init(&vc->shell);  /* the shell is linked to the USB middle-ware in usbd_cdc_if.c */

	VibeCheckShell_InputHandler strobe_cmd = {
			.name = "strobe",
			.execute = VibeCheckStrobeCMD_Execute,
			.obj = &vc->strobe
	};

	VibeCheckShell_RegisterInputHandler(&vc->shell, strobe_cmd);

	VibeCheckShell_InputHandler accel_cmd = {
			.name = "accel",
			.execute = VibeCheckAccelCMD_Execute,
			.obj = &vc->accel
	};

	VibeCheckShell_OutputHandler accel_sender = {
			.execute = VibeCheckAccelSender_Execute,
			.obj = &vc->accel
	};

	VibeCheckShell_RegisterInputHandler(&vc->shell, accel_cmd);
	VibeCheckShell_RegisterOutputHandler(&vc->shell, accel_sender);

	VibeCheckStrobe_Init(&vc->strobe, htim_strobe);
	VibeCheckAccel_Init(&vc->accel);
}

void VibeCheck_Loop(VibeCheck* vc)
{
	VibeCheckAccel_Update(&vc->accel);

	VibeCheckShell_Status shell_status = VibeCheckShell_Update(&vc->shell);

	char* usb_tx;
	uint32_t usb_tx_len;
	if (VibeCheckShell_GetOutput(&vc->shell, &usb_tx, &usb_tx_len))
	{
		CDC_Transmit_HS((uint8_t*)usb_tx, usb_tx_len);
	}
}
