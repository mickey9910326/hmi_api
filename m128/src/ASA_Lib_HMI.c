// Last Build : 2017/08/15 11:55
// Author : Chu LiYu
// encode in utf-8

#include "ASA_Lib_HMI.h"
#include <avr\io.h>
#include <avr\interrupt.h>
#include <stdio.h>
#include <util\delay.h>
#include <string.h>
#include <inttypes.h>
#include "bit_op.h"

// baud 38400 -> 1 bit trismit use 1/38400=2.604e-5 sec
// baud 38400 -> 1 byte trismit use 8/38400=2.08e-4 sec
// timer is 11059200UL
// 11059200/15 =         737280
// uint32_t = 2^32 = 4294967296

// register define
#define HMI_UCSRA UCSR0A
#define HMI_UDR UDR0

// pin number define
#define HMI_UDRE UDRE0
#define HMI_RXC RXC0

//
#define ONESEC_COUNT 921

// global variable
uint32_t timeout_count=0;
uint32_t Timeout=921UL*1000UL;
uint8_t TimeoutMode=1;

// function
void uart_put(uint8_t data);
uint8_t uart_get(void);
char HMI_timeout_set(int count);
char M128_HMI_set(char LSBytes, char Mask, char shift, char Data);
char M128_HMI_put(char Bytes, char Type, void *Data_p);
char M128_HMI_get(char Bytes, void *Data_p);
char M128_HMI_Form_put(char *FormatString, char Bytes, void *Data_p);
char M128_HMI_Form_get(char *FormatString, char Bytes, void *Data_p);
char update_status(char Bytes, char* pResumingMode, char* pPackBytes, char* pBatchesBytes);

// function content
void uart_put( unsigned char data )
{
	while ( bit_is_clear(HMI_UCSRA, HMI_UDRE));
	HMI_UDR = data;
}

uint8_t uart_get( void )
{
	while ( bit_is_clear(HMI_UCSRA, HMI_RXC));
	return HMI_UDR;
}

char M128_HMI_set(char LSBytes, char Mask, char shift, char Data) {
	if (LSBytes < 200 || LSBytes>200) {
		return 1;
	}
	switch (LSBytes) {
		case 200:
			if (Mask != 0xFF || shift != 0) {
				return 1;
			}
			Timeout = Data;
			break;
		case 201:
			if (Mask != 0x01 || shift != 0) {
				return 1;
			}
			TimeoutMode = Data;
			break;
	}
	return 0;
	// error code:
	//   0 - correct
	//   1 - wrong settings of LSBytes, Mask, shift
};

char HMI_timeout_set(int count)
{
	if (count < 50 || count > 30000) {
		return 1;
	}
	Timeout = ONESEC_COUNT * count;
	return 0;
}


char M128_HMI_put(char Bytes, char Type, void *Data_p)
{
	static char BatchesBytes ;
	static char ResumingMode = 0;
	char PackBytes;
	char res = 0;
	// res = update_status(Bytes,&ResumingMode,&PackBytes,&BatchesBytes);
	// if (res) {
	// 	ResumingMode = 0;
	// 	return res;
	// }

	uint8_t i = 0;
    uint8_t CheckSum = Bytes;
    PackBytes = Bytes;

	uart_put( 0xAA );
	uart_put( 0xAA );
	uart_put( 0xAA );
	uart_put( Type );
	uart_put( PackBytes );

	for(i=0; i<PackBytes; i++)
	{
		uart_put( ((char*)Data_p)[i] );
		CheckSum += ((char*)Data_p)[i];
	}
	uart_put( CheckSum );

	return 0;
}

char M128_HMI_get(char Bytes, void *Data_p)
{
	uint8_t PackBytes = Bytes;
	uint8_t i, GetBytes, CheckSum, GetCheckSum, GetType;

	// if( Bytes > 32 )
	// 	return 1;

	if( uart_get() != 0xAB )
		return 2;
	if( uart_get() != 0xAB )
		return 2;
	if( uart_get() != 0xAB )
		return 2;
	GetType  = uart_get();
	GetBytes = uart_get();
	CheckSum = GetBytes;
	for(i=0; i<PackBytes; i++)
	{
		((char*)Data_p)[i] = uart_get();
		CheckSum += ((char*)Data_p)[i];
	}
	GetCheckSum = uart_get();

	if( GetBytes != PackBytes )
		return 3;

	if( GetCheckSum != CheckSum )
		return 4;

	return 0;
}

char M128_HMI_Form_put(char *FormatString, char Bytes, void *Data_p)
{
	unsigned char i, FormatBytes = 0, CheckSum = 0;

	uart_put( 0xBB );
	uart_put( 0xBB );
	uart_put( 0xBB );

	FormatBytes = strlen(FormatString);

	if( (1+(int)FormatBytes+(int)Bytes)>255 )	// 資料結構字串長度過長
		return 1;

	uart_put( 1+FormatBytes+Bytes );
	CheckSum = 1+FormatBytes+Bytes;

	uart_put( FormatBytes );
	CheckSum += FormatBytes;

	for(i=0; i<FormatBytes; i++)
	{
		uart_put( ((char*)FormatString)[i] );
		CheckSum += ((char*)FormatString)[i];
	}

	for(i=0; i<Bytes; i++)
	{
		uart_put( ((char*)Data_p)[i] );
		CheckSum += ((char*)Data_p)[i];
	}
	uart_put( CheckSum );

	return 0;
}

char M128_HMI_Form_get(char *FormatString, char Bytes, void *Data_p)
{
	unsigned char i, FormatBytes = 0, GetTotalBytes, GetFormatBytes, GetBytes, CheckSum, GetCheckSum;

	if( (1+(int)FormatBytes+(int)Bytes)>255 )	// 資料結構字串長度過長
		return 1;

	if( uart_get() != 0xAB )
		return 2;
	if( uart_get() != 0xAB )
		return 2;
	if( uart_get() != 0xAB )
		return 2;
	GetTotalBytes = uart_get();
	CheckSum = GetTotalBytes;

	GetFormatBytes = uart_get();
	CheckSum = GetFormatBytes;
	FormatBytes = strlen(FormatString);

	GetBytes = GetTotalBytes - GetFormatBytes;

	for(i=0; i<FormatBytes; i++)
	{
		((char*)FormatString)[i] = uart_get();
		CheckSum += ((char*)FormatString)[i];
	}

	for(i=0; i<Bytes; i++)
	{
		((char*)Data_p)[i] = uart_get();
		CheckSum += ((char*)Data_p)[i];
	}
	GetCheckSum = uart_get();

	if( GetBytes != Bytes )
		return 3;

	if( GetCheckSum != CheckSum )
		return 4;

	return 0;
}


/** NOTE
 * No ResumingMode now, the function still has problem.
 */

// char update_status(char Bytes, char* pResumingMode, char* pPackBytes, char* pBatchesBytes) {
// 	// 一批封包，組成一個完整的資料
// 	// a batch of packet build a complete data
// 	// 封包bytes上限為32
// 	// packet max Byte is 32
// 	// 一批資料bytes上限為255
// 	// pResumingMode
// 	// 續傳模式
// 	//  0 - 未開啟
// 	//  1 - 第一包資料
// 	//  2 - 中間資料，每包需32bytes
// 	//  3 - 最後一包資料
// 	if (Bytes <= 32 && *pResumingMode == 0) { // no ResumingMode
// 		*pPackBytes = Bytes;
// 	} else if( Bytes > 32 && *pResumingMode == 0) { // first packages
// 		*pBatchesBytes = Bytes;
// 		*pPackBytes = 32;
// 		*pResumingMode = 1;
// 		*pBatchesBytes -= *pPackBytes;
// 	} else if( Bytes == 0 && *pResumingMode == 1 ) { // medium packages
// 		*pPackBytes = 32;
// 		*pResumingMode = 1;
// 		*pBatchesBytes -= *pPackBytes;
// 		if (*pBatchesBytes > 32) {
// 			*pResumingMode = 1;
// 		} else {
// 			*pResumingMode = 2;
// 		}
// 	} else if ( Bytes > 0 && *pResumingMode == 2) { // last package
// 		*pPackBytes = Bytes;
// 		*pResumingMode = 0;
// 	} else {
// 		return 1;
// 	}
// 	return 0;
// }
