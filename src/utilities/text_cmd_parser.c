/*
* text_command_parser.c
*
* Created: 5/1/2016 3:25:32 PM
*  Author: Daniel
*/
#include <string.h>

#include "text_cmd_parser.h"
#include "utilities/debug.h"
#include "utilities/stream_handler.h"

typedef enum
{
	INITIALIZATION = 0,
	INITIAL_MATCH_SEARCH,
	PARTIAL_MATCH_IN_PROGRESS,
	PARTIAL_MATCH_FAIL,
	FULL_MATCH_FOUND
}CommandParserState;

typedef struct COMMAND_PARSER_CONTROL COMMAND_PARSER_CONTROL;

struct COMMAND_PARSER_CONTROL
{
	CommandParserState commandParserState;
	uint8_t commandStringIndex;
	uint8_t commandStringMatchCharIndex;
	const uint8_t numberOfCommands;
};

COMMAND_PARSER_CONTROL commandParserControl =
{
	.commandParserState = INITIAL_MATCH_SEARCH,
	.commandStringIndex = 0,
	.commandStringMatchCharIndex = 0,
	.numberOfCommands = NUMBER_OF_COMMANDS
};

char textCommandParserBuffer[LENGTH_OF_COMMANDS + 1] = { 0 };

void ReInitialize_TextCommandParser(void);

int8_t RXStreamHandler_TextCommandParser(char *streamIn, uint8_t streamInSize, STREAM_BUFFER *serialStreamBuffer)
{
	#ifdef TEXT_COMMAND_PARSER_WRITE_DEBUG
	//WriteDebugString(" Command Parser State: ");

	//WriteIntToString(commandParserControl.commandParserState);

	//WriteDebugString("\r\n");

	//WriteDebugStringCRNL();
	#endif

	//look through the input stream
	for(uint8_t streamInIndex = 0; streamInIndex < streamInSize; streamInIndex++)
	{
		switch(commandParserControl.commandParserState)
		{
			//INITIAL_MATCH_SEARCH
			#pragma region
			case INITIAL_MATCH_SEARCH:

			#ifdef TEXT_COMMAND_PARSER_WRITE_DEBUG
			//WriteDebugString("Initial Match Search\r\n");			
			#endif
			
			//This loop looks through all the available command strings
			for(uint8_t commandIndex = 0; commandIndex < commandParserControl.numberOfCommands; commandIndex++)
			{
				if(streamIn[streamInIndex] == commandStrings[commandParserControl.commandStringIndex][commandParserControl.commandStringMatchCharIndex])
				{
					serialStreamBuffer->serialStreamBuffer[serialStreamBuffer->nextCharIndex] = streamIn[streamInIndex];

					serialStreamBuffer->nextCharIndex++;
					
					commandParserControl.commandStringMatchCharIndex++;

					commandParserControl.commandParserState = PARTIAL_MATCH_IN_PROGRESS;
					
					#ifdef TEXT_COMMAND_PARSER_WRITE_DEBUG
					//WriteDebugString("Initial Match Found\r\n");
					#endif
				}

				if(commandParserControl.commandParserState == PARTIAL_MATCH_IN_PROGRESS)
				break;
				
				else
				commandParserControl.commandStringIndex++;
			}

			if(commandParserControl.commandParserState == INITIAL_MATCH_SEARCH)
			{
				ClearStreamBuffer(serialStreamBuffer);

				ReInitialize_TextCommandParser();
				
				#ifdef TEXT_COMMAND_PARSER_WRITE_DEBUG
				//WriteDebugString("Initial Match Fail\r\n");
				#endif
			}

			break;
			#pragma endregion

			//PARTIAL_MATCH_IN_PROGRESS
			#pragma region
			case PARTIAL_MATCH_IN_PROGRESS:

			//continue looking through current command string
			if(streamIn[streamInIndex] == commandStrings[commandParserControl.commandStringIndex][commandParserControl.commandStringMatchCharIndex])
			{
				serialStreamBuffer->serialStreamBuffer[serialStreamBuffer->nextCharIndex] = streamIn[streamInIndex];

				serialStreamBuffer->nextCharIndex++;

				commandParserControl.commandStringMatchCharIndex++;
				
				#ifdef TEXT_COMMAND_PARSER_WRITE_DEBUG
				//WriteDebugString("Match In Progress\r\n");
				#endif

				//check to see if next character in the commandString is a null
				if(commandStrings[commandParserControl.commandStringIndex][commandParserControl.commandStringMatchCharIndex] == '\0')
				{
					commandParserControl.commandParserState = FULL_MATCH_FOUND;

					#ifdef TEXT_COMMAND_PARSER_WRITE_DEBUG
					//WriteDebugString("Full Match Found\r\n");
					#endif
				}
			}

			//look through other command strings at current character match position, starting with the next command string in the list
			else
			{				
				bool alternativeStringMatchFound = false;

				for(uint8_t commandIndex = commandParserControl.commandStringIndex + 1; commandIndex < commandParserControl.numberOfCommands; commandIndex++)
				{//
					if(streamIn[streamInIndex] == commandStrings[commandIndex][commandParserControl.commandStringMatchCharIndex])
					{
						//verify that previous characters match with what is in the serial stream buffer
						for(int8_t i = serialStreamBuffer->nextCharIndex - 1, j = commandParserControl.commandStringMatchCharIndex - 1; i >= 0; i--, j--)
						{//
							//match
							if(serialStreamBuffer->serialStreamBuffer[i] == commandStrings[commandIndex][j])
							{
								//is this last character being compared?
								if(i == 0)
								{
									serialStreamBuffer->serialStreamBuffer[serialStreamBuffer->nextCharIndex] = streamIn[streamInIndex];

									serialStreamBuffer->nextCharIndex++;
									
									commandParserControl.commandStringMatchCharIndex++;

									alternativeStringMatchFound = true;

									//store the new command string index
									commandParserControl.commandStringIndex = commandIndex;
									
									#ifdef TEXT_COMMAND_PARSER_WRITE_DEBUG
									//WriteDebugString("Match found in alternative command string\r\n");
									#endif

									//check to see if next character in the commandString is a null
									if(commandStrings[commandParserControl.commandStringIndex][commandParserControl.commandStringMatchCharIndex] == '\0')
									{
										commandParserControl.commandParserState = FULL_MATCH_FOUND;

										#ifdef TEXT_COMMAND_PARSER_WRITE_DEBUG
										//WriteDebugString("Full Match Found\r\n");
										#endif
									}
																		
									break;
								}
							}

							//no match
							else //if(streamIn[i] != commandStrings[commandIndex][i])						
								break;							
						}//
					}

					if(alternativeStringMatchFound)
						break;
				}//

				//nothing found in other command strings, so start over
				if(!alternativeStringMatchFound)
				{
					ClearStreamBuffer(serialStreamBuffer);

					ReInitialize_TextCommandParser();

					#ifdef TEXT_COMMAND_PARSER_WRITE_DEBUG
					//WriteDebugString("Match In Progress Failed\r\n");
					#endif
				}
			}
			
			break;
			#pragma endregion

			//FULL_MATCH_FOUND
			#pragma region
			case FULL_MATCH_FOUND:
			
			//copy over any remaining bytes
			if(commandParserControl.commandStringMatchCharIndex < serialStreamBuffer->streamBufferSize)
			{
				serialStreamBuffer->serialStreamBuffer[serialStreamBuffer->nextCharIndex] = streamIn[streamInIndex];

				serialStreamBuffer->nextCharIndex++;

				commandParserControl.commandStringMatchCharIndex++;

				#ifdef TEXT_COMMAND_PARSER_WRITE_DEBUG
				//WriteDebugString("Copy In Progress...\r\n");
				#endif
			}
			
			else
			{
				//debug outputs
				if(commandParserControl.commandStringIndex == 0)
				{
					WriteDebugString("\r\n***led5***\r\n");					
				}

				else if(commandParserControl.commandStringIndex == 4)
				{
					WriteDebugString("\r\n***led3***\r\n");					
				}

				else if(commandParserControl.commandStringIndex == 1)
				{
					WriteDebugString("\r\n***led4***\r\n");
				}

				else if(commandParserControl.commandStringIndex == 7)
				{
					WriteDebugString("\r\n***dan\\r***\r\n");
				}

				ClearStreamBuffer(serialStreamBuffer);

				ReInitialize_TextCommandParser();
								
				#ifdef TEXT_COMMAND_PARSER_WRITE_DEBUG
				//WriteDebugString("Serial Stream Buffer Full, Reset State Machine...\r\n");
				#endif
			}
			
			break;
			#pragma endregion

			//DEFAULT
			#pragma region
			default:

			ClearStreamBuffer(serialStreamBuffer);

			ReInitialize_TextCommandParser();
			
			break;
			#pragma endregion
		}
	}
	
	return 0;
}

void ReInitialize_TextCommandParser()
{
	commandParserControl.commandStringIndex = 0;

	commandParserControl.commandStringMatchCharIndex = 0;

	commandParserControl.commandParserState = INITIAL_MATCH_SEARCH;
}