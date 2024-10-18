/*
 * vibecheck_connect_handler.h
 *
 *  Created on: Oct 18, 2024
 *      Author: jantolik
 */

#ifndef INC_VIBECHECK_CONNECT_HANDLER_H_
#define INC_VIBECHECK_CONNECT_HANDLER_H_

#include "vibecheck_shell.h"
#include "vibecheck_rgb.h"

uint32_t VibeCheckConnectCMD_Execute(void* obj, VibeCheckShell* shell);
uint32_t VibeCheckDisconnectCMD_Execute(void* obj, VibeCheckShell* shell);

#endif /* INC_VIBECHECK_CONNECT_HANDLER_H_ */
