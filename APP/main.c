#include "stm8l15x.h"
#include "Common.h"
#include "UART.h"
#include "UART2.h"

u32 freqMain;
void ClockSwitch_HSI();


#define COM1_R_MAX   (256*2)
#define COM2_R_MAX   (256*2)
static u8 Com1_R_Buf[COM1_R_MAX];
static u8 Com2_R_Buf[COM2_R_MAX];
u8 len1;
u8 len2;

u32 led_count;
u32 com1_rev_all;
u32 com2_rev_all;

u32 com1_send_all;
u32 com2_send_all;

void _Delay(u16 time)
{
  for(u16 i=0; i<time; i++)
  {
    nop();nop();nop();nop();nop();nop();
  }
}

void main()
{
  disableInterrupts();     //关闭系统总中断
  ClockSwitch_HSI();      //采用内部16M高速时钟作为系统时钟源
  Work_PeriphOFF();

  CLK_LSEConfig(CLK_LSE_OFF);
  TIM4Delayms(5);
  All_IO_Init();
  //ReadConfig();
  
  UART1_Init(19200);
  TIM4Delayms(5);
  UART2_Init(19200);
  
  //printf("\r\n\r\nRST_SWIMF\r\n\r\n");
  TIM4Delayms(200);
  RTC_Init_TypeDef();  
  TIM4Delayms(50);
  enableInterrupts();     //使能系统总中断

  //RTC_Set(16,17,0,18,11,13,2);//test debug only
  //TIM4Delayms(200);//test debug only
  //Get_TheTime();  //get RTC time
  
  while(1)
  {
    //freqMain = CLK_GetClockFreq();
    led_count++;
    if(led_count>70)
    {
      LED_PWR_ON();
    }
    if(led_count>75)
    {
      LED_PWR_OFF();
      led_count = 0;
    }
    
//UART 1 -> UART 2   
    len1 = ReadDataFromUart1(Com1_R_Buf);
    if(len1 > 0)
    {
      com1_rev_all += len1;
    }
    if(len1 > 0)
    {
      com1_send_all += SendDataToUart2(Com1_R_Buf, len1);
    }
    _Delay(7000);//baud 38400
    
//UART 2 -> UART 1    
    len2 = ReadDataFromUart2(Com2_R_Buf);
    if(len2 > 0)
    {
      com2_rev_all += len2;
    }
    if(len2 > 0)
    {  
      com2_send_all += SendDataToUart1(Com2_R_Buf, len2);
    }
    _Delay(7000);//baud 38400

  }

}

void ClockSwitch_HSI()
{
  CLK_DeInit();
  CLK_HSEConfig(CLK_HSE_OFF);
  CLK_LSICmd(DISABLE);
  CLK_HSICmd (ENABLE);    //使能HSI
  while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY)== RESET);  //等待HSI准备就绪
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);   //则设置为1分频，16M
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);//系统主时钟为HSI  
  CLK_SYSCLKSourceSwitchCmd(ENABLE);  //使能时钟切换
  while(CLK_GetFlagStatus(CLK_FLAG_SWBSY) != RESET);
  CLK_SYSCLKSourceSwitchCmd(DISABLE);     //关闭切换
}