/*
 * vibecheck_wavegen.h
 *
 *  Created on: Sep 12, 2024
 *      Author: jantolik
 */

#ifndef INC_VIBECHECK_WAVEGEN_H_
#define INC_VIBECHECK_WAVEGEN_H_

/* generates the waveform using the DAC to excite the structure */

/* - wavegen owns the mute output pin
 * - generates sine wave, varying amplitude and frequency, 2 channels same output
 * - update amplitude and the frequency on the fly
 * 		- 1st option: pre-compute sine buffer fixed length, change frequency at which we send DMA requests (this is what we're gonna do, but have to check range of frequencies based on desired timing resolution)
 *  	- 2nd option: software compute the waveform, big double buffer for DMAing to the DAC (arbitrary waveform)
 */

typedef struct
{

} VibeCheckWaveGen;

#endif /* INC_VIBECHECK_WAVEGEN_H_ */
