


/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef __FLASH_16M_H__
#define __FLASH_16M_H__


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

#define SECTOR_ERASE_4K  0x20
#define BLOCK_ERASE_32K  0x52
#define BLOCK_ERASE_64K  0xD8

//W25X 指令定义
#define W25X_WriteEnable_CMD             0x06	   //写使能
#define W25X_WriteDisable_CMD            0x04	   //写禁能
#define W25X_ReadStatusReg_CMD           0x05	   //读状态寄存器
#define W25X_WriteStatusReg              0x01	   //写状态寄存器
#define W25X_ReadData_CMD                0x03	   //读数据
#define W25X_FastReadData_CMD            0x0B
#define W25X_FastReadDual_CMD            0x3B
#define W25X_PageProgram_CMD             0x02
#define W25X_BlockErase_CMD              0xD8
#define W25X_SectorErase_CMD             0x20
#define W25X_ChipErase_CMD               0xC7
#define W25X_PowerDown_CMD               0xB9
#define W25X_ReleasePowerDown_CMD        0xAB
#define W25X_DeviceID_CMD                0xAB
#define W25X_ManufacturerDeviceID_CMD    0x90
#define W25X_JedecID_CMD                 0x9F
/*********************************************************************************
数据类型定义
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
函数声明区
*********************************************************************************/
void Launch_W25QXX (void);
unsigned char WriteReadByte_W25QXX ( unsigned char dt);
void Flash_SR_Init(void );
void Flash_Read(unsigned long address ,unsigned char *readbuff,unsigned int datalen);
void Flash_Write(unsigned long address ,unsigned char *writebuff,unsigned short datalen);
void Page_Erase(unsigned char CMD ,unsigned long address);
void Flash_ReadID(unsigned char *readbuff);
void Flash_readSR(unsigned char *readbuff);

void SectorErase_W25Q( unsigned long Dst_adr );
void ReadData_W25Q( unsigned long adr, unsigned char * pbuff, unsigned short NumByte );
void Writedata_NoCheck_W25Q( unsigned long adr, unsigned char * pbuff, unsigned short NumByte );
void Writedata_Check_Sector_W25Q( unsigned long adr, unsigned char * pbuff, unsigned long NumByte );

/********************************************************************************/
#endif