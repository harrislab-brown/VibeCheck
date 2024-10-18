/*
 * vibecheck_wavegen.h
 *
 *  Created on: Sep 12, 2024
 *      Author: jantolik
 */

#ifndef INC_VIBECHECK_WAVEGEN_H_
#define INC_VIBECHECK_WAVEGEN_H_

#include <stdint.h>
#include "main.h"
#include "sequencer.h"

/* generates the waveform using the DAC to excite the structure */

/* - wavegen owns the mute output pin, button and indicator LED
 * - generates sine wave, varying amplitude and frequency, 2 channels same output
 * - update amplitude and the frequency on the fly
 * 		- 1st option: pre-compute sine buffer fixed length, change frequency at which we send DMA requests (this is what we're gonna do, but have to check range of frequencies based on desired timing resolution)
 *  	- 2nd option: software compute the waveform, big double buffer for DMAing to the DAC (arbitrary waveform)
 */

/*
 * MUTE_SIGNAL high to turn off the audio output
 * MUTE_BUTTON high when button pressed
 * Indicator LED on when MUTE_INDICATOR high
 */

/*
 * Mute button functionality:
 * starts un-muted, at any time if you press the button it toggles the mute which is just the
 * signal to the line driver. LED comes on when we are muted. Also sends a message to the shell
 * each time the button is pressed. Only the UI starts/stops the audio wave generation.
 */

#define VC_WAVE_TIM_BASE_HZ 240000000
#define VC_WAVE_TIM_PSC 12
#define VC_WAVE_TIM_MAX_COUNTS 65535
#define VC_WAVE_TIM_COUNTS_PER_SECOND (VC_WAVE_TIM_BASE_HZ / VC_WAVE_TIM_PSC)
#define VC_WAVE_BUF_LEN 512  /* number of elements in the wave, double buffered */

#define VC_WAVE_MIN_FREQ_HZ (VC_WAVE_TIM_COUNTS_PER_SECOND / VC_WAVE_BUF_LEN / VC_WAVE_TIM_MAX_COUNTS)
#define VC_WAVE_MAX_FREQ_HZ (VC_WAVE_TIM_COUNTS_PER_SECOND / VC_WAVE_BUF_LEN / 2)

#define VC_WAVE_DAC_FULL_SCALE 4095.0f  /* max value of the 12 bit DAC */

#define VC_WAVE_BUTTON_DEBOUNCE_MS 250

#define VC_WAVE_DEFAULT_FREQ_HZ 60.0f;
#define VC_WAVE_DEFAULT_AMPLITUDE 0.5f;

typedef enum
{
	VC_WAVE_SINE,
	VC_WAVE_SQUARE,
	VC_WAVE_SAW,
	VC_WAVE_TRIANGLE
} VibeCheckWaveGen_Waveform;

typedef struct
{

	TIM_HandleTypeDef* htim;  /* timer handle: DMA loads a new value into the DAC when this timer triggers, so it controls the frequency of the waveform */
	DAC_HandleTypeDef* hdac;

	uint32_t wave[2 * VC_WAVE_BUF_LEN];
	volatile uint32_t wave_ping_compute_pending;
	volatile uint32_t wave_pong_compute_pending;
	volatile uint32_t wave_ping_compute_ready;
	volatile uint32_t wave_pong_compute_ready;

	VibeCheckWaveGen_Waveform waveform;

	float freq_hz;
	float amplitude;  /* amplitude of the wave, between 0 and 1 */

	uint32_t is_running;

	uint32_t demo_num;
	Sequencer sequencer;  /* in case we want to do frequency sweeps or something */

} VibeCheckWaveGen;

void VibeCheckWaveGen_Init(VibeCheckWaveGen* wavegen, DAC_HandleTypeDef *hdac, TIM_HandleTypeDef* htim);
void VibeCheckWaveGen_Update(VibeCheckWaveGen* wavegen);  /* call in the main loop to check for mute button press */

void VibeCheckWaveGen_Start(VibeCheckWaveGen* wavegen);
void VibeCheckWaveGen_Stop(VibeCheckWaveGen* wavegen);

void VibeCheckWaveGen_SetFrequency(VibeCheckWaveGen* wavegen, float freq_hz);
float VibeCheckWaveGen_GetFrequency(VibeCheckWaveGen* wavegen);

void VibeCheckWaveGen_SetAmplitude(VibeCheckWaveGen* wavegen, float amplitude);
float VibeCheckWaveGen_GetAmplitude(VibeCheckWaveGen* wavegen);

void VibeCheckWaveGen_SetWaveform(VibeCheckWaveGen* wavegen, VibeCheckWaveGen_Waveform waveform);
VibeCheckWaveGen_Waveform VibeCheckWaveGen_GetWaveform(VibeCheckWaveGen* wavegen);

/* keeps track of which end of the double buffer to compute when updating the wave */
void VibeCheckWaveGen_DMAHalfCpltCallback(VibeCheckWaveGen* wavegen);
void VibeCheckWaveGen_DMACpltCallback(VibeCheckWaveGen* wavegen);

/* just for fun */
void VibeCheckWaveGen_StartDemo(VibeCheckWaveGen* wavegen);
void VibeCheckWaveGen_StopDemo(VibeCheckWaveGen* wavegen);

#endif /* INC_VIBECHECK_WAVEGEN_H_ */
