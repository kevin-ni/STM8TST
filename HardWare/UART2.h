


#ifndef __UARTT2_H
#define __UARTT2_H


#include "stm8l15x.h"
#include <stdio.h>
#include <string.h>
#include "LED.h"


void UART2_Init(u32 baudrate);
u8 SendDataToUart2(u8 *buf, u8 len);
u8 ReadDataFromUart2(u8 *buf);

#endif

