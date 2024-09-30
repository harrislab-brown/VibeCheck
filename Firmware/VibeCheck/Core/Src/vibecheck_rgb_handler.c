/*
 * vibecheck_rgb_handler.c
 *
 *  Created on: Sep 30, 2024
 *      Author: johnt
 */

#include "vibecheck_rgb_handler.h"


/*

Set of commands to implement:

>>rgb set [index] [r] [g] [b]

>>rgb get [index]

*/


uint32_t VibeCheckRGBCMD_Execute(void* obj, VibeCheckShell* shell)
{
	VibeCheckRGB* rgb = (VibeCheckRGB*) obj;

	char str[VC_SHELL_MAX_TOKEN_LEN];
	if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
	{
		if (!strcmp(str, "set"))
		{
			int32_t index, r, g, b;
			if (VibeCheckShell_GetNextInt(shell, &index)
					&& VibeCheckShell_GetNextInt(shell, &r)
					&& VibeCheckShell_GetNextInt(shell, &g)
					&& VibeCheckShell_GetNextInt(shell, &b))
			{
				VibeCheckRGB_SetColor(rgb, index, r, g, b);
				VibeCheckRGB_SendColors(rgb);
				VibeCheckShell_PutOutputString(shell, "ack");
				VibeCheckShell_PutOutputDelimiter(shell);
				return 1;
			}
		}
		else if (!strcmp(str, "get"))
		{
			int32_t index;
			if (VibeCheckShell_GetNextInt(shell, &index))
			{
				VibeCheckRGB_Color color = VibeCheckRGB_GetColor(rgb, index);

				VibeCheckShell_PutOutputString(shell, "ack");
				VibeCheckShell_PutOutputSeparator(shell);
				VibeCheckShell_PutOutputInt(shell, color.r);
				VibeCheckShell_PutOutputSeparator(shell);
				VibeCheckShell_PutOutputInt(shell, color.g);
				VibeCheckShell_PutOutputSeparator(shell);
				VibeCheckShell_PutOutputInt(shell, color.b);
				VibeCheckShell_PutOutputDelimiter(shell);
				return 1;
			}
		}
	}

	return 0;
}

