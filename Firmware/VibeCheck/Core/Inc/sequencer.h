/*
 * sequencer.h
 *
 *  Created on: Jul 6, 2024
 *      Author: johnt
 */

#ifndef INC_SEQUENCER_H_
#define INC_SEQUENCER_H_

#include <stddef.h>
#include <stdint.h>

typedef struct
{

	const uint32_t* sequence_time_array;
	uint32_t sequence_len;
	uint32_t sequence_index;
	uint32_t time;

	uint8_t is_running, is_looping;

} Sequencer;

void Sequencer_Init(Sequencer* sequencer);
void Sequencer_SetSequence(Sequencer* sequencer, const uint32_t* sequence_time_array, const uint32_t sequence_len, uint8_t is_looping);
uint32_t Sequencer_Update(Sequencer* sequencer, uint32_t time, uint32_t* index);
void Sequencer_Start(Sequencer* sequencer, uint32_t time);
void Sequencer_Stop(Sequencer* sequencer);

#endif /* INC_SEQUENCER_H_ */
