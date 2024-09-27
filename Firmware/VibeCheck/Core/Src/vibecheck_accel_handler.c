/*
 * vibecheck_accel_handler.c
 *
 *  Created on: Sep 20, 2024
 *      Author: johnt
 */

#include "vibecheck_accel_handler.h"

uint32_t VibeCheckAccelCMD_Execute(void* obj, VibeCheckShell* shell)
{
	VibeCheckAccel* accel = (VibeCheckAccel*)obj;

	char str[VC_SHELL_MAX_TOKEN_LEN];
	if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
	{
		if (!strcmp(str, "fakedata"))
		{
			if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
			{
				if (!strcmp(str, "start"))
				{
					VibeCheckAccel_StartFakeData(accel);
					VibeCheckShell_PutOutputString(shell, "ack");
					VibeCheckShell_PutOutputDelimiter(shell);
					return 1;
				}
				else if (!strcmp(str, "stop"))
				{
					VibeCheckAccel_StopFakeData(accel);
					VibeCheckShell_PutOutputString(shell, "ack");
					VibeCheckShell_PutOutputDelimiter(shell);
					return 1;
				}
			}
		}
	}

	return 0;
}


uint32_t VibeCheckAccelSender_Execute(void* obj, VibeCheckShell* shell)
{
	VibeCheckAccel* accel = (VibeCheckAccel*)obj;

	VibeCheckAccel_Data* data;
	if (VibeCheckAccel_GetDataReady(accel, &data))
	{
		VibeCheckShell_PutOutputString(shell, "data");
		VibeCheckShell_PutOutputSeparator(shell);
		VibeCheckShell_PutOutputInt(shell, VC_ACCEL_DATA_PER_PACKET);

		uint32_t data_written = 0;
		while (data_written < VC_ACCEL_DATA_PER_PACKET)
		{
			VibeCheckShell_PutOutputSeparator(shell);
			VibeCheckShell_PutOutputInt(shell, data->id);
			VibeCheckShell_PutOutputSeparator(shell);
			VibeCheckShell_PutOutputInt(shell, data->time);
			VibeCheckShell_PutOutputSeparator(shell);
			VibeCheckShell_PutOutputFloat(shell, data->x);
			VibeCheckShell_PutOutputSeparator(shell);
			VibeCheckShell_PutOutputFloat(shell, data->y);
			VibeCheckShell_PutOutputSeparator(shell);
			VibeCheckShell_PutOutputFloat(shell, data->z);

			data++;
			data_written++;
		}

		VibeCheckShell_PutOutputDelimiter(shell);

		return 1;
	}
	else
	{
		return 0;
	}
}


