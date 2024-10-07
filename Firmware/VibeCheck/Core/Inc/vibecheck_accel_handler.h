/*
 * vibecheck_accel_handler.h
 *
 *  Created on: Sep 20, 2024
 *      Author: johnt
 */

#ifndef INC_VIBECHECK_ACCEL_HANDLER_H_
#define INC_VIBECHECK_ACCEL_HANDLER_H_

#include "vibecheck_accel.h"
#include "vibecheck_shell.h"

uint32_t VibeCheckAccelCMD_Execute(void* obj, VibeCheckShell* shell);
uint32_t VibeCheckAccelSender_Data_Execute(void* obj, VibeCheckShell* shell);  /* sends data from the sensors */
uint32_t VibeCheckAccelSender_Status_Execute(void* obj, VibeCheckShell* shell);  /* sends sensor connection status updates */

#endif /* INC_VIBECHECK_ACCEL_HANDLER_H_ */
