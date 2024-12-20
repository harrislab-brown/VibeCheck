/*
 * vibecheck_shell.h
 *
 *  Created on: Sep 18, 2024
 *      Author: johnt
 */

#ifndef INC_VIBECHECK_SHELL_H_
#define INC_VIBECHECK_SHELL_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*
 * control the VibeCheck through a text-based shell
 */

/*
 * The shell contains FIFO text buffers for inputs and outputs.
 * When using this class, write to the input buffer with whatever interface hardware
 * we are using (UBS/UART/I2C/...). The shell contains arrays of generic input and output
 * handlers. The input handlers represent commands that we want to send to the device
 * to make it do something with the hardware. The output handlers represent messages that the
 * device is sending to the host, or streams of data generated by sensors. The design is decoupled
 * through the command pattern: the individual hardware subsystems and the shell do not need to know
 * about each other. A specific command can be implemented which knows about the shell and can access
 * the subsystem member functions.
 *
 * The shell expects commands to be terminated with a delimiter, and it will terminate its outgoing
 * messages with that same delimiter. It expects the input to consist of tokens separated by one of
 * the specified separator characters. The shell implements token-ization functions that can be used
 * by the concrete input handler implementations to easily parse the input string. Likewise the concrete
 * output handlers can choose to format the output string however they want.
 *
 * Inputs can be added to the input buffer at any point, and output handlers can generate data at any point
 * but they should buffer it locally since there is some delay between shell updates. When we call the shell
 * update function, it will check whether it has received any full messages (ending in delimiters) in the input buffer.
 * It will call any input handlers it has for the input messages. Then it will check through all of the output
 * handlers to see whether they have data ready. The update function will return a status flag which tells whether
 * it parsed inputs or outputs - this flag can be used to know when to start the hardware data transfer.
 */

#define VC_SHELL_IO_BUF_LEN 32768

#define VC_SHELL_MAX_OUTPUT_PACKET_LEN 4096  /*
												this is the assumed longest output packet - we always want to send a complete packet with delimiter if possible.
												If we exceed the assumed packet length, it should send in two separate outputs without breaking everything.
												*/

#define VC_SHELL_MAX_TOKEN_LEN 64
#define VC_SHELL_MAX_NUM_HANDLERS 64


#define VC_SHELL_DELIMITER "\n"
#define VC_SHELL_INPUT_SEPARATORS " ,"
#define VC_SHELL_OUTPUT_SEPARATOR " "

struct VibeCheckShell_s;

typedef struct
{
	char name[VC_SHELL_MAX_TOKEN_LEN];  /* first token of the input command string should be the name of which command to invoke */
	uint32_t (*execute)(void*, struct VibeCheckShell_s*);  /* return true if we parse and execute successfully */
	void* obj;  /* object that the command acts on */
} VibeCheckShell_InputHandler;


typedef struct
{
	uint32_t (*execute)(void*, struct VibeCheckShell_s*);  /* return true when this output handler is ready to send data */
	void* obj;  /* object that the handler gets data from */
} VibeCheckShell_OutputHandler;


typedef enum
{
	VC_SHELL_NO_INPUT,  /* we didn't get a complete message on this update */
	VC_SHELL_INPUT_PROCESSED,  /* successfully handled the input status */
	VC_SHELL_INPUT_ERROR_NO_HANDLER,  /* we received a message that we don't have a handler for */
	VC_SHELL_INPUT_ERROR_EXECUTING  /* we have a handler but it could not execute successfully */
} VibeCheckShell_InputHandlerStatus;


typedef enum
{
	VC_SHELL_NO_OUTPUT,  /* none of the output handlers are ready to send data */
	VC_SHELL_OUTPUT_PROCESSED  /* we successfully executed an output handler */
} VibeCheckShell_OutputHandlerStatus;


typedef enum
{
	VC_SHELL_INPUT_BUFFER_OK,
	VC_SHELL_INPUT_BUFFER_OVERFLOW
} VibeCheckShell_InputBufferStatus;


typedef enum
{
	VC_SHELL_OUTPUT_BUFFER_OK,
	VC_SHELL_OUTPUT_BUFFER_OVERFLOW
} VibeCheckShell_OutputBufferStatus;


typedef struct
{
	VibeCheckShell_InputHandlerStatus ihandl_status;
	VibeCheckShell_OutputHandlerStatus ohandl_status;
	VibeCheckShell_InputBufferStatus ibuf_status;
	VibeCheckShell_OutputBufferStatus obuf_status;
} VibeCheckShell_Status;


struct VibeCheckShell_s
{
	char input[VC_SHELL_IO_BUF_LEN];
	uint32_t input_head, input_tail;
	uint32_t input_count;  /* keeps track of how full the input buffer is */
	uint32_t input_delimiter_count;  /* keeps track of how many complete messages we have */

	char output[VC_SHELL_IO_BUF_LEN];
	uint32_t output_head, output_tail;
	uint32_t output_end;  /* 0 if no output packet has wrapped, else it is the location of the end of the packet */
	uint32_t output_count;  /* keeps track of how full the output buffer is */

	VibeCheckShell_InputHandler input_handlers[VC_SHELL_MAX_NUM_HANDLERS];
	uint32_t ih_count;
	VibeCheckShell_OutputHandler output_handlers[VC_SHELL_MAX_NUM_HANDLERS];
	uint32_t oh_count;
};

typedef struct VibeCheckShell_s VibeCheckShell;

void VibeCheckShell_Init(VibeCheckShell* shell);
VibeCheckShell_Status VibeCheckShell_Update(VibeCheckShell* shell);

void VibeCheckShell_RegisterInputHandler(VibeCheckShell* shell, VibeCheckShell_InputHandler handler);
void VibeCheckShell_RegisterOutputHandler(VibeCheckShell* shell, VibeCheckShell_OutputHandler handler);

void VibeCheckShell_PutInput(VibeCheckShell* shell, char* input, uint32_t input_max_len);  /* add to the input buffer */
uint32_t VibeCheckShell_GetOutput(VibeCheckShell* shell, char** output, uint32_t* len);  /* returns true if there is stuff in the output buffer we haven't yet sent */
void VibeCheckShell_UpdateOutputBuffer(VibeCheckShell* shell, uint32_t len);  /* once we have successfully sent the output data, update the buffer pointers */

uint32_t VibeCheckShell_GetNextString(VibeCheckShell* shell, char* next, uint32_t max_len);
uint32_t VibeCheckShell_GetNextInt(VibeCheckShell* shell, int32_t* next);
uint32_t VibeCheckShell_GetNextFloat(VibeCheckShell* shell, float* next);

uint32_t VibeCheckShell_TurnToInt(char* str, int32_t* next);  /* if the type of the next token is not known, get it as a string and then try to convert it to numeric using these functions */
uint32_t VibeCheckShell_TurnToFloat(char* str, float* next);

void VibeCheckShell_PutOutputString(VibeCheckShell* shell, char* str);  /* add things to the output */
void VibeCheckShell_PutOutputInt(VibeCheckShell* shell, uint32_t val);
void VibeCheckShell_PutOutputFloat(VibeCheckShell* shell, float val);

void VibeCheckShell_PutOutputSeparator(VibeCheckShell* shell);
void VibeCheckShell_PutOutputDelimiter(VibeCheckShell* shell);


#endif /* INC_VIBECHECK_SHELL_H_ */
