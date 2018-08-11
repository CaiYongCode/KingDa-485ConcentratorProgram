


/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
#ifndef __FLASH_16M_H__
#define __FLASH_16M_H__


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

#define SECTOR_ERASE_4K  0x20
#define BLOCK_ERASE_32K  0x52
#define BLOCK_ERASE_64K  0xD8

//W25X ָ���
#define W25X_WriteEnable_CMD             0x06	   //дʹ��
#define W25X_WriteDisable_CMD            0x04	   //д����
#define W25X_ReadStatusReg_CMD           0x05	   //��״̬�Ĵ���
#define W25X_WriteStatusReg              0x01	   //д״̬�Ĵ���
#define W25X_ReadData_CMD                0x03	   //������
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
�������Ͷ���
*********************************************************************************/

/*********************************************************************************
�ⲿ����������
*********************************************************************************/

/*********************************************************************************
����������
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