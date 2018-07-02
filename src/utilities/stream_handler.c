/*
 * stream_handler.c
 *
 * Created: 5/7/2016 3:00:47 PM
 *  Author: Daniel
 */ 

 #include "stream_handler.h"

 RXStreamHandler _rxStreamHandler;

 void SetRXStreamHandler(RXStreamHandler rxStreamHandler)
 {
	 _rxStreamHandler = rxStreamHandler;
	 return;
 }

 RXStreamHandler GetRXStreamHandler(void)
 {
	 return _rxStreamHandler;
 }

 void ClearStreamBuffer(STREAM_BUFFER *serialStreamBuffer)
 {
	 for(int i = 0; i < serialStreamBuffer->nextCharIndex; i++)
	 {
		 serialStreamBuffer->serialStreamBuffer[i] = '\0';
	 }

	 serialStreamBuffer->nextCharIndex = 0;
 }