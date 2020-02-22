
#ifndef __LED_H
#define __LED_H

#include "stm8l15x.h"
#include "UART.h"

extern RTC_TimeTypeDef GETRTC_Time;
extern RTC_DateTypeDef GETRTC_Data;

//#define  Version 1000

#define NeedUpdate  0x5A

#define LED_PWR_ON()  {GPIO_WriteBit(GPIOD , GPIO_Pin_1 ,SET);}
#define LED_PWR_OFF() {GPIO_WriteBit(GPIOD , GPIO_Pin_1 ,RESET);}

#define LED_DATA_ON()   {GPIO_WriteBit(GPIOD , GPIO_Pin_2 ,SET);}
#define LED_DATA_OFF()  {GPIO_WriteBit(GPIOD , GPIO_Pin_2 ,RESET);}

#define LASER_ON() GPIO_Init(GPIOD , GPIO_Pin_7 , GPIO_Mode_Out_PP_High_Fast)//GPIO_WriteBit(GPIOD , GPIO_Pin_7 ,SET)
#define LASER_OFF() GPIO_Init(GPIOD , GPIO_Pin_7 , GPIO_Mode_In_FL_No_IT)//GPIO_WriteBit(GPIOD , GPIO_Pin_7 ,RESET)

typedef enum {FAILED = 0, PASSED = !FAILED} Status;

void JumpToBoot();
void Work_PeriphOFF();
void All_IO_Init();
void ADCx_ReadVOL_mV(u16 *MBatmV,u16 *SBatmV);
void RTC_CLOCK_Init();
void RTC_Init_TypeDef();
void RTC_Set(u8 hour , u8 min , u8 second , u8 year ,u8 month ,u8 day ,u8 week);
void Get_TheTime();
void Timer4Init();
void TIM4Delayms(u16 ms);
u8 crc_8(u8 *ptr, u8 len);

#endif


