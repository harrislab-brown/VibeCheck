/*
 * vibecheck_accel.c
 *
 *  Created on: Sep 20, 2024
 *      Author: johnt
 */

#include "vibecheck_accel.h"


void VibeCheckAccel_Init(VibeCheckAccel* accel, SPI_HandleTypeDef* hspi0, SPI_HandleTypeDef* hspi1, SPI_HandleTypeDef* hspi2)
{
	accel->data_ind = 0;
	accel->data_ready = 0;
	accel->time_prev_update = 0;
	accel->generate_fake_data = 0;

	/* set all the configurations to defaults */
	for (uint32_t i = 0; i < VC_ACCEL_NUM_SENSORS; i++)
	{
		accel->sensor_config[i].usr_offset_x = 0.0f;
		accel->sensor_config[i].usr_offset_y = 0.0f;
		accel->sensor_config[i].usr_offset_z = 0.0f;
		accel->sensor_config[i].accel_odr_hz = VC_ACCEL_DEFAULT_ODR;
		accel->sensor_config[i].g_range = VC_ACCEL_DEFAULT_G_RANGE;
		accel->sensor_config[i].gyro_odr_hz = VC_ACCEL_DEFAULT_ODR;
		accel->sensor_config[i].dps_range = VC_ACCEL_DEFAULT_DPS_RANGE;
	}


	/* initialize the sensor chips */
	SPI_HandleTypeDef* hspi[VC_ACCEL_NUM_SENSORS] = {hspi0, hspi1, hspi2};

	GPIO_TypeDef* cs_ports[VC_ACCEL_NUM_SENSORS] = {ACCEL_NCS1_GPIO_Port, ACCEL_NCS2_GPIO_Port, ACCEL_NCS3_GPIO_Port};
	uint16_t cs_pins[VC_ACCEL_NUM_SENSORS] = {ACCEL_NCS1_Pin, ACCEL_NCS2_Pin, ACCEL_NCS3_Pin};
	GPIO_TypeDef* int1_ports[VC_ACCEL_NUM_SENSORS] = {ACCEL_INTA1_GPIO_Port, ACCEL_INTA2_GPIO_Port, ACCEL_INTA3_GPIO_Port};
	uint16_t int1_pins[VC_ACCEL_NUM_SENSORS] = {ACCEL_INTA1_Pin, ACCEL_INTA2_Pin, ACCEL_INTA3_Pin};
	GPIO_TypeDef* int2_ports[VC_ACCEL_NUM_SENSORS] = {ACCEL_INTB1_GPIO_Port, ACCEL_INTB2_GPIO_Port, ACCEL_INTB3_GPIO_Port};
	uint16_t int2_pins[VC_ACCEL_NUM_SENSORS] = {ACCEL_INTB1_Pin, ACCEL_INTB2_Pin, ACCEL_INTB3_Pin};

	for (uint32_t i = 0; i < VC_ACCEL_NUM_SENSORS; i++)
	{
		LSM6DS3_Init(&accel->sensor[i], &accel->sensor_config[i],
				hspi[i], cs_ports[i], cs_pins[i], int1_ports[i], int1_pins[i], int2_ports[i], int2_pins[i]);
	}

}


void VibeCheckAccel_Update(VibeCheckAccel* accel)
{
	uint32_t time = HAL_GetTick();
	if (time - accel->time_prev_update > VC_ACCEL_UPDATE_INTERVAL_MS)
	{
		/* check if accelerometers have been plugged/unplugged */

		/* if not connected -> test for connection -> if success, set connected flag, configure the sensor, and send message to host */
		/* if connected but not running -> test for connection -> if failure, reset connected flag, send message to host */
		/* if running either accelerometer or gyro -> check data received status flag and reset it -> if no data received, reset connected flag, send message to host */

		for (uint32_t i = 0; i < VC_ACCEL_NUM_SENSORS; i++)
		{
			if (!accel->status[i].is_connected)
			{
				if (LSM6DS3_TestCommunication(&accel->sensor[i]))
				{
					accel->status[i].is_connected = 1;
					accel->status[i].connection_change_flag = 1;
					VibeCheckAccel_UpdateSensor(accel, i);
				}
			}
			else
			{
				if (!accel->status[i].accel_measuring && !accel->status[i].gyro_measuring)
				{
					if (!LSM6DS3_TestCommunication(&accel->sensor[i]))
					{
						accel->status[i].is_connected = 0;
						accel->status[i].connection_change_flag = 1;
					}
				}
				else
				{
					if (accel->status[i].received_data_flag)
					{
						accel->status[i].received_data_flag = 0;
					}
					else
					{
						accel->status[i].is_connected = 0;
						accel->status[i].connection_change_flag = 1;
					}
				}
			}
		}

		/* make some random data (3 sine wave phases) for testing the host plotting/data logging */
		if (accel->generate_fake_data)
		{
			float val1 = sinf(2.0f * 3.14159f * time);
			float val2 = sinf(2.0f * 3.14159f * time + 3.14159f / 3.0f);
			float val3 = sinf(2.0f * 3.14159f * time + 2.0f * 3.14159f / 3.0f);

			VibeCheckAccel_AddData(accel, 1, time, val1, val2, val3);
			VibeCheckAccel_AddData(accel, 2, time, val3, val1, val2);
			VibeCheckAccel_AddData(accel, 3, time, val2, val3, val1);
		}

		accel->time_prev_update = time;
	}
}

void VibeCheckAccel_StartAccel(VibeCheckAccel* accel, uint32_t channel)  /* start acceleration measurement of specified channel */
{
	if (channel >= VC_ACCEL_NUM_SENSORS) channel = VC_ACCEL_NUM_SENSORS - 1;

	if (accel->status[channel].is_connected && !accel->status[channel].accel_measuring)
		LSM6DS3_StartAccel(&accel->sensor[channel]);

	/*
	 * I choose to set the flag here immediately, even if the accelerometer is not currently connected. This way, it can be set
	 * to start sampling immediately once the accelerometer is plugged in.
	 */

	accel->status[channel].accel_measuring = 1;
}

void VibeCheckAccel_StopAccel(VibeCheckAccel* accel, uint32_t channel)
{
	if (channel >= VC_ACCEL_NUM_SENSORS) channel = VC_ACCEL_NUM_SENSORS - 1;

	if (accel->status[channel].is_connected && accel->status[channel].accel_measuring)
		LSM6DS3_StopAccel(&accel->sensor[channel]);

	accel->status[channel].accel_measuring = 0;
}

void VibeCheckAccel_StartGyro(VibeCheckAccel* accel, uint32_t channel)  /* start gyroscope measurement of specified channel */
{
	if (channel >= VC_ACCEL_NUM_SENSORS) channel = VC_ACCEL_NUM_SENSORS - 1;

	if (accel->status[channel].is_connected && !accel->status[channel].gyro_measuring)
		LSM6DS3_StartGyro(&accel->sensor[channel]);

	accel->status[channel].gyro_measuring = 1;
}

void VibeCheckAccel_StopGyro(VibeCheckAccel* accel, uint32_t channel)
{
	if (channel >= VC_ACCEL_NUM_SENSORS) channel = VC_ACCEL_NUM_SENSORS - 1;

	if (accel->status[channel].is_connected && accel->status[channel].gyro_measuring)
		LSM6DS3_StopGyro(&accel->sensor[channel]);

	accel->status[channel].gyro_measuring = 0;
}

void VibeCheckAccel_SetAccelODR(VibeCheckAccel* accel, uint32_t channel, uint32_t odr)
{
	if (channel >= VC_ACCEL_NUM_SENSORS) channel = VC_ACCEL_NUM_SENSORS - 1;

	accel->sensor_config[channel].accel_odr_hz = FindClosest(VC_ACCEL_ALLOWED_ODR, sizeof(VC_ACCEL_ALLOWED_ODR) / sizeof(VC_ACCEL_ALLOWED_ODR[0]), odr);
	VibeCheckAccel_UpdateSensor(accel, channel);
}

void VibeCheckAccel_SetGyroODR(VibeCheckAccel* accel, uint32_t channel, uint32_t odr)
{
	if (channel >= VC_ACCEL_NUM_SENSORS) channel = VC_ACCEL_NUM_SENSORS - 1;

	accel->sensor_config[channel].gyro_odr_hz = FindClosest(VC_ACCEL_ALLOWED_ODR, sizeof(VC_ACCEL_ALLOWED_ODR) / sizeof(VC_ACCEL_ALLOWED_ODR[0]), odr);
	VibeCheckAccel_UpdateSensor(accel, channel);
}

void VibeCheckAccel_SetAccelRange(VibeCheckAccel* accel, uint32_t channel, uint32_t range)
{
	if (channel >= VC_ACCEL_NUM_SENSORS) channel = VC_ACCEL_NUM_SENSORS - 1;

	accel->sensor_config[channel].g_range = FindClosest(VC_ACCEL_ALLOWED_G_RANGE, sizeof(VC_ACCEL_ALLOWED_G_RANGE) / sizeof(VC_ACCEL_ALLOWED_G_RANGE[0]), range);
	VibeCheckAccel_UpdateSensor(accel, channel);
}

void VibeCheckAccel_SetGyroRange(VibeCheckAccel* accel, uint32_t channel, uint32_t range)
{
	if (channel >= VC_ACCEL_NUM_SENSORS) channel = VC_ACCEL_NUM_SENSORS - 1;

	accel->sensor_config[channel].dps_range = FindClosest(VC_ACCEL_ALLOWED_DPS_RANGE, sizeof(VC_ACCEL_ALLOWED_DPS_RANGE) / sizeof(VC_ACCEL_ALLOWED_DPS_RANGE[0]), range);
	VibeCheckAccel_UpdateSensor(accel, channel);
}

void VibeCheckAccel_SetOffsets(VibeCheckAccel* accel, uint32_t channel, float x, float y, float z)  /* accelerometer DC offsets in g */
{
	if (channel >= VC_ACCEL_NUM_SENSORS) channel = VC_ACCEL_NUM_SENSORS - 1;

	if (x > VC_ACCEL_MAX_OFFSET) x = VC_ACCEL_MAX_OFFSET;  /* clamp the offsets to the max value that can fit in the register */
	if (x < -VC_ACCEL_MAX_OFFSET) x = -VC_ACCEL_MAX_OFFSET;
	if (y > VC_ACCEL_MAX_OFFSET) y = VC_ACCEL_MAX_OFFSET;
	if (y < -VC_ACCEL_MAX_OFFSET) y = -VC_ACCEL_MAX_OFFSET;
	if (z > VC_ACCEL_MAX_OFFSET) z = VC_ACCEL_MAX_OFFSET;
	if (z < -VC_ACCEL_MAX_OFFSET) z = -VC_ACCEL_MAX_OFFSET;

	accel->sensor_config[channel].usr_offset_x = x;
	accel->sensor_config[channel].usr_offset_y = y;
	accel->sensor_config[channel].usr_offset_z = z;

	VibeCheckAccel_UpdateSensor(accel, channel);
}

uint32_t VibeCheckAccel_GetAccelODR(VibeCheckAccel* accel, uint32_t channel)
{
	if (channel >= VC_ACCEL_NUM_SENSORS) channel = VC_ACCEL_NUM_SENSORS - 1;
	return accel->sensor_config[channel].accel_odr_hz;
}

uint32_t VibeCheckAccel_GetGyroODR(VibeCheckAccel* accel, uint32_t channel)
{
	if (channel >= VC_ACCEL_NUM_SENSORS) channel = VC_ACCEL_NUM_SENSORS - 1;
	return accel->sensor_config[channel].gyro_odr_hz;
}

uint32_t VibeCheckAccel_GetAccelRange(VibeCheckAccel* accel, uint32_t channel)
{
	if (channel >= VC_ACCEL_NUM_SENSORS) channel = VC_ACCEL_NUM_SENSORS - 1;
	return accel->sensor_config[channel].g_range;
}

uint32_t VibeCheckAccel_GetGyroRange(VibeCheckAccel* accel, uint32_t channel)
{
	if (channel >= VC_ACCEL_NUM_SENSORS) channel = VC_ACCEL_NUM_SENSORS - 1;
	return accel->sensor_config[channel].dps_range;
}

void VibeCheckAccel_GetOffsets(VibeCheckAccel* accel, uint32_t channel, float* x, float* y, float* z)
{
	if (channel >= VC_ACCEL_NUM_SENSORS) channel = VC_ACCEL_NUM_SENSORS - 1;
	*x = accel->sensor_config[channel].usr_offset_x;
	*y = accel->sensor_config[channel].usr_offset_y;
	*z = accel->sensor_config[channel].usr_offset_z;
}


void VibeCheckAccel_StartFakeData(VibeCheckAccel* accel)
{
	accel->generate_fake_data = 1;
}


void VibeCheckAccel_StopFakeData(VibeCheckAccel* accel)
{
	accel->generate_fake_data = 0;
}


uint32_t VibeCheckAccel_ConnectionChanged(VibeCheckAccel* accel, uint32_t* channel, uint32_t* is_connected)
{
	for (uint32_t i = 0; i < VC_ACCEL_NUM_SENSORS; i++)
	{
		if (accel->status[i].connection_change_flag)
		{
			accel->status[i].connection_change_flag = 0;
			*channel = i;
			*is_connected = accel->status[i].is_connected;
			return 1;
		}
	}

	return 0;
}


void VibeCheckAccel_UpdateSensor(VibeCheckAccel* accel, uint32_t channel)  /* send the new configuration parameters to a sensor chip */
{
	if (channel >= VC_ACCEL_NUM_SENSORS) channel = VC_ACCEL_NUM_SENSORS - 1;

	if (accel->status[channel].is_connected)
	{
		LSM6DS3_Configure(&accel->sensor[channel]);
		if (accel->status[channel].accel_measuring)
			LSM6DS3_StartAccel(&accel->sensor[channel]);
		if (accel->status[channel].gyro_measuring)
			LSM6DS3_StartGyro(&accel->sensor[channel]);
	}
}


void VibeCheckAccel_AddData(VibeCheckAccel* accel, uint8_t id, uint32_t time, float x, float y, float z)
{
	accel->data[accel->data_ind].id = id;
	accel->data[accel->data_ind].time = time;
	accel->data[accel->data_ind].x = x;
	accel->data[accel->data_ind].y = y;
	accel->data[accel->data_ind].z = z;

	accel->data_ind++;
	if (accel->data_ind == VC_ACCEL_DATA_PER_PACKET)
	{
		accel->data_ready = 1;
	}
	else if (accel->data_ind == 2 * VC_ACCEL_DATA_PER_PACKET)
	{
		accel->data_ind = 0;
		accel->data_ready = 1;
	}
}


uint32_t VibeCheckAccel_GetDataReady(VibeCheckAccel* accel, VibeCheckAccel_Data** data)
{
	if (accel->data_ready)
	{
		accel->data_ready = 0;
		if (accel->data_ind < VC_ACCEL_DATA_PER_PACKET)
		{
			/* ready to send the second half */
			*data = &accel->data[VC_ACCEL_DATA_PER_PACKET];
		}
		else
		{
			/* ready to send the first half */
			*data = &accel->data[0];
		}
		return 1;
	}
	return 0;
}


void VibeCheckAccel_EXTICallback(VibeCheckAccel* accel, uint16_t GPIO_Pin)
{
	/*
	 * Read the sensor data when a signal occurs on a data ready pin. We assume that the INT1 pin indicates acceleration
	 * data ready and that the INT2 pin indicates gyroscope data ready.
	 */

	for (uint32_t i = 0; i < VC_ACCEL_NUM_SENSORS; i++)
	{
		if (GPIO_Pin == accel->sensor[i].int1_pin && accel->status[i].accel_measuring)
		{
			float x, y, z;
			LSM6DS3_ReadAccel(&accel->sensor[i], &x, &y, &z);
			VibeCheckAccel_AddData(accel, 2 * i, 0, x, y, z);  /* TODO: add time stamps */
			accel->status[i].received_data_flag = 1;
			break;
		}

		if (GPIO_Pin == accel->sensor[i].int2_pin && accel->status[i].gyro_measuring)
		{
			float x, y, z;
			LSM6DS3_ReadGyro(&accel->sensor[i], &x, &y, &z);
			VibeCheckAccel_AddData(accel, 2 * i + 1, 0, x, y, z);  /* TODO: add time stamps */
			accel->status[i].received_data_flag = 1;
			break;
		}
	}
}
