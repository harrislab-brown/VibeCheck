/*
 * vibecheck_shell.h
 *
 *  Created on: Sep 18, 2024
 *      Author: johnt
 */

#ifndef INC_VIBECHECK_SHELL_H_
#define INC_VIBECHECK_SHELL_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * control the VibeCheck through a text-based shell
 */

#define VC_SHELL_BUF_LEN 512
#define VC_SHELL_TOKEN_LEN 64
#define VC_SHELL_MAX_NUM_CMDS 64

struct VibeCheckShell_s;

typedef struct
{
	char name[VC_SHELL_TOKEN_LEN];
	uint32_t (*execute)(void*, struct VibeCheckShell_s*);  /* return true if we parse and execute successfully */
	void* obj;  /* object that the command acts on */
} VibeCheckShell_CMD;

struct VibeCheckShell_s
{
	char input[VC_SHELL_BUF_LEN];
	char output[VC_SHELL_BUF_LEN];
	uint32_t input_pos, output_pos;

	char token[VC_SHELL_TOKEN_LEN];

	VibeCheckShell_CMD commands[VC_SHELL_MAX_NUM_CMDS];
	uint32_t cmd_count;
};

typedef struct VibeCheckShell_s VibeCheckShell;

void VibeCheckShell_Init(VibeCheckShell* shell);
void VibeCheckShell_RegisterCommand(VibeCheckShell* shell, VibeCheckShell_CMD cmd);
uint32_t VibeCheckShell_ProcessInput(VibeCheckShell* shell, char* input, char** output, uint32_t* output_len);  /* returns the output string as a result of running the command */

uint32_t VibeCheckShell_GetNextString(VibeCheckShell* shell, char** next);
uint32_t VibeCheckShell_GetNextInt(VibeCheckShell* shell, int32_t* next);
uint32_t VibeCheckShell_GetNextFloat(VibeCheckShell* shell, float* next);

void VibeCheckShell_PutString(VibeCheckShell* shell, char* str);  /* add things to the output */
void VibeCheckShell_PutInt(VibeCheckShell* shell, uint32_t val);
void VibeCheckShell_PutFloat(VibeCheckShell* shell, float val);

void VibeCheckShell_Ack(VibeCheckShell* shell);  /* add acknowledgment to output */



#endif /* INC_VIBECHECK_SHELL_H_ */
