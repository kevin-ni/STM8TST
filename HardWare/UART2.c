#include "UART2.h"

#define TX2_MAX   (256)
#define RX2_MAX   (256)

static u8 Uart2RxIdx;
static u8 Rx2Buf[RX2_MAX];

static u8 Uart2TxIdx;
static u8 Uart2TxCount;
static u8 Tx2Buf[TX2_MAX];

void UART2_Init(u32 baudrate){
  CLK_PeripheralClockConfig(CLK_Peripheral_USART2 , ENABLE); //使能USART2时钟
  
  GPIO_Init(GPIOE, GPIO_Pin_3, GPIO_Mode_In_FL_No_IT );  
  GPIO_Init(GPIOE, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast ); 
  
  USART_Init(USART2,                //设置USART1
            baudrate,               //波特率设置
            USART_WordLength_8b,    //数据长度设为8位
            USART_StopBits_1,       //1位停止位
            USART_Parity_No,        //无校验
            (USART_Mode_Rx | USART_Mode_Tx));         //设置为发送模式
  
  USART_ITConfig(USART2, USART_IT_RXNE , ENABLE);  //使能接收中断
  USART_Cmd(USART2 , ENABLE);       //使能USART1开始工作
}

u8 SendDataToUart2(u8 *buf, u8 len)
{
  u8 i = 0;
  
  USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
  
  while(len>0)
  {
    if(Uart2TxCount == (TX2_MAX-1))
    {
      Uart2TxCount=0;
    }else{
      Uart2TxCount++;
    }
    Tx2Buf[Uart2TxCount] = buf[i++];
    
    if(Uart2TxIdx == (Uart2TxCount+1))
    {//overflow
      len = 0;
    }else{
      len--;
    }
  }
  
  USART_ITConfig(USART2, USART_IT_TXE , ENABLE);
  
  return i;
}

INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler, 19)
{
  if(USART_GetITStatus(USART2, USART_IT_TXE) == SET)
  {
    if(Uart2TxIdx == Uart2TxCount)
    {//send over
      USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
    }else{
      if(Uart2TxIdx==(TX2_MAX-1))
      {
        Uart2TxIdx=0;
      }else{
        Uart2TxIdx++;
      }
      
      USART_SendData8(USART2, Tx2Buf[Uart2TxIdx]);   
      USART_ClearITPendingBit(USART2, USART_IT_TXE);
      
      if(Uart2TxIdx == Uart2TxCount)
      {//send over
        USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
      }
    }
  }
}

u8 ReadDataFromUart2(u8 *buf)
{
  u8 ret=0;
  USART_ITConfig(USART2, USART_IT_RXNE , DISABLE);
  if(Uart2RxIdx > 0)
  {
    for(int i=0; i<Uart2RxIdx; i++)
    {
      buf[i] = Rx2Buf[i];
    }
    ret = Uart2RxIdx;
    Uart2RxIdx = 0;
  }
  USART_ITConfig(USART2, USART_IT_RXNE , ENABLE);
  
  return ret;
}

INTERRUPT_HANDLER(TIM2_CC_USART2_RX_IRQHandler, 20)
{
  u8 data;
  
  if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
  {
    data = USART_ReceiveData8(USART2);
    
    Rx2Buf[Uart2RxIdx] = data;
    if(Uart2RxIdx != (RX2_MAX-1))
    {
      Uart2RxIdx++;
    }
    USART_ClearITPendingBit(USART2,USART_IT_RXNE);
  }else if(USART_GetITStatus(USART2,USART_IT_OR) == SET){
    data = 0;
  }else if(USART_GetITStatus(USART2,USART_IT_IDLE) == SET){
    data = 1;
  }else if(USART_GetITStatus(USART2,USART_IT_ERR) == SET){
    data = 2;
  }else if(USART_GetITStatus(USART2,USART_IT_TXE) == SET){
    data = 3;
  }else if(USART_GetITStatus(USART2,USART_IT_TC) == SET){
    data = 4;
  }
}



