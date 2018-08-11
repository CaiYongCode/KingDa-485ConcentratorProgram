/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef  __SYSTEM_DEFINE_H
#define  __SYSTEM_DEFINE_H 
/*********************************************************************************
文件包含区
*********************************************************************************/
/*********************************************************************************
宏定义区
*********************************************************************************/
#ifndef  P_NULL 
#define  P_NULL    (void *)(0x00000000)
#endif
/*********************************************************************************
数据类型定义
*********************************************************************************/
typedef enum{
  NO_ERR                  = 0,
  ParamRisky              = 1,               //参数值存在风险
  
  Warning                 = 100,             //警告性错误
  IllegalParam            = Warning + 1,     //非法参数
  PortBusy                = Warning + 2,     //端口忙
  Nothing                 = Warning + 3,     //资源不足导致操作未完成
  OperateCmd_err          = Warning + 4,     //非法操作指令
  FramePreamble_err       = Warning + 5,     //同步位错误 
  FrameSync_err           = Warning + 6,     //起始字节错误
  Length_err              = Warning + 7,     //长度错误
  FrameInvalid            = Warning + 8,     //帧格式错误
  Verify_err              = Warning + 9,     //校验错误
  Nodata                  = Warning + 10,    //没有数据
  Unsuccessful            = Warning + 11,    //操作失败  
  Eend                    = Warning + 12,    //已经结束了
  Port_OFF                = Warning + 13,    //端口关闭
  DataFileLost            = Warning + 14,    //数据文件丢失
  DataReadErr             = Warning + 15,    //数据文件丢失
  NoData                  = Warning + 16,    //没有数据
  CmdValid                = Warning + 20,    //操作命令无效或未被支持
  DataValid               = Warning + 21,    //数据无效
}SystemErrName;

typedef enum{
  OffState= 0,
  RunState = 1,

}DriverState_type;

typedef enum {OFF = 0, Idle = 1, Busy = 2, WaitAck = 3} PortStatus;

typedef struct{
 unsigned char Second;
 unsigned char Minute;
 unsigned char Hour;
 unsigned char WeekDay;
 unsigned char Day;
 unsigned char Month;
 unsigned short Year;
}TimerData_Struct;

/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
函数声明区
*********************************************************************************/

/********************************************************************************/
#endif