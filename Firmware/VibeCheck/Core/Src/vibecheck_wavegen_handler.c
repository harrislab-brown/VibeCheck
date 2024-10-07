/*
 * vibecheck_wavegen_handler.c
 *
 *  Created on: Sep 27, 2024
 *      Author: johnt
 */


#include "vibecheck_wavegen_handler.h"



/*

Set of commands to implement:

>>wavegen start

>>wavegen stop

>>wavegen demo start

>>wavegen demo stop

>>wavegen set frequency 100.0

>>wavegen get frequency

>>wavegen set amplitude 0.5

>>wavegen get amplitude

>>wavegen set waveform [sine, square, saw, triangle]

>>wavegen get waveform

*/



static uint32_t VibeCheckWaveGenCMD_Set(VibeCheckWaveGen* wavegen, VibeCheckShell* shell)
{
	char str[VC_SHELL_MAX_TOKEN_LEN];
	if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
	{
		if (!strcmp(str, "frequency"))
		{
			float val;
			if (VibeCheckShell_GetNextFloat(shell, &val))
			{
				VibeCheckWaveGen_SetFrequency(wavegen, val);
				VibeCheckShell_PutOutputString(shell, "ack");
				VibeCheckShell_PutOutputDelimiter(shell);
				return 1;
			}
		}
		else if (!strcmp(str, "amplitude"))
		{
			float val;
			if (VibeCheckShell_GetNextFloat(shell, &val))
			{
				VibeCheckWaveGen_SetAmplitude(wavegen, val);
				VibeCheckShell_PutOutputString(shell, "ack");
				VibeCheckShell_PutOutputDelimiter(shell);
				return 1;
			}
		}
		else if (!strcmp(str, "waveform"))
		{
			if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
			{
				if (!strcmp(str, "sine"))
				{
					VibeCheckWaveGen_SetWaveform(wavegen, VC_WAVE_SINE);
					VibeCheckShell_PutOutputString(shell, "ack");
					VibeCheckShell_PutOutputDelimiter(shell);
					return 1;
				}
				else if (!strcmp(str, "square"))
				{
					VibeCheckWaveGen_SetWaveform(wavegen, VC_WAVE_SQUARE);
					VibeCheckShell_PutOutputString(shell, "ack");
					VibeCheckShell_PutOutputDelimiter(shell);
					return 1;
				}
				else if (!strcmp(str, "saw"))
				{
					VibeCheckWaveGen_SetWaveform(wavegen, VC_WAVE_SAW);
					VibeCheckShell_PutOutputString(shell, "ack");
					VibeCheckShell_PutOutputDelimiter(shell);
					return 1;
				}
				else if (!strcmp(str, "triangle"))
				{
					VibeCheckWaveGen_SetWaveform(wavegen, VC_WAVE_TRIANGLE);
					VibeCheckShell_PutOutputString(shell, "ack");
					VibeCheckShell_PutOutputDelimiter(shell);
					return 1;
				}
			}
		}
	}

	return 0;
}

static uint32_t VibeCheckWaveGenCMD_Get(VibeCheckWaveGen* wavegen, VibeCheckShell* shell)
{
	char str[VC_SHELL_MAX_TOKEN_LEN];
	if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
	{
		if (!strcmp(str, "frequency"))
		{
			VibeCheckShell_PutOutputString(shell, "ack");
			VibeCheckShell_PutOutputSeparator(shell);
			VibeCheckShell_PutOutputFloat(shell, VibeCheckWaveGen_GetFrequency(wavegen));
			VibeCheckShell_PutOutputDelimiter(shell);
			return 1;
		}
		else if (!strcmp(str, "amplitude"))
		{
			VibeCheckShell_PutOutputString(shell, "ack");
			VibeCheckShell_PutOutputSeparator(shell);
			VibeCheckShell_PutOutputFloat(shell, VibeCheckWaveGen_GetAmplitude(wavegen));
			VibeCheckShell_PutOutputDelimiter(shell);
			return 1;
		}
		else if (!strcmp(str, "waveform"))
		{
			VibeCheckShell_PutOutputString(shell, "ack");
			VibeCheckShell_PutOutputSeparator(shell);
			switch (VibeCheckWaveGen_GetWaveform(wavegen))
			{
			case VC_WAVE_SINE:
				VibeCheckShell_PutOutputString(shell, "sine");
				break;
			case VC_WAVE_SQUARE:
				VibeCheckShell_PutOutputString(shell, "square");
				break;
			case VC_WAVE_SAW:
				VibeCheckShell_PutOutputString(shell, "saw");
				break;
			case VC_WAVE_TRIANGLE:
				VibeCheckShell_PutOutputString(shell, "triangle");
				break;
			}
			VibeCheckShell_PutOutputDelimiter(shell);
			return 1;
		}
	}

	return 0;
}


uint32_t VibeCheckWaveGenCMD_Execute(void* obj, VibeCheckShell* shell)
{
	VibeCheckWaveGen* wavegen = (VibeCheckWaveGen*)obj;

	char str[VC_SHELL_MAX_TOKEN_LEN];
	if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
	{
		if (!strcmp(str, "start"))
		{
			VibeCheckWaveGen_Start(wavegen);
			VibeCheckShell_PutOutputString(shell, "ack");
			VibeCheckShell_PutOutputDelimiter(shell);
			return 1;
		}
		else if (!strcmp(str, "stop"))
		{
			VibeCheckWaveGen_Stop(wavegen);
			VibeCheckShell_PutOutputString(shell, "ack");
			VibeCheckShell_PutOutputDelimiter(shell);
			return 1;
		}
		else if (!strcmp(str, "set"))
		{
			if (VibeCheckWaveGenCMD_Set(wavegen, shell))
			{
				return 1;
			}
		}
		else if (!strcmp(str, "get"))
		{
			if (VibeCheckWaveGenCMD_Get(wavegen, shell))
			{
				return 1;
			}
		}
		else if (!strcmp(str, "demo"))
		{
			if (VibeCheckShell_GetNextString(shell, str, VC_SHELL_MAX_TOKEN_LEN))
			{
				if (!strcmp(str, "start"))
				{
					VibeCheckWaveGen_StartDemo(wavegen);
					VibeCheckShell_PutOutputString(shell, "ack");
					VibeCheckShell_PutOutputDelimiter(shell);
					return 1;
				}
				else if (!strcmp(str, "stop"))
				{
					VibeCheckWaveGen_StopDemo(wavegen);
					VibeCheckShell_PutOutputString(shell, "ack");
					VibeCheckShell_PutOutputDelimiter(shell);
					return 1;
				}
			}
		}
	}

	return 0;
}


uint32_t VibeCheckWaveGenSender_Execute(void* obj, VibeCheckShell* shell)
{
	/* this will send a message to the host when the mute button is pressed */

	VibeCheckWaveGen* wavegen = (VibeCheckWaveGen*)obj;

	uint32_t is_muted;
	if (VibeCheckWaveGen_WasMuteButtonPressed(wavegen, &is_muted))
	{
		VibeCheckShell_PutOutputString(shell, "msg");
		VibeCheckShell_PutOutputSeparator(shell);
		VibeCheckShell_PutOutputString(shell, "wavegen");
		VibeCheckShell_PutOutputSeparator(shell);

		if (is_muted)
			VibeCheckShell_PutOutputString(shell, "muted");
		else
			VibeCheckShell_PutOutputString(shell, "unmuted");

		VibeCheckShell_PutOutputDelimiter(shell);

		return 1;
	}

	return 0;
}
