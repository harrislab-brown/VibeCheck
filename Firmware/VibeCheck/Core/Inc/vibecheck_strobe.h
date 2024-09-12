/*
 * vibecheck_strobe.h
 *
 *  Created on: Sep 12, 2024
 *      Author: jantolik
 */

#ifndef INC_VIBECHECK_STROBE_H_
#define INC_VIBECHECK_STROBE_H_

/* controls the bright ring of strobe lights using timer hardware */

/*
 * - variable frequency, duty cycle and phase of the strobing (directly manipulate CNT register to shift phase or change ARR for one cycle )
 * - potentially synchronize the phase with the wavegen
 */


typedef struct
{

} VibeCheckStrobe;

#endif /* INC_VIBECHECK_STROBE_H_ */
