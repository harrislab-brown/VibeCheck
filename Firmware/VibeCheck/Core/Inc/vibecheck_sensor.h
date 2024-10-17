/*
 * vibecheck_sensor.h
 *
 *  Created on: Sep 20, 2024
 *      Author: johnt
 */

#ifndef INC_VIBECHECK_SENSOR_H_
#define INC_VIBECHECK_SENSOR_H_

#include <stdint.h>
#include <math.h>
#include "stm32h7xx_hal.h"
#include "LSM6DS3.h"
#include "util.h"


/* interfaces with and manages data from the sensor array */

#define VC_SENSOR_NUM_SENSORS 3
#define VC_SENSOR_DATA_PER_PACKET 100  /* TODO: have the user set the data per packet on the fly to account for different data rates */
#define VC_SENSOR_UPDATE_INTERVAL_MS 500

#define VC_SENSOR_DEFAULT_ODR 13
#define VC_SENSOR_DEFAULT_G_RANGE 2
#define VC_SENSOR_DEFAULT_DPS_RANGE 125

#define VC_SENSOR_MAX_OFFSET 0.1240234375f
static const uint32_t VC_SENSOR_ALLOWED_ODR[] = {13, 26, 52, 104, 208, 416, 833, 1660, 3330, 6660};
static const uint32_t VC_SENSOR_ALLOWED_G_RANGE[] = {2, 4, 8, 16};
static const uint32_t VC_SENSOR_ALLOWED_DPS_RANGE[] = {125, 245, 500, 1000, 2000};


typedef struct
{
	uint8_t id;
	uint32_t time;
	float x, y, z;
} VibeCheckSensor_Data;

typedef struct
{
	uint32_t is_connected;
	uint32_t accel_measuring;
	uint32_t gyro_measuring;
	volatile uint32_t received_data_flag;  /* flag set in interrupt when data received */
	uint32_t connection_change_flag;  /* flag set when the connection status was changed */
} VibeCheckSensor_Status;

typedef struct
{
	/* these will be zero-indexed which doesn't match the labels on the board - too bad */
	/*
	 * This code could perhaps be made cleaner by including the sensor status and
	 * configuration parameters directly in the LSM6DS3 struct.
	 */
	LSM6DS3_Config sensor_config[VC_SENSOR_NUM_SENSORS];
	LSM6DS3 sensor_array[VC_SENSOR_NUM_SENSORS];
	VibeCheckSensor_Status status[VC_SENSOR_NUM_SENSORS];

	/* combined sensor data buffer, and flags to handle double buffering */
	volatile VibeCheckSensor_Data data[2 * VC_SENSOR_DATA_PER_PACKET];
	volatile uint32_t data_ind;
	volatile uint32_t data_ready;

	uint32_t time_prev_update;
	uint32_t generate_fake_data;  /* for testing */

	volatile uint32_t* time_micros;  /* pointer to CNT register of free-running microsecond timer */
	uint32_t start_time;

} VibeCheckSensor;

void VibeCheckSensor_Init(VibeCheckSensor* sensor, volatile uint32_t* time_micros, SPI_HandleTypeDef* hspi0, SPI_HandleTypeDef* hspi1, SPI_HandleTypeDef* hspi2);
void VibeCheckSensor_Update(VibeCheckSensor* sensor);

/* interface functions */
void VibeCheckSensor_StartAccel(VibeCheckSensor* sensor, uint32_t channel);  /* start acceleration measurement of specified channel */
void VibeCheckSensor_StopAccel(VibeCheckSensor* sensor, uint32_t channel);

void VibeCheckSensor_StartGyro(VibeCheckSensor* sensor, uint32_t channel);  /* start gyroscope measurement of specified channel */
void VibeCheckSensor_StopGyro(VibeCheckSensor* sensor, uint32_t channel);

void VibeCheckSensor_SetAccelODR(VibeCheckSensor* sensor, uint32_t channel, uint32_t odr);
void VibeCheckSensor_SetGyroODR(VibeCheckSensor* sensor, uint32_t channel, uint32_t odr);
void VibeCheckSensor_SetAccelRange(VibeCheckSensor* sensor, uint32_t channel, uint32_t range);
void VibeCheckSensor_SetGyroRange(VibeCheckSensor* sensor, uint32_t channel, uint32_t range);
void VibeCheckSensor_SetOffsets(VibeCheckSensor* sensor, uint32_t channel, float x, float y, float z);  /* accelerometer DC offsets in g */

uint32_t VibeCheckSensor_GetAccelODR(VibeCheckSensor* sensor, uint32_t channel);
uint32_t VibeCheckSensor_GetGyroODR(VibeCheckSensor* sensor, uint32_t channel);
uint32_t VibeCheckSensor_GetAccelRange(VibeCheckSensor* sensor, uint32_t channel);
uint32_t VibeCheckSensor_GetGyroRange(VibeCheckSensor* sensor, uint32_t channel);
void VibeCheckSensor_GetOffsets(VibeCheckSensor* sensor, uint32_t channel, float* x, float* y, float* z);

void VibeCheckSensor_StartFakeData(VibeCheckSensor* sensor);  /* for testing the host UI */
void VibeCheckSensor_StopFakeData(VibeCheckSensor* sensor);

void VibeCheckSensor_ResetTime(VibeCheckSensor* sensor);

uint32_t VibeCheckSensor_IsConnected(VibeCheckSensor* sensor, uint32_t channel);
uint32_t VibeCheckSensor_ConnectionChanged(VibeCheckSensor* sensor, uint32_t* channel, uint32_t* is_connected);
void VibeCheckSensor_ResetConnectionFlag(VibeCheckSensor* sensor, uint32_t channel);

/* helper functions */
void VibeCheckSensor_UpdateSensor(VibeCheckSensor* sensor, uint32_t channel);  /* send the new configuration parameters to a sensor chip */

/* data handling */
void VibeCheckSensor_AddData(VibeCheckSensor* sensor, uint8_t id, uint32_t time, float x, float y, float z);
uint32_t VibeCheckSensor_GetDataReady(VibeCheckSensor* sensor, volatile VibeCheckSensor_Data** data);
void VibeCheckSensor_EXTICallback(VibeCheckSensor* sensor, uint16_t GPIO_Pin);  /* call when the accelerometer data ready signal occurs */


#endif /* INC_VIBECHECK_SENSOR_H_ */
