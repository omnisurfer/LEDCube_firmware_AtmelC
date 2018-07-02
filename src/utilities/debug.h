/*
* debug.h
*
* Created: 4/3/2016 2:52:55 PM
*  Author: Daniel
*/


#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>

#include "drivers/ASFUSBCDC/ASFCDCInterface.h"

#define WRITE_DEBUG

void WriteDebugCharArray(char message[], int length);

/**
 * \brief	Writes a string out the CDC serial port line
			How to use:
			The string can be a char array with a null or typed as a literal:
			i.e. "STRING". The max length of the string is 99 characters + null.
			Nothing beyond 100 elements will be written out.

* \param message      Message to write
*
* \return void
*/
void WriteDebugString(char message[]);

void WriteIntToString(int16_t value);

void WriteDebugStringCRNL(void);

#endif /* DEBUG_H_ */