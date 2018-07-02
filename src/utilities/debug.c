/*
* debug.c
*
* Created: 4/3/2016 2:59:57 PM
*  Author: Daniel
*/

#include "debug.h"

void WriteDebugCharArray(char message[], int length)
{	
	USBSerialPort_WriteCharArray(message, length);
}

void WriteDebugString(char message[])
{	
	USBSerialPort_WriteStringLiteral(message);
}

void WriteIntToString(int16_t value)
{	
	char buffer[8];

	//buffer[15] = 'A';
	//buffer[14] = 'B';

	sprintf(buffer, "%d", value);
	
	WriteDebugString(buffer);		
}

void WriteDebugStringCRNL(void)
{
	WriteDebugString("\r\n");
}