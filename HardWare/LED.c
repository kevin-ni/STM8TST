

#include "LED.h"

extern RTC_TimeTypeDef GETRTC_Time;
extern RTC_DateTypeDef GETRTC_Data;

void JumpToBoot(){
  disableInterrupts();
  //Work_PeriphOFF();
  //while(1);
  
  asm("LDW X,  SP ");
  asm("LD  A,  $FF");
  asm("LD  XL, A  ");
  asm("LDW SP, X  ");
  asm("JPF $8000");
}

void Work_PeriphOFF(){
      USART_DeInit(USART1);
      USART_DeInit(USART2);
      SPI_DeInit(SPI1);
      CLK_PeripheralClockConfig(CLK_Peripheral_USART1 , DISABLE);
      CLK_PeripheralClockConfig(CLK_Peripheral_USART2, DISABLE);
      CLK_PeripheralClockConfig(CLK_Peripheral_USART3, DISABLE);
      CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, DISABLE);
      CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, DISABLE);  
      CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, DISABLE);  
      CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, DISABLE);  
      CLK_PeripheralClockConfig(CLK_Peripheral_TIM5, DISABLE);  
      CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, DISABLE);  
      CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, DISABLE);
      CLK_PeripheralClockConfig(CLK_Peripheral_SPI2, DISABLE);
      CLK_PeripheralClockConfig(CLK_Peripheral_COMP, DISABLE);  
      CLK_PeripheralClockConfig(CLK_Peripheral_LCD, DISABLE);  
      CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, DISABLE);
      CLK_PeripheralClockConfig(CLK_Peripheral_AES, DISABLE);
      CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);  
      CLK_PeripheralClockConfig(CLK_Peripheral_DAC, DISABLE);
      CLK_PeripheralClockConfig(CLK_Peripheral_BEEP, DISABLE);
      CLK_PeripheralClockConfig(CLK_Peripheral_BOOTROM, DISABLE);
      
      CLK_PeripheralClockConfig(CLK_Peripheral_RTC , DISABLE);
}

void All_IO_Init()
{
  //����IO����͵�ƽ
  GPIO_Init(GPIOA, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOB, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOD, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
  //GPIO_Init(GPIOE, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOF, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
  
  RTC_CLOCK_Init();
}

void Timer4Init()
{
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4 , ENABLE);//ʹ�ܶ�ʱ��4ʱ��
  TIM4_TimeBaseInit(TIM4_Prescaler_128 , //������ʱ��=��ʱ��16MHz / 128 = 125KHz
                    125);                //���ö�ʱ��4Ϊ1��Ƶ�����ϼ���������ֵΪ125��Ϊ1����ļ���ֵ
} 

void TIM4Delayms(u16 ms)
{
  Timer4Init();
  TIM4_ARRPreloadConfig(ENABLE);  //ʹ�ܶ�ʱ��4�Զ����ع��� 
  TIM4->CNTR = 0;
  TIM4_Cmd(ENABLE);               //������ʱ��4��ʼ����
  TIM4_ClearFlag(TIM4_FLAG_Update);
  
  while(ms--)
  {
    while( TIM4_GetFlagStatus(TIM4_FLAG_Update) == RESET); //�ȴ������Ƿ�ﵽ1����
    TIM4_ClearFlag(TIM4_FLAG_Update);  //�������1���룬�����Ӧ�ı�־
  }
  TIM4_Cmd(DISABLE);                   //��ʱȫ���������رն�ʱ��4
  TIM4_DeInit();
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4 , DISABLE);
}

/*******************************************************************************
**�������ƣ�void RTC_CLOCK_Init()
**������������RTC��ʱ��ѡ��Ϊ�ⲿ����ʱ��
**��ڲ�������
**�������
*******************************************************************************/
void RTC_CLOCK_Init()
{
    CLK_LSEConfig(CLK_LSE_ON);    //ʹ��LSE
    while(CLK_GetFlagStatus(CLK_FLAG_LSERDY)== RESET);  //�ȴ�LSE׼������
   
    CLK_RTCClockConfig(CLK_RTCCLKSource_LSE , //ѡ���ⲿ���ٵ�Ƶ32.768KHZʱ��Դ��ΪRTCʱ��Դ
                       CLK_RTCCLKDiv_1        //����Ϊ1��Ƶ
                       );
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC , ENABLE);    //ʹ��ʵʱʱ��RTCʱ��
}

void RTC_Init_TypeDef()
{
  RTC_InitTypeDef  RTCInit;
  RTCInit.RTC_HourFormat = RTC_HourFormat_24;
  RTCInit.RTC_AsynchPrediv = 31;
  RTCInit.RTC_SynchPrediv = 1023;
  RTC_Init(&RTCInit);
  
  RTC_RatioCmd(ENABLE);
}


void RTC_Set(u8 hour , u8 min , u8 second , u8 year ,u8 month ,u8 day ,u8 week)
{
  RTC_TimeTypeDef RTCTime;
  RTC_DateTypeDef RTCData;
  
  RTCTime.RTC_Hours = hour;
  RTCTime.RTC_Minutes = min;
  RTCTime.RTC_Seconds = second;
  RTCTime.RTC_H12 = RTC_H12_AM;     //24
  RTC_SetTime(RTC_Format_BIN , &RTCTime);
  
  RTCData.RTC_WeekDay = (RTC_Weekday_TypeDef)week;   //RTC_Weekday_Sunday;    //sunday
  RTCData.RTC_Month = (RTC_Month_TypeDef)month;    //RTC_Month_August;      //8month
  RTCData.RTC_Date = day;      //14days
  RTCData.RTC_Year = year;
  
  if(RTCData.RTC_WeekDay == 0)
  {
    RTCData.RTC_WeekDay = RTC_Weekday_Monday;
  }
  if(RTCData.RTC_WeekDay > 7)
  {
    RTCData.RTC_WeekDay = RTC_Weekday_Monday;
  }
  RTC_SetDate(RTC_Format_BIN , &RTCData);
}

void Get_TheTime()
{
    RTC_GetDate(RTC_Format_BIN , &GETRTC_Data);  //��ȡ��������
    RTC_GetTime(RTC_Format_BIN , &GETRTC_Time);  //��ȡʱ������
}

u8 crc_8(u8 *ptr, u8 len)
{
    u8 i; 
    u8 crc=0x00; /* ����ĳ�ʼcrcֵ */ 
    while(len--)
    {
        crc ^= *ptr++;  /* ÿ��������Ҫ������������,������ָ����һ���� */  
        for (i=8; i>0; --i)   /* ������μ�����������һ���ֽ�crcһ�� */  
        { 
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc = (crc << 1);
        }
    }
    return (crc); 
}

//--------------------------------------------------------------------------------


//u32 Time30Second = 0;  //10��һ���ж�,��ʱ
INTERRUPT_HANDLER(RTC_CSSLSE_IRQHandler,4)
{
  //Time30Second++;
  //WakeUpScr |= RTC_INT;
  RTC_ClearITPendingBit(RTC_IT_WUT);  //��������жϱ�־
}











