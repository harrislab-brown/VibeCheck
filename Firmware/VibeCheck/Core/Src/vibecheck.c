/*
 * vibecheck.c
 *
 *  Created on: Sep 12, 2024
 *      Author: jantolik
 */


#include "vibecheck.h"
#include "vibecheck_rgb_sequences.h"


static uint32_t time_prev_led_update;
volatile uint32_t usb_ready = 1;


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


	/* FIXME: sometimes the accelerometers disconnected unexpectedly */


	/* call object update functions */
	VibeCheckWaveGen_Update(&vc->wavegen);
	VibeCheckRGB_Update(&vc->rgb);
	VibeCheckSensor_Update(&vc->sensor);

	/* update the shell */
	VibeCheckShell_Status shell_status = VibeCheckShell_Update(&vc->shell);
	char* usb_tx;
	uint32_t usb_tx_len;
	if (VibeCheckShell_GetOutput(&vc->shell, &usb_tx, &usb_tx_len) && usb_ready)
	{
		CDC_Transmit_HS((uint8_t*)usb_tx, usb_tx_len);
		VibeCheckShell_UpdateOutputBuffer(&vc->shell, usb_tx_len);
	}


	/* blink indicator LEDs based on shell status */
	if (shell_status.ihandl_status == VC_SHELL_INPUT_PROCESSED)
	{
		VibeCheckRGB_SetTopSequence(&vc->rgb, led_shell_success_times, led_shell_success_colors, led_shell_success_len);
		VibeCheckRGB_StartTopSequence(&vc->rgb);
	}
	else if (shell_status.ihandl_status == VC_SHELL_INPUT_ERROR_NO_HANDLER || shell_status.ihandl_status == VC_SHELL_INPUT_ERROR_EXECUTING)
	{
		VibeCheckRGB_SetTopSequence(&vc->rgb, led_shell_failure_times, led_shell_failure_colors, led_shell_failure_len);
		VibeCheckRGB_StartTopSequence(&vc->rgb);
	}


	/* visualize the acceleration with the RGB LEDs */
	uint32_t time = HAL_GetTick();
	if (time - time_prev_led_update > 30)
	{
		time_prev_led_update = time;

		if (!vc->rgb.top_sequence.is_running)  /* let the top sequence have precedence over the visualization */
		{
			for (uint32_t i = 0; i < VC_SENSOR_NUM_SENSORS; i++)
			{
				if (vc->sensor.status[i].is_connected)
				{
					if (vc->sensor.status[i].accel_measuring)
					{
						/* write the LEDs */
						VibeCheckRGB_SetColor(&vc->rgb, 3 * i + 0, 255 * fabs(vc->sensor.sensor_array[i].accel_x) / vc->sensor.sensor_config[i].g_range, 0, 0);
						VibeCheckRGB_SetColor(&vc->rgb, 3 * i + 1, 0, 255 * fabs(vc->sensor.sensor_array[i].accel_y) / vc->sensor.sensor_config[i].g_range, 0);
						VibeCheckRGB_SetColor(&vc->rgb, 3 * i + 2, 0, 0, 255 * fabs(vc->sensor.sensor_array[i].accel_z) / vc->sensor.sensor_config[i].g_range);
						VibeCheckRGB_SendColors(&vc->rgb);
					}
					else if (vc->sensor.status[i].gyro_measuring)
					{
						/* write the LEDs */
						VibeCheckRGB_SetColor(&vc->rgb, 3 * i + 0, 255 * fabs(vc->sensor.sensor_array[i].gyro_x) / vc->sensor.sensor_config[i].dps_range, 0, 0);
						VibeCheckRGB_SetColor(&vc->rgb, 3 * i + 1, 0, 255 * fabs(vc->sensor.sensor_array[i].gyro_y) / vc->sensor.sensor_config[i].dps_range, 0);
						VibeCheckRGB_SetColor(&vc->rgb, 3 * i + 2, 0, 0, 255 * fabs(vc->sensor.sensor_array[i].gyro_z) / vc->sensor.sensor_config[i].dps_range);
						VibeCheckRGB_SendColors(&vc->rgb);
					}
				}
			}
		}
	}
}
