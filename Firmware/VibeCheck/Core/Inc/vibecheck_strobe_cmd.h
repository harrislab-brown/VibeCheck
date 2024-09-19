/*
 * vibecheck_strobe_cmd.h
 *
 *  Created on: Sep 18, 2024
 *      Author: johnt
 */

#ifndef INC_VIBECHECK_STROBE_CMD_H_
#define INC_VIBECHECK_STROBE_CMD_H_

#include "vibecheck_strobe.h"
#include "vibecheck_shell.h"

uint32_t VibeCheckStrobeCMD_Execute(void* obj, VibeCheckShell* shell);
uint32_t VibeCheckStrobeCMD_Set(VibeCheckStrobe* strobe, VibeCheckShell* shell);
uint32_t VibeCheckStrobeCMD_Get(VibeCheckStrobe* strobe, VibeCheckShell* shell);


#endif /* INC_VIBECHECK_STROBE_CMD_H_ */
