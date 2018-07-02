/*
* ASFCDCInterface.h
*
* Created: 3/20/2016 6:11:18 PM
*  Author: Daniel
*/


#ifndef ASFCDCINTERFACE_H_
#define ASFCDCINTERFACE_H_

#include "processor/processor.h"
#include "conf_usb.h"

bool USBSerialPort_IsPortOpen(void);

uint16_t USBSerialPort_NumberRXReveived(void);

bool USBSerialPort_RXReady(void); 

void USBSerialPort_WriteChar(char);

void USBSerialPort_WriteCharArray(char* string, int length);

/**
* \brief Writes a string out the USB CDC serial port

How to use:
The string can be a char array with a null or typed as a literal:
i.e. "STRING". The max length of the string is 99 characters + null.
Nothing beyond 100 elements will be written out.

* \param message      Message to write
*
* \return void
*/
void USBSerialPort_WriteStringLiteral(char message[]);

/**
* \brief Writes the buffer out the USB CDC serial port

How to use:
The contents of the buffer are written out without any modification or interpretation.

* \param bufferPointer      pointer to the buffer
* \param bufferSize			size of the buffer (use sizeof() to get correct size of elements in memory)

* \return void
*/
void USBSerialPort_WriteBuffer(void *bufferPointer, uint16_t bufferSize);

/**
* \brief Reads the buffer from the USB CDC serial port

How to use:
The contents of the buffer are read in without any modification or interpretation.

* \param bufferPointer      pointer to the buffer
* \param bufferSize			size of the buffer (use sizeof() to get correct size of elements in memory)

* \return void
*/
void USBSerialPort_ReadBuffer(void *bufferPointer, uint16_t bufferSize);

#endif /* ASFCDCINTERFACE_H_ */