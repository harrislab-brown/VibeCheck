/*
 * util.h
 *
 *  Created on: Oct 7, 2024
 *      Author: johnt
 */

#ifndef INC_UTIL_H_
#define INC_UTIL_H_

#include <stdint.h>
#include <stdlib.h>

/* find the closest value to the target in an array */
uint32_t FindClosest(const uint32_t* arr, uint32_t len, uint32_t target);

#endif /* INC_UTIL_H_ */
