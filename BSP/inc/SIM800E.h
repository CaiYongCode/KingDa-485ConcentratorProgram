/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef  __BSP_SIM800E_H
#define  __BSP_SIM800E_H 

#ifndef  IDLE 
#define  IDLE  0
#endif

#ifndef  INUSE
#define  INUSE  1
#endif
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "PowerConfig.h"
/*********************************************************************************
宏定义区
*********************************************************************************/

#define   GPRS_Revision_ByteNumber   32



#define  GPRS_USART_Config(BaudRate, Parity, StopBits, mod)      Launch_USART2 (BaudRate, Parity, StopBits, mod )                           
#define  GPRS_GetDataForBuf( )                                   GetDataForBuff_USART2 ( )
#define  GPRS_FreeBuff(buff)                                     FreeBuff_USART2(buff)
#define  GPRS_sendBuffByIT(buff,length,callBack,callBack_arg)    sendBuffByIT_USART2 (buff,length,callBack,callBack_arg )
#define  GPRS_sendStringByIT(buff)                               sendStringByIT_USART2 (buff)
#define  GPRS_sendBuffByDMA(buff, length,callBack,callBack_arg)  sendBuffByDMA_USART2 (buff, length,callBack,callBack_arg )
#define  GPRS_sendStringByDMA(buff )                             sendStringByDMA_USART2 (buff )
/*********************************************************************************
数据类型定义
*********************************************************************************/
typedef enum{
  Power_OFF = 0,
  Power_ON  = 1,
  GPRS_OK = 2,
  CONNECT_OK = 3,
}SIM800E_STATE_TYPE;

typedef enum{
  No_err           = 0x00,
  CPIN_err         = 0x01<<0,    //SIM 卡错误
  CGREG_err        = 0x01<<1,    //GPRS注册错误
  CREG_err         = 0x01<<2,    //GSM网络注册错误
  CONNECT_err      = 0x01<<3,    //服务器连接错误错误  
}ErrorType_GPRS;

typedef struct{
  unsigned char length;
  unsigned char* pbuff;
}GPRS_BuffStruct;

typedef void (*GPRSLaunch_CallBack)(ErrorType_GPRS  err);
/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
函数声明区
*********************************************************************************/
SystemErrName Launch_GPRS (SIM800E_STATE_TYPE  state,    //预期工作状态
                           unsigned char* server_ip,   //预期连接服务器的IP地址
                           unsigned short server_port,   //预期连接的 
                           GPRSLaunch_CallBack  CallBack); 
SystemErrName SendString_GPRS (void *p);
 unsigned short GetDataForBuff_GPRS (unsigned char* pbuff, unsigned short pbuffSize);
SystemErrName GPRS_SendBbuff ( void *buff, 
                               unsigned short length,
                               SerialPort_SendEndCallBack_Type callBack,
                               void *callBack_arg) ;
/********************************************************************************/
#endif
