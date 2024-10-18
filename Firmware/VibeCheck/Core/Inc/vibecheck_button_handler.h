/*
 * vibecheck_button_handler.h
 *
 *  Created on: Oct 18, 2024
 *      Author: jantolik
 */

#ifndef INC_VIBECHECK_BUTTON_HANDLER_H_
#define INC_VIBECHECK_BUTTON_HANDLER_H_

#include "vibecheck_shell.h"
#include "main.h"

uint32_t VibeCheckRecordCMD_Execute(void* obj, VibeCheckShell* shell);  /* for host to set recording LED */

uint32_t VibeCheckMuteSender_Execute(void* obj, VibeCheckShell* shell);  /* mute button */
uint32_t VibeCheckRecordSender_Execute(void* obj, VibeCheckShell* shell);  /* record button */


#endif /* INC_VIBECHECK_BUTTON_HANDLER_H_ */
