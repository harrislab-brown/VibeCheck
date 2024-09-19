/*
 * vibecheck_usb.c
 *
 *  Created on: Sep 17, 2024
 *      Author: jantolik
 */


#include "vibecheck_USB.h"

uint8_t usb_rx_buf[64];  /* see USB_DEVICE -> App -> usbd_cdc_if.c for implementation */
uint8_t usb_rx_ready = 0;  /* flag when USB data has been received */

void VibeCheckUSB_Init(VibeCheckUSB* usb, VibeCheckShell* shell)
{
	usb->shell = shell;

	usb->rx_buf = usb_rx_buf;
	usb->rx_ready = &usb_rx_ready;

	usb->data_buf_head = 0;
	usb->data_buf_tail = 0;
	usb->data_buf_count = 0;

	usb->data_str_ind = 0;
}

void VibeCheckUSB_Update(VibeCheckUSB* usb)  /* single function to call in main loop to update the USB class */
{

	/* process any commands that we have received from the host */
	VibeCheckUSB_ProcessCommand(usb);

	/* encode any data in the data buffer, send to the host if the data packet is full */
	VibeCheckUSB_ProcessData(usb);

}

uint32_t VibeCheckUSB_ProcessCommand(VibeCheckUSB* usb)
{
	/* return true if we have successfully processed a command */
	if (*usb->rx_ready)
	{
		*usb->rx_ready = 0;

		/* have the shell process the command */
		char* output;
		uint32_t output_len;
		if (VibeCheckShell_ProcessInput(usb->shell, (char*)usb->rx_buf, &output, &output_len))
		{
			if (output_len)
				VibeCheckUSB_Send_BlockUntilStarted((uint8_t*)output, output_len);
			return 1;
		}
	}

	return 0;
}

void VibeCheckUSB_AddData(VibeCheckUSB* usb, uint8_t id, uint32_t time, float x, float y, float z)
{
	/* USB class contains a big buffer (double buffered) in which to accumulate data, so add new data to the end */
	/* all of the accelerometer/gyro data has 3 components and we should include an ID for which chip generated it */
	/* potentially we could also add time stamps for when the samples were taken */

	/* in general we would like a circular buffer for this to keep the raw data. Then as CPU is free convert the data to strings */

	usb->data_buf[usb->data_buf_head].id = id;
	usb->data_buf[usb->data_buf_head].time = time;
	usb->data_buf[usb->data_buf_head].x = x;
	usb->data_buf[usb->data_buf_head].y = y;
	usb->data_buf[usb->data_buf_head].z = z;

	if (++usb->data_buf_head == VC_USB_DATA_BUF_LEN)
		usb->data_buf_head = 0;

}

void VibeCheckUSB_ProcessData(VibeCheckUSB* usb)
{
	/* encode all of the elements in the data buffer and put them in the data string to send to the host */
	while (usb->data_buf_head != usb->data_buf_tail && usb->data_buf_count < VC_USB_DATA_BUF_LEN)  /* TODO: this breaks if the data buffer is full so don't let that happen */
	{
		VibeCheckUSBData data_to_encode = usb->data_buf[usb->data_buf_tail];

		/* encode the data here and add it to the data string */
		usb->data_str_ind += sprintf(usb->data_str + usb->data_str_ind, "%u,%lu,%2.3f,%2.3f,%2.3f,",
				data_to_encode.id,
				data_to_encode.time,
				data_to_encode.x,
				data_to_encode.y,
				data_to_encode.z);

		usb->data_buf_count++;
		if (++usb->data_buf_tail == VC_USB_DATA_BUF_LEN)
			usb->data_buf_tail = 0;
	}

	if (usb->data_buf_count == VC_USB_DATA_PER_PACKET)
	{
		/* we have a full data packet so send it */

		/* add a newline */
		usb->data_str_ind += sprintf(usb->data_str + usb->data_str_ind - 1, "\r\n");

		if (usb->data_str_ind < VC_USB_DATA_STR_LEN / 2)
		{
			/* send the first half buffer and prepare to fill the second half */
			VibeCheckUSB_Send_BlockUntilStarted((uint8_t*)&usb->data_str[0], usb->data_str_ind);  /* !!! blocking until the transmission starts successfully !!! */
			usb->data_str_ind = VC_USB_DATA_STR_LEN / 2;
		}
		else
		{
			/* send the second half buffer and prepare to fill the first half */
			VibeCheckUSB_Send_BlockUntilStarted((uint8_t*)&usb->data_str[VC_USB_DATA_STR_LEN / 2], usb->data_str_ind - VC_USB_DATA_STR_LEN / 2);  /* !!! blocking until the transmission starts successfully !!! */
			usb->data_str_ind = 0;
		}

		/* prepare the next data packet */
		usb->data_buf_count = 0;

		/* write the header */
		usb->data_str_ind += sprintf(usb->data_str + usb->data_str_ind, VC_USB_DATA_HEADER);
	}
}

uint32_t VibeCheckUSB_Send(uint8_t* data, uint32_t len)
{
	/* returns true if transmission started successfully */
	return CDC_Transmit_HS(data, len) == USBD_OK;
}

/* TODO: lifetime issue with the data pointer */
void VibeCheckUSB_Send_BlockUntilStarted(uint8_t* data, uint32_t len)
{
	/* returns true if transmission started successfully */
	while(CDC_Transmit_HS(data, len) != USBD_OK);
}

