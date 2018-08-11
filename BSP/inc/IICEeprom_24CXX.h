/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
#ifndef __IIC_EEPROM_24CXX_H__
#define __IIC_EEPROM_24CXX_H__


/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "MCU_Config.h"
#include "SerialPort.h"
#include "..\..\Projects\inc\SystemDefine.h"
#include "..\..\Projects\inc\Common_API.h"
/*********************************************************************************
�궨����
*********************************************************************************/
#define I2C_Speed              10000
#define EEPROM_ADDRESS        0xA0

#define EEPROM_PageSize          8
  
/*********************************************************************************
�������Ͷ���
*********************************************************************************/

/*********************************************************************************
�ⲿ����������
*********************************************************************************/

/*********************************************************************************
����������
*********************************************************************************/
SystemErrName  Launch_24CXX (unsigned char DriverAddr);

void OFF_24CXX (void);

void  EepromPort_rset( unsigned char DriverAddr) ;

//SystemErrName ReadEeprom_24CXX(short address ,unsigned char *readbuff,unsigned short datalen);

SystemErrName EEPROM_I2C_BufferRead(void* Buffer, u8 ReadAddr, u16 NumByteToRead);//��EEPROM�����ݶ��뻺����

SystemErrName EEPROM_I2C_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite);//д����һҳ������

SystemErrName EEPROM_I2C_ByteWrite(unsigned char data, unsigned char WriteAddr);//дһ���ֽڵ�EEPROM

SystemErrName EEPROM_I2C_BufferWrite(void* Buffer, short WriteAddr, short NumByteToWrite);//��������������д��EEPROM

SystemErrName EEPROM_I2C_MultipleRead(void* Buffer, unsigned short ReadAddr, unsigned short NumByteToRead);

SystemErrName EEPROM_I2C_MultipleWrite(void* Buffer, unsigned short WriteAddr, unsigned short NumByteToWrite);
/********************************************************************************/
#endif