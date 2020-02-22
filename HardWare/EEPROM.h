

#ifndef	_AT24C256_H
#define	_AT24C256_H
#include "stm8l15x.h"
#include "LED.h"

#define OLDCONFIGSTART  0

#define EEPROM_GATEWAY_PARA   (OLDCONFIGSTART+sizeof(gConfig))
#define EEPROM_SENSOR_REG     (EEPROM_GATEWAY_PARA+sizeof(gGatewayInfo))

#define EEPROM_WHITELIST_TOTAL (EEPROM_SENSOR_REG + MAXSENSORSNUM*sizeof(gSensorRegBuffer)+ 2)
#define EEPROM_WHITELIST_START (EEPROM_WHITELIST_TOTAL + 4)
#define MAXSENSORSNUM    20    //ż�� �Ķ����ֵ��Ҫ�Ķ���ز���(��Ҫ)
#define DEFGATEWAYFREQ   436    
#define MINSENSORSINT    1      //��С���������
#define TICKS            (MINSENSORSINT*60/MAXSENSORSNUM)  //3��
#define MAXMAGICNUM      4

//GLOBAL_DATA gConfig;
struct GLOBAL_DATA{
  u8 IsAct;
  u8 temp1;
  u8 temp2;
  u8 temp3;
  uint32_t isWred;
  uint32_t Setting;
};

//���ز���
//GATEWAY_PARA gGatewayInfo;
struct GATEWAY_PARA{
  u16 isAct;
  u8 gatewayId;//����ID
  u8 totalNum;//����Ŀǰ��ע���ܵ�sensor�� <maxNum
  u16 gatewayfreq;//����Fre
  u8 temp1;
  u8 crc;
};

extern struct GLOBAL_DATA gConfig;

#define AutoSend   	0x80
#define UnAuto     	0x00
#define DefaultADD  14
#define Baud9600   	0x00
#define Baud115200  0x40

uint8_t WriteConfig(void);
uint8_t ReadConfig(void);

void EEPROM_Byte_Read(u16 address , u16 length, u8 *data);
u8 EEPROM_Byte_Write(u16 address , u16 length, u8 *data);
u8 EEPROM_ReadGateway_AllPara();
u8 EEPROM_ReadSensorReg_AllPara();
u8 EEPROM_ClearSensorReg_AllPara();
u8 EEPROM_DelSensorReg_Para(u8 index);
//void SetLoraFreq(u16 Freq);
u8 accordingIdFindIndexSensorInfo(u8 id);
u8 accordingIdFindIndexSensorReg(u8 id);
u8 accordingNameFindIndexSensorReg(u8* name);
u8 EEPROM_CopySensorReg_Para(u8 src_index, u8 des_index);
//u8 accordingIdFindIndexSensorInfo(u8 id);
u8 Ram_DelSensorInfo(u8 index);
u8 Ram_CopySensorInfo(u8 src_index, u8 des_index);
u8 EEPROM_WriteGateway_AllPara();
u8 GetTickFinalID(u8 index);
u8 EEPROM_AddSensorReg_Para(u8 index);
void EEPROM_InitGateway_AllPara();
u8 WriteWhiteListTotalNumber(u8 num);
u8 ReadWhiteListTotalNumber();
u8 WriteWhiteListName(u8 index, u8* name);
u8 ReadWhiteListName(u8 index, u8* name);
u8 getMagicCmdEmptyIndex();

#endif
