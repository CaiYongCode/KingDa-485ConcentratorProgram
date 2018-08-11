/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef __IIC_EEPROM_24CXX_H__
#define __IIC_EEPROM_24CXX_H__


/*********************************************************************************
文件包含区
*********************************************************************************/
#include "MCU_Config.h"
#include "SerialPort.h"
#include "..\..\Projects\inc\SystemDefine.h"
#include "..\..\Projects\inc\Common_API.h"
/*********************************************************************************
宏定义区
*********************************************************************************/
#define I2C_Speed              10000
#define EEPROM_ADDRESS        0xA0

#define EEPROM_PageSize          8
  
/*********************************************************************************
数据类型定义
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
函数声明区
*********************************************************************************/
SystemErrName  Launch_24CXX (unsigned char DriverAddr);

void OFF_24CXX (void);

void  EepromPort_rset( unsigned char DriverAddr) ;

//SystemErrName ReadEeprom_24CXX(short address ,unsigned char *readbuff,unsigned short datalen);

SystemErrName EEPROM_I2C_BufferRead(void* Buffer, u8 ReadAddr, u16 NumByteToRead);//将EEPROM的数据读入缓冲器

SystemErrName EEPROM_I2C_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite);//写少于一页的数据

SystemErrName EEPROM_I2C_ByteWrite(unsigned char data, unsigned char WriteAddr);//写一个字节到EEPROM

SystemErrName EEPROM_I2C_BufferWrite(void* Buffer, short WriteAddr, short NumByteToWrite);//将缓冲器的数据写入EEPROM

SystemErrName EEPROM_I2C_MultipleRead(void* Buffer, unsigned short ReadAddr, unsigned short NumByteToRead);

SystemErrName EEPROM_I2C_MultipleWrite(void* Buffer, unsigned short WriteAddr, unsigned short NumByteToWrite);
/********************************************************************************/
#endif