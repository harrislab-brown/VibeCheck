/*
 * vibecheck_shell.c
 *
 *  Created on: Sep 18, 2024
 *      Author: johnt
 */

#include "vibecheck_shell.h"


void VibeCheckShell_Init(VibeCheckShell* shell)
{
	shell->input_head = 0;
	shell->input_tail = 0;
	shell->input_count = 0;
	shell->input_delimiter_count = 0;

	shell->output_head = 0;
	shell->output_tail = 0;
	shell->output_count = 0;

	shell->ih_count = 0;
	shell->oh_count = 0;
}

VibeCheckShell_Status VibeCheckShell_Update(VibeCheckShell* shell)
{

	VibeCheckShell_Status status = {
			.ihandl_status = VC_SHELL_NO_INPUT,
			.ohandl_status = VC_SHELL_NO_OUTPUT,
			.ibuf_status = VC_SHELL_INPUT_BUFFER_OK,
			.obuf_status = VC_SHELL_OUTPUT_BUFFER_OK
	};

	/* check if character buffer overflow */
	if (shell->input_count >= VC_SHELL_IO_BUF_LEN)
		status.ibuf_status = VC_SHELL_INPUT_BUFFER_OVERFLOW;


	/* handle the inputs */
	if (shell->input_delimiter_count)
		status.ihandl_status = VC_SHELL_INPUT_PROCESSED;

	while (shell->input_delimiter_count)
	{
		/* look for a handler */
		char ih_name[VC_SHELL_MAX_TOKEN_LEN];
		if (VibeCheckShell_GetNextString(shell, ih_name, VC_SHELL_MAX_TOKEN_LEN))
		{
			for (uint32_t i = 0; i < shell->ih_count; i++)
			{
				if (!strcmp(shell->input_handlers[i].name, ih_name))
				{
					/* found a handler for this input */
					if (!shell->input_handlers[i].execute(shell->input_handlers[i].obj, shell))
						status.ihandl_status = VC_SHELL_INPUT_ERROR_EXECUTING;
					break;
				}

				if (i == shell->ih_count - 1)
					status.ihandl_status = VC_SHELL_INPUT_ERROR_NO_HANDLER;
			}
		}
		else
		{
			/* couldn't get a string from the input for handler name */
			status.ihandl_status = VC_SHELL_INPUT_ERROR_NO_HANDLER;
		}

		/* clean up the remainder of the previous message */
		while (1)
		{
			char x = shell->input[shell->input_tail];

			shell->input_count--;
			shell->input_tail++;
			if (shell->input_tail == VC_SHELL_IO_BUF_LEN)
				shell->input_tail = 0;

			if(strchr(VC_SHELL_DELIMITER, x) != NULL)
				break;  /* found the delimiter */
		}


		shell->input_delimiter_count--;
	}


	/* handle the outputs */
	for (uint32_t i = 0; i < shell->oh_count; i++)
	{
		if (shell->output_handlers[i].execute(shell->output_handlers[i].obj, shell))
		{
			status.ohandl_status = VC_SHELL_OUTPUT_PROCESSED;
		}
	}

	/* check if character buffer overflow */
	if (shell->output_count >= VC_SHELL_IO_BUF_LEN)
		status.obuf_status = VC_SHELL_OUTPUT_BUFFER_OVERFLOW;


	return status;
}

void VibeCheckShell_RegisterInputHandler(VibeCheckShell* shell, VibeCheckShell_InputHandler handler)
{
	if (shell->ih_count < VC_SHELL_MAX_NUM_HANDLERS)
		shell->input_handlers[shell->ih_count++] = handler;
}

void VibeCheckShell_RegisterOutputHandler(VibeCheckShell* shell, VibeCheckShell_OutputHandler handler)
{
	if (shell->oh_count < VC_SHELL_MAX_NUM_HANDLERS)
		shell->output_handlers[shell->oh_count++] = handler;
}

void VibeCheckShell_PutInput(VibeCheckShell* shell, char* input, uint32_t input_max_len)
{
	/* add to the input buffer */
	for (uint32_t i = 0; i < input_max_len; i++)
	{
		if (*input)
		{
			shell->input[shell->input_head++] = *input;
			if (shell->input_head == VC_SHELL_IO_BUF_LEN)
					shell->input_head = 0;
			shell->input_count++;

			if (strchr(VC_SHELL_DELIMITER, *input) != NULL)
				shell->input_delimiter_count++;

			input++;
		}
		else
			break;
	}
}


uint32_t VibeCheckShell_GetOutput(VibeCheckShell* shell, char** output, uint32_t* len)
{
	/* returns true if there is stuff in the output buffer we haven't yet sent */
	uint32_t head = shell->output_head;
	uint32_t tail = shell->output_tail;

	if (head == tail)
		return 0;

	if (head > tail)
	{
		/* the output doesn't wrap so send it 'normally' */
		*output = &shell->output[tail];
		*len = head - tail;
		return 1;
	}
	else
	{
		/* the output wraps: only return up to the end of the buffer region so our output is contiguous in memory */
		*output = &shell->output[tail];
		*len = VC_SHELL_IO_BUF_LEN - tail;
		return 1;
	}
}


void VibeCheckShell_UpdateOutputBuffer(VibeCheckShell* shell, uint32_t len)
{
	/* length argument tells how many characters we successfully transmitted */

	/* update the tail of the output buffer now that we have confirmed transmission */
	shell->output_count -= len;
	shell->output_tail += len;
	while (shell->output_tail >= VC_SHELL_IO_BUF_LEN)
		shell->output_tail -= VC_SHELL_IO_BUF_LEN;
}


/* these return true if they successfully get the next token */
uint32_t VibeCheckShell_GetNextString(VibeCheckShell* shell, char* next, uint32_t max_len)
{

	for (uint32_t i = 0; i < max_len; )
	{

		char x = shell->input[shell->input_tail];

		if (strchr(VC_SHELL_DELIMITER, x) != NULL)
		{
			/* this is a delimiter */
			if (i)  /* we have something in the token so return it */
			{
				*next = '\0';
				return 1;
			}
			else
				return 0;  /* don't go any further than the delimiter */
		}
		else
		{
			shell->input_count--;
			shell->input_tail++;
			if (shell->input_tail == VC_SHELL_IO_BUF_LEN)
				shell->input_tail = 0;

			if (strchr(VC_SHELL_INPUT_SEPARATORS, x) != NULL)
			{
				/* this is a separator */
				if (i) /* if we have something in the token return it, otherwise just continue in order to skip leading separators */
				{
					*next = '\0';
					return 1;
				}
			}
			else
			{
				/* this is a character */
				*(next++) = tolower(x);  /* turn to all lower case letters */
				i++;
			}
		}
	}

	return 0;
}


uint32_t VibeCheckShell_GetNextInt(VibeCheckShell* shell, int32_t* next)
{
	char str[VC_SHELL_MAX_TOKEN_LEN];
	if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
	{
		return VibeCheckShell_TurnToInt(str, next);
	}

	return 0;
}


uint32_t VibeCheckShell_GetNextFloat(VibeCheckShell* shell, float* next)
{
	char str[VC_SHELL_MAX_TOKEN_LEN];
	if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
	{
		return VibeCheckShell_TurnToFloat(str, next);
	}

	return 0;
}

/* if the type of the next token is not known, get it as a string and then try to convert it to numeric using these functions */
uint32_t VibeCheckShell_TurnToInt(char* str, int32_t* next)
{
	char valid[] = "-0123456789";
	for (uint32_t i = 0; i < strlen(str); i++)
		if (strchr(valid, str[i]) == NULL)
			return 0;  /* next token contains non-numeric characters */
	*next = atoi(str);
	return 1;
}

uint32_t VibeCheckShell_TurnToFloat(char* str, float* next)
{
	char valid[] = ".-0123456789";
	for (uint32_t i = 0; i < strlen(str); i++)
		if (strchr(valid, str[i]) == NULL)
			return 0;  /* next token contains non-numeric characters */
	*next = atof(str);
	return 1;
}



/* add things to the output */
void VibeCheckShell_PutOutputString(VibeCheckShell* shell, char* str)
{
	while (*str)
	{
		shell->output[shell->output_head++] = *(str++);
		shell->output_count++;
		if (shell->output_head == VC_SHELL_IO_BUF_LEN)
			shell->output_head = 0;
	}
}

void VibeCheckShell_PutOutputInt(VibeCheckShell* shell, uint32_t val)
{
	char str[VC_SHELL_MAX_TOKEN_LEN];
	sprintf(str, "%ld", val);
	VibeCheckShell_PutOutputString(shell, str);
}

void VibeCheckShell_PutOutputFloat(VibeCheckShell* shell, float val)
{
	char str[VC_SHELL_MAX_TOKEN_LEN];
	sprintf(str, "%.3f", val);
	VibeCheckShell_PutOutputString(shell, str);
}

void VibeCheckShell_PutOutputSeparator(VibeCheckShell* shell)
{
	VibeCheckShell_PutOutputString(shell, VC_SHELL_OUTPUT_SEPARATOR);
}

void VibeCheckShell_PutOutputDelimiter(VibeCheckShell* shell)
{
	VibeCheckShell_PutOutputString(shell, VC_SHELL_DELIMITER);
}


