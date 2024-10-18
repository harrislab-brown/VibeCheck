/*
 * vibecheck_connect_handler.c
 *
 *  Created on: Oct 18, 2024
 *      Author: jantolik
 */


#include "vibecheck_connect_handler.h"
#include "vibecheck_rgb_sequences.h"

uint32_t VibeCheckConnectCMD_Execute(void* obj, VibeCheckShell* shell)
{
	VibeCheckRGB* rgb = (VibeCheckRGB*) obj;
	VibeCheckRGB_SetTopSequence(rgb, connect_sequence_times, connect_sequence_colors, connect_sequence_len);
	VibeCheckRGB_StartTopSequence(rgb);
	VibeCheckShell_PutOutputString(shell, "ack");
	VibeCheckShell_PutOutputDelimiter(shell);
	return 1;
}


uint32_t VibeCheckDisconnectCMD_Execute(void* obj, VibeCheckShell* shell)
{
	VibeCheckRGB* rgb = (VibeCheckRGB*) obj;
	VibeCheckRGB_SetTopSequence(rgb, connect_sequence_times, connect_sequence_colors, connect_sequence_len);
	VibeCheckRGB_StartTopSequence(rgb);
	VibeCheckShell_PutOutputString(shell, "ack");
	VibeCheckShell_PutOutputDelimiter(shell);
	return 1;
}
