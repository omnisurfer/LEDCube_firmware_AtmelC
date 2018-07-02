/*
* LEDCube.c
*
* Created: 10/31/2015 4:30:30 PM
*  Author: Daniel
*/
#include <string.h>
//#include <stdio.h>

#include "processor/processor.h"
#include "processor/interrupts.h"
#include "processor/sysclk.h"

#include "drivers/ws2812/ws2812.h"
#include "drivers/ASFUSBCDC/ASFCDCInterface.h"

#include "utilities/port_hal.h"
#include "utilities/timers.h"
#include "utilities/debug.h"
#include "utilities/text_cmd_parser.h"
#include "utilities/stream_handler.h"

#include "devices/LEDCube.h"

void InitTimer(void);

//void InitInterrupts(void);

#define CUBE_LAYER_LEDS_PER_LINE	4
#define CUBE_LAYER_LINES			4
#define CUBE_LAYER_COUNT			4
#define NUM_OF_LEDS_PER_CUBE		CUBE_LAYER_LEDS_PER_LINE * CUBE_LAYER_LINES * CUBE_LAYER_COUNT

#define versionString "0.0.3"

static char firstRunOutputString[] = "LED Cube Controller Code v" versionString " " __TIMESTAMP__ "\r\n";
static char promptString[] = "\nledCube-" versionString "$ ";

int main(void)
{
	irq_initialize_vectors();
	cpu_irq_enable();

	//init IRQ
	//InitInterrupts();
	
	//init Clock - works with current code
	//try http://www.avrfreaks.net/forum/xmegaa3u-usb-and-clock-options
	InitSysClk();

	//does not work with current code - ASF is broken for setting PLL as clock
	//sysclk_init();
	
	//init Board
	//init_board()
	
	//init Peripherals
	InitTimer();
	
	//start the ASF USB CDC service
	udc_start();

	//DEBUG - Set PORTR direction - 0, 7 output
	SetPortPinDirection(PORT_A, PIN_0, PORT_OUTPUT);
	SetPortPinDirection(PORT_A, PIN_7, PORT_OUTPUT);
	
	LED_CUBE_DEFINITION LEDCube =
	{
		.cubeOrigin = BOTTOM_LEFT,
		.cubeClimbDirection = FLOOR_TO_CEILING,
		.cubePathLayout = SWITCHBACK,
		.cubeTravelDirection = LEFT_TO_RIGHT,
		.cubeBridging = NONE,
		.cubeLayerLEDsPerLine = CUBE_LAYER_LEDS_PER_LINE,
		.cubeLayerLineCount = CUBE_LAYER_LINES,
		.cubeLayerCount = CUBE_LAYER_COUNT
	};
	
	WS2812_GRB_BYTES ledCubeArrayWorkingBuffer[CUBE_LAYER_COUNT * CUBE_LAYER_LEDS_PER_LINE * CUBE_LAYER_LINES] =
	{
		//{{0}}
		//Layer 1
		{{0x7f, 0x00, 0x00}},
		{{0x7f, 0x00, 0x00}},
		{{0x7f, 0x00, 0x00}},
		{{0x7f, 0x00, 0x00}},

		{{0x00, 0x7f, 0x00}},
		{{0x00, 0x7f, 0x00}},
		{{0x00, 0x7f, 0x00}},
		{{0x00, 0x7f, 0x00}},

		{{0x00, 0x00, 0x7f}},
		{{0x00, 0x00, 0x7f}},
		{{0x00, 0x00, 0x7f}},
		{{0x00, 0x00, 0x7f}},

		{{0x7f, 0x7f, 0x7f}},
		{{0x7f, 0x7f, 0x7f}},
		{{0x7f, 0x7f, 0x7f}},
		{{0x7f, 0x7f, 0x7f}},
		
		//Layer 2
		{{0x7f, 0x00, 0x00}},
		{{0x7f, 0x00, 0x00}},
		{{0x7f, 0x00, 0x00}},
		{{0x7f, 0x00, 0x00}},

		{{0x00, 0x7f, 0x00}},
		{{0x00, 0x7f, 0x00}},
		{{0x00, 0x7f, 0x00}},
		{{0x00, 0x7f, 0x00}},

		{{0x00, 0x00, 0x7f}},
		{{0x00, 0x00, 0x7f}},
		{{0x00, 0x00, 0x7f}},
		{{0x00, 0x00, 0x7f}},

		{{0x7f, 0x7f, 0x7f}},
		{{0x7f, 0x7f, 0x7f}},
		{{0x7f, 0x7f, 0x7f}},
		{{0x7f, 0x7f, 0x7f}},

		//Layer 3
		{{0x7f, 0x00, 0x00}},
		{{0x7f, 0x00, 0x00}},
		{{0x7f, 0x00, 0x00}},
		{{0x7f, 0x00, 0x00}},

		{{0x00, 0x7f, 0x00}},
		{{0x00, 0x7f, 0x00}},
		{{0x00, 0x7f, 0x00}},
		{{0x00, 0x7f, 0x00}},

		{{0x00, 0x00, 0x7f}},
		{{0x00, 0x00, 0x7f}},
		{{0x00, 0x00, 0x7f}},
		{{0x00, 0x00, 0x7f}},

		{{0x7f, 0x7f, 0x7f}},
		{{0x7f, 0x7f, 0x7f}},
		{{0x7f, 0x7f, 0x7f}},
		{{0x7f, 0x7f, 0x7f}},

		//Layer 4
		{{0x7f, 0x00, 0x00}},
		{{0x7f, 0x00, 0x00}},
		{{0x7f, 0x00, 0x00}},
		{{0x7f, 0x00, 0x00}},

		{{0x00, 0x7f, 0x00}},
		{{0x00, 0x7f, 0x00}},
		{{0x00, 0x7f, 0x00}},
		{{0x00, 0x7f, 0x00}},

		{{0x00, 0x00, 0x7f}},
		{{0x00, 0x00, 0x7f}},
		{{0x00, 0x00, 0x7f}},
		{{0x00, 0x00, 0x7f}},

		{{0x7f, 0x7f, 0x7f}},
		{{0x7f, 0x7f, 0x7f}},
		{{0x7f, 0x7f, 0x7f}},
		{{0x7f, 0x7f, 0x7f}}
	};

	WS2812_GRB_BYTES ledCubeArrayOutputBuffer[CUBE_LAYER_COUNT * CUBE_LAYER_LEDS_PER_LINE * CUBE_LAYER_LINES] =
	{
		{{0}}
	};

	/*
	WS2812_GRB_BYTES ledCubeArrayBlank[CUBE_LAYER_COUNT * CUBE_LAYER_LEDS_PER_LINE * CUBE_LAYER_LINES] =
	{
	{{0}}
	};
	*/

	LED_CUBE_RENDER_CONTAINER ledCubeContainer =
	{
		.renderStatus = RENDER_PENDING,
		.cubeDefinition = LEDCube,
		.ledBytes = ledCubeArrayOutputBuffer
	};
		
	STREAM_BUFFER streamBuffer = 
	{	
		.streamBufferSize = SERIAL_STREAM_BUFFER_SIZE,
		.nextCharIndex = 0,
		.serialStreamBuffer = {0}
	};

	//Task Counters
	uint8_t cubeDrawTaskCounter = 0, cdcReceiveTaskCounter = 0;

	//Temporary values for rendering to the LEDs
	uint8_t red = 255, blue = 0, green = 0, ledWalkCount = 0;

	//set RX Stream Handler to command parser
	SetRXStreamHandler(RX_STREAM_HANDLER_COMMAND_PARSER);

	//bool to tell if startup initialization is complete
	bool startupInitComplete = false;

	//#pragma region note
	#pragma region TEST
	/*Example pragma use set to ignore pragma warnings: -Wno-unknown-pragmas
	http://stackoverflow.com/questions/12894454/how-to-remove-gcc-warning-on-pragma-region
	*/
	#pragma endregion
	
	while(true)
	{		
		if(!startupInitComplete)
		{
			if(USBSerialPort_IsPortOpen())
			{
				//Write out a message to indicate the code has started execution of the main loop
				WriteDebugString(firstRunOutputString);
				startupInitComplete = true;
			}
		}		

		//Serial Stream Receive Task
		#pragma region
		//Test code for receiving serial data from USB CDC and sending it back out.
		if(cdcReceiveTaskCounter >= 5)
		{
			cdcReceiveTaskCounter = 0;
			
			//Need to make a text mode that acts like Linux command line. Nothing is passed to the TextCommandParser until the a carriage return is sent. Otherwise, it will keep track of backspaces and such.
			//Stream mode will be entered through a command

			if(USBSerialPort_RXReady())
			{				
				uint16_t usbSerialPortBytesReceived = USBSerialPort_NumberRXReveived();

				char usbSerialBuffer[usbSerialPortBytesReceived];
								
				USBSerialPort_ReadBuffer(usbSerialBuffer, usbSerialPortBytesReceived);
																
				switch(GetRXStreamHandler())
				{
					case RX_STREAM_HANDLER_COMMAND_PARSER:
						RXStreamHandler_TextCommandParser(usbSerialBuffer, (uint8_t)usbSerialPortBytesReceived, &streamBuffer);													
					break;

					case RX_STREAM_HANDLER_LED_CUBE:
					
					break;

					default:
					
					break;
				}

				//USBSerialPort_WriteStringLiteral("\r\n");
				//USBSerialPort_WriteBuffer(streamBuffer.serialStreamBuffer, sizeof(streamBuffer.serialStreamBuffer));				
				USBSerialPort_WriteBuffer(usbSerialBuffer, sizeof(usbSerialBuffer));

				//check for a carriage return, and insert a newline to make things more readable
				if(usbSerialBuffer[usbSerialPortBytesReceived - 1] == '\r')
					USBSerialPort_WriteStringLiteral(promptString);

			}
		}
		#pragma endregion

		//Cube Draw Task
		#pragma region
		if(cubeDrawTaskCounter >= 100)
		{
			cubeDrawTaskCounter = 0;

			//Basic LED Cube Test Pattern
			#pragma region
			//write from the ledCubeArrayOneLayerWorkingBuffer filled with a test pattern
			if(0)
			{
				//PORTA_OUT |= 0x01;
				//copy working buffer to output buffer
				memcpy(ledCubeArrayOutputBuffer, ledCubeArrayWorkingBuffer, sizeof(ledCubeArrayWorkingBuffer));
				
				LEDCubeArrayRemap(&LEDCube, ledCubeArrayOutputBuffer);
				
				LEDCubeWriteToCube(&LEDCube, ledCubeArrayOutputBuffer);

				//PORTA_OUT &= 0xFE;
			}
			#pragma endregion

			//Basic LED Cube Test Pattern
			#pragma region
			//Start moving a red LED from left to right 0,0 to 4,4. One is added so the 15th LED can be cleared (15+1 = 16 counts)
			if(1)
			{
				if(ledWalkCount <= NUM_OF_LEDS_PER_CUBE)
				{
					//clear the array and initialize first pixel as green
					if(ledWalkCount == 0)
					{
						ledCubeArrayWorkingBuffer[0].data[0] = green;
						ledCubeArrayWorkingBuffer[0].data[1] = red;
						ledCubeArrayWorkingBuffer[0].data[2] = blue;
					}
					
					else if(ledWalkCount < NUM_OF_LEDS_PER_CUBE)
					{
						ledCubeArrayWorkingBuffer[ledWalkCount].data[0] = ledCubeArrayWorkingBuffer[ledWalkCount-1].data[0];
						ledCubeArrayWorkingBuffer[ledWalkCount].data[1] = ledCubeArrayWorkingBuffer[ledWalkCount-1].data[1];
						ledCubeArrayWorkingBuffer[ledWalkCount].data[2] = ledCubeArrayWorkingBuffer[ledWalkCount-1].data[2];

						ledCubeArrayWorkingBuffer[ledWalkCount-1].data[0] = 0;
						ledCubeArrayWorkingBuffer[ledWalkCount-1].data[1] = 0;
						ledCubeArrayWorkingBuffer[ledWalkCount-1].data[2] = 0;
					}
					
					else
					{
						ledCubeArrayWorkingBuffer[ledWalkCount-1].data[0] = 0;
						ledCubeArrayWorkingBuffer[ledWalkCount-1].data[1] = 0;
						ledCubeArrayWorkingBuffer[ledWalkCount-1].data[2] = 0;
					}
					
					++ledWalkCount;
					
					if(ledWalkCount > NUM_OF_LEDS_PER_CUBE)
					ledWalkCount = 0;

					//copy working buffer to output buffer
					memcpy(ledCubeArrayOutputBuffer, ledCubeArrayWorkingBuffer, sizeof(ledCubeArrayWorkingBuffer));

					//remap the just made array
					LEDCubeArrayRemap(&LEDCube, ledCubeArrayOutputBuffer);
					
					LEDCubeWriteToCube(&LEDCube, ledCubeArrayOutputBuffer);									
				}
			}
			#pragma endregion
		}
		#pragma endregion
		
		//Timer Tick Task - 1 count ~1ms
		#pragma region
		if(timerTick)
		{
			timerTick = false;
			cubeDrawTaskCounter++;
			cdcReceiveTaskCounter++;
		}
		#pragma endregion
	}
}