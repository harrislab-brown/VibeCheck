/*
 * vibecheck_accel.h
 *
 *  Created on: Sep 20, 2024
 *      Author: johnt
 */

#ifndef INC_VIBECHECK_ACCEL_H_
#define INC_VIBECHECK_ACCEL_H_

#include <stdint.h>
#include <math.h>
#include "stm32h7xx_hal.h"


/* interfaces with and manages data from the accelerometers */

#define VC_ACCEL_DATA_PER_PACKET 10
#define VC_ACCEL_UPDATE_INTERVAL_MS 10

typedef struct
{
	uint8_t id;
	uint32_t time;
	float x, y, z;
} VibeCheckAccel_Data;

typedef struct
{
	VibeCheckAccel_Data data[2 * VC_ACCEL_DATA_PER_PACKET];
	uint32_t data_ind;
	uint32_t data_ready;

	uint32_t time_prev_update;
	uint32_t generate_fake_data;
} VibeCheckAccel;

void VibeCheckAccel_Init(VibeCheckAccel* accel);
void VibeCheckAccel_Update(VibeCheckAccel* accel);

void VibeCheckAccel_AddData(VibeCheckAccel* accel, uint8_t id, uint32_t time, float x, float y, float z);

void VibeCheckAccel_StartFakeData(VibeCheckAccel* accel);
void VibeCheckAccel_StopFakeData(VibeCheckAccel* accel);

uint32_t VibeCheckAccel_GetDataReady(VibeCheckAccel* accel, VibeCheckAccel_Data** data);


#endif /* INC_VIBECHECK_ACCEL_H_ */
