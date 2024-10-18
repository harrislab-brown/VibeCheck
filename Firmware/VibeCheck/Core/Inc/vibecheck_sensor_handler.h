/*
 * vibecheck_sensor_handler.h
 *
 *  Created on: Sep 20, 2024
 *      Author: johnt
 */

#ifndef INC_VIBECHECK_SENSOR_HANDLER_H_
#define INC_VIBECHECK_SENSOR_HANDLER_H_

#include "vibecheck_sensor.h"
#include "vibecheck_shell.h"

uint32_t VibeCheckSensorCMD_Execute(void* obj, VibeCheckShell* shell);
uint32_t VibeCheckSensorSender_Data_Execute(void* obj, VibeCheckShell* shell);  /* sends data from the sensors */
uint32_t VibeCheckSensorSender_Status_Execute(void* obj, VibeCheckShell* shell);  /* sends sensor connection status updates */

#endif /* INC_VIBECHECK_SENSOR_HANDLER_H_ */
