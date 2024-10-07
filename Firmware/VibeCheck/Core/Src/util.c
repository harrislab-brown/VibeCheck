/*
 * util.c
 *
 *  Created on: Oct 7, 2024
 *      Author: johnt
 */

#include "util.h"

uint32_t FindClosest(const uint32_t* arr, uint32_t len, uint32_t target)
{
	uint32_t min_diff = 0xFFFFFFFF;
	uint32_t closest_value = arr[0];

	for (uint32_t i = 0; i < len; i++)
	{
		uint32_t diff = abs(arr[i] - target);

		if (diff < min_diff)
		{
			min_diff = diff;
			closest_value = arr[i];
		}
	}

	return closest_value;
}
