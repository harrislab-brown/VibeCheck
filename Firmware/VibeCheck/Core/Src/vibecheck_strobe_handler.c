/*
 * vibecheck_strobe_cmd.c
 *
 *  Created on: Sep 18, 2024
 *      Author: johnt
 */

#include "vibecheck_strobe_handler.h"

/*

Set of commands to implement:

>>strobe start

>>strobe stop

>>strobe set frequency 10.0

>>strobe get frequency

>>strobe set phase 180.0

>>strobe get phase

>>strobe set exposure 1.0

>>strobe get exposure

*/



static uint32_t VibeCheckStrobeCMD_Set(VibeCheckStrobe* strobe, VibeCheckShell* shell)
{
	char str[VC_SHELL_MAX_TOKEN_LEN];
	if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
	{
		if (!strcmp(str, "frequency"))
		{
			float val;
			if (VibeCheckShell_GetNextFloat(shell, &val))
			{
				VibeCheckStrobe_SetFrequency(strobe, val);
				VibeCheckShell_PutOutputString(shell, "ack");
				VibeCheckShell_PutOutputDelimiter(shell);
				return 1;
			}
		}
		else if (!strcmp(str, "phase"))
		{
			float val;
			if (VibeCheckShell_GetNextFloat(shell, &val))
			{
				VibeCheckStrobe_SetPhase(strobe, val);
				VibeCheckShell_PutOutputString(shell, "ack");
				VibeCheckShell_PutOutputDelimiter(shell);
				return 1;
			}
		}
		else if (!strcmp(str, "exposure"))
		{
			float val;
			if (VibeCheckShell_GetNextFloat(shell, &val))
			{
				VibeCheckStrobe_SetExposure(strobe, val);
				VibeCheckShell_PutOutputString(shell, "ack");
				VibeCheckShell_PutOutputDelimiter(shell);
				return 1;
			}
		}
	}

	return 0;
}

static uint32_t VibeCheckStrobeCMD_Get(VibeCheckStrobe* strobe, VibeCheckShell* shell)
{
	char str[VC_SHELL_MAX_TOKEN_LEN];
	if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
	{
		if (!strcmp(str, "frequency"))
		{
			VibeCheckShell_PutOutputString(shell, "ack");
			VibeCheckShell_PutOutputSeparator(shell);
			VibeCheckShell_PutOutputFloat(shell, VibeCheckStrobe_GetFrequency(strobe));
			VibeCheckShell_PutOutputDelimiter(shell);
			return 1;
		}
		else if (!strcmp(str, "phase"))
		{
			VibeCheckShell_PutOutputString(shell, "ack");
			VibeCheckShell_PutOutputSeparator(shell);
			VibeCheckShell_PutOutputFloat(shell, VibeCheckStrobe_GetPhase(strobe));
			VibeCheckShell_PutOutputDelimiter(shell);
			return 1;
		}
		else if (!strcmp(str, "exposure"))
		{
			VibeCheckShell_PutOutputString(shell, "ack");
			VibeCheckShell_PutOutputSeparator(shell);
			VibeCheckShell_PutOutputFloat(shell, VibeCheckStrobe_GetExposure(strobe));
			VibeCheckShell_PutOutputDelimiter(shell);
			return 1;
		}
	}

	return 0;
}

uint32_t VibeCheckStrobeCMD_Execute(void* obj, VibeCheckShell* shell)
{
	VibeCheckStrobe* strobe = (VibeCheckStrobe*)obj;

	char str[VC_SHELL_MAX_TOKEN_LEN];
	if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
	{
		if (!strcmp(str, "start"))
		{
			VibeCheckStrobe_Start(strobe);
			VibeCheckShell_PutOutputString(shell, "ack");
			VibeCheckShell_PutOutputDelimiter(shell);
			return 1;
		}
		else if (!strcmp(str, "stop"))
		{
			VibeCheckStrobe_Stop(strobe);
			VibeCheckShell_PutOutputString(shell, "ack");
			VibeCheckShell_PutOutputDelimiter(shell);
			return 1;
		}
		else if (!strcmp(str, "set"))
		{
			if (VibeCheckStrobeCMD_Set(strobe, shell))
			{
				return 1;
			}
		}
		else if (!strcmp(str, "get"))
		{
			if (VibeCheckStrobeCMD_Get(strobe, shell))
			{
				return 1;
			}
		}
	}

	return 0;
}

