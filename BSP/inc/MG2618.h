/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef  __BSP_MG2618_H
#define  __BSP_MG2618_H 

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
#include "..\..\Projects\inc\SofTimer.h"
/*********************************************************************************
宏定义区
*********************************************************************************/
#define  MG2618_PowerFlg              (GPRS_ENpin)            
#define  MG2618_PowerON( )            set_GPRS_ENpin         
#define  MG2618_PowerOFF( )           clr_GPRS_ENpin

#define  GET_MG2616_ONOF_PIN         ~(GPRS_ONOFFpin) 
#define  SET_MG2616_ONOF_PIN          clr_GPRS_ONOFFpin
#define  RSET_MG2616_ONOF_PIN         set_GPRS_ONOFFpin      

#define  GET_MG2616_RSET_PIN         ~(GPRS_RESTpin)  
#define  SET_MG2616_RSET_PIN          clr_GPRS_RESTpin  
#define  RSET_MG2616_RSET_PIN         set_GPRS_RESTpin  

#define  ConfigUsart_MG2616(BaudRate,Parity,StopBits,mod)           Launch_USART2(BaudRate, Parity, StopBits, mod)                           
#define  GetDataForUsart_MG261( )                                   GetDataForBuff_USART2( )
#define  FreeUsartInputBuff_MG261(buff)                             FreeBuff_USART2(buff)
#define  SendDataToMG261_ByIT(buff,length,callBack,callBack_arg)    sendBuffByIT_USART2(buff,length,callBack,callBack_arg)
#define  SendDataToMG261_ByDMA(buff,length,callBack,callBack_arg)   sendBuffByDMA_USART2(buff, length,callBack,callBack_arg)
#define  SendStringToMG261_ByIT(String)                             sendStringByIT_USART2(String)
#define  SendStringToMG261_ByDMA(String)                            sendStringByDMA_USART2(String)
#define  SendDataToMG2618(buff,length,callBack)                     SendDataToMG261_ByDMA(buff,length,callBack,P_NULL)
#define  SendStringToMG2618(String)                                 SendStringToMG261_ByDMA(String)    

#define  MG2616_ATORDER_LEN   64

#define AT_Again_Command_EN  0   //该指令用于重复上一条指令。
#define ATA_Command_EN       0   //应答呼叫
#define ATD_Command_EN       0   //发起呼叫
#define ATDL_Command_EN      0   //拨打最近呼出的电话号码 
#define ATE_Command_EN       0   //使能回馈通道
#define ATH_Command_EN       0   //断开当前连接
#define ATI_Command_EN       0   //提示模块生产厂家信息 
#define ATQ_Command_EN       0   //设置是否在终端回显返回值
#define AtData_Command_EN    0   //从数据模式切换到命令模式 
#define ATO_Command_EN       0   //从命令模式切换到数据模式 
#define ATP_Command_EN       0  //使用脉冲拨号
#define ATS0_Command_EN      0  //自动应答设置
#define AT_CRC_Command_EN    0  //设置来电类型
#define CLVL_Command_EN      0  //受话音量
#define CLIP_Command_EN      0  //设置来电号码显示
#define ZSETMUTE_Command_EN  0  //静音控制
#define CIMI_Command_EN      0  //查询国际 ID 号
#define CGMR_Command_EN      0  //获取产品版本号
#define ECHO_Command_EN      0  //回音消除控制
#define GSN_Command_EN       0  //获取当前 IMEI 号
#define ZVERS_Command_EN     0  //获取当前软件版本号
#define CLCK_Command_EN      0  //功能锁定
#define CCFC_Command_EN      0  //呼叫转移号码和条件设置 
#define CCWA_Command_EN      0  //呼叫等待控制
#define CHLD_Command_EN      0  //呼叫保持和多方会议
#define TSIMINS_Command_EN   0  //查询 SIM 卡状态
#define CPWD_Command_EN      0  //修改密码
#define CGMI_Command_EN      0  //查询模块厂家
#define CSCS_Command_EN      0  //字符集种类的选择
#define CLCC_Command_EN      0  //呼叫状态查询
//DTMF指令
#define VTS_Command_EN       0  //发送 DTMF 音
//网络服务指令 
#define CREG_Command_EN      0  //网络注册和漫游
#define COPS_Command_EN      1  //网络选择
//移动设备控制和状态报告
#define CPAS_Command_EN      0  //模块状态查询
#define CFUN_Command_EN      0  //设置模块功能
#define CMEE_Command_EN      0  //移动设备错误报告
#define ZPWROFF_Command_EN   0  //模块关机
#define CPIN_Command_EN      0  //输入 PIN 码
#define EPIN1_Command_EN     0  //输入 PIN1 码
#define EPIN2_Command_EN     0  //输入 PIN2 码
#define EPINC_Command_EN     0  //查询 PIN1， PIN2 剩余输入次数
#define CSQ_Command_EN       1  //信号强度查询
#define CCLK_Command_EN      0  //时钟管理
// 消息服务指令 
#define CSCA_Command_EN    0   //短信中心号码
#define CNMA_Command_EN    0    //短信确认
#define CMGF_Command_EN    0   //设置短消息模式
#define CNMI_Command_EN    0    //设置短信指示格式
#define CMGR_Command_EN    0    //读短消息
#define CMGW_Command_EN    0    //写短消息
#define CSMS_Command_EN    0    //选择短信服务
#define CMGS_Command_EN    0    //发送短信
#define CPMS_Command_EN    0    //首选短消息存储器
#define CMGD_Command_EN    0    //删除短信
#define CMGL_Command_EN    0    //短消息列表
#define CMSS_Command_EN    0    //发送存贮的短信
//电话本指令 
#define CPBS_Command_EN    0    //选择电话本存储
#define CPBR_Command_EN    0    //读取电话本信息
#define CPBW_Command_EN    0    //向电话本写信息
#define CPBF_Command_EN    0    //查找电话本信息
#define CNUM_Command_EN    0    //获取本机号码
//数据压缩指令 
#define IFC_Command_EN        0    //流控制
#define DTR_Command_EN        0    //设置 DTR 模式
#define DCD_Command_EN        0    //设置 DCD 模式
#define IPR_Command_EN        0    //设定模块波特率
#define AT_F_Command_EN       0    //恢复出厂设置
#define AT_W_Command_EN       0    //保存设置
//GPRS指令
#define CGDCONT_Command_EN     0    //设置 PDP 格式
#define CGACT_Command_EN       0    //激活/解除 PDP 设置
#define CGATT_Command_EN       1    //设置 GPRS 业务 
#define CGCLASS_Command_EN     0    //GPRS 设备等级
//ZTE 特有指令
#define ZSTR_Command_EN          0    //查询模块状态信息
#define ZGETICCID_Command_EN     0    //获取 ICCID
#define ZCSQ_Command_EN          0    //设置自动显示 CSQ 的条件 
#define ZEDT_Command_EN          0    //设置 DTR 的检测模式 
#define ZDSLEEP_Command_EN       0   //32KHz 深度睡眠模式
#define CUSD_Command_EN          0    //发送 USSD 数据
#define ZRINGPINMODE_Command_EN  0    //设置 RING 引脚的信号模式 
#define ZVERSWITCH_Command_EN    0    //定制版本切换
// 网络参数指令 
#define CSTT_Command_EN          1    //设置 APN，用户名，密码 
#define CIICR_Command_EN         1    //打开 GPRS 数据连接 
#define ZPPPCLOSE_Command_EN     0   //关闭 GPRS 数据连接 
#define CIFSR_Command_EN         1    //查询当前模块 IP 地址值 
#define ZDNSSERV_Command_EN      0    //设置 DNS 服务器的 IP 地址 
#define ZDNSGETIP_Command_EN     0   //获取对应 Internet 域名的 IP 地址 
//TCP 连接指令 
#define CIPSTART_Command_EN      1   //建立 TCP 服务器链接
#define CIPSEND_Command_EN       1  //向目标地址发送 TCP 数据 
#define ZPPPSTATUS_Command_EN    0   //查询 GPRS 连接状态 
#define CIPCLOSE_Command_EN      1   //关闭 TCP 链接
#define ZIPSTATUS_Command_EN     0   //查询当前 TCP 连接状态 
// UDP 连接指令 
#define ZIPSETUPU_Command_EN     0  //建立 UDP 服务器链接 
#define ZIPSENDU_Command_EN      0 //向 UDP 服务器发送数据 
#define ZIPSTATUSU_Command_EN    0 //查询 UDP 状态
#define ZIPCLOSEU_Command_EN     0 //关闭 UDP 链接
// SERVER 指令 
#define ZTCPLISTEN_Command_EN    0  //端口监听功能设置 
#define ZTCPSENDP_Command_EN     0  //在被动打开的链路上发送数据 
#define ZTCPCLOSEP_Command_EN    0 //关闭监听到的连接
#define ZTCPSTATUSP_Command_EN   0  //查询被动打开链路
#define ZIPTIMEOUT_Command_EN    0  //设置模块连接服务器和发送数据的超时时间
#define ZUDPLISTEN_Command_EN    0  //端口监听功能设置
#define ZUDPSENDP_Command_EN     0  //在被动打开的链路上发送数据
// FTP 指令
#define ZFTPLOGIN_Command_EN     0 //登录 FTP 服务器
#define ZFTPTYPE_Command_EN      0 //设置 FTP 文件类型
#define ZFTPUPLOAD_Command_EN    0 //上传文件
#define ZFTPSIZE_Command_EN      0 //获取文件大小
#define ZFTPDNLOAD_Command_EN    0 //下载文件
#define ZFTPDNLOADEX_Command_EN  0 //下载 FTP 文件
#define ZFTPDEL_Command_EN       0 //删除文件
#define ZFTPQUIT_Command_EN      0 //退出 FTP 
// 透明传输指令 
#define ZTRANSFER_Command_EN     0 //透明传输
//音频相关指令 
#define ZCALLTONE_Command_EN     0 //摘机音设置
#define ZDTMFTONE_Command_EN     0 //ZDTMF 拨号音设置 
#define ZKTDSWITCH_Command_EN    0 //打开 DTMF 检测功能
#define ZKTDIND_Command_EN       0 //主动上报检测的 DTMF 按键
#define SPEAKER_Command_EN       0 //语音通道切换指令 
#define ZMICGB_Command_EN        0 //设置 MIC 音频参数
//基站相关指令(直放站需求指令) 
#define CCED_Command_EN          0 //获取小区信息
#define ZBCCH_Command_EN         0 //锁定 BCCH 信道（暂不支持）
#define ZBAND_Command_EN         0 //锁定 GSM 频段 
#define ZOPT_Command_EN          0 //锁定网络运营商
#define ZCALIST_Command_EN       0 //读取载波列表
// TTS 相关指令
#define ZTTS_Command_EN          0 //TTS(Text to Speech)语音播报业务 
#define ZTTSP_Command_EN         0 //设置 TTS(Text to Speech)语音播报参数(暂不支持切换发音人)
// 录音相关指令
#define ZAUDREC_Command_EN       0 //录音功能
#define ZFILEREAD_Command_EN     0 //读取文件
// GPS 相关指令 
#define ZGPSOPEN_Command_EN      0 //开关 GPS 指令
#define ZGPSSLEEP_Command_EN     0 //GPS 休眠指令 
#define ZGPSNMEARPT_Command_EN   0 //NMEA 上报控制指令 
#define ZGPSLOC_Command_EN       0 //获取 GPS 定位经纬度指令 
#define ZGPSUTC_Command_EN       0 //获取 GPS 时间指令
#define ZGPSHS_Command_EN        0 //获取角度速度指令
#define ZGPSIND_Command_EN       0 //GPS 定位状态上报 

#define SyncRate_Command_EN      1
#define CIPMUX_Command_EN        1
  
#define TCP_LinkType       1   //TCP客户端 
#define UDP_LinkType       2   //UDP 
#define SERVER_LinkType    3   //本地SERVER
/*********************************************************************************
数据类型定义
*********************************************************************************/
typedef enum
{
  Void      = 0,        //无参数指令
  Param     = 1,        //带 参 数 指 令 
  Query     = 2,        //查询指令 查询该指令当前设置的值
  Help      = 3,        //帮助指令： 列出该指令的可能参数，
}AT_Format;


typedef enum{ 
//2.1 普通指令
 AT_Again  = 0,   //该指令用于重复上一条指令
 ATA       = 1,   //应答呼叫
 ATD       = 2,   //发起呼叫
 ATDL      = 3,   //拨打最近呼出的电话号码 
 ATE       = 4,   //使能回馈通道
 ATH       = 5,   //断开当前连接
 ATI       = 6,   //提示模块生产厂家信息 
 ATQ       = 7,   //设置是否在终端回显返回值
 AtData    = 8,   //从数据模式切换到命令模式 
 ATO       = 9,   //从命令模式切换到数据模式 
 ATP       = 10,  //使用脉冲拨号
 ATS0      = 11,  //自动应答设置
 AT_CRC    = 12,  //设置来电类型
 CLVL      = 13,  //受话音量
 CLIP      = 14,  //设置来电号码显示
 ZSETMUTE  = 15,  //静音控制
 CIMI      = 16,  //查询国际 ID 号
 CGMR      = 17,  //获取产品版本号
 ECHO      = 18,  //回音消除控制
 GSN       = 19,  //获取当前 IMEI 号
 ZVERS     = 20,  //获取当前软件版本号
 CLCK      = 21,  //功能锁定
 CCFC      = 22,  //呼叫转移号码和条件设置 
 CCWA      = 23,  //呼叫等待控制
 CHLD      = 24,  //呼叫保持和多方会议
 TSIMINS   = 25,  //查询 SIM 卡状态
 CPWD      = 26,  //修改密码
 CGMI      = 27,  //查询模块厂家
 CSCS      = 28,  //字符集种类的选择
 CLCC      = 29,  //呼叫状态查询
//DTMF指令
 VTS     = 30,  //发送 DTMF 音
//网络服务指令 
 CREG    = 31,  //检查网络注册状态 
 COPS    = 32,  //网络选择
//移动设备控制和状态报告
 CPAS    = 33,  //模块状态查询
 CFUN    = 34,  //设置模块功能
 CMEE    = 35,  //移动设备错误报告
 ZPWROFF = 36,  //模块关机
 CPIN    = 37,  //输入 PIN 码
 EPIN1   = 38,  //输入 PIN1 码
 EPIN2   = 39,  //输入 PIN2 码
 EPINC   = 40,  //查询 PIN1， PIN2 剩余输入次数
 CSQ     = 41,  //信号强度查询
 CCLK    = 42,  //时钟管理
// 消息服务指令 
 CSCA    = 43,    //短信中心号码
 CNMA    = 44,    //短信确认
 CMGF    = 45,    //设置短消息模式
 CNMI    = 46,    //设置短信指示格式
 CMGR    = 47,    //读短消息
 CMGW    = 48,    //写短消息
 CSMS    = 49,    //选择短信服务
 CMGS    = 50,    //发送短信
 CPMS    = 51,    //首选短消息存储器
 CMGD    = 52,    //删除短信
 CMGL    = 53,    //短消息列表
 CMSS    = 54,    //发送存贮的短信
//电话本指令 
 CPBS    = 55,    //选择电话本存储
 CPBR    = 56,    //读取电话本信息
 CPBW    = 57,    //向电话本写信息
 CPBF    = 58,    //查找电话本信息
 CNUM    = 59,    //获取本机号码
//数据压缩指令 
 IFC     = 60,    //流控制
 DTR     = 61,    //设置 DTR 模式
 DCD     = 62,    //设置 DCD 模式
 IPR     = 63,    //设定模块波特率
 AT_F      = 64,    //恢复出厂设置
 AT_W       = 65,   //保存设置
//GPRS指令
 CGDCONT = 66,    //设置 PDP 格式
 CGACT   = 67,    //激活/解除 PDP 设置
 CGATT   = 68,    //检查GPRS 附着状态 
 CGCLASS = 69,    //GPRS 设备等级
//ZTE 特有指令
 ZSTR          = 70,    //查询模块状态信息
 ZGETICCID     = 71,    //获取 ICCID
 ZCSQ          = 72,    //设置自动显示 CSQ 的条件 
 ZEDT          = 73,    //设置 DTR 的检测模式 
 ZDSLEEP       = 74,    //32KHz 深度睡眠模式
 CUSD          = 75,    //发送 USSD 数据
 ZRINGPINMODE  = 76,    //设置 RING 引脚的信号模式 
 ZVERSWITCH    = 77,    //定制版本切换
// 网络参数指令 
 CSTT         = 78,    //设置 APN，
 CIICR        = 79,    //建立无线链路（GPRS或者CSD） 
 ZPPPCLOSE     = 80,    //关闭 GPRS 数据连接 
 CIFSR      = 81,    //查询当前模块 IP 地址值 
 ZDNSSERV      = 82,    //设置 DNS 服务器的 IP 地址 
 ZDNSGETIP     = 83,    //获取对应 Internet 域名的 IP 地址 
//TP 连接指令 
 CIPSTART      = 84,   //建立 TCP 服务器链接
 CIPSEND       = 85,   //向目标地址发送 TCP 数据 
 ZPPPSTATUS    = 86,   //查询 GPRS 连接状态 
 CIPCLOSE      = 87,   //关闭 TCP 链接
 ZIPSTATUS     = 88,   //查询当前 TCP 连接状态 
// UDP 连接指令 
 CIPSTARTU     = 89,  //建立 UDP 服务器链接 
 ZIPSENDU      = 90,  //向 UDP 服务器发送数据 
 ZIPSTATUSU    = 91,  //查询 UDP 状态
 ZIPCLOSEU     = 92,  //关闭 UDP 链接
// SERVER 指令 
 ZTCPLISTEN    = 93,  //端口监听功能设置 
 ZTCPSENDP     = 94,  //在被动打开的链路上发送数据 
 ZTCPCLOSEP    = 95,  //关闭监听到的连接
 ZTCPSTATUSP   = 96,  //查询被动打开链路
 ZIPTIMEOUT    = 97,  //设置模块连接服务器和发送数据的超时时间
 ZUDPLISTEN    = 98,  //端口监听功能设置
 ZUDPSENDP     = 99,  //在被动打开的链路上发送数据
// FTP 指令
 ZFTPLOGIN     = 100, //登录 FTP 服务器
 ZFTPTYPE      = 101, //设置 FTP 文件类型
 ZFTPUPLOAD    = 102, //上传文件
 ZFTPSIZE      = 103, //获取文件大小
 ZFTPDNLOAD    = 104, //下载文件
 ZFTPDNLOADEX  = 105, //下载 FTP 文件
 ZFTPDEL       = 106, //删除文件
 ZFTPQUIT      = 107, //退出 FTP 
// 透明传输指令 
 ZTRANSFER     = 108, //透明传输
//音频相关指令 
 ZCALLTONE     = 109, //摘机音设置
 ZDTMFTONE     = 110, //ZDTMF 拨号音设置 
 ZKTDSWITCH    = 111, //打开 DTMF 检测功能
 ZKTDIND       = 112, //主动上报检测的 DTMF 按键
 SPEAKER       = 113, //语音通道切换指令 
 ZMICGB        = 114, //设置 MIC 音频参数
//基站相关指令(直放站需求指令) 
 CCED          = 115, //获取小区信息
 ZBCCH         = 116, //锁定 BCCH 信道（暂不支持）
 ZBAND         = 117, //锁定 GSM 频段 
 ZOPT          = 118, //锁定网络运营商
 ZCALIST       = 119, //读取载波列表
// TTS 相关指令
 ZTTS          = 120, //TTS(Text to Speech)语音播报业务 
 ZTTSP         = 121, //设置 TTS(Text to Speech)语音播报参数(暂不支持切换发音人)
// 录音相关指令
 ZAUDREC       = 122, //录音功能
 ZFILEREAD     = 123, //读取文件
// GPS 相关指令 
 ZGPSOPEN      = 124, //开关 GPS 指令
 ZGPSSLEEP     = 125, //GPS 休眠指令 
 ZGPSNMEARPT   = 126, //NMEA 上报控制指令 
 ZGPSLOC       = 127, //获取 GPS 定位经纬度指令 
 ZGPSUTC       = 128, //获取 GPS 时间指令
 ZGPSHS        = 129, //获取角度速度指令
 ZGPSIND       = 130,  //GPS 定位状态上报 
 
 SyncRate      = 131,
 CIPMUX        = 132,
 NC_Command_MG2618 = 255 
}Command_MG2618; 

//该指令用于进行网络选择。
//操作命令参数
typedef struct {
  enum{
    Auto          = '0',  //自动网络选择，忽略参数<format><oper>
    Manual        = '1',  //人工选择网络，需要参数<format><oper>
    Repeal        = '2',  //从网络解除注册（ MTK此平台不支持此功能）
    SetingFormat  = '3',  //不牵涉网络注册的动作，此命令是用来设置<format>的。此时必须有参数<format>。
    Auto_Manual   = '4',  //4 manual/auto.人工注册网络不成功后，就自动注册网络。
  } mode;    
 enum{ 
   LongChar       = '0',  //长字符格式
   ShortChar      = '1',  //短字符格式
   Digit          = '2'   //数字格式
 }format;   //此值为设定运营商<oper>显示的格式） 
unsigned char* oper;   //运营商名称，根据 format 给出
}COPS_PramStructure_MG2618;

//该指令用于进行网络选择。
//操作命令参数
typedef struct {
 unsigned char mode;   //运营商名称，根据 format 给出
}CIPMUX_PramStructure_MG2618;


//设置 APN，用户名，密码
//操作命令参数
typedef struct { 
 unsigned char* APN;   //GPRS 运营商提供的 GPRS 访问节点(APN)。
 //unsigned char* user;   //用户登陆名
 //unsigned char* pwd;   //  用户登陆密码
 //enum{
  // PS = '0',  //PS域上网
   //CS =  '1'  //CS域上网
 //}MODE;  //选择上网方式
 //enum{
  // pap  = '0', 
  // chap = '1'
 //}AUTH;  //授权方式  
}ZPNUM_PramStructure_MG2618;

//打开 GPRS 数据连接  
//操作命令参数
typedef struct {
unsigned char* cmne;     //GPRS运营商提供的 GPRS 访问节点(APN)。
unsigned char* USER;    //用户登陆名。
unsigned char* PWD;     //用户登陆密码。
}ZPPPOPEN_PramStructure_MG2618;

//设置 DNS 服务器的 IP 地址 
//操作命令参数
typedef struct {
 unsigned char IP1[4];          //主DNS服务器地址
 unsigned char IP2[4];          //次DNS服务器地址
}IZDNSSERV_PramStructure_MG2618; 

//获取对应 Internet 域名的 IP 地址
//操作命令参数
typedef struct {
 unsigned char* domainname;     //Internet域名
}ZDNSGETIP_PramStructure_MG2618; 

//建立 TCP/UDP 服务器链接 
//操作命令参数
typedef struct {
  enum{
   TCP  = 0, 
   UDP  = 1
 }Mode;  //授权方式  
 unsigned char  CH;             //TCP 连接的通道号，取值范围:0-4
 unsigned char  IP[4];          //为一个目标地址 IP 值    
 unsigned short PORT;           //服务器端口
 //unsigned char* DomainName;
}IP_Connect_PramStructure_MG2618; 
//向目标地址发送 TCP / UDP 数据 
//操作命令参数
typedef struct {
 unsigned char  CH;            //TCP 连接通道号  取值范围:0-4
 unsigned short LENGTH;        //数据长度(最大支持 4K 个字符，支持 0x00~0xff 发送)
 unsigned char  *buff;         //指向数据缓冲区
 SerialPort_SendEndCallBack_Type callBack;
 void *callBack_arg;
}IP_Send_PramStructure_MG2618; 
//查询当前 TCP/UDP 连接状态 或 关闭 TCP/UDP 链接
//操作命令参数
typedef struct {
 unsigned char CH;             //TCP 连接通道号  取值范围:0-4 
}CIPCLOSE_PramStructure_MG2618; 

//端口监听功能设置
//操作命令参数
typedef struct {
 unsigned char State;          //'1':start listening '2':stop listening
 unsigned short PortNum;       //被监听的端口   
}ZTCPLISTEN_ZUDPLISTEN_PramStructure_MG2618;
//在被动打开的链路上发送数据
//操作命令参数
typedef struct {
 unsigned char CH;             //连接进来的客户端的标识
 unsigned short Length;        //要发送数据的长度 
 unsigned char *buff;          //指向数据缓冲区
}ZTCPSENDP_PramStructure_MG2618;
//关闭监听到的连接
//操作命令参数
typedef struct {
 unsigned char CH;             //连接进来的客户端的标识
}ZTCPCLOSEP_PramStructure_MG2618;
//查询被动打开链路
//操作命令参数
typedef struct {
 unsigned char CH;             //查询的通道号
}ZTCPSTATUSP_PramStructure_MG2618;
//设置模块连接服务器和发送数据的超时时间
//操作命令参数
typedef struct {
 unsigned short connect_timeout;      //模块作为客户端的连接超时时间，单位 s。
 unsigned short send_data_timeout;    //模块发送数据的超时时间，模块在此时间内数据还没有发送出去，就认为服务器或者网络异常，关闭此连接
}ZIPTIMEOUT_PramStructure_MG2618;
//在被动打开的链路上发送数据 UDP
//操作命令参数
typedef struct {
unsigned char IP[4];     //目的方的 IP 地址（依据接收数据上报） 
unsigned short PORT;     //目的方的端口（依据接收数据上报）
unsigned short LEN;      //要发送数据的长度 
unsigned char *buff;     //指向数据缓冲区
}ZUDPSENDP_PramStructure_MG2618;

//命令参数描述符
typedef  union{
   unsigned char                    CH_No;
   ZPNUM_PramStructure_MG2618        ZPNUM;            //设置 APN，用户名，密码 
   IP_Connect_PramStructure_MG2618   TcpUdp_Connect;
   IP_Send_PramStructure_MG2618      TcpUdp_Send; 
   CIPCLOSE_PramStructure_MG2618     CIPCLOSE; 
   COPS_PramStructure_MG2618         COPS;             //该指令用于进行网络选择。
   CIPMUX_PramStructure_MG2618       CIPMUX;           //设置多链路模式
  }CommandParameter_MG2618; 
//命令描述符
typedef struct{                   
  Command_MG2618              Cmd;       //操作命令码
  unsigned char              Step;
  AT_Format                   Format;
  CommandParameter_MG2618     CmdPram;   //命令参数
}CommandDescriptor_MG2618;

typedef struct{
  unsigned char  IP[4];          //为一个目标地址 IP 值    
  unsigned short PORT;           //服务器端口
  //unsigned short LOCAL_PORT;    //本地绑定端口，建议使用 1024 以上的端口绑定 当没有设置 LOCAL_PORT 时，端口随机分配 
}GprsInfr_Struct;  



typedef void (*GPRS_FinishCallBack)(Command_MG2618 cmd, DataBuffSpec_Struct *AckData);

typedef struct {
  
 // struct{
   // unsigned char RTEN:1;   //应答超时重发使能 
  //}CSR; 
  
  union
  {
    unsigned char Byte;
    struct{
      unsigned char Flg :1;  //通道占用状态 0 空闲 1 占用  
      unsigned char CN :1;   //通道连接状态 0 没有连接 1  以连接 
    }Bit;
  }CH_SR[4];
    
  GprsInfr_Struct GprsInfr[4];
    
  CommandDescriptor_MG2618   Command;    //当前命令描述符 

  GPRS_FinishCallBack        CallBack;   //操作完成回掉 
  unsigned char             Cnt;        //发送次数计数器
  SofTimer_Struct            *TimeOut;   //超时定时器
  SofTimer_Struct            *HeartBeatTime;  //GPRS心跳定时器
  //SofTimer_Struct            *CH_Time[4];  //

  struct{
   PortStatus      Status;           //端口状态指示 
   unsigned char   LOCAL_IP[4];      //本地IP地址   
   unsigned char   Rssi;             //接收信号强度
   unsigned char   Ber;              //信道位错误率
  }ModInf;  
  unsigned char  String[MG2616_ATORDER_LEN];    //存储ASCII命令序列
}MG2618DescriptorStruct;

typedef struct {
  struct {
    unsigned char  Valid:1;    //该命令是否有效
    unsigned char  Step :1;    //该密令是否需要多步A操作 
  }SR;                         //制状态寄存器
 unsigned char  PramLen;       //操作命令参数字节数
}CmdInformationStructure_MG2618;       
/*********************************************************************************
外部变量声明区
*********************************************************************************/
extern MG2618DescriptorStruct MG2618Infr;
/*********************************************************************************
函数声明区
*********************************************************************************/
FunctionalState Get_MG2168PowerState (void);

void PowerOff_GPRS (void);

void Decode_MG2618(unsigned char *gprs_ch, DataBuffSpec_Struct *Data);

unsigned short CompString (unsigned char *data,const char *sp,unsigned char **p);

void StopListen_MG2618(void);

SystemErrName SendCmd_MG2618(CommandDescriptor_MG2618 *Command,
                             GPRS_FinishCallBack CallBack,
                             unsigned char Reps,
                             unsigned short timer);


/********************************************************************************/
#endif
