/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef  __EQUIP_MGMT_H
#define  __EQUIP_MGMT_H 
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\..\SPCP\SPCP_Config.h"
/*********************************************************************************
宏定义区
*********************************************************************************/


/*********************************************************************************
数据类型定义
*********************************************************************************/
//中端节点协议类型
typedef enum{
  UnFind   = 0xFF,
  KingDa   = 1,
  CJT_188  = 2,
  DLT_645  = 3,
  ModBus   = 4,
}Equip_SPCP_type;
 
typedef union {
    KingDaEquipInfor_struct   KingDaNode;
  } EquipRecordData_struct;
//设备描述信息记录格式  
typedef struct{
  union{
    unsigned char Byte;
    struct{
      unsigned char Idel :1;  //该记录空间是否 空闲    1 空闲    0 占用
      unsigned char Sync :1; //该条记录是否同步完成   0 未完成  1 完成
    }Bit;
  }CSR;
  Equip_SPCP_type Spcp;
  unsigned char  Channel1;  //设备一级通道号 
  unsigned char  Channel2;  //设备二级通道号  
  EquipRecordData_struct Data;
}EquipRecord_struct;
/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
函数声明区
*********************************************************************************/
SystemErrName MountMemory(void); //挂载外部存储器
SystemErrName CreateBlank_Equip( unsigned long  *number); //创建空白设备描述符
SystemErrName GetBlank_Equip( unsigned long  *number);    //获取空白设备描述符

SystemErrName Config_Equip(unsigned long  number,     //设备描述记录的序号
                            Equip_SPCP_type  spcp_tpye, //设备的通信协议号
                            unsigned char  channel1,  //设备一级通道号 
                            unsigned char  channel2,  //设备二级通道号
                            void*           data,      //设备详细信息
                            unsigned char  length);   //信息长度

SystemErrName ReadForNumber_Equip(unsigned long  number, EquipRecord_struct* equip);
SystemErrName  ReadnextOChannel_Equip( unsigned char channel1, 
                                       unsigned char channel2,
                                       unsigned char opt,
                                       EquipRecord_struct* equip);
/********************************************************************************/
#endif
