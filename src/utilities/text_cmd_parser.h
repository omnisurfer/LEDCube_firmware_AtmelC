/*
* text_cmd_parser.h
*
* Created: 5/1/2016 3:25:49 PM
*  Author: Daniel
*/

#include "processor/processor.h"
#include "processor/interrupts.h"

#include "utilities/stream_handler.h"

#ifndef TEXT_CMD_PARSER_H_
#define TEXT_CMD_PARSER_H_

#define TEXT_COMMAND_PARSER_WRITE_DEBUG

#define TEXT_COMMAND_PARSER_BUFFER_SIZE 8
#define NUMBER_OF_COMMANDS 9
#define LENGTH_OF_COMMANDS 16

static char commandStrings[NUMBER_OF_COMMANDS][LENGTH_OF_COMMANDS + 1] = 
{
	"led5\r",
	"led4\r",
	"abca\r",
	"abcb\r",
	"led3\r",
	"abcc\r",
	"lec3\r",
	"dan\r",
	{0x41, 0x41, 0x41, 0x41, 0x00}
};

/*Function prototypes */
int8_t RXStreamHandler_TextCommandParser(char *streamIn, uint8_t streamInSize, STREAM_BUFFER *streamBuffer);

#endif /* TEXT_CMD_PARSER_H_ */