/*
 * vibecheck_sensor.c
 *
 *  Created on: Sep 20, 2024
 *      Author: johnt
 */

#include "vibecheck_sensor.h"


void VibeCheckSensor_Init(VibeCheckSensor* sensor, volatile uint32_t* time_micros, SPI_HandleTypeDef* hspi0, SPI_HandleTypeDef* hspi1, SPI_HandleTypeDef* hspi2)
{
	sensor->data_ind = 0;
	sensor->data_ready = 0;
	sensor->data_packet_size = VC_SENSOR_DEFAULT_PACKET_SIZE;
	sensor->time_prev_update = 0;
	sensor->generate_fake_data = 0;

	sensor->time_micros = time_micros;

	/* set all the configurations to defaults */
	for (uint32_t i = 0; i < VC_SENSOR_NUM_SENSORS; i++)
	{
		sensor->sensor_config[i].usr_offset_x = 0.0f;
		sensor->sensor_config[i].usr_offset_y = 0.0f;
		sensor->sensor_config[i].usr_offset_z = 0.0f;
		sensor->sensor_config[i].accel_odr_hz = VC_SENSOR_DEFAULT_ODR;
		sensor->sensor_config[i].g_range = VC_SENSOR_DEFAULT_G_RANGE;
		sensor->sensor_config[i].gyro_odr_hz = VC_SENSOR_DEFAULT_ODR;
		sensor->sensor_config[i].dps_range = VC_SENSOR_DEFAULT_DPS_RANGE;
	}


	/* initialize the sensor chips */
	SPI_HandleTypeDef* hspi[VC_SENSOR_NUM_SENSORS] = {hspi0, hspi1, hspi2};

	GPIO_TypeDef* cs_ports[VC_SENSOR_NUM_SENSORS] = {ACCEL_NCS1_GPIO_Port, ACCEL_NCS2_GPIO_Port, ACCEL_NCS3_GPIO_Port};
	uint16_t cs_pins[VC_SENSOR_NUM_SENSORS] = {ACCEL_NCS1_Pin, ACCEL_NCS2_Pin, ACCEL_NCS3_Pin};
	GPIO_TypeDef* int1_ports[VC_SENSOR_NUM_SENSORS] = {ACCEL_INTA1_GPIO_Port, ACCEL_INTA2_GPIO_Port, ACCEL_INTA3_GPIO_Port};
	uint16_t int1_pins[VC_SENSOR_NUM_SENSORS] = {ACCEL_INTA1_Pin, ACCEL_INTA2_Pin, ACCEL_INTA3_Pin};
	GPIO_TypeDef* int2_ports[VC_SENSOR_NUM_SENSORS] = {ACCEL_INTB1_GPIO_Port, ACCEL_INTB2_GPIO_Port, ACCEL_INTB3_GPIO_Port};
	uint16_t int2_pins[VC_SENSOR_NUM_SENSORS] = {ACCEL_INTB1_Pin, ACCEL_INTB2_Pin, ACCEL_INTB3_Pin};

	for (uint32_t i = 0; i < VC_SENSOR_NUM_SENSORS; i++)
	{
		LSM6DS3_Init(&sensor->sensor_array[i], &sensor->sensor_config[i],
				hspi[i], cs_ports[i], cs_pins[i], int1_ports[i], int1_pins[i], int2_ports[i], int2_pins[i]);
	}

}


void VibeCheckSensor_Update(VibeCheckSensor* sensor)
{
	uint32_t time = HAL_GetTick();
	if (time - sensor->time_prev_update > VC_SENSOR_UPDATE_INTERVAL_MS)
	{
		/* check if accelerometers have been plugged/unplugged */

		/* if not connected -> test for connection -> if success, set connected flag, configure the sensor, and send message to host */
		/* if connected but not running -> test for connection -> if failure, reset connected flag, send message to host */
		/* if running either accelerometer or gyro -> check data received status flag and reset it -> if no data received, reset connected flag, send message to host */

		for (uint32_t i = 0; i < VC_SENSOR_NUM_SENSORS; i++)
		{
			if (!sensor->status[i].is_connected)
			{
				if (LSM6DS3_TestCommunication(&sensor->sensor_array[i]))
				{
					sensor->status[i].is_connected = 1;
					sensor->status[i].connection_change_flag = 1;
					sensor->status[i].received_data_flag = 1;  /* give ourselves a grace period to receive data by artificially setting the flag */
					VibeCheckSensor_UpdateSensor(sensor, i);
				}
			}
			else
			{
				if (!sensor->status[i].accel_measuring && !sensor->status[i].gyro_measuring)
				{
					if (!LSM6DS3_TestCommunication(&sensor->sensor_array[i]))
					{
						sensor->status[i].is_connected = 0;
						sensor->status[i].connection_change_flag = 1;
					}
				}
				else
				{
					if (sensor->status[i].received_data_flag)
					{
						sensor->status[i].received_data_flag = 0;
					}
					else
					{
						sensor->status[i].is_connected = 0;
						sensor->status[i].connection_change_flag = 1;
					}
				}
			}
		}

		/* make some random data (3 sine wave phases) for testing the host plotting/data logging */
		if (sensor->generate_fake_data)
		{
			float val1 = sinf(2.0f * 3.14159f * time);
			float val2 = sinf(2.0f * 3.14159f * time + 3.14159f / 3.0f);
			float val3 = sinf(2.0f * 3.14159f * time + 2.0f * 3.14159f / 3.0f);

			VibeCheckSensor_AddData(sensor, 1, time, val1, val2, val3);
			VibeCheckSensor_AddData(sensor, 2, time, val3, val1, val2);
			VibeCheckSensor_AddData(sensor, 3, time, val2, val3, val1);
		}

		sensor->time_prev_update = time;
	}
}

void VibeCheckSensor_StartAccel(VibeCheckSensor* sensor, uint32_t channel)  /* start acceleration measurement of specified channel */
{
	if (channel >= VC_SENSOR_NUM_SENSORS) channel = VC_SENSOR_NUM_SENSORS - 1;

	if (sensor->status[channel].is_connected && !sensor->status[channel].accel_measuring)
		LSM6DS3_StartAccel(&sensor->sensor_array[channel]);

	/*
	 * I choose to set the flag here immediately, even if the accelerometer is not currently connected. This way, it can be set
	 * to start sampling immediately once the accelerometer is plugged in.
	 */

	sensor->status[channel].accel_measuring = 1;
}

void VibeCheckSensor_StopAccel(VibeCheckSensor* sensor, uint32_t channel)
{
	if (channel >= VC_SENSOR_NUM_SENSORS) channel = VC_SENSOR_NUM_SENSORS - 1;

	if (sensor->status[channel].is_connected && sensor->status[channel].accel_measuring)
		LSM6DS3_StopAccel(&sensor->sensor_array[channel]);

	sensor->status[channel].accel_measuring = 0;
}

void VibeCheckSensor_StartGyro(VibeCheckSensor* sensor, uint32_t channel)  /* start gyroscope measurement of specified channel */
{
	if (channel >= VC_SENSOR_NUM_SENSORS) channel = VC_SENSOR_NUM_SENSORS - 1;

	if (sensor->status[channel].is_connected && !sensor->status[channel].gyro_measuring)
		LSM6DS3_StartGyro(&sensor->sensor_array[channel]);

	sensor->status[channel].gyro_measuring = 1;
}

void VibeCheckSensor_StopGyro(VibeCheckSensor* sensor, uint32_t channel)
{
	if (channel >= VC_SENSOR_NUM_SENSORS) channel = VC_SENSOR_NUM_SENSORS - 1;

	if (sensor->status[channel].is_connected && sensor->status[channel].gyro_measuring)
		LSM6DS3_StopGyro(&sensor->sensor_array[channel]);

	sensor->status[channel].gyro_measuring = 0;
}

void VibeCheckSensor_SetAccelODR(VibeCheckSensor* sensor, uint32_t channel, uint32_t odr)
{
	if (channel >= VC_SENSOR_NUM_SENSORS) channel = VC_SENSOR_NUM_SENSORS - 1;

	sensor->sensor_config[channel].accel_odr_hz = FindClosest(VC_SENSOR_ALLOWED_ODR, sizeof(VC_SENSOR_ALLOWED_ODR) / sizeof(VC_SENSOR_ALLOWED_ODR[0]), odr);
	VibeCheckSensor_UpdateSensor(sensor, channel);
}

void VibeCheckSensor_SetGyroODR(VibeCheckSensor* sensor, uint32_t channel, uint32_t odr)
{
	if (channel >= VC_SENSOR_NUM_SENSORS) channel = VC_SENSOR_NUM_SENSORS - 1;

	sensor->sensor_config[channel].gyro_odr_hz = FindClosest(VC_SENSOR_ALLOWED_ODR, sizeof(VC_SENSOR_ALLOWED_ODR) / sizeof(VC_SENSOR_ALLOWED_ODR[0]), odr);
	VibeCheckSensor_UpdateSensor(sensor, channel);
}

void VibeCheckSensor_SetAccelRange(VibeCheckSensor* sensor, uint32_t channel, uint32_t range)
{
	if (channel >= VC_SENSOR_NUM_SENSORS) channel = VC_SENSOR_NUM_SENSORS - 1;

	sensor->sensor_config[channel].g_range = FindClosest(VC_SENSOR_ALLOWED_G_RANGE, sizeof(VC_SENSOR_ALLOWED_G_RANGE) / sizeof(VC_SENSOR_ALLOWED_G_RANGE[0]), range);
	VibeCheckSensor_UpdateSensor(sensor, channel);
}

void VibeCheckSensor_SetGyroRange(VibeCheckSensor* sensor, uint32_t channel, uint32_t range)
{
	if (channel >= VC_SENSOR_NUM_SENSORS) channel = VC_SENSOR_NUM_SENSORS - 1;

	sensor->sensor_config[channel].dps_range = FindClosest(VC_SENSOR_ALLOWED_DPS_RANGE, sizeof(VC_SENSOR_ALLOWED_DPS_RANGE) / sizeof(VC_SENSOR_ALLOWED_DPS_RANGE[0]), range);
	VibeCheckSensor_UpdateSensor(sensor, channel);
}

void VibeCheckSensor_SetOffsets(VibeCheckSensor* sensor, uint32_t channel, float x, float y, float z)  /* accelerometer DC offsets in g */
{
	if (channel >= VC_SENSOR_NUM_SENSORS) channel = VC_SENSOR_NUM_SENSORS - 1;

	/* TODO: test that setting the sensor user offset registers works */

	if (x > VC_SENSOR_MAX_OFFSET) x = VC_SENSOR_MAX_OFFSET;  /* clamp the offsets to the max value that can fit in the register */
	if (x < -VC_SENSOR_MAX_OFFSET) x = -VC_SENSOR_MAX_OFFSET;
	if (y > VC_SENSOR_MAX_OFFSET) y = VC_SENSOR_MAX_OFFSET;
	if (y < -VC_SENSOR_MAX_OFFSET) y = -VC_SENSOR_MAX_OFFSET;
	if (z > VC_SENSOR_MAX_OFFSET) z = VC_SENSOR_MAX_OFFSET;
	if (z < -VC_SENSOR_MAX_OFFSET) z = -VC_SENSOR_MAX_OFFSET;

	sensor->sensor_config[channel].usr_offset_x = x;
	sensor->sensor_config[channel].usr_offset_y = y;
	sensor->sensor_config[channel].usr_offset_z = z;

	VibeCheckSensor_UpdateSensor(sensor, channel);
}

uint32_t VibeCheckSensor_GetAccelODR(VibeCheckSensor* sensor, uint32_t channel)
{
	if (channel >= VC_SENSOR_NUM_SENSORS) channel = VC_SENSOR_NUM_SENSORS - 1;
	return sensor->sensor_config[channel].accel_odr_hz;
}

uint32_t VibeCheckSensor_GetGyroODR(VibeCheckSensor* sensor, uint32_t channel)
{
	if (channel >= VC_SENSOR_NUM_SENSORS) channel = VC_SENSOR_NUM_SENSORS - 1;
	return sensor->sensor_config[channel].gyro_odr_hz;
}

uint32_t VibeCheckSensor_GetAccelRange(VibeCheckSensor* sensor, uint32_t channel)
{
	if (channel >= VC_SENSOR_NUM_SENSORS) channel = VC_SENSOR_NUM_SENSORS - 1;
	return sensor->sensor_config[channel].g_range;
}

uint32_t VibeCheckSensor_GetGyroRange(VibeCheckSensor* sensor, uint32_t channel)
{
	if (channel >= VC_SENSOR_NUM_SENSORS) channel = VC_SENSOR_NUM_SENSORS - 1;
	return sensor->sensor_config[channel].dps_range;
}

void VibeCheckSensor_GetOffsets(VibeCheckSensor* sensor, uint32_t channel, float* x, float* y, float* z)
{
	if (channel >= VC_SENSOR_NUM_SENSORS) channel = VC_SENSOR_NUM_SENSORS - 1;
	*x = sensor->sensor_config[channel].usr_offset_x;
	*y = sensor->sensor_config[channel].usr_offset_y;
	*z = sensor->sensor_config[channel].usr_offset_z;
}


void VibeCheckSensor_StartFakeData(VibeCheckSensor* sensor)
{
	sensor->generate_fake_data = 1;
}


void VibeCheckSensor_StopFakeData(VibeCheckSensor* sensor)
{
	sensor->generate_fake_data = 0;
}


void VibeCheckSensor_SetPacketSize(VibeCheckSensor* sensor, uint32_t size)
{
	if (size < 1) size = 1;
	if (size > VC_SENSOR_MAX_PACKET_SIZE) size = VC_SENSOR_MAX_PACKET_SIZE;

	/* reset the state of the data buffer */
	sensor->data_packet_size = size;
	sensor->data_ready = 0;
	sensor->data_ind = 0;
}


uint32_t VibeCheckSensor_GetPacketSize(VibeCheckSensor* sensor)
{
	return sensor->data_packet_size;
}


void VibeCheckSensor_ResetTime(VibeCheckSensor* sensor)
{
	sensor->start_time = *sensor->time_micros;
}


uint32_t VibeCheckSensor_IsConnected(VibeCheckSensor* sensor, uint32_t channel)
{
	if (channel >= VC_SENSOR_NUM_SENSORS) channel = VC_SENSOR_NUM_SENSORS - 1;
	return sensor->status[channel].is_connected;
}


uint32_t VibeCheckSensor_ConnectionChanged(VibeCheckSensor* sensor, uint32_t* channel, uint32_t* is_connected)
{
	for (uint32_t i = 0; i < VC_SENSOR_NUM_SENSORS; i++)
	{
		if (sensor->status[i].connection_change_flag)
		{
			*channel = i;
			*is_connected = sensor->status[i].is_connected;
			return 1;
		}
	}

	return 0;
}


void VibeCheckSensor_ResetConnectionFlag(VibeCheckSensor* sensor, uint32_t channel)
{
	if (channel >= VC_SENSOR_NUM_SENSORS) channel = VC_SENSOR_NUM_SENSORS - 1;
	sensor->status[channel].connection_change_flag = 0;
}


void VibeCheckSensor_UpdateSensor(VibeCheckSensor* sensor, uint32_t channel)  /* send the new configuration parameters to a sensor chip */
{
	if (channel >= VC_SENSOR_NUM_SENSORS) channel = VC_SENSOR_NUM_SENSORS - 1;

	if (sensor->status[channel].is_connected)
	{
		LSM6DS3_Configure(&sensor->sensor_array[channel]);
		if (sensor->status[channel].accel_measuring)
			LSM6DS3_StartAccel(&sensor->sensor_array[channel]);
		if (sensor->status[channel].gyro_measuring)
			LSM6DS3_StartGyro(&sensor->sensor_array[channel]);
	}
}


void VibeCheckSensor_AddData(VibeCheckSensor* sensor, uint8_t id, uint32_t time, float x, float y, float z)
{
	sensor->data[sensor->data_ind].id = id;
	sensor->data[sensor->data_ind].time = time;
	sensor->data[sensor->data_ind].x = x;
	sensor->data[sensor->data_ind].y = y;
	sensor->data[sensor->data_ind].z = z;

	sensor->data_ind++;
	if (sensor->data_ind == sensor->data_packet_size)
	{
		sensor->data_ready = 1;
	}
	else if (sensor->data_ind == 2 * sensor->data_packet_size)
	{
		sensor->data_ind = 0;
		sensor->data_ready = 1;
	}
}


uint32_t VibeCheckSensor_GetDataReady(VibeCheckSensor* sensor, volatile VibeCheckSensor_Data** data)
{
	if (sensor->data_ready)
	{
		sensor->data_ready = 0;
		if (sensor->data_ind < sensor->data_packet_size)
		{
			/* ready to send the second half */
			*data = &sensor->data[sensor->data_packet_size];
		}
		else
		{
			/* ready to send the first half */
			*data = &sensor->data[0];
		}
		return 1;
	}
	return 0;
}


void VibeCheckSensor_EXTICallback(VibeCheckSensor* sensor, uint16_t GPIO_Pin)
{
	/*
	 * Read the sensor data when a signal occurs on a data ready pin. We assume that the INT1 pin indicates acceleration
	 * data ready and that the INT2 pin indicates gyroscope data ready.
	 */

	for (uint32_t i = 0; i < VC_SENSOR_NUM_SENSORS; i++)
	{
		if (GPIO_Pin == sensor->sensor_array[i].int1_pin && sensor->status[i].accel_measuring)
		{
			float x, y, z;
			LSM6DS3_ReadAccel(&sensor->sensor_array[i], &x, &y, &z);
			VibeCheckSensor_AddData(sensor, 2 * i, *sensor->time_micros - sensor->start_time, x, y, z);  /* time stamps are in microseconds */
			sensor->status[i].received_data_flag = 1;
			break;
		}

		if (GPIO_Pin == sensor->sensor_array[i].int2_pin && sensor->status[i].gyro_measuring)
		{
			float x, y, z;
			LSM6DS3_ReadGyro(&sensor->sensor_array[i], &x, &y, &z);
			VibeCheckSensor_AddData(sensor, 2 * i + 1, *sensor->time_micros - sensor->start_time, x, y, z);  /* time stamps are in microseconds */
			sensor->status[i].received_data_flag = 1;
			break;
		}
	}
}
