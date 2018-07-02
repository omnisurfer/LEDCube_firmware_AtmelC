/*
 * stream_handler.h
 *
 * Created: 5/7/2016 3:01:00 PM
 *  Author: Daniel
 */ 


#ifndef STREAM_HANDLER_H_
#define STREAM_HANDLER_H_

#include "processor/processor.h"

#define SERIAL_STREAM_BUFFER_SIZE	8

typedef enum
{
	RX_STREAM_HANDLER_COMMAND_PARSER,
	RX_STREAM_HANDLER_LED_CUBE		
}RXStreamHandler;

typedef struct STREAM_BUFFER STREAM_BUFFER;

struct STREAM_BUFFER
{	
	uint8_t streamBufferSize;
	uint8_t nextCharIndex;
	char serialStreamBuffer[SERIAL_STREAM_BUFFER_SIZE];	
};

void SetRXStreamHandler(RXStreamHandler rxStreamHandler);

RXStreamHandler GetRXStreamHandler(void);

void ClearStreamBuffer(STREAM_BUFFER *serialStreamBuffer);

#endif /* STREAM_HANDLER_H_ */