/*********************************************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************************************/
#ifndef  __JINDA_H
#define  __JINDA_H
/*********************************************************************************************************
文件包含区
*********************************************************************************************************/
#include "..\..\APP\inc\MCPX.h"
/*********************************************************************************************************
宏定义区
*********************************************************************************************************/

#define  JingDa_OutPutBuff_Number     1        //输出缓冲区数量

#define  JD_MeterConfigFile_PathName               "FLASH:/JDMeter"       //存储集中器管理的所有水表信息
#define  JD_OperateRecordFile_PathName             "FLASH:/JDOperate"     //存储操作记录
#define  JD_AllMeterCurrentValueFile_PathName      "FLASH:/JDValue"       //存储所有表计当前数据
#define  JD_AllMeterFreezingValueFile_PathName     "FLASH:/JDFreezing"    //存储集器的所有表计日冻结数据





#define  JINGDA_AreaCode             0xEE      //集中器 区域码

#define  JINGDA_Preamble             0xFEFE     //帧头
#define  JINGDA_StartCharacter       0x68     //帧头
#define  JINGDA_Pause                0x16     //帧尾

#define  ConcentratorAddr_Length     5        //集中器地址长度 
#define	 LSSB			     0x10     //冷水水表

//主站与集中器之间 命令码定义
#define      JD_Time_Wcmd		0x01	//1设置集中器时间														
#define      JD_Time_Rcmd		0x02	//2读取集中器时间															
#define      JD_ConfigData_Wcmd		0x03	//3设置集中器抄表参数													
#define      JD_ConfigData_Rcmd		0x04	//4读取集中器抄表参数													
#define      JD_Gprs_Wcmd		0x07	//5设置集中器GPRS参数													
#define      JD_Gprs_Rcmd		0x08	//6读取集中器GPRS参数													
#define      JD_Meter_Wcmd		0x09	//7下载表计地址															
#define      JD_Meter_Rcmd		0x0B	//8读取下载成功表计地址													
#define      JD_MeterErr_Rcmd		0x0C	//9读取下载失败表计地址													
#define      JD_Operate_Wcmd		0x0E	//10下达控制命令															
#define      JD_Operate_Rcmd		0x0F	//11读取控制命令															
#define      JD_Reading_Wcmd		0x14	//12启动立即抄表															
#define      JD_AllCurrentData_Rcmd	0x15	//13读取某一采集器的所有表计当前数据									
#define      JD_AllFreezingData_Rcmd	0x16	//14读取某一采集器的所有表计日冻结数据									
#define      JD_SingCurrentData_Rcmd	0x17	//15读取单表的当前数据													
#define      JD_SingFreezingData_Rcmd	0x18	//16读取单表的日冻结数据													
#define      JD_AlterMeter_Wcmd		0x1C	//17替换表计																
#define      JD_DeleteMeter_Wcmd	0x1D	//18删除表计																
//#define      JD_Beat_cmd		0x1D	//19登录帧与心跳帧
    
/*********************************************************************************************************
数据类型定义
*********************************************************************************************************/
#pragma pack(1)
//集中器时间  数据格式
typedef struct {
  unsigned char Year;   	//年只取低两位 (BCD)
  unsigned char	Month;  	//月 (BCD)
  unsigned char	Day;    	//日 (BCD)
  unsigned char	Hour;		//时 (BCD)
  unsigned char Minute;		//分 (BCD)
  unsigned char Second;		//秒 (BCD)
}JD_Time_Stuct;

//表计地址格式
typedef struct {
  unsigned char  ID[5];	//表地址1位
  union{
    unsigned short Word; 
    struct{
      unsigned char Byte1;   //低字节
      unsigned char Byte2;   //高字节
    }Byte;
  }FactoryCode;  //厂商代码
}JD_MeterAddr_Stuct;

//规约类型及通道号 数据格式
typedef  union{
  unsigned char Byte; 
  struct{
    unsigned char Protocol: 4;    /* 规约类型   0000:	阀控188 */
    unsigned char Channel : 3;    /*通道号   0000:	未指定通道
                                             0001:	M-BUS通道1
                                             0010:	M-BUS通道2
                                             0011:	M-BUS通道3
                                             0100:	M-BUS通道4
                                             0101:	RS485通道
                                             0110:	无线通道 */
    unsigned char Poll_EN : 1;   //是否巡检   0:不巡检  1:巡检
  }Bit;
}JD_LinkDeclarator;    

//通讯参数 数据格式	
typedef union{
  unsigned char Byte; 
  struct{
    unsigned char BaudRate: 3; /*波特率 000: 1200bps
					001: 2400bps
					010: 4800bps
		 			011: 9600bps
		 			100: 19200bps
		 			101: 38400bps  暂时未用
		 			110: 57600bps  暂时未用	
                                        111: 115200bps*/
    unsigned char DataNumber : 2;   //数据位  00: 6位数据位  01: 7位数据位 10: 8位数据位
    unsigned char StopBits : 1;   //停止位  0 :1位停止位   1 :2位停止位
    unsigned char Parity : 2;   //校验位  00:无校验     01:偶校验       10:奇校验
  }Bit;
}JD_CommDeclarator; 

//集中器抄表参数  数据格式
typedef struct {
  union{
    unsigned short Word; 
    struct{
      unsigned char Byte_L;   //低字节
      unsigned char Byte_H;   //高字节
    }Byte;
  }ReadingCycle;	
  unsigned char	DayFrozen;	//日冻结时刻 0—23；对所有通道有效  HEX
  unsigned char	MonthFrozenH;	//月冻结时间，1-31  HEX 	
  unsigned char	MonthFrozenL;	//月冻结时刻，0-23  HEX
  union{
    unsigned char Byte; 
    struct{
      unsigned char D0 : 1;     //1为关闭抄表间隔。
      unsigned char D1 : 1;     //1为关闭日冻结，但是常规情况下不关闭日冻结。
      unsigned char D2 : 1;	//1为关闭月冻结。
      unsigned char D3 : 1;	//1为关闭整点抄表。
    }Bit;
  }FreezingSwitch;
}JD_ConfigData_Stuct;

//集中器GPRS参数  数据格式
typedef struct {
  union{
    unsigned char Byte; 
    struct{
      unsigned char D0 : 1;     //代表是否使用DNS，0，使用IP地址；1使用DNS
      unsigned char D1 : 1;     //代表是否使用APN，0，不使用APN；1使用APN 
    }Bit;
  }ConnectionMode;	        //GPRS连接方式(HEX)
  unsigned char IP[4];		//十六进制码 第一字节(HEX)
  union{
    unsigned short Word; 
    struct{
      unsigned char Byte_L;   //低字节
      unsigned char Byte_H;   //高字节
    }Byte;
  }Port;	              //低字节 十六进制 低位在前 高位在后 (HEX)       
  unsigned char APN[16];  //16字节；若不足16字节，空余部分填充‘\0’ (ASCII)
  unsigned char DNS[37];  //37字节；若不足37字节，空余部分填充 ‘\0’(ASCII)	
  unsigned char	BeatCycle; //1字节（单位：分钟）(HEX)
}JD_Gprs_Stuct;

//节点描述符格式
typedef struct {
  unsigned char        MeterType;  //节点类型ID (HEX)  
  JD_MeterAddr_Stuct   Addr;       //节点地址
  JD_LinkDeclarator    CR1;        //节点规约类型及通道号 (HEX)
  JD_CommDeclarator    CR2;	   //节点通讯参数
}JD_Meter_Stuct;

//表计数据格式
typedef struct{
  JD_Meter_Stuct Meter;
   // Byte1;    //当前累积流量小数点后两位(BCD)
   // Byte2;    //当前累积流量整数部分的低两位(BCD)
   // Byte3;    //当前累积流量整数部分的中间两位(BCD)
   // Byte4;    //当前累积流量整数部分的高两位(BCD)
  unsigned char Value[4];
  unsigned char    State1;     //表计状态1
  unsigned char    State2;     //表计状态2
}JD_CJT188_MeterValue_Stuct;

//控制记录格式：
typedef struct{
  unsigned char         OPID;	    //操作员ID(HEX)　
  JD_Time_Stuct	        Time;
  unsigned char         MeterType;  //被控表类型ID(HEX)
  JD_MeterAddr_Stuct    Addr;       //表地址
  JD_LinkDeclarator     CR1;        //节点规约类型及通道号 (HEX)
  JD_CommDeclarator     CR2;        //下行通讯参数(HEX)
  unsigned char  MeterState;        //阀门状态(HEX) 0X99：关阀；0X55：开阀
}JD_OperateRecord_Stuct;


//设置集中器时间命令 主站——>集中器 命令参数定义 (控制码：0X01)
typedef struct {
  unsigned char Year;   	//年 BCD
  unsigned char	Month;  	//月 BCD
  unsigned char	Day;    	//日 BCD
  unsigned char	Hour;		//时 BCD 
  unsigned char Minute;		//分 BCD
  unsigned char Second;		//秒 BCD
  unsigned char WeekDay;	//周几 BCD
  unsigned char SER;	        /*序列号(HEX)*/
}JD_TimeWcmd_askParam_Stuct; 
//设置集中器时间命令 集中器——>主站 命令参数定义 (控制码：0X01)
typedef struct{
  unsigned char ReplyFlag;     /*应答标志：(HEX)
                                            0x00 :正常
                                            0xCC : 时间不合法 */
  unsigned char SER;	        /*序列号(HEX)*/
}JD_TimeWcmd_ackParam_Stuct;

//读取集中器时间命令 主站——>集中器 命令参数定义 (控制码：0X02)
typedef struct{
  unsigned char SER;	        /*序列号(HEX)*/
}JD_TimeRcmd_askParam_Stuct;
//读取集中器时间命令 集中器——>主站 命令参数定义 (控制码：0X02)
typedef struct
{
  unsigned char Year;   	//年 BCD
  unsigned char	Month;  	//月 BCD
  unsigned char	Day;    	//日 BCD
  unsigned char	Hour;		//时 BCD 
  unsigned char Minute;		//分 BCD
  unsigned char Second;		//秒 BCD
  unsigned char WeekDay;	//周几 BCD
  unsigned char ReplyFlag;      //应答标志(HEX)：0x00正常，无异常应答
  unsigned char SER;	        /*序列号(HEX)*/  
}JD_TimeRcmd_ackParam_Stuct;


//设置集中器抄表参数   主站——>集中器  命令参数定义 (控制码：0X03)
typedef struct{
  JD_ConfigData_Stuct   data;
  unsigned char         SER;	/*序列号(HEX)*/
}JD_ConfigDataWcmd_askParam_Stuct;
//设置集中器抄表参数   集中器——> 主站 命令参数定义 (控制码：0X03)
typedef struct{
  unsigned char  ReplyFlag;     /*应答标志：(HEX)
                                            0x00 : 正常 
                                            0xCC : 数据写入eeprom不正确*/
  unsigned char        SER;	/*序列号(HEX)*/
}JD_ConfigDataWcmd_ackParam_Stuct;

//读取集中器抄表参数   主站——>集中器  命令参数定义 (控制码：0X04)
typedef struct{
  unsigned char         SER;	/*序列号(HEX)*/
}JD_ConfigDataRcmd_askParam_Stuct;
			
//读取集中器抄表参数   集中器——> 主站  命令参数定义 (控制码：0X04) 	
typedef struct{
  JD_ConfigData_Stuct Data;
  unsigned char  ReplyFlag;   //应答标志(HEX)：0x00正常，无异常应答
  unsigned char  SER;         /*序列号(HEX)*/  
}JD_ConfigDataRcmd_ackParam_Stuct;

//设置集中器GPRS参数   主站——>集中器  命令参数定义 (控制码：0x07) 
typedef struct{
  JD_Gprs_Stuct   data;
  unsigned char   SER;	/*序列号(HEX)*/
}JD_GprsWcmd_askParam_Stuct;
//设置集中器GPRS参数   集中器——> 主站 命令参数定义 (控制码：0X07)
typedef struct{
  unsigned char  ReplyFlag;   //应答标志(HEX)：0x00正常，无异常应答
  unsigned char  SER;	      /*序列号(HEX)*/
}JD_GprsWcmd_ackParam_Stuct;

//读取集中器抄表参数   主站——>集中器  命令参数定义 (控制码：0X08)
typedef struct{
  unsigned char         SER;	/*序列号(HEX)*/
}JD_GprsRcmd_askParam_Stuct;

//读取集中器GPRS参数   集中器——>主站  命令参数定义 (控制码：0X08)
typedef struct{
  JD_Gprs_Stuct Data;
  unsigned char ReplyFlag;   //应答标志(HEX)：0x00正常，无异常应答
  unsigned char  SER;	     /*序列号(HEX)*/
}JD_GprsRcmd_ackParam_Stuct;

//下载表计地址   主站——>集中器  命令参数定义(控制码：0X09)
typedef struct {
  unsigned char  PackageSum;      //共有分包(HEX)
  union{
    unsigned short Word;
    struct{
      unsigned char Byte_L;   //低字节
      unsigned char Byte_H;   //高字节
    }Byte;		
  }MeterSum;                     //共有表数(HEX)         
  unsigned char  PackageNumber;	 //第几包数据(HEX)
  unsigned char  MetNumber; 	 //该包表计地址个数(HEX)
  JD_Meter_Stuct Meter[25];      //表计说明数据(HEX)
  unsigned char  SER;	     /*序列号(HEX)*/
}JD_MeterWcmd_askParam_Stuct;

//下载表计地址   集中器——>主站 命令参数定义 (控制码：0X09)
typedef struct{
  unsigned char  ReplyFlag;   //应答标志(HEX)：0x00 下载成功，无异常
  unsigned char  SER;	     /*序列号(HEX)*/
}JD_MeterWcmd_ackParam_Stuct;

//读取下载成功表计地址  主站——>集中器  命令参数定义(控制码：0x0B)
typedef struct {
  unsigned char  Channel;   /*通道号:(HEX)0x01—0x06									
                                     00000001:	M-BUS通道1							
                                     00000010:	M-BUS通道2							
                                     00000011:	M-BUS通道3
                                     00000100:	M-BUS通道4
                                     00000101:	RS485通道
                                     00000110:	无线通道*/
  unsigned char  PackageNumber;   //第几包数据(HEX)
  unsigned char  SER;	     /*序列号(HEX)*/
}JD_MeterRcmd_askParam_Stuct;

//读取下载成功表计地址  集中器——>主站  命令参数定义(控制码：0x0B)
typedef struct {
  unsigned char  PackageSum;	 //共有分包(HEX)
  union{
    unsigned short Word;
    struct{
      unsigned char Byte_L;      //低字节
      unsigned char Byte_H;      //高字节
    }Byte;
  }MeterSum;                     //共有表数(HEX)       
  unsigned char  PackageNumber;	 //第几包数据(HEX)	
  unsigned char  MetNumber; 	 //该包表计地址个数
  JD_Meter_Stuct Meter[25];      //表计说明数据  
  unsigned char	 ReplyFlag;      //应答标志(HEX):0x00:正常；0xCB:无此包
  unsigned char  SER;	         /*序列号(HEX)*/
}JD_MeterRcmd_ackParam_Stuct;

//读取下载失败表计地址  主站——>集中器  命令参数定义(控制码：0x0C)
typedef struct {
  unsigned char  PackageNumber;	//第几包数据(HEX)
  unsigned char  SER;	        /*序列号(HEX)*/
}JD_MeterErrRcmd_askParam_Stuct;

//读取下载成功表计地址  集中器——>主站  命令参数定义(控制码：0x0C)
typedef struct {
  unsigned char  PackageSum;    //共有分包(HEX)
  union{
    unsigned short Word;
    struct{
      unsigned char Byte_L;     //低字节
      unsigned char Byte_H;     //高字节
    }Byte;		
  }MeterSum;                    //  共有表数(HEX)
  unsigned char  PackageNumber;	//第几包数据(HEX)	
  unsigned char  MetNumber; 	//该包表计地址个数(HEX)
  JD_Meter_Stuct Meter[25];     //表计说明数据(HEX)  
  unsigned char  ReplyFlag;     //应答标志  0x00:正常; 0xCB:无此包
  unsigned char  SER;	        /*序列号(HEX)*/
}JD_MeterErrRcmd_ackParam_Stuct;

//下达控制命令 主站——>集中器  命令参数定义(控制码：0X0E)  
typedef struct {
  JD_OperateRecord_Stuct  Operate;
  unsigned char           SER;	    /*序列号(HEX)*/
}JD_OperateWcmd_askParam_Stuct;

//下达控制命令  集中器——>主站  命令参数定义(控制码：0X0E)
typedef struct {
  unsigned char  MeterType;	  	//表计类型ID   0x16   HEX	    
  JD_MeterAddr_Stuct  Addr;
  unsigned char  MetState;  /*阀门状态(HEX)  0x00：关阀成功
                                             0x01：关阀不到位
					     0x02：关阀失败
					     0x03：开阀成功
					     0x04：开阀不到位
					     0x05：开阀失败*/
  unsigned char ReplyFlag;  /*应答标志 (HEX) 0x00: 成功，
					     0x01: 未查询到表计；
					     0x02: 参数不合法；
					     0x05: MBUS通道异常；
					     0x06: 表计无响应；
                                             0x07: 无应答
					     0xCB: 下行忙；
					     0xCE: 下行操作超时*/
  unsigned char SER;	    /*序列号(HEX)*/ 
}JD_OperateWcmd_ackParam_Stuct;

//取控制记录   主站——>集中器  命令参数定义(控制码：0X0F	)  
typedef struct {
  unsigned char  PackageNumber;	//第几包数据  HEX
  unsigned char  SER;	        /*序列号(HEX)*/
}JD_OperateRcmd_askParam_Stuct;

//取控制记录   集中器——>主站  命令参数定义(控制码：0X0F)
typedef struct {
  unsigned char  PackageSum;	       //共几包数据(HEX)
  unsigned char  PackageNumber;	       //第几包数据(HEX)
  unsigned char  ConNumber;	       //控制记录总数N (HEX)
  JD_OperateRecord_Stuct  Record[15];  //控制记录数据;
  unsigned char  ReplyFlag;	       /*应答标志(HEX)：0x00:正常
                                                        0xCA:无此包*/
  unsigned char  SER;	               /*序列号(HEX)*/
}JD_OperateRcmd_ackParam_Stuct;

//启动立即抄表  主站——>集中器  命令参数定义(控制码：0X14)  
typedef struct {
  unsigned char SER;	   /*序列号(HEX)*/  	
}JD_ReadingWcmd_askParam_Stuct;

//启动立即抄表  集中器——>主站  命令参数定义(控制码：0X14)  
typedef struct {
  unsigned char ReplyFlag; /*应答标志 0x00:正常启动抄表
				      0x01:下行忙已经在抄表了,不需再次启动
			              0x02:下行正在巡检表地址,不能启动抄表*/
  unsigned char SER;	   /*序列号(HEX)*/  	
}JD_ReadingWcmd_ackParam_Stuct;

//读取所有表计当前数据  主站——>集中器  命令参数定义(控制码：0X15)
typedef struct {
  unsigned char PackageNumber;	//第几包数据(HEX)
  unsigned char SER;	   /*序列号(HEX)*/  
}JD_AllCurrentDataRcmd_askParam_Stuct; 

//读取所有表计当前数据  集中器——>主站  命令参数定义(控制码：0X15)  
typedef struct {
  unsigned char FileSum;    //文件总数(HEX)	
  unsigned char Filecount;  //文件序号(HEX)
  unsigned char RecordNum;  //纪录数；包含表数量 (HEX)
  JD_Time_Stuct Time;       //时间
  union{
    JD_CJT188_MeterValue_Stuct CJT188[17];
    unsigned char buff[sizeof (JD_CJT188_MeterValue_Stuct )*17];
  }Value;
  unsigned char ReplyFlag;  /*应答标志(HEX)：0x00:正常; 0xCA:无此包*/
  unsigned char SER;	    /*序列号(HEX)*/  
}JD_AllCurrentDataRcmd_ackParam_Stuct;

//读取全部表计日冻结数据   主站——>集中器  命令参数定义(控制码：0X16)
typedef struct {
  unsigned char PackageNumber;	//第几包数据(HEX)
  unsigned char Year;	        //年 低字节(BCD)
  unsigned char Month;          //月(BCD)
  unsigned char Day;		//日(BCD)
  unsigned char SER;	        /*序列号(HEX)*/  
}JD_AllFreezingDataRcmd_askParam_Stuct;  

//读取全部表计日冻结数据   集中器——>主站  命令参数定义(控制码：0X16)  
typedef struct {
  unsigned char FileSum;       //文件总数(HEX)	
  unsigned char Filecount;     //文件序号(HEX)
  unsigned char RecordNum;     //纪录数；包含表数量 (HEX)
  JD_Time_Stuct Time;          //时间
  union{
    JD_CJT188_MeterValue_Stuct  CJT188[17];
    unsigned char buff[sizeof(JD_CJT188_MeterValue_Stuct )*17];
  }Value;
  unsigned char ReplyFlag; /*应答标志(HEX)：0x00:正常;
					    0xCD:未找到该日期 
					    0xCA无此包*/
  unsigned char SER;	   /*序列号(HEX)*/  
}JD_AllFreezingDataRcmd_ackParam_Stuct;

//读单表的当前数据   主站——>集中器  命令参数定义(控制码：0X17)
typedef struct {
  JD_Meter_Stuct  Meter; //表计描述
  unsigned char   SER;	 /*序列号(HEX)*/  
}JD_SingCurrentDataRcmd_askParam_Stuct;  

//读单表的当前数据   集中器——>主站  命令参数定义(控制码：0X17)  
typedef struct {
  JD_CJT188_MeterValue_Stuct Data;
  unsigned char ReplyFlag; /*应答标志(HEX)：0x00:正常，0xCF:未找到表 */
  unsigned char SER;	   /*序列号(HEX)*/  
}JD_SingCurrentDataRcmd_CJT188_ackParam_Stuct;

//读取单表的日冻结数据   主站——>集中器  命令参数定义(控制码：0X18)
typedef struct {
  unsigned char	  Year;	   //年 低字节(BCD)
  unsigned char	  Month;   //月(BCD)
  unsigned char	  Day;	   //日(BCD)
  JD_Meter_Stuct  Meter;   //表计描述
  unsigned char   SER;	   /*序列号(HEX)*/  
}JD_SingFreezingDataRcmd_askParam_Stuct;

//读取单表的日冻结数据   集中器——>主站  命令参数定义(控制码：0X18)  
typedef struct {
  JD_Time_Stuct Time;      //时间
  union{
    JD_CJT188_MeterValue_Stuct CJT188;
    unsigned char buff[sizeof (JD_CJT188_MeterValue_Stuct )];
  }Value;
  unsigned char ReplyFlag; /*应答标志(HEX)：0x00:正常;
				            0xCD:未找到该日期 
				            0xCF:未找到表 */
  unsigned char SER;	  /*序列号(HEX)*/  
}JD_SingFreezingDataRcmd_ackParam_Stuct;

//替换表计   主站——>集中器  命令参数定义(控制码：0X1C)
typedef struct {
  JD_Meter_Stuct  OldMeter; 	//表计描述
  JD_Meter_Stuct  NewMeter; 	//表计描述
  unsigned char   SER;	        /*序列号(HEX)*/  
}JD_AlterMeterWcmd_askParam_Stuct;  
//替换表计   集中器——>主站  命令参数定义(控制码：0X1C) 
typedef struct {
  unsigned char ReplyFlag; /*应答标志(HEX): 0x00: 替换成功
                                            0x01: 未查询到表计
                                            0x02: 参数不合法
					    0x04: 地址重复
                                            0x05: MBUS通道异常
					    0x06: 表计无响应
					    0xCB: 下行忙
					    0xCE: 下行操作超时*/
  unsigned char   SER;	        /*序列号(HEX)*/ 
}JD_AlterMeterWcmd_ackParam_Stuct;   

//删除表计   主站——>集中器  命令参数定义(控制码：0X1c)
typedef struct {
  unsigned char   TotalNumber;    //删除记录条数(HEX)  
  JD_Meter_Stuct  Meter[25]; 	  //表计描述
  unsigned char   SER;	          /*序列号(HEX)*/ 
}JD_DeleteMeterWcmd_askParam_Stuct; 

//删除表计   集中器——>主站  命令参数定义(控制码：0X1c) 
typedef struct {
  unsigned char TotalNumber;      //删除记录条数(HEX)
  unsigned char DeleteResult[25]; // 第N条 00成功，01删除失败  
  unsigned char ReplyFlag;        /*应答标志(HEX): 0x00: 删除成功; 
                                                   0xCB: 下行忙; 
                                                   0xCE: 下行操作超时*/
  unsigned char SER;	          /*序列号(HEX)*/ 
}JD_DeleteMeterWcmd_ackParam_Stuct; 

typedef unsigned short   JingDa_Preamble;
//帧帧头格式
typedef struct {	
  unsigned char StartCharacter;  //帧头值为0x68H
  unsigned char LAI;             //区域识别码	1Byte	用于识别数据传输的区域
  unsigned char Concentrator[ConcentratorAddr_Length]; //集中器地址 5字节BCD码
  unsigned char Code;            //功能码
  union{
    unsigned short Word;
    struct{
      unsigned char Byte_L;
      unsigned char Byte_H;
    }Byte;
  }DataLength;                   //数据段长度
}JingDa_FrameHead_Struct;

//帧尾格式
typedef struct {
  unsigned char CS;     //校验码,从帧头开始到帧校验标志之前所有字节的累加和低8位	
  unsigned char Pause;  //结束符0x16H
}JingDa_FrameStern_Struct;


//正常帧格式
typedef struct{
  JingDa_FrameHead_Struct  Head;
  union{ 
    //设置集中器时间命令 主站——>集中器 命令参数定义 (控制码：0X01)
    JD_TimeWcmd_askParam_Stuct              TimeWcmd_ask; 
    //设置集中器时间命令 集中器——>主站 命令参数定义 (控制码：0X01)
    JD_TimeWcmd_ackParam_Stuct              TimeWcmd_ack;
    //读取集中器时间命令 主站——>集中器 命令参数定义 (控制码：0X02)
    JD_TimeRcmd_askParam_Stuct              TimeRcmd_ask;
    //读取集中器时间命令 集中器——>主站 命令参数定义 (控制码：0X02)
    JD_TimeRcmd_ackParam_Stuct              TimeRcmd_ack;
    //设置集中器抄表参数   主站——>集中器  命令参数定义 (控制码：0X03)
    JD_ConfigDataWcmd_askParam_Stuct        ConfigDataWcmd_ask;
    //设置集中器抄表参数   集中器——> 主站 命令参数定义 (控制码：0X03)
    JD_ConfigDataWcmd_ackParam_Stuct        ConfigDataWcmd_ack;
    //读取集中器抄表参数   主站——>集中器  命令参数定义 (控制码：0X04)
    JD_ConfigDataRcmd_askParam_Stuct        ConfigDataRcmd_ask; 	
    //读取集中器抄表参数   集中器——> 主站  命令参数定义 (控制码：0X04) 	
    JD_ConfigDataRcmd_ackParam_Stuct        ConfigDataRcmd_ack;
    //设置集中器GPRS参数   主站——>集中器  命令参数定义 (控制码：0x07) 
    JD_GprsWcmd_askParam_Stuct              GprsWcmd_ask;
    //设置集中器GPRS参数   集中器——> 主站 命令参数定义 (控制码：0X07)
    JD_GprsWcmd_ackParam_Stuct              GprsWcmd_ack;
    //读取集中器抄表参数   主站——>集中器  命令参数定义 (控制码：0X08)
    JD_GprsRcmd_askParam_Stuct              GprsRcmd_ask;
    //读取集中器GPRS参数   集中器——>主站  命令参数定义 (控制码：0X08)
    JD_GprsRcmd_ackParam_Stuct              GprsRcmd_ack;
    //下载表计地址   主站——>集中器  命令参数定义(控制码：0X09)
    JD_MeterWcmd_askParam_Stuct             MeterWcmd_ask;
    //下载表计地址   集中器——>主站 命令参数定义 (控制码：0X09)
    JD_MeterWcmd_ackParam_Stuct             MeterWcmd_ack;
    //读取下载成功表计地址  主站——>集中器  命令参数定义(控制码：0x0B)
    JD_MeterRcmd_askParam_Stuct             MeterRcmd_ask;
    //读取下载成功表计地址  集中器——>主站  命令参数定义(控制码：0x0B)
    JD_MeterRcmd_ackParam_Stuct             MeterRcmd_ack;
    //读取下载失败表计地址  主站——>集中器  命令参数定义(控制码：0x0C)
    JD_MeterErrRcmd_askParam_Stuct          MeterErrRcmd_ask;
    //读取下载成功表计地址  集中器——>主站  命令参数定义(控制码：0x0C)
    JD_MeterErrRcmd_ackParam_Stuct          MeterErrRcmd_ack;
    //下达控制命令 主站——>集中器  命令参数定义(控制码：0X0E)  
    JD_OperateWcmd_askParam_Stuct          OperateWcmd_ask;
    //下达控制命令  集中器——>主站  命令参数定义(控制码：0X0E)
    JD_OperateWcmd_ackParam_Stuct          OperateWcmd_ack;
    //取控制记录   主站——>集中器  命令参数定义(控制码：0X0F	)  
    JD_OperateRcmd_askParam_Stuct          OperateRcmd_ask;
    //取控制记录   集中器——>主站  命令参数定义(控制码：0X0F)
    JD_OperateRcmd_ackParam_Stuct          OperateRcmd_ack;
    //启动立即抄表  主站——>集中器  命令参数定义(控制码：0X14)  
    JD_ReadingWcmd_askParam_Stuct          ReadingWcmd_ask;
    //启动立即抄表  集中器——>主站  命令参数定义(控制码：0X14)  
    JD_ReadingWcmd_ackParam_Stuct          ReadingWcmd_ack;
    //读取所有表计当前数据  主站——>集中器  命令参数定义(控制码：0X15)
    JD_AllCurrentDataRcmd_askParam_Stuct   AllCurrentDataRcmd_ask; 
    //读取所有表计当前数据  集中器——>主站  命令参数定义(控制码：0X15)  
    JD_AllCurrentDataRcmd_ackParam_Stuct   AllCurrentDataRcmd_ack;
    //读取全部表计日冻结数据   主站——>集中器  命令参数定义(控制码：0X16)
    JD_AllFreezingDataRcmd_askParam_Stuct   AllFreezingDataRcmd_ask;  
    //读取全部表计日冻结数据   集中器——>主站  命令参数定义(控制码：0X16)  
    JD_AllFreezingDataRcmd_ackParam_Stuct     AllFreezingDataRcmd_ack;
    //读单表的当前数据   主站——>集中器  命令参数定义(控制码：0X17)
    JD_SingCurrentDataRcmd_askParam_Stuct         SingCurrentDataRcmd_ask;  
    //读单表的当前数据   集中器——>主站  命令参数定义(控制码：0X17)  
    JD_SingCurrentDataRcmd_CJT188_ackParam_Stuct  SingCurrentDataRcmd_CJT188_ack;
    //读取单表的日冻结数据   主站——>集中器  命令参数定义(控制码：0X18)
    JD_SingFreezingDataRcmd_askParam_Stuct       SingFreezingDataRcmd_ask;
    //读取单表的日冻结数据   集中器——>主站  命令参数定义(控制码：0X18)  
    JD_SingFreezingDataRcmd_ackParam_Stuct       SingFreezingDataRcmd_ack;
    //替换表计   主站——>集中器  命令参数定义(控制码：0X1C)
    JD_AlterMeterWcmd_askParam_Stuct    AlterMeterWcmd_ask; 
    //替换表计   集中器——>主站  命令参数定义(控制码：0X1C) 
    JD_AlterMeterWcmd_ackParam_Stuct    AlterMeterWcmd_ack;   
    //删除表计   主站——>集中器  命令参数定义(控制码：0X1c)
    JD_DeleteMeterWcmd_askParam_Stuct   DeleteMeterWcmd_ask;
    //删除表计   集中器——>主站  命令参数定义(控制码：0X1c) 
    JD_DeleteMeterWcmd_ackParam_Stuct   DeleteMeterWcmd_ack;
    unsigned char                       Data[256]; 
  }Param;
}JingDa_FrameDatas_Struct;

//正常帧格式
typedef struct{
  JingDa_Preamble          Preamble; //前导字节  至少2个字节FEH（不计入校验）。
  JingDa_FrameDatas_Struct Datas;
}JingDa_Frame_Struct;

#pragma pack()

//typedef void(*JingDa_AckFunc)(CJT188_SendBuff_Struct* SendData, CJT188_Frame_Struct* AckData)
//竟达命令描述符
typedef struct{
 CommPortNumber_Type SourcePort;
 unsigned char Code;
 unsigned char ParamLength;
 unsigned char *Param;
}JingDa_CmdDeclarator;


typedef struct {
  union{
    unsigned char Byte;
    struct{
      unsigned char idle :1; //缓冲区空闲
    }Bit;
  }CSR; //控制状态寄存器
 CommPortNumber_Type   SourcePort;
 JingDa_Frame_Struct   Frame; 
}JingDa_OutPutBuff_Struct;
/*********************************************************************************************************
外部变量声明区
*********************************************************************************************************/

/*********************************************************************************************************
函数声明区
*********************************************************************************************************/
void JingDaSPCP_init(void);

JingDa_OutPutBuff_Struct* getJingDaSendBuff(void);

void freeJingDaSendBuff(void* frame );

SystemErrName JingDa_InPut (void *pbuff, unsigned length,CommPortNumber_Type port);

SystemErrName sendJingDaCmd (CommPortNumber_Type Port,
                             unsigned char Code,
                             unsigned short ParamLength,
                             void *Param);

SystemErrName sendJingDa_WLMQSY (CommPortNumber_Type Port);

SystemErrName JingDa_Send(CommPortNumber_Type Port, JingDa_Frame_Struct* farame);
/********************************************************************************************************/
#endif
/******************************************END********************************************************/