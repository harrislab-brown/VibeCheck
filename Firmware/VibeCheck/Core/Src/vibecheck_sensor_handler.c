/*
 * vibecheck_sensor_handler.c
 *
 *  Created on: Sep 20, 2024
 *      Author: johnt
 */

#include "vibecheck_sensor_handler.h"

uint32_t VibeCheckSensorCMD_Execute(void* obj, VibeCheckShell* shell)
{
	VibeCheckSensor* sensor = (VibeCheckSensor*)obj;

	char str[VC_SHELL_MAX_TOKEN_LEN];
	if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
	{
		if (!strcmp(str, "fakedata"))
		{
			if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
			{
				if (!strcmp(str, "start"))
				{
					VibeCheckSensor_StartFakeData(sensor);
					VibeCheckShell_PutOutputString(shell, "ack");
					VibeCheckShell_PutOutputDelimiter(shell);
					return 1;
				}
				else if (!strcmp(str, "stop"))
				{
					VibeCheckSensor_StopFakeData(sensor);
					VibeCheckShell_PutOutputString(shell, "ack");
					VibeCheckShell_PutOutputDelimiter(shell);
					return 1;
				}
			}
		}
	}

	return 0;
}


uint32_t VibeCheckSensorSender_Data_Execute(void* obj, VibeCheckShell* shell)
{
	VibeCheckSensor* sensor = (VibeCheckSensor*)obj;

	VibeCheckSensor_Data* data;
	if (VibeCheckSensor_GetDataReady(sensor, &data))
	{
		VibeCheckShell_PutOutputString(shell, "data");
		VibeCheckShell_PutOutputSeparator(shell);
		VibeCheckShell_PutOutputInt(shell, VC_SENSOR_DATA_PER_PACKET);

		uint32_t data_written = 0;
		while (data_written < VC_SENSOR_DATA_PER_PACKET)
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

	return 0;

}


uint32_t VibeCheckSensorSender_Status_Execute(void* obj, VibeCheckShell* shell)
{
	VibeCheckSensor* sensor = (VibeCheckSensor*)obj;

	uint32_t channel, is_connected;
	if (VibeCheckSensor_ConnectionChanged(sensor, &channel, &is_connected))
	{
		VibeCheckShell_PutOutputString(shell, "msg");
		VibeCheckShell_PutOutputSeparator(shell);
		VibeCheckShell_PutOutputString(shell, "sensor");
		VibeCheckShell_PutOutputSeparator(shell);
		VibeCheckShell_PutOutputInt(shell, channel);
		VibeCheckShell_PutOutputSeparator(shell);

		if (is_connected)
			VibeCheckShell_PutOutputString(shell, "connected");
		else
			VibeCheckShell_PutOutputString(shell, "disconnected");

		VibeCheckShell_PutOutputDelimiter(shell);
		return 1;
	}

	return 0;
}


