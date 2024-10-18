/*
 * vibecheck.c
 *
 *  Created on: Sep 12, 2024
 *      Author: jantolik
 */


#include "vibecheck.h"
#include "vibecheck_rgb_sequences.h"


static uint32_t time_prev_led_update;
static uint32_t time_prev_mute_press;
static uint32_t time_prev_record_press;


extern uint32_t is_muted, mute_pressed, record_pressed;


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

	VibeCheckShell_InputHandler record_cmd = {
			.name = "record",
			.execute = VibeCheckRecordCMD_Execute,
			.obj = NULL
	};

	VibeCheckShell_InputHandler connect_cmd = {
			.name = "connect",
			.execute = VibeCheckConnectCMD_Execute,
			.obj = &vc->rgb
	};

	VibeCheckShell_InputHandler disconnect_cmd = {
			.name = "disconnect",
			.execute = VibeCheckDisconnectCMD_Execute,
			.obj = &vc->rgb
	};

	VibeCheckShell_RegisterInputHandler(&vc->shell, strobe_cmd);
	VibeCheckShell_RegisterInputHandler(&vc->shell, wavegen_cmd);
	VibeCheckShell_RegisterInputHandler(&vc->shell, rgb_cmd);
	VibeCheckShell_RegisterInputHandler(&vc->shell, sensor_cmd);
	VibeCheckShell_RegisterInputHandler(&vc->shell, record_cmd);
	VibeCheckShell_RegisterInputHandler(&vc->shell, connect_cmd);
	VibeCheckShell_RegisterInputHandler(&vc->shell, disconnect_cmd);


	VibeCheckShell_OutputHandler sensor_data_sender = {
			.execute = VibeCheckSensorSender_Data_Execute,
			.obj = &vc->sensor
	};

	VibeCheckShell_OutputHandler sensor_status_sender = {
			.execute = VibeCheckSensorSender_Status_Execute,
			.obj = &vc->sensor
	};

	VibeCheckShell_OutputHandler mute_button_sender = {
			.execute = VibeCheckMuteSender_Execute,
			.obj = NULL
	};

	VibeCheckShell_OutputHandler record_button_sender = {
			.execute = VibeCheckRecordSender_Execute,
			.obj = NULL
	};


	VibeCheckShell_RegisterOutputHandler(&vc->shell, sensor_data_sender);
	VibeCheckShell_RegisterOutputHandler(&vc->shell, sensor_status_sender);
	VibeCheckShell_RegisterOutputHandler(&vc->shell, mute_button_sender);
	VibeCheckShell_RegisterOutputHandler(&vc->shell, record_button_sender);


	VibeCheckStrobe_Init(&vc->strobe, htim_strobe);
	VibeCheckWaveGen_Init(&vc->wavegen, hdac_wavegen, htim_wavegen);
	VibeCheckRGB_Init(&vc->rgb, htim_rgb);
	VibeCheckRGB_SetBaseSequence(&vc->rgb, connect_sequence_times, connect_sequence_colors, connect_sequence_len);
	VibeCheckRGB_SetTopSequence(&vc->rgb, top_sequence_times, top_sequence_colors, top_sequence_len);
	VibeCheckSensor_Init(&vc->sensor, time_micros, hspi_accel0, hspi_accel1, hspi_accel2);
}

void VibeCheck_Loop(VibeCheck* vc)
{
	uint32_t time = HAL_GetTick();


	/* call object update functions */
	VibeCheckWaveGen_Update(&vc->wavegen);
	VibeCheckRGB_Update(&vc->rgb);
	VibeCheckSensor_Update(&vc->sensor);


	/* update the shell */
	VibeCheckShell_Status shell_status = VibeCheckShell_Update(&vc->shell);

	/* blink indicator LEDs based on shell status */
//	if (shell_status.ihandl_status == VC_SHELL_INPUT_PROCESSED)
//	{
//		VibeCheckRGB_SetTopSequence(&vc->rgb, led_shell_success_times, led_shell_success_colors, led_shell_success_len);
//		VibeCheckRGB_StartTopSequence(&vc->rgb);
//	}
//	else if (shell_status.ihandl_status == VC_SHELL_INPUT_ERROR_NO_HANDLER || shell_status.ihandl_status == VC_SHELL_INPUT_ERROR_EXECUTING)
//	{
//		VibeCheckRGB_SetTopSequence(&vc->rgb, led_shell_failure_times, led_shell_failure_colors, led_shell_failure_len);
//		VibeCheckRGB_StartTopSequence(&vc->rgb);
//	}


	/* send over USB */
	char* usb_tx;
	uint32_t usb_tx_len;
	if (VibeCheckShell_GetOutput(&vc->shell, &usb_tx, &usb_tx_len))
	{
		/*
		 * CDC_Transmit_HS is a mystery and seems to sometimes reorder messages.
		 * However, it seems every call to CDC_Transmit_HS keeps its buffer intact.
		 * And it seems that all data is getting to the host with this scheme, but it
		 * will need to be sorted on the host side (this is easy because we have the time-stamps).
		 * Another requirement seems to be not calling CDC_Transmit_HS too fast, or else it will
		 * reorder the packets anyways. (calling every 15 ms seems safe but calling every 1.5 ms re-orders the packets)
		 */

		/*
		 * We can keep track of whether all data is reaching the host based on the shell output buffer count.
		 * If this buffer overflows then we need to reduce the data rate. The shell status output will tell us
		 * if a buffer overflow occurs.
		 */

		/*
		 * The only real issue is what to do when a data packet wraps the shell output buffer - we need to send this
		 * with a single call to CDC_Transmit_HS but the memory is not contiguous. We fixed this by having the shell
		 * output buffer wrap before getting the next packet so that each packet is continuous in memory
		 */

		/* XXX: for checking framing errors */
//		char start = usb_tx[0];  /* should be 'd', 'a', or 'e' */
//		char end = usb_tx[usb_tx_len - 1];  /* should be '\n' */
//
//		if ((end != '\n') || (start != 'a' && start != 'd' && start != 'e'))
//			HAL_Delay(1000);
		/* XXX */

		if (CDC_Transmit_HS((uint8_t*)usb_tx, usb_tx_len) == USBD_OK)
			VibeCheckShell_UpdateOutputBuffer(&vc->shell, usb_tx_len);
	}


	/* visualize the acceleration with the RGB LEDs */
	/* TODO: turn off the LEDs when done measuring */
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


	/* use RGB LEDs to indicate when sensors are connected or disconnected */

	/* TODO: make these only affect the LEDs corresponding the recently connected sensor (transparency?) */
	uint32_t channel;
	uint32_t is_connected;
	if (VibeCheckSensor_ConnectionChanged(&vc->sensor, &channel, &is_connected))
	{
		if (is_connected)
		{
			VibeCheckRGB_SetTopSequence(&vc->rgb, led_sensor_connected_times[channel], led_sensor_connected_colors[channel], led_sensor_connected_len[channel]);
			VibeCheckRGB_StartTopSequence(&vc->rgb);
		}
		else
		{
			VibeCheckRGB_SetTopSequence(&vc->rgb, led_sensor_disconnected_times[channel], led_sensor_disconnected_colors[channel], led_sensor_disconnected_len[channel]);
			VibeCheckRGB_StartTopSequence(&vc->rgb);
		}

		VibeCheckSensor_ResetConnectionFlag(&vc->sensor, channel);
	}


	/* check the buttons */
	if (time - time_prev_mute_press > 250 && HAL_GPIO_ReadPin(MUTE_BUTTON_GPIO_Port, MUTE_BUTTON_Pin))
	{
		time_prev_mute_press = time;
		mute_pressed = 1;

		if (is_muted)
		{
			is_muted = 0;
			HAL_GPIO_WritePin(MUTE_SIGNAL_GPIO_Port, MUTE_SIGNAL_Pin, GPIO_PIN_RESET);  /* un-mute the output */
			HAL_GPIO_WritePin(MUTE_INDICATOR_GPIO_Port, MUTE_INDICATOR_Pin, GPIO_PIN_RESET);  /* turn off the LED */
			VibeCheckStrobe_Unmute(&vc->strobe);
		}
		else
		{
			is_muted = 1;
			HAL_GPIO_WritePin(MUTE_SIGNAL_GPIO_Port, MUTE_SIGNAL_Pin, GPIO_PIN_SET);  /* mute the output */
			HAL_GPIO_WritePin(MUTE_INDICATOR_GPIO_Port, MUTE_INDICATOR_Pin, GPIO_PIN_SET);  /* turn on the LED */
			VibeCheckStrobe_Mute(&vc->strobe);
		}
	}

	if (time - time_prev_record_press > 250 && HAL_GPIO_ReadPin(RECORD_BUTTON_GPIO_Port, RECORD_BUTTON_Pin))
	{
		time_prev_record_press = time;
		record_pressed = 1;
	}
}
