/*
 * vibecheck_accel.c
 *
 *  Created on: Sep 20, 2024
 *      Author: johnt
 */

#include "vibecheck_accel.h"

void VibeCheckAccel_Init(VibeCheckAccel* accel)
{
	accel->data_ind = 0;
	accel->data_ready = 0;
	accel->time_prev_update = 0;
	accel->generate_fake_data = 0;
}

void VibeCheckAccel_Update(VibeCheckAccel* accel)
{
	uint32_t time = HAL_GetTick();
	if (time - accel->time_prev_update > VC_ACCEL_UPDATE_INTERVAL_MS)
	{
		/* TODO: check if accelerometers have been plugged/unplugged */

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

void VibeCheckAccel_StartFakeData(VibeCheckAccel* accel)
{
	accel->generate_fake_data = 1;
}

void VibeCheckAccel_StopFakeData(VibeCheckAccel* accel)
{
	accel->generate_fake_data = 0;
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
