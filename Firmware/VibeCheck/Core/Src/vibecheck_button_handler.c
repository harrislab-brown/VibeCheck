/*
 * vibecheck_button_handler.c
 *
 *  Created on: Oct 18, 2024
 *      Author: jantolik
 */

/*
 * commands to change the led
 *
 * >>record start
 *
 * >>record stop
 */

#include "vibecheck_button_handler.h"

uint32_t is_muted, mute_pressed, record_pressed;

uint32_t VibeCheckRecordCMD_Execute(void* obj, VibeCheckShell* shell)
{
	/* for host to set recording LED */
	char str[VC_SHELL_MAX_TOKEN_LEN];
	if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
	{
		if (!strcmp(str, "start"))
		{
			HAL_GPIO_WritePin(RECORD_INDICATOR_GPIO_Port, RECORD_INDICATOR_Pin, GPIO_PIN_SET); /* turn on the record LED */
			VibeCheckShell_PutOutputString(shell, "ack");
			VibeCheckShell_PutOutputDelimiter(shell);
			return 1;
		}
		else if (!strcmp(str, "stop"))
		{
			HAL_GPIO_WritePin(RECORD_INDICATOR_GPIO_Port, RECORD_INDICATOR_Pin, GPIO_PIN_RESET); /* turn off the record LED */
			VibeCheckShell_PutOutputString(shell, "ack");
			VibeCheckShell_PutOutputDelimiter(shell);
			return 1;
		}
	}

	return 0;
}

uint32_t VibeCheckMuteSender_Execute(void* obj, VibeCheckShell* shell)
{
	/* mute button */
	if (mute_pressed)
	{
		mute_pressed = 0;
		VibeCheckShell_PutOutputString(shell, "event");
		VibeCheckShell_PutOutputSeparator(shell);

		if (is_muted)
			VibeCheckShell_PutOutputString(shell, "muted");
		else
			VibeCheckShell_PutOutputString(shell, "unmuted");

		VibeCheckShell_PutOutputDelimiter(shell);

		return 1;
	}

	return 0;
}


uint32_t VibeCheckRecordSender_Execute(void* obj, VibeCheckShell* shell)
{
	/* record button */
	if (record_pressed)
	{
		record_pressed = 0;
		VibeCheckShell_PutOutputString(shell, "event");
		VibeCheckShell_PutOutputSeparator(shell);
		VibeCheckShell_PutOutputString(shell, "record");
		VibeCheckShell_PutOutputDelimiter(shell);
		return 1;
	}

	return 0;
}
