#include "UART.h"

#define TX1_MAX   (256)
#define RX1_MAX   (256)

static u8 Uart1RxIdx;
static u8 Rx1Buf[RX1_MAX];

static u8 Uart1TxIdx;
static u8 Uart1TxCount;
static u8 Tx1Buf[TX1_MAX];

void UART1_Init(u32 baudrate){   
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1 , ENABLE); //使能USART1时钟
  
  GPIO_Init(GPIOC , GPIO_Pin_2 , GPIO_Mode_In_FL_No_IT );  
  GPIO_Init(GPIOC , GPIO_Pin_3 , GPIO_Mode_Out_PP_High_Fast ); 
  
  USART_Init(USART1,                //设置USART1
    baudrate,               //波特率设置
    USART_WordLength_8b,    //数据长度设为8位
    USART_StopBits_1,       //1位停止位
    USART_Parity_No,        //无校验
    (USART_Mode_Rx | USART_Mode_Tx));//设置为发送模式
  
  USART_ITConfig(USART1, USART_IT_RXNE , ENABLE);  //使能接收中断
  USART_Cmd(USART1 , ENABLE);       //使能USART1开始工作
}

u8 SendDataToUart1(u8 *buf, u8 len)
{
  u8 i = 0;
  
  USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
  
  while(len>0)
  {
    if(Uart1TxCount == (TX1_MAX-1))
    {
      Uart1TxCount=0;
    }else{
      Uart1TxCount++;
    }
    Tx1Buf[Uart1TxCount] = buf[i++];
    
    if(Uart1TxIdx == (Uart1TxCount+1))
    {//overflow
      len = 0;
    }else{
      len--;
    }
  }
  
  USART_ITConfig(USART1, USART_IT_TXE , ENABLE);
  
  return i;
}

INTERRUPT_HANDLER(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler, 27)
{
  if(USART_GetITStatus(USART1, USART_IT_TXE) == SET)
  {
    if(Uart1TxIdx == Uart1TxCount)
    {//send over
      USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    }else{
      if(Uart1TxIdx==(TX1_MAX-1))
      {
        Uart1TxIdx=0;
      }else{
        Uart1TxIdx++;
      }
      
      USART_SendData8(USART1, Tx1Buf[Uart1TxIdx]);   
      USART_ClearITPendingBit(USART1, USART_IT_TXE);
      
      if(Uart1TxIdx == Uart1TxCount)
      {//send over
        USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
      }
    }
  }
}
u8 ReadDataFromUart1(u8 *buf)
{
  u8 ret=0;
  USART_ITConfig(USART1, USART_IT_RXNE , DISABLE);
  if(Uart1RxIdx > 0)
  {
    for(int i=0; i<Uart1RxIdx; i++)
    {
      buf[i] = Rx1Buf[i];
    }
    ret = Uart1RxIdx;
    Uart1RxIdx = 0;
  }
  USART_ITConfig(USART1, USART_IT_RXNE , ENABLE);
  
  return ret;
}

INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler,28)
{
  u8 data;
  
  if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
  {
    data = USART_ReceiveData8(USART1);
    
    Rx1Buf[Uart1RxIdx] = data;
    if(Uart1RxIdx != (RX1_MAX-1))
    {
      Uart1RxIdx++;
    }
    USART_ClearITPendingBit(USART1,USART_IT_RXNE);
  }else if(USART_GetITStatus(USART1,USART_IT_OR) == SET){
    data = 0;
  }else if(USART_GetITStatus(USART1,USART_IT_IDLE) == SET){
    data = 1;
  }else if(USART_GetITStatus(USART1,USART_IT_ERR) == SET){
    data = 2;
  }else if(USART_GetITStatus(USART1,USART_IT_TXE) == SET){
    data = 3;
  }else if(USART_GetITStatus(USART1,USART_IT_TC) == SET){
    data = 4;
  }
}

//处理UART 1的buf
/*
u8 UART_GetCMD()
{
  //u8 wr_ee_flag;
  //u8 res;
  if(RC_Flag&b_rx_over){  //已经接收完一帧?   
    RC_Flag &= ~b_rx_over; //清标志先
    u8 mcrc = crc_8(rx_buffer,rx_buffer[2]+1);
                
    if(mcrc == rx_buffer[rx_buffer[2]+1])
    {
      if(rx_buffer[3] == 0xF1){  //上位机要求升级
        
        u8 BootFlag = NeedUpdate;
        EEPROM_Byte_Write(2048-10,1,&BootFlag);
        UART1_Back_OK(rx_buffer[3]);
        TIM4Delayms(40);
        JumpToBoot();
      }
            
      //if(rx_buffer[3]==0x06){  //发送给从机
      //  u8 i;
      //  for(i=0;i<rx_buffer[4];i++){
      //    sendBuf[i] = rx_buffer[5+i];
      //  }
      //  LORA_Write(sendBuf,rx_buffer[4]);
      //}
      
      return rx_buffer[3];
    }else {
      return 0xff; //CRC Error
    }
  }
  return 0xff;
}
*/








