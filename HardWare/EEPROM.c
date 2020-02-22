
#include "EEPROM.h"

u16 EEPROM_STARTADDR = 0x1000;//2K_EEPROM  4K_RAM   64K_flash

struct GLOBAL_DATA  gConfig;//old action

RTC_TimeTypeDef GETRTC_Time;
RTC_DateTypeDef GETRTC_Data;

/******************************************************************************/
u8 EEPROM_Byte_Write(u16 address , u16 length, u8 *data)
{
  u16 eepromaddress = EEPROM_STARTADDR + address;
  u16 T;
  //u16 len;
  FLASH_SetProgrammingTime(FLASH_ProgramTime_TProg); //�趨���ʱ��Ϊ��׼���ʱ��
  //MASS ��Կ�����EEPROM�ı���
  FLASH_Unlock(FLASH_MemType_Data);
  while(length--){   
    T=0;  
    FLASH_ProgramByte(eepromaddress , *data);  //������д����Ӧ�Ĵ洢��ַ
    while((FLASH_GetFlagStatus(FLASH_FLAG_EOP) == 1)&&(T<0xFFFE));     //�ȴ���̽���
      eepromaddress++;data++;
  }
  //if(length>10000) length=10000;
  //len = length*5;
  //TIM4Delayms(len);
  TIM4Delayms(30);
  return 0;
}

void EEPROM_Byte_Read(u16 address , u16 length, u8 *data)
{
  u16 eepromaddress = EEPROM_STARTADDR + address;
  //MASS ��Կ�����EEPROM�ı���
  FLASH_Unlock(FLASH_MemType_Data);
  while(length--){    
    *data = FLASH_ReadByte(eepromaddress++);
    data++;
  }
  TIM4Delayms(10);
}

uint8_t WriteConfig(void){
  u8 *ptr = &gConfig.IsAct;
  EEPROM_Byte_Write(OLDCONFIGSTART,sizeof(gConfig),ptr);
  return 0;
}

uint8_t ReadConfig(void){
  u8 *ptr = &gConfig.IsAct;
  EEPROM_Byte_Read(OLDCONFIGSTART,sizeof(gConfig),ptr);
  if(gConfig.isWred != 0x1234A55A){
    gConfig.isWred = 0x1234A55A;
    gConfig.Setting = AutoSend | DefaultADD | Baud115200;
    WriteConfig();
  }
  return 0;
}
