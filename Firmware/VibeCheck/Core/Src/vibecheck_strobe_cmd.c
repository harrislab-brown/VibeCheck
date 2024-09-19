/*
 * vibecheck_strobe_cmd.c
 *
 *  Created on: Sep 18, 2024
 *      Author: johnt
 */

#include "vibecheck_strobe_cmd.h"

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

uint32_t VibeCheckStrobeCMD_Execute(void* obj, VibeCheckShell* shell)
{
	VibeCheckStrobe* strobe = (VibeCheckStrobe*)obj;

	char* str;
	if (VibeCheckShell_GetNextString(shell, &str))
	{
		if (!strcmp(str, "start"))
		{
			VibeCheckStrobe_Start(strobe);
			VibeCheckShell_Ack(shell);
			return 1;
		}
		else if (!strcmp(str, "stop"))
		{
			VibeCheckStrobe_Stop(strobe);
			VibeCheckShell_Ack(shell);
			return 1;
		}
		else if (!strcmp(str, "set"))
		{
			if (VibeCheckStrobeCMD_Set(strobe, shell))
			{
				VibeCheckShell_Ack(shell);
				return 1;
			}
		}
		else if (!strcmp(str, "get"))
		{
			if (VibeCheckStrobeCMD_Get(strobe, shell))
			{
				VibeCheckShell_Ack(shell);
				return 1;
			}
		}
	}

	return 0;
}

uint32_t VibeCheckStrobeCMD_Set(VibeCheckStrobe* strobe, VibeCheckShell* shell)
{
	char* str;
	if (VibeCheckShell_GetNextString(shell, &str))
	{
		if (!strcmp(str, "frequency"))
		{
			float val;
			if (VibeCheckShell_GetNextFloat(shell, &val))
			{
				VibeCheckStrobe_SetFrequency(strobe, val);
				return 1;
			}
		}
		else if (!strcmp(str, "phase"))
		{
			float val;
			if (VibeCheckShell_GetNextFloat(shell, &val))
			{
				VibeCheckStrobe_SetPhase(strobe, val);
				return 1;
			}
		}
		else if (!strcmp(str, "exposure"))
		{
			float val;
			if (VibeCheckShell_GetNextFloat(shell, &val))
			{
				VibeCheckStrobe_SetExposure(strobe, val);
				return 1;
			}
		}
	}

	return 0;
}

uint32_t VibeCheckStrobeCMD_Get(VibeCheckStrobe* strobe, VibeCheckShell* shell)
{
	char* str;
	if (VibeCheckShell_GetNextString(shell, &str))
	{
		if (!strcmp(str, "frequency"))
		{
			VibeCheckShell_PutFloat(shell, VibeCheckStrobe_GetFrequency(strobe));
			return 1;
		}
		else if (!strcmp(str, "phase"))
		{
			VibeCheckShell_PutFloat(shell, VibeCheckStrobe_GetPhase(strobe));
			return 1;
		}
		else if (!strcmp(str, "exposure"))
		{
			VibeCheckShell_PutFloat(shell, VibeCheckStrobe_GetExposure(strobe));
			return 1;
		}
	}

	return 0;
}
