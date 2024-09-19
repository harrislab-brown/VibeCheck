/*
 * vibecheckUSB.h
 *
 *  Created on: Sep 12, 2024
 *      Author: jantolik
 */

#ifndef INC_VIBECHECK_USB_H_
#define INC_VIBECHECK_USB_H_

#include "usbd_cdc_if.h"

/*
 * - sends data to the host PC (COBS encoding)
 * - receives commands in 64 byte packets
 * - controls status of the board (this is a high level object)
 */

#define VC_USB_DATA_BUF_LEN 100
#define VC_USB_DATA_STR_LEN 2048
#define VC_USB_DATA_PER_PACKET 2
#define VC_USB_DATA_HEADER "DATA "

/*
 * VibeCheckUSB class has a small circular buffer to which we can add data quickly (i.e. in the accelerometer interrupt)
 * When the CPU is free this data will be converted to strings (or encoded COBS if necessary) and build up in a double buffer to be sent to the host
 */

typedef struct
{
	uint8_t id;
	uint32_t time;
	float x, y, z;
} VibeCheckUSBData;

typedef struct
{
	uint32_t (*execute)(void* );
} VibeCheckUSBCommand;

typedef struct
{
	/* to receive messages and commands from the USB middleware */
	uint8_t* rx_buf;
	uint8_t* rx_ready;

	/* circular buffer of raw data to be encoded for sending to host */
	VibeCheckUSBData data_buf[VC_USB_DATA_BUF_LEN];
	uint32_t data_buf_head, data_buf_tail;
	uint32_t data_buf_count;

	/* big double buffered data string to send to host and its ready flag */
	char data_str[VC_USB_DATA_STR_LEN];
	uint32_t data_str_ind;

} VibeCheckUSB;

void VibeCheckUSB_Init(VibeCheckUSB* usb);
void VibeCheckUSB_Update(VibeCheckUSB* usb);  /* single function to call in main loop to update the USB class */

uint32_t VibeCheckUSB_ProcessCommand(VibeCheckUSB* usb);

void VibeCheckUSB_AddData(VibeCheckUSB* usb, uint8_t id, uint32_t time, float x, float y, float z);
void VibeCheckUSB_ProcessData(VibeCheckUSB* usb);

uint32_t VibeCheckUSB_Send(uint8_t* data, uint32_t len);  /* for sending brief messages, such as response to a getter command from host */
void VibeCheckUSB_SendBlocking(uint8_t* data, uint32_t len);
void VibeCheckUSB_TrySendData(VibeCheckUSB* usb);

#endif /* INC_VIBECHECK_USB_H_ */
