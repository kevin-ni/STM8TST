


#ifndef __UARTT_H
#define __UARTT_H


#include "stm8l15x.h"
#include <stdio.h>
#include <string.h>
#include "LED.h"


void UART1_Init(u32 baudrate);
u8 SendDataToUart1(u8 *buf, u8 len);
u8 ReadDataFromUart1(u8 *buf);

#endif






