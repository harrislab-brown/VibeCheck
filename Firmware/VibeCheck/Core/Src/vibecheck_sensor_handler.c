/*
 * vibecheck_sensor_handler.c
 *
 *  Created on: Sep 20, 2024
 *      Author: johnt
 */


/*

Set of commands:

>>sensor fakedata start

>>sensor fakedata stop

>>sensor [channel] start accel

>>sensor [channel] stop accel

>>sensor [channel] start gyro

>>sensor [channel] stop gyro

>>sensor [channel] set accel odr [value]

>>sensor [channel] get accel odr

>>sensor [channel] set accel range [value]

>>sensor [channel] get accel range

>>sensor [channel] set gyro odr [value]

>>sensor [channel] get gyro odr

>>sensor [channel] set gyro range [value]

>>sensor [channel] get gyro range

>>sensor [channel] set offsets [x] [y] [z]

>>sensor [channel] get offsets

*/


#include "vibecheck_sensor_handler.h"


static uint32_t VibeCheckSensorCMD_Set(VibeCheckSensor* sensor, VibeCheckShell* shell, uint32_t channel)
{
	char str[VC_SHELL_MAX_TOKEN_LEN];
	if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
	{
		if (!strcmp(str, "accel"))
		{
			if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
			{
				if (!strcmp(str, "odr"))
				{
					int32_t odr;
					if (VibeCheckShell_GetNextInt(shell, &odr))
					{
						VibeCheckSensor_SetAccelODR(sensor, channel, odr);
						VibeCheckShell_PutOutputString(shell, "ack");
						VibeCheckShell_PutOutputDelimiter(shell);
						return 1;
					}
				}
				else if (!strcmp(str, "range"))
				{
					int32_t range;
					if (VibeCheckShell_GetNextInt(shell, &range))
					{
						VibeCheckSensor_SetAccelRange(sensor, channel, range);
						VibeCheckShell_PutOutputString(shell, "ack");
						VibeCheckShell_PutOutputDelimiter(shell);
						return 1;
					}
				}
			}
		}
		else if (!strcmp(str, "gyro"))
		{
			if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
			{
				if (!strcmp(str, "odr"))
				{
					int32_t odr;
					if (VibeCheckShell_GetNextInt(shell, &odr))
					{
						VibeCheckSensor_SetGyroODR(sensor, channel, odr);
						VibeCheckShell_PutOutputString(shell, "ack");
						VibeCheckShell_PutOutputDelimiter(shell);
						return 1;
					}
				}
				else if (!strcmp(str, "range"))
				{
					int32_t range;
					if (VibeCheckShell_GetNextInt(shell, &range))
					{
						VibeCheckSensor_SetGyroRange(sensor, channel, range);
						VibeCheckShell_PutOutputString(shell, "ack");
						VibeCheckShell_PutOutputDelimiter(shell);
						return 1;
					}
				}
			}
		}
		else if (!strcmp(str, "offsets"))
		{
			float x, y, z;
			if (VibeCheckShell_GetNextFloat(shell, &x)
					&& VibeCheckShell_GetNextFloat(shell, &y)
					&& VibeCheckShell_GetNextFloat(shell, &z))
			{
				VibeCheckSensor_SetOffsets(sensor, channel, x, y, z);
				VibeCheckShell_PutOutputString(shell, "ack");
				VibeCheckShell_PutOutputDelimiter(shell);
				return 1;
			}
		}
	}


	return 0;
}


static uint32_t VibeCheckSensorCMD_Get(VibeCheckSensor* sensor, VibeCheckShell* shell, uint32_t channel)
{
	char str[VC_SHELL_MAX_TOKEN_LEN];
	if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
	{
		if (!strcmp(str, "accel"))
		{
			if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
			{
				if (!strcmp(str, "odr"))
				{
					VibeCheckShell_PutOutputString(shell, "ack");
					VibeCheckShell_PutOutputSeparator(shell);
					VibeCheckShell_PutOutputInt(shell, VibeCheckSensor_GetAccelODR(sensor, channel));
					VibeCheckShell_PutOutputDelimiter(shell);
					return 1;
				}
				else if (!strcmp(str, "range"))
				{
					VibeCheckShell_PutOutputString(shell, "ack");
					VibeCheckShell_PutOutputSeparator(shell);
					VibeCheckShell_PutOutputInt(shell, VibeCheckSensor_GetAccelRange(sensor, channel));
					VibeCheckShell_PutOutputDelimiter(shell);
					return 1;
				}
			}
		}
		else if (!strcmp(str, "gyro"))
		{
			if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
			{
				if (!strcmp(str, "odr"))
				{
					VibeCheckShell_PutOutputString(shell, "ack");
					VibeCheckShell_PutOutputSeparator(shell);
					VibeCheckShell_PutOutputInt(shell, VibeCheckSensor_GetGyroODR(sensor, channel));
					VibeCheckShell_PutOutputDelimiter(shell);
					return 1;
				}
				else if (!strcmp(str, "range"))
				{
					VibeCheckShell_PutOutputString(shell, "ack");
					VibeCheckShell_PutOutputSeparator(shell);
					VibeCheckShell_PutOutputInt(shell, VibeCheckSensor_GetGyroRange(sensor, channel));
					VibeCheckShell_PutOutputDelimiter(shell);
					return 1;
				}
			}
		}
		else if (!strcmp(str, "offsets"))
		{
			float x, y, z;
			VibeCheckSensor_GetOffsets(sensor, channel, &x, &y, &z);

			VibeCheckShell_PutOutputString(shell, "ack");
			VibeCheckShell_PutOutputSeparator(shell);
			VibeCheckShell_PutOutputFloat(shell, x);
			VibeCheckShell_PutOutputSeparator(shell);
			VibeCheckShell_PutOutputFloat(shell, y);
			VibeCheckShell_PutOutputSeparator(shell);
			VibeCheckShell_PutOutputFloat(shell, z);
			VibeCheckShell_PutOutputDelimiter(shell);
			return 1;
		}
	}

	return 0;
}


uint32_t VibeCheckSensorCMD_Execute(void* obj, VibeCheckShell* shell)
{
	VibeCheckSensor* sensor = (VibeCheckSensor*)obj;

	char str[VC_SHELL_MAX_TOKEN_LEN];
	int32_t channel;
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
		else if (VibeCheckShell_TurnToInt(str, &channel))
		{
			if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
			{
				if (!strcmp(str, "start"))
				{
					if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
					{
						if (!strcmp(str, "accel"))
						{
							VibeCheckSensor_StartAccel(sensor, channel);
							VibeCheckShell_PutOutputString(shell, "ack");
							VibeCheckShell_PutOutputDelimiter(shell);
							return 1;
						}
						else if (!strcmp(str, "gyro"))
						{
							VibeCheckSensor_StartGyro(sensor, channel);
							VibeCheckShell_PutOutputString(shell, "ack");
							VibeCheckShell_PutOutputDelimiter(shell);
							return 1;
						}
					}
				}
				else if (!strcmp(str, "stop"))
				{
					if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
					{
						if (!strcmp(str, "accel"))
						{
							VibeCheckSensor_StopAccel(sensor, channel);
							VibeCheckShell_PutOutputString(shell, "ack");
							VibeCheckShell_PutOutputDelimiter(shell);
							return 1;
						}
						else if (!strcmp(str, "gyro"))
						{
							VibeCheckSensor_StopGyro(sensor, channel);
							VibeCheckShell_PutOutputString(shell, "ack");
							VibeCheckShell_PutOutputDelimiter(shell);
							return 1;
						}
					}
				}
				else if (!strcmp(str, "set"))
				{
					if (VibeCheckSensorCMD_Set(sensor, shell, channel))
					{
						return 1;
					}
				}
				else if (!strcmp(str, "get"))
				{
					if (VibeCheckSensorCMD_Get(sensor, shell, channel))
					{
						return 1;
					}
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


