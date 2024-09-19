/*
 * vibecheck_shell.h
 *
 *  Created on: Sep 18, 2024
 *      Author: johnt
 */

#ifndef INC_VIBECHECK_SHELL_H_
#define INC_VIBECHECK_SHELL_H_

/*
 * control the VibeCheck through a text-based shell
 */

#define VC_SHELL_STR_LEN 512
#define VC_SHELL_CMD_NAME_LEN 64
#define VC_SHELL_MAX_NUM_CMDS 128

typedef struct
{
	char name[VC_SHELL_CMD_NAME_LEN];
	uint32_t (*execute)(VibeCheckShell* shell, void* obj);  /* return true if we parse and execute successfully */
} VibeCheckShell_CMD;

typedef struct
{
	char input[VC_SHELL_STR_LEN];
	char output[VC_SHELL_STR_LEN];

	VibeCheckShell_CMD commands[VC_SHELL_MAX_NUM_CMDS];
} VibeCheckShell;

void VibeCheckShell_Init(VibeCheckShell* shell);
void VibeCheckShell_RegisterCommand(VibeCheckShell* shell, VibeCheckShell_CMD cmd);



#endif /* INC_VIBECHECK_SHELL_H_ */
