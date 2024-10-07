/*
 * vibecheck.c
 *
 *  Created on: Sep 12, 2024
 *      Author: jantolik
 */


#include "vibecheck.h"
#include "vibecheck_rgb_sequences.h"

void VibeCheck_Init(VibeCheck* vc,
		TIM_HandleTypeDef* htim_strobe,
		TIM_HandleTypeDef* htim_wavegen,
		DAC_HandleTypeDef* hdac_wavegen,
		TIM_HandleTypeDef* htim_rgb,
		volatile uint32_t* time_micros,
		SPI_HandleTypeDef* hspi_accel0,
		SPI_HandleTypeDef* hspi_accel1,
		SPI_HandleTypeDef* hspi_accel2)
{

	HAL_Delay(10);  /* wait for steady power so the RGB LEDs don't get into a weird state */

	VibeCheckShell_Init(&vc->shell);  /* the shell is linked to the USB middle-ware in usbd_cdc_if.c */

	VibeCheckShell_InputHandler strobe_cmd = {
			.name = "strobe",
			.execute = VibeCheckStrobeCMD_Execute,
			.obj = &vc->strobe
	};

	VibeCheckShell_InputHandler wavegen_cmd = {
			.name = "wavegen",
			.execute = VibeCheckWaveGenCMD_Execute,
			.obj = &vc->wavegen
	};

	VibeCheckShell_InputHandler rgb_cmd = {
			.name = "rgb",
			.execute = VibeCheckRGBCMD_Execute,
			.obj = &vc->rgb
	};

	VibeCheckShell_InputHandler sensor_cmd = {
			.name = "sensor",
			.execute = VibeCheckSensorCMD_Execute,
			.obj = &vc->sensor
	};

	VibeCheckShell_RegisterInputHandler(&vc->shell, strobe_cmd);
	VibeCheckShell_RegisterInputHandler(&vc->shell, wavegen_cmd);
	VibeCheckShell_RegisterInputHandler(&vc->shell, rgb_cmd);
	VibeCheckShell_RegisterInputHandler(&vc->shell, sensor_cmd);

	VibeCheckShell_OutputHandler wavegen_sender = {
			.execute = VibeCheckWaveGenSender_Execute,
			.obj = &vc->wavegen
	};

	VibeCheckShell_OutputHandler sensor_data_sender = {
			.execute = VibeCheckSensorSender_Data_Execute,
			.obj = &vc->sensor
	};

	VibeCheckShell_OutputHandler sensor_status_sender = {
			.execute = VibeCheckSensorSender_Status_Execute,
			.obj = &vc->sensor
	};

	VibeCheckShell_RegisterOutputHandler(&vc->shell, wavegen_sender);
	VibeCheckShell_RegisterOutputHandler(&vc->shell, sensor_data_sender);
	VibeCheckShell_RegisterOutputHandler(&vc->shell, sensor_status_sender);

	VibeCheckStrobe_Init(&vc->strobe, htim_strobe);
	VibeCheckWaveGen_Init(&vc->wavegen, hdac_wavegen, htim_wavegen);
	VibeCheckRGB_Init(&vc->rgb, htim_rgb);
	VibeCheckRGB_SetBaseSequence(&vc->rgb, base_sequence_times, base_sequence_colors, base_sequence_len);
	VibeCheckRGB_SetTopSequence(&vc->rgb, top_sequence_times, top_sequence_colors, top_sequence_len);
	VibeCheckSensor_Init(&vc->sensor, time_micros, hspi_accel0, hspi_accel1, hspi_accel2);
}

void VibeCheck_Loop(VibeCheck* vc)
{
	VibeCheckWaveGen_Update(&vc->wavegen);
	VibeCheckRGB_Update(&vc->rgb);
	VibeCheckSensor_Update(&vc->sensor);

	VibeCheckShell_Status shell_status = VibeCheckShell_Update(&vc->shell);

	char* usb_tx;
	uint32_t usb_tx_len;
	if (VibeCheckShell_GetOutput(&vc->shell, &usb_tx, &usb_tx_len))
		while (CDC_Transmit_HS((uint8_t*)usb_tx, usb_tx_len) != USBD_OK);  /* block until the USB transmission starts to make sure we send all data */
}
