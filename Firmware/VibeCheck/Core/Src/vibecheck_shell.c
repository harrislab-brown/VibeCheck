/*
 * vibecheck_shell.c
 *
 *  Created on: Sep 18, 2024
 *      Author: johnt
 */

#include "vibecheck_shell.h"


void VibeCheckShell_Init(VibeCheckShell* shell)
{
	shell->input_pos = 0;
	shell->output_pos = 0;
	shell->cmd_count = 0;
}

void VibeCheckShell_RegisterCommand(VibeCheckShell* shell, VibeCheckShell_CMD cmd)
{
	if (shell->cmd_count < VC_SHELL_MAX_NUM_CMDS)
	{
		shell->commands[shell->cmd_count++] = cmd;
	}
}

uint32_t VibeCheckShell_ProcessInput(VibeCheckShell* shell, char* input, char** output, uint32_t* output_len)  /* returns the output string as a result of running the command */
{

	strcpy(shell->input, input);  /* TODO: revisit safety of copying strings once it works */
	shell->input_pos = 0;
	shell->output_pos = 0;

	uint32_t ret = 0;

	char* cmd_name;
	VibeCheckShell_GetNextString(shell, &cmd_name);

	for (uint32_t i = 0; i < shell->cmd_count; i++)
	{
		if (!strcmp(shell->commands[i].name, cmd_name))
		{
			ret = shell->commands[i].execute(shell->commands[i].obj, shell);
			break;
		}
	}

	*output = shell->output;
	*output_len = shell->output_pos;
	return ret;
}


/* these return true if they successfully get the next token */
uint32_t VibeCheckShell_GetNextString(VibeCheckShell* shell, char** next)
{
	/* delimiters could be spaces or commas */
	/* turn to all lower case letters */
	/* the next token will be stored in the shell struct, return a pointer to the token char array */

	char separator[] = " ,";
	char* token = shell->token;
	while (shell->input[shell->input_pos] && shell->input_pos < VC_SHELL_STR_LEN)
	{
		if (strchr(separator, shell->input[shell->input_pos]) == NULL)
		{
			/* this is a character */
			*(token++) = shell->input[shell->input_pos];
		}
		else
		{
			/* this is a separator */
			if (token != shell->token) /* we have something in the token and have found a separator */
			{
				*next = shell->token;
				return 1;
			}
		}
		shell->input_pos++;
	}

	return 0;
}

uint32_t VibeCheckShell_GetNextInt(VibeCheckShell* shell, int32_t* next)
{
	char* str;
	if (VibeCheckShell_GetNextString(shell, &str))
	{
		char valid[] = "-0123456789";
		for (uint32_t i = 0; i < strlen(str); i++)
			if (strchr(valid, str[i]) == NULL)
				return 0;  /* next token contains non-numeric characters */
		*next = atoi(str);
		return 1;
	}

	return 0;
}

uint32_t VibeCheckShell_GetNextFloat(VibeCheckShell* shell, float* next)
{
	char* str;
	if (VibeCheckShell_GetNextString(shell, &str))
	{
		char valid[] = ".-0123456789";
		for (uint32_t i = 0; i < strlen(str); i++)
			if (strchr(valid, str[i]) == NULL)
				return 0;  /* next token contains non-numeric characters */
		*next = atof(str);
		return 1;
	}

	return 0;
}


/* add things to the output */
void VibeCheckShell_PutString(VibeCheckShell* shell, char* str)
{
	shell->output_pos += sprintf(shell->output + shell->output_pos, "%s ", str);
}

void VibeCheckShell_PutInt(VibeCheckShell* shell, uint32_t val)
{
	shell->output_pos += sprintf(shell->output + shell->output_pos, "%ld ", val);
}

void VibeCheckShell_PutFloat(VibeCheckShell* shell, float val)
{
	shell->output_pos += sprintf(shell->output + shell->output_pos, "%f ", val);
}

void VibeCheckShell_Ack(VibeCheckShell* shell)
{
	VibeCheckShell_PutString(shell, "ack");
}


