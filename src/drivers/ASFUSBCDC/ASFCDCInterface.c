/*
* ASFCDCInterface.c
*
* Created: 3/20/2016 6:10:57 PM
*  Author: Daniel
*/

#include "ASFCDCInterface.h"


//look into Interface for application with multi CDC interfaces support for more than one CDC port
static volatile bool app_usb_cdc_open = false;

bool callback_usb_cdc_enable(void)
{
	app_usb_cdc_open = true;
	return true;
}
void callback_usb_cdc_disable(void)
{
	app_usb_cdc_open = false;
}

bool USBSerialPort_IsPortOpen()
{
	return app_usb_cdc_open;
}

bool USBSerialPort_RXReady(void)
{
	return udi_cdc_is_rx_ready();
}

uint16_t USBSerialPort_NumberRXReveived()
{
	return udi_cdc_get_nb_received_data();
}

void USBSerialPort_WriteChar(char data)
{
	if (app_usb_cdc_open)
	{
		udi_cdc_putc(data);
	}
}

//this does not check boundaries so can be very dangerous!
void USBSerialPort_WriteCharArray(char message[], int length)
{
	//iram_size_t udi_cdc_get_nb_received_data(void);
	//iram_size_t udi_cdc_write_buf(const void* buf, iram_size_t size);
	if (app_usb_cdc_open)
	{
		udi_cdc_write_buf(message, length);
	}
}

void USBSerialPort_WriteStringLiteral(char message[])
{
	//find how long the message is
	int messageLength, messageMax = 100;

	for(messageLength = 0; messageLength < messageMax; messageLength++)
	{
		if(message[messageLength] == '\0')
		break;
	}

	USBSerialPort_WriteCharArray(message, messageLength);
}

void USBSerialPort_WriteBuffer(void *bufferPointer, uint16_t bufferSize)
{
	//Default port is 0
	udi_cdc_multi_write_buf(0, bufferPointer, bufferSize);	
}

void USBSerialPort_ReadBuffer(void *bufferPointer, uint16_t bufferSize)
{
	//Default port is 0
	udi_cdc_multi_read_buf(0, bufferPointer, bufferSize);
}