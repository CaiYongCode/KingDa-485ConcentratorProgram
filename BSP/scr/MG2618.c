/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\inc\MG2618.h"
#include "..\..\Projects\inc\Common_API.h"
#include "..\..\Projects\inc\SofTimer.h"
//#include "..\..\APP\inc\MCPX.h"
#include "..\..\APP\inc\GPRS.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
//AT 指令列表
const char*  const CmdNameList_MG2618[] = { 
//2.1 普通指令
/*000*/ "A/",        //重复上一条指令
/*001*/ "A",       //应答呼叫
/*002*/ "D" ,      //发起呼叫
/*003*/ "DL",      //拨打最近呼出的电话号码 
/*004*/ "E",      //使能回馈通道
/*005*/ "H",      //断开当前连接
/*006*/ "I",       //提示模块生产厂家信息 
/*007*/ "Q",       //设置是否在终端回显返回值
/*008*/ "+++",       //从数据模式切换到命令模式 
/*009*/ "O",       //从命令模式切换到数据模式 
/*010*/ "P",       //使用脉冲拨号
/*011*/ "S0",      //自动应答设置
/*012*/ "+CRC",     //设置来电类型
/*013*/ "+CLVL",     //受话音量
/*014*/ "+CLIP",     //设置来电号码显示
/*015*/ "+ZSETMUTE", //静音控制
/*016*/ "+CIMI",    //查询国际 ID 号
/*017*/ "+CGMR",     //获取产品版本号
/*018*/ "+ECHO",     //回音消除控制
/*019*/ "+(C)GSN",   //获取当前 IMEI 号
/*020*/ "+ZVERS",    //获取当前软件版本号
/*021*/ "+CLCK" ,    //功能锁定
/*022*/ "+CCFC",     //呼叫转移号码和条件设置 
/*023*/ "+CCWA",     //呼叫等待控制
/*024*/ "+CHLD",     //呼叫保持和多方会议
/*025*/ "*TSIMINS",  //查询 SIM 卡状态
/*026*/ "+CPWD",     //修改密码
/*027*/ "+CGMI",     //查询模块厂家
/*028*/ "+CSCS",     //字符集种类的选择
/*029*/ "+CLCC",     //呼叫状态查询
//DTMF指令
/*030*/ "+VTS",     //发送 DTMF 音
//网络服务指令 
/*031*/ "+CREG",    //网络注册和漫游
/*032*/ "+COPS",    //网络选择
//移动设备控制和状态报告
/*033*/ "+CPAS",    //模块状态查询
/*034*/ "+CFUN",    //设置模块功能
/*035*/ "+CMEE",    //移动设备错误报告
/*036*/ "+ZPWROFF", //模块关机
/*037*/ "+CPIN",    //输入 PIN 码
/*038*/ "+EPIN1",   //输入 PIN1 码
/*039*/ "+EPIN2",   //输入 PIN2 码
/*040*/ "+EPINC",   //查询 PIN1， PIN2 剩余输入次数
/*041*/ "+CSQ",     //信号强度查询
/*042*/ "+CCLK",    //时钟管理
// 消息服务指令 
/*043*/ "+CSCA",    //短信中心号码
/*044*/ "+CNMA",    //短信确认
/*045*/ "+CMGF",    //设置短消息模式
/*046*/ "+CNMI",    //设置短信指示格式
/*047*/ "+CMGR",    //读短消息
/*048*/ "+CMGW",    //写短消息
/*049*/ "+CSMS",    //选择短信服务
/*050*/ "+CMGS",    //发送短信
/*051*/ "+CPMS",    //首选短消息存储器
/*052*/ "+CMGD",    //删除短信
/*053*/ "+CMGL",    //短消息列表
/*054*/ "+CMSS",    //发送存贮的短信
//电话本指令 
/*055*/ "+CPBS",    //选择电话本存储
/*056*/ "+CPBR",    //读取电话本信息
/*057*/ "+CPBW",    //向电话本写信息
/*058*/ "+CPBF",    //查找电话本信息
/*059*/ "+CNUM",    //获取本机号码
//数据压缩指令 
/*060*/ "+IFC",       //流控制
/*061*/ "&D",           //设置 DTR 模式
/*062*/ "&C",           //设置 DCD 模式
/*063*/ "+IPR",       //设定模块波特率
/*064*/ "&F",           //恢复出厂设置
/*065*/ "&W",           //保存设置
//GPRS//指令
/*066*/ "+CGDCONT",       //设置 PDP 格式
/*067*/ "+CGACT",         //激活/解除 PDP 设置
/*068*/ "+CGATT",         //设置 GPRS 业务 
/*069*/ "+CGCLASS",       //GPRS 设备等级
//ZTE 特有指令
/*070*/ "+ZSTR",           //查询模块状态信息
/*071*/ "+ZGETICCID",      //获取 ICCID
/*072*/ "+ZCSQ",           //设置自动显示 CSQ 的条件 
/*073*/ "+ZEDT" ,          //设置 DTR 的检测模式 
/*074*/ "+ZDSLEEP",        //32KHz 深度睡眠模式
/*075*/ "+CUSD",           //发送 USSD 数据
/*076*/ "+ZRINGPINMODE",   //设置 RING 引脚的信号模式 
/*077*/ "+ZVERSWITCH",     //定制版本切换
// 网络参数指令 
/*078*/ "+CSTT",           //设置 APN，用户名，密码 
/*079*/ "+CIICR",        //打开 GPRS 数据连接 
/*080*/ "+ZPPPCLOSE",       //关闭 GPRS 数据连接 
/*081*/ "+CIFSR",       //查询当前模块 IP 地址值 
/*082*/ "+ZDNSSERV",        //设置 DNS 服务器的 IP 地址 
/*083*/ "+ZDNSGETIP",       //获取对应 Internet 域名的 IP 地址 
//TCP 连接指令 
/*084*/ "+CIPSTART",        //建立 TCP 服务器链接
/*085*/ "+CIPSEND" ,        //向目标地址发送 TCP 数据 
/*086*/ "+ZPPPSTATUS",      //查询 GPRS 连接状态 
/*087*/ "+CIPCLOSE" ,       //关闭 TCP 链接
/*088*/ "+ZIPSTATUS",       //查询当前 TCP 连接状态 
// UDP 连接指令 
/*089*/ "+CIPSTARTU",       //建立 UDP 服务器链接 
/*090*/ "+CIPSENDU",        //向 UDP 服务器发送数据 
/*091*/ "+ZIPSTATUSU",      //查询 UDP 状态
/*092*/ "+CIPCLOSEU",       //关闭 UDP 链接
// SERVER 指令 
/*093*/ "+ZTCPLISTEN",  //端口监听功能设置 
/*094*/ "+ZTCPSENDP",   //在被动打开的链路上发送数据 
/*095*/ "+ZTCPCLOSEP",  //关闭监听到的连接
/*096*/ "+ZTCPSTATUSP", //查询被动打开链路
/*097*/ "+ZIPTIMEOUT",  //设置模块连接服务器和发送数据的超时时间
/*098*/ "+ZUDPLISTEN",  //端口监听功能设置
/*099*/ "+ZUDPSENDP" ,  //在被动打开的链路上发送数据
// FTP 指令
/*100*/ "+ZFTPLOGIN",     //登录 FTP 服务器
/*101*/ "+ZFTPTYPE",      //设置 FTP 文件类型
/*102*/ "+ZFTPUPLOAD",    //上传文件
/*103*/ "+ZFTPSIZE",      //获取文件大小
/*104*/ "+ZFTPDNLOAD",    //下载文件
/*105*/ "+ZFTPDNLOADEX",  //下载 FTP 文件
/*106*/ "+ZFTPDEL",       //删除文件
/*107*/ "+ZFTPQUIT",      //退出 FTP 
// 透明传输指令 
/*108*/ "+ZTRANSFER",  //透明传输
//音频相关指令 
/*109*/ "+ZCALLTONE",  //摘机音设置
/*110*/ "+ZDTMFTONE",  //ZDTMF 拨号音设置 
/*111*/ "+ZKTDSWITCH", //打开 DTMF 检测功能
/*112*/ "+ZKTDIND",    //主动上报检测的 DTMF 按键
/*113*/ "+SPEAKER",    //语音通道切换指令 
/*114*/ "+ZMICGB",     //设置 MIC 音频参数
//基站相关指令(直放站需求指令) ...................................................... 78
/*115*/ "+CCED",       //获取小区信息
/*116*/ "+ZBCCH",      //锁定 BCCH 信道（暂不支持）
/*117*/ "+ZBAND",      //锁定 GSM 频段 
/*118*/ "+ZOPT",       //锁定网络运营商
/*119*/ "+ZCALIST",    //读取载波列表
// TTS 相关指令
/*120*/ "+ZTTS" ,      //TTS(Text to Speech)语音播报业务 
/*121*/ "+ZTTSP",      //设置 TTS(Text to Speech)语音播报参数(暂不支持切换发音人)
// 录音相关指令
/*122*/ "+ZAUDREC",     //录音功能
/*123*/ "+ZFILEREAD",   //读取文件
// GPS 相关指令 
/*124*/ "+ZGPSOPEN",     //开关 GPS 指令
/*125*/ "+ZGPSSLEEP",    //GPS 休眠指令 
/*126*/ "+ZGPSNMEARPT",  //NMEA 上报控制指令 
/*127*/ "+ZGPSLOC",      //获取 GPS 定位经纬度指令 
/*128*/ "+ZGPSUTC",      //获取 GPS 时间指令
/*139*/ "+ZGPSHS",       //获取角度速度指令
/*130*/ "+ZGPSIND",       //GPS 定位状态上报 

/*131*/ "ATATATATATAT",       //

/*132*/ "+CIPMUX"       //GPS 定位状态上报
};   
/*********************************************************************************
公共变量定义区
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
私有变量定义区
*********************************************************************************/
MG2618DescriptorStruct MG2618Infr;
/*********************************************************************************
测试变量定义区
*********************************************************************************/
Command_MG2618 Command_pin  = AT_Again;
/*********************************************************************************
内部函数定义区
*********************************************************************************/
static SystemErrName CommandSerialization_MG2618(CommandDescriptor_MG2618 *Command);
static SystemErrName CSTT_PramFill(CommandDescriptor_MG2618 *Command);
static SystemErrName CIPMUX_PramFill(CommandDescriptor_MG2618 *Command);
static SystemErrName TcpUdpConnect_PramFill(CommandDescriptor_MG2618 *buff);
//static SystemErrName UpdSend_PramFill(CommandDescriptor_MG2618 *buff);
static SystemErrName TcpSend_PramFill(CommandDescriptor_MG2618 *buff);
static SystemErrName CIPCLOSE_PramFill(CommandDescriptor_MG2618 *buff);
static void AckTimeOut_MG2618 (void *p_tmr, void *p_arg);
static SystemErrName Default_ACK_Func(unsigned char *data,unsigned short length);
//static void TcpClose_ACK(DataBuffSpec_Struct *Data);
//static unsigned short MatchinString (unsigned char **data, const char **StringList, unsigned short ListSize);
//static unsigned char* AddIntToString(unsigned char* tp, unsigned long vlue);
static unsigned char GetData_TCP_UDP(DataBuffSpec_Struct *Data, DataBuffSpec_Struct* Rec);
//应答呼叫
#if (0 == ATA_Command_EN )
  #define AT_Again_ACK_Func   P_NULL
# else
  #define AT_Again_ACK_Func   P_NULL   
  //SystemErrName AT_Again_ACK_Func_ACK_Func(unsigned char *data, unsigned short length);
#endif
//应答呼叫
#if (0 == ATA_Command_EN )
  #define ATA_ACK_Func   P_NULL
# else
  SystemErrName ATA_ACK_Func(unsigned char *data, unsigned short length);
#endif
//发起呼叫
#if (0 == ATD_Command_EN )
#define ATD_ACK_Func   P_NULL
# else
SystemErrName ATD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//拨打最近呼出的电话号码 
#if (0 == ATDL_Command_EN )
#define ATDL_ACK_Func   P_NULL
# else
SystemErrName ATDL_ACK_Func(unsigned char *data, unsigned short length);
#endif
//使能回馈通道
#if (0 == ATE_Command_EN )
#define ATE_ACK_Func   P_NULL
# else
SystemErrName ATE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//断开当前连接
#if (0 == ATH_Command_EN )
#define ATH_ACK_Func   P_NULL
# else
SystemErrName ATH_ACK_Func(unsigned char *data, unsigned short length);
#endif
//提示模块生产厂家信息
#if (0 == ATI_Command_EN )
#define ATI_ACK_Func   P_NULL
# else
SystemErrName ATI_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置是否在终端回显返回值
#if (0 == ATQ_Command_EN )
#define ATQ_ACK_Func   P_NULL
# else
SystemErrName ATQ_ACK_Func(unsigned char *data, unsigned short length);
#endif
//从数据模式切换到命令模式 
#if (0 == AtData_Command_EN )
#define AtData_ACK_Func   P_NULL
# else
SystemErrName AtData_ACK_Func(unsigned char *data, unsigned short length);
#endif
//从命令模式切换到数据模式
#if (0 == ATO_Command_EN )
#define ATO_ACK_Func   P_NULL
# else
SystemErrName ATO_ACK_Func(unsigned char *data, unsigned short length); 
#endif
//使用脉冲拨号
#if (0 == ATP_Command_EN )
#define ATP_ACK_Func   P_NULL
# else
SystemErrName ATP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//自动应答设置
#if (0 == ATS0_Command_EN )
#define ATS0_ACK_Func   P_NULL
# else
SystemErrName ATS0_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置来电类型
#if (0 == AT_CRC_Command_EN )
#define AT_CRC_ACK_Func   P_NULL
# else
SystemErrName AT_CRC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//受话音量
#if (0 == CLVL_Command_EN )
#define CLVL_ACK_Func   P_NULL
# else
SystemErrName CLVL_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置来电号码显示
#if (0 == CLIP_Command_EN )
#define CLIP_ACK_Func   P_NULL
# else
SystemErrName CLIP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//静音控制
#if (0 == ZSETMUTE_Command_EN )
#define ZSETMUTE_ACK_Func   P_NULL
# else
SystemErrName ZSETMUTE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查询国际 ID 号
#if (0 == CIMI_Command_EN )
#define CIMI_ACK_Func   P_NULL
# else
SystemErrName CIMI_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取产品版本号
#if (0 == CGMR_Command_EN )
#define CGMR_ACK_Func   P_NULL
# else
SystemErrName CGMR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//回音消除控制
#if (0 == ECHO_Command_EN)
#define ECHO_ACK_Func   P_NULL
# else
SystemErrName ECHO_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取当前 IMEI 号
#if (0 == GSN_Command_EN)
#define GSN_ACK_Func   P_NULL
# else
SystemErrName GSN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取当前软件版本号
#if (0 == ZVERS_Command_EN)
#define ZVERS_ACK_Func   P_NULL
# else
SystemErrName ZVERS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//功能锁定
#if (0 == CLCK_Command_EN)
#define CLCK_ACK_Func   P_NULL
# else
SystemErrName CLCK_ACK_Func(unsigned char *data, unsigned short length);
#endif
//呼叫转移号码和条件设置

#if (0 == CCFC_Command_EN)
#define CCFC_ACK_Func   P_NULL
# else
SystemErrName CCFC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//呼叫等待控制
#if (0 == CCWA_Command_EN)
#define CCWA_ACK_Func   P_NULL
# else
SystemErrName CCWA_ACK_Func(unsigned char *data, unsigned short length);
#endif
//呼叫保持和多方会议
#if (0 == CHLD_Command_EN)
#define CHLD_ACK_Func   P_NULL
# else
SystemErrName CHLD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查询 SIM 卡状态
#if (0 == TSIMINS_Command_EN)
#define TSIMINS_ACK_Func   P_NULL
# else
SystemErrName TSIMINS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//修改密码
#if (0 == CPWD_Command_EN)
#define CPWD_ACK_Func   P_NULL
# else
SystemErrName CPWD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查询模块厂家
#if (0 == CGMI_Command_EN)
#define CGMI_ACK_Func   P_NULL
# else
SystemErrName CGMI_ACK_Func(unsigned char *data, unsigned short length);
#endif
//字符集种类的选择
#if (0 == CSCS_Command_EN)
#define CSCS_ACK_Func   P_NULL
# else
SystemErrName CSCS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//呼叫状态查询
#if (0 == CLCC_Command_EN)
#define CLCC_ACK_Func   P_NULL
# else
SystemErrName CLCC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//发送 DTMF 音
#if (0 == VTS_Command_EN)
#define VTS_ACK_Func   P_NULL
# else
SystemErrName VTS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//网络注册和漫游
#if (0 == CREG_Command_EN)
#define CREG_ACK_Func   P_NULL
# else
//网络服务指令 
SystemErrName CREG_ACK_Func(unsigned char *data, unsigned short length);
#endif
//网络选择
#if (0 == COPS_Command_EN)
#define COPS_ACK_Func   P_NULL
# else
SystemErrName COPS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//模块状态查询
#if (0 == CPAS_Command_EN)
#define CPAS_ACK_Func   P_NULL
# else
SystemErrName CPAS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置模块功能
#if (0 == CFUN_Command_EN)
#define CFUN_ACK_Func   P_NULL
# else
SystemErrName CFUN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//移动设备错误报告
#if (0 == CMEE_Command_EN)
#define CMEE_ACK_Func   P_NULL
# else
SystemErrName CMEE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//模块关机
#if (0 == ZPWROFF_Command_EN)
#define ZPWROFF_ACK_Func   P_NULL
# else
SystemErrName ZPWROFF_ACK_Func(unsigned char *data, unsigned short length);
#endif
//输入 PIN 码
#if (0 == CPIN_Command_EN)
#define CPIN_ACK_Func   P_NULL
# else
SystemErrName CPIN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//输入 PIN1 码
#if (0 == EPIN1_Command_EN)
#define EPIN1_ACK_Func   P_NULL
# else
SystemErrName EPIN1_ACK_Func(unsigned char *data, unsigned short length);
#endif
//输入 PIN2 码
#if (0 == EPIN2_Command_EN)
#define EPIN2_ACK_Func   P_NULL
# else
SystemErrName EPIN2_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查询 PIN1， PIN2 剩余输入次数
#if (0 == EPINC_Command_EN)
#define EPINC_ACK_Func   P_NULL
# else
SystemErrName EPINC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//信号强度查询
#if (0 == CSQ_Command_EN)
#define CSQ_ACK_Func   P_NULL
# else
SystemErrName CSQ_ACK_Func(unsigned char *data, unsigned short length);
#endif
//时钟管理
#if (0 == CCLK_Command_EN)
#define CCLK_ACK_Func   P_NULL
# else
SystemErrName CCLK_ACK_Func(unsigned char *data, unsigned short length);
#endif
//短信中心号码
#if (0 == CSCA_Command_EN)
#define CSCA_ACK_Func   P_NULL
# else
SystemErrName CSCA_ACK_Func(unsigned char *data, unsigned short length);
#endif
//短信确认
#if (0 == CNMA_Command_EN)
#define CNMA_ACK_Func   P_NULL
# else
SystemErrName CNMA_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置短消息模式
#if (0 == CMGF_Command_EN)
#define CMGF_ACK_Func   P_NULL
# else
SystemErrName CMGF_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置短信指示格式
#if (0 == CNMI_Command_EN)
#define CNMI_ACK_Func   P_NULL
# else
SystemErrName CNMI_ACK_Func(unsigned char *data, unsigned short length);
#endif
//读短消息
#if (0 == CMGW_Command_EN)
#define CMGR_ACK_Func   P_NULL
# else
SystemErrName CMGR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//写短消息
#if (0 == CMGW_Command_EN)
#define CMGW_ACK_Func   P_NULL
# else
SystemErrName CMGW_ACK_Func(unsigned char *data, unsigned short length);
#endif
//选择短信服务
#if (0 == CSMS_Command_EN)
#define CSMS_ACK_Func   P_NULL
# else
SystemErrName CSMS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//发送短信
#if (0 == CMGS_Command_EN)
#define CMGS_ACK_Func   P_NULL
# else
SystemErrName CMGS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//首选短消息存储器
#if (0 == CPMS_Command_EN)
#define CPMS_ACK_Func   P_NULL
# else
SystemErrName CPMS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//删除短信
#if (0 == CMGD_Command_EN)
#define CMGD_ACK_Func   P_NULL
# else
SystemErrName CMGD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//短消息列表
#if (0 == CMGL_Command_EN)
#define CMGL_ACK_Func   P_NULL
# else
SystemErrName CMGL_ACK_Func(unsigned char *data, unsigned short length);
#endif
//发送存贮的短信
#if (0 == CMSS_Command_EN)
#define CMSS_ACK_Func   P_NULL
# else
SystemErrName CMSS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//选择电话本存储
#if (0 == CPBS_Command_EN)
#define CPBS_ACK_Func   P_NULL
# else 
SystemErrName CPBS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//读取电话本信息
#if (0 == CPBR_Command_EN)
#define CPBR_ACK_Func   P_NULL
# else
SystemErrName CPBR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//向电话本写信息
#if (0 == CPBW_Command_EN)
#define CPBW_ACK_Func   P_NULL
# else
SystemErrName CPBW_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查找电话本信息
#if (0 == CPBF_Command_EN)
#define CPBF_ACK_Func   P_NULL
# else
SystemErrName CPBF_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取本机号码
#if (0 == CNUM_Command_EN)
#define CNUM_ACK_Func   P_NULL
# else
SystemErrName CNUM_ACK_Func(unsigned char *data, unsigned short length);
#endif
//流控制
#if (0 == IFC_Command_EN)
#define IFC_ACK_Func   P_NULL
# else
SystemErrName IFC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 DTR 模式
#if (0 == DTR_Command_EN)
#define DTR_ACK_Func   P_NULL
# else
SystemErrName DTR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 DCD 模式
#if (0 == DCD_Command_EN)
#define DCD_ACK_Func   P_NULL
# else
SystemErrName DCD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设定模块波特率
#if (0 == IPR_Command_EN)
#define IPR_ACK_Func   P_NULL
# else
SystemErrName IPR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//恢复出厂设置
#if (0 == AT_F_Command_EN)
#define AT_F_ACK_Func   P_NULL
# else
SystemErrName AT_F_ACK_Func(unsigned char *data, unsigned short length);
#endif
//保存设置
#if (0 == AT_W_Command_EN)
#define AT_W_ACK_Func   P_NULL
# else
SystemErrName AT_W_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 PDP 格式
#if (0 == CGDCONT_Command_EN)
#define CGDCONT_ACK_Func   P_NULL
# else
SystemErrName CGDCONT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//激活/解除 PDP 设置
#if (0 == CGACT_Command_EN)
#define CGACT_ACK_Func   P_NULL
# else
SystemErrName CGACT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 GPRS 业务
#if (0 == CGATT_Command_EN)
#define CGATT_ACK_Func   P_NULL
# else
SystemErrName CGATT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//GPRS 设备等级
#if (0 == CGCLASS_Command_EN)
#define CGCLASS_ACK_Func   P_NULL
# else
SystemErrName CGCLASS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查询模块状态信息
#if (0 == ZSTR_Command_EN)
#define ZSTR_ACK_Func   P_NULL
# else
SystemErrName ZSTR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取 ICCID
#if (0 == ZGETICCID_Command_EN)
#define ZGETICCID_ACK_Func   P_NULL
# else
SystemErrName ZGETICCID_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置自动显示 CSQ 的条件
#if (0 == ZCSQ_Command_EN)
#define ZCSQ_ACK_Func   P_NULL
# else
SystemErrName ZCSQ_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 DTR 的检测模式
#if (0 == ZEDT_Command_EN)
#define ZEDT_ACK_Func   P_NULL
# else
SystemErrName ZEDT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//32KHz 深度睡眠模式
#if (0 == ZDSLEEP_Command_EN)
#define ZDSLEEP_ACK_Func   P_NULL
# else
SystemErrName ZDSLEEP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//发送 USSD 数据
#if (0 == CUSD_Command_EN)
#define CUSD_ACK_Func   P_NULL
# else
SystemErrName CUSD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 RING 引脚的信号模式
#if (0 == ZRINGPINMODE_Command_EN)
#define ZRINGPINMODE_ACK_Func   P_NULL
# else
SystemErrName ZRINGPINMODE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//定制版本切换
#if (0 == ZVERSWITCH_Command_EN)
#define ZVERSWITCH_ACK_Func   P_NULL
# else
SystemErrName ZVERSWITCH_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 APN，用户名，密码 
#if (0 == CSTT_Command_EN)
#define CSTT_ACK_Func   P_NULL
# else
SystemErrName CSTT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//打开 GPRS 数据连接 
#if (0 == CIICR_Command_EN)  
#define CIICR_ACK_Func   P_NULL
# else
SystemErrName CIICR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//关闭 GPRS 数据连接
#if (0 == ZPPPCLOSE_Command_EN)
#define ZPPPCLOSE_ACK_Func   P_NULL
# else
SystemErrName ZPPPCLOSE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查询当前模块 IP 地址值
#if (0 == CIFSR_Command_EN)
#define CIFSR_ACK_Func   P_NULL
# else
SystemErrName CIFSR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 DNS 服务器的 IP 地址 
#if (0 == ZDNSSERV_Command_EN)
#define ZDNSSERV_ACK_Func   P_NULL
# else
SystemErrName ZDNSSERV_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取对应 Internet 域名的 IP 地址
#if (0 == ZDNSGETIP_Command_EN)
#define ZDNSGETIP_ACK_Func   P_NULL
# else
SystemErrName ZDNSGETIP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//建立 TCP 服务器链接
#if (0 == CIPSTART_Command_EN)
#define CIPSTART_ACK_Func   P_NULL
# else 
SystemErrName CIPSTART_ACK_Func(unsigned char *data, unsigned short length);
#endif
//向目标地址发送 TCP 数据 
#if (0 == CIPSEND_Command_EN)
#define CIPSEND_ACK_Func   P_NULL
# else
SystemErrName CIPSEND_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查询 GPRS 连接状态 
#if (0 == ZPPPSTATUS_Command_EN)
#define ZPPPSTATUS_ACK_Func   P_NULL
# else
SystemErrName ZPPPSTATUS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//关闭 TCP 链接
#if (0 == CIPCLOSE_Command_EN)
#define CIPCLOSE_ACK_Func   P_NULL
# else
SystemErrName CIPCLOSE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查询当前 TCP 连接状态
#if (0 == ZIPSTATUS_Command_EN)
#define ZIPSTATUS_ACK_Func   P_NULL
# else
SystemErrName ZIPSTATUS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//建立 UDP 服务器链接 
#if (0 == CIPSTARTU_Command_EN)
#define CIPSTARTU_ACK_Func   P_NULL
# else
SystemErrName CIPSTARTU_ACK_Func(unsigned char *data, unsigned short length);
#endif
//向 UDP 服务器发送数据
#if (0 == CIPSENDU_Command_EN)
#define CIPSENDU_ACK_Func   P_NULL
# else
SystemErrName CIPSENDU_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查询 UDP 状态
#if (0 == ZIPSTATUSU_Command_EN)
#define ZIPSTATUSU_ACK_Func   P_NULL
# else
SystemErrName ZIPSTATUSU_ACK_Func(unsigned char *data, unsigned short length);
#endif
//关闭 UDP 链接
#if (0 == CIPCLOSEU_Command_EN)
#define CIPCLOSEU_ACK_Func   P_NULL
# else
SystemErrName CIPCLOSEU_ACK_Func(unsigned char *data, unsigned short length);
#endif
//端口监听功能设置 
#if (0 == ZTCPLISTEN_Command_EN)
#define ZTCPLISTEN_ACK_Func   P_NULL
# else
SystemErrName ZTCPLISTEN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//在被动打开的链路上发送数据
#if (0 == ZTCPSENDP_Command_EN)
#define ZTCPSENDP_ACK_Func   P_NULL
# else
SystemErrName ZTCPSENDP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//关闭监听到的连接
#if (0 == ZTCPCLOSEP_Command_EN)
#define ZTCPCLOSEP_ACK_Func   P_NULL
# else
SystemErrName ZTCPCLOSEP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查询被动打开链路
#if (0 == ZTCPSTATUSP_Command_EN)
#define ZTCPSTATUSP_ACK_Func   P_NULL
# else
SystemErrName ZTCPSTATUSP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置模块连接服务器和发送数据的超时时间
#if (0 == ZIPTIMEOUT_Command_EN)
#define ZIPTIMEOUT_ACK_Func   P_NULL
# else
SystemErrName ZIPTIMEOUT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//端口监听功能设置
#if (0 == ZUDPLISTEN_Command_EN)
#define ZUDPLISTEN_ACK_Func   P_NULL
# else
SystemErrName ZUDPLISTEN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//在被动打开的链路上发送数据
#if (0 == ZUDPSENDP_Command_EN)
#define ZUDPSENDP_ACK_Func   P_NULL
# else
SystemErrName ZUDPSENDP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//登录 FTP 服务器
#if (0 == ZFTPLOGIN_Command_EN)
#define ZFTPLOGIN_ACK_Func   P_NULL
# else
SystemErrName ZFTPLOGIN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 FTP 文件类型
#if (0 == ZFTPTYPE_Command_EN)
#define ZFTPTYPE_ACK_Func   P_NULL
# else
SystemErrName ZFTPTYPE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//上传文件
#if (0 == ZFTPUPLOAD_Command_EN)
#define ZFTPUPLOAD_ACK_Func   P_NULL
# else
SystemErrName ZFTPUPLOAD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取文件大小
#if (0 == ZFTPSIZE_Command_EN)
#define ZFTPSIZE_ACK_Func   P_NULL
# else
SystemErrName ZFTPSIZE_ACK_Func(unsigned char *data, unsigned short length); 
#endif
//下载文件
#if (0 == ZFTPDNLOAD_Command_EN)
#define ZFTPDNLOAD_ACK_Func   P_NULL
# else
SystemErrName ZFTPDNLOAD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//下载 FTP 文件
#if (0 == ZFTPDNLOADEX_Command_EN)
#define ZFTPDNLOADEX_ACK_Func   P_NULL
# else
SystemErrName ZFTPDNLOADEX_ACK_Func(unsigned char *data, unsigned short length);
#endif
//删除文件
#if (0 == ZFTPDEL_Command_EN)
#define ZFTPDEL_ACK_Func   P_NULL
# else
SystemErrName ZFTPDEL_ACK_Func(unsigned char *data, unsigned short length);
#endif
//退出 FTP
#if (0 == ZFTPQUIT_Command_EN)
#define ZFTPQUIT_ACK_Func   P_NULL
# else
SystemErrName ZFTPQUIT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//透明传输
#if (0 == ZTRANSFER_Command_EN)
#define ZTRANSFER_ACK_Func   P_NULL
# else
SystemErrName ZTRANSFER_ACK_Func(unsigned char *data, unsigned short length);
#endif
//摘机音设置
#if (0 == ZCALLTONE_Command_EN)
#define ZCALLTONE_ACK_Func   P_NULL
# else
SystemErrName ZCALLTONE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//ZDTMF 拨号音设置 
#if (0 == ZDTMFTONE_Command_EN)
#define ZDTMFTONE_ACK_Func   P_NULL
# else
SystemErrName ZDTMFTONE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//打开 DTMF 检测功能
#if (0 == ZKTDSWITCH_Command_EN)
#define ZKTDSWITCH_ACK_Func   P_NULL
# else
SystemErrName ZKTDSWITCH_ACK_Func(unsigned char *data, unsigned short length);
#endif
//主动上报检测的 DTMF 按键
#if (0 == ZKTDIND_Command_EN)
#define ZKTDIND_ACK_Func   P_NULL
# else
SystemErrName ZKTDIND_ACK_Func(unsigned char *data, unsigned short length);
#endif
//语音通道切换指令 
#if (0 == SPEAKER_Command_EN)
#define SPEAKER_ACK_Func   P_NULL
# else
SystemErrName SPEAKER_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 MIC 音频参数
#if (0 == ZMICGB_Command_EN)
#define ZMICGB_ACK_Func   P_NULL
# else
SystemErrName ZMICGB_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取小区信息
#if (0 == CCED_Command_EN)
#define CCED_ACK_Func   P_NULL
# else
SystemErrName CCED_ACK_Func(unsigned char *data, unsigned short length);
#endif
//锁定 BCCH 信道（暂不支持）
#if (0 == ZBCCH_Command_EN)
#define ZBCCH_ACK_Func   P_NULL
# else
SystemErrName ZBCCH_ACK_Func(unsigned char *data, unsigned short length);
#endif
//锁定 GSM 频段 
#if (0 == ZBAND_Command_EN)
#define ZBAND_ACK_Func   P_NULL
# else
SystemErrName ZBAND_ACK_Func(unsigned char *data, unsigned short length);
#endif
//锁定网络运营商
#if (0 == ZOPT_Command_EN)
#define ZOPT_ACK_Func   P_NULL
# else
SystemErrName ZOPT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//读取载波列表
#if (0 == ZCALIST_Command_EN)
#define ZCALIST_ACK_Func   P_NULL
# else
SystemErrName ZCALIST_ACK_Func(unsigned char *data, unsigned short length);
#endif
//TTS(Text to Speech)语音播报业务
#if (0 == ZTTS_Command_EN)
#define ZTTS_ACK_Func   P_NULL
# else
SystemErrName ZTTS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 TTS(Text to Speech)语音播报参数(暂不支持切换发音人)
#if (0 == ZTTSP_Command_EN)
#define ZTTSP_ACK_Func   P_NULL
# else
SystemErrName ZTTSP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//录音功能
#if (0 == ZAUDREC_Command_EN)
#define ZAUDREC_ACK_Func   P_NULL
# else
SystemErrName ZAUDREC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//读取文件
#if (0 == ZFILEREAD_Command_EN)
#define ZFILEREAD_ACK_Func   P_NULL
# else
SystemErrName ZFILEREAD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//开关 GPS 指令
#if (0 == ZGPSOPEN_Command_EN)
#define ZGPSOPEN_ACK_Func   P_NULL
# else
SystemErrName ZGPSOPEN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//GPS 休眠指令 
#if (0 == ZGPSSLEEP_Command_EN)
#define ZGPSSLEEP_ACK_Func   P_NULL
# else
SystemErrName ZGPSSLEEP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//NMEA 上报控制指令
#if (0 == ZGPSNMEARPT_Command_EN)
#define ZGPSNMEARPT_ACK_Func   P_NULL
# else
SystemErrName ZGPSNMEARPT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取 GPS 定位经纬度指令
#if (0 == ZGPSLOC_Command_EN)
#define ZGPSLOC_ACK_Func   P_NULL
# else
SystemErrName ZGPSLOC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取 GPS 时间指令
#if (0 == ZGPSUTC_Command_EN)
#define ZGPSUTC_ACK_Func   P_NULL
# else
SystemErrName ZGPSUTC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取角度速度指令
#if (0 == ZGPSHS_Command_EN)
#define ZGPSHS_ACK_Func   P_NULL
# else
SystemErrName ZGPSHS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//GPS 定位状态上报
#if (0 == ZGPSIND_Command_EN)
#define ZGPSIND_ACK_Func   P_NULL
# else
SystemErrName ZGPSIND_ACK_Func(unsigned char *data, unsigned short length);
#endif

//
#if (0 == SyncRate_Command_EN)
#define SyncRate_ACK_Func   P_NULL
# else
SystemErrName SyncRate_ACK_Func(unsigned char *data, unsigned short length);
#endif

//
#if (0 == CIPMUX_Command_EN)
#define CIPMUX_ACK_Func   P_NULL
# else
SystemErrName CIPMUX_ACK_Func(unsigned char *data, unsigned short length);
#endif

typedef SystemErrName (*Command_ACK_Func) (unsigned char *data, unsigned short length);
const Command_ACK_Func Command_ACK_FuncList_MG2618[] = { 
//2.1 普通指令
 AT_Again_ACK_Func,  //= 0,   //该指令用于重复上一条指令
 ATA_ACK_Func,       //= 1,   //应答呼叫
 ATD_ACK_Func,       //= 2,   //发起呼叫
 ATDL_ACK_Func,      //= 3,   //拨打最近呼出的电话号码 
 ATE_ACK_Func,       //= 4,   //使能回馈通道
 ATH_ACK_Func,       //= 5,   //断开当前连接
 ATI_ACK_Func,       //= 6,   //提示模块生产厂家信息 
 ATQ_ACK_Func,       //= 7,   //设置是否在终端回显返回值
 AtData_ACK_Func,    //= 8,   //从数据模式切换到命令模式 
 ATO_ACK_Func,       //= 9,   //从命令模式切换到数据模式 
 ATP_ACK_Func,       //= 10,  //使用脉冲拨号
 ATS0_ACK_Func,      //= 11,  //自动应答设置
 AT_CRC_ACK_Func,    //= 12,  //设置来电类型
 CLVL_ACK_Func,      //= 13,  //受话音量
 CLIP_ACK_Func,      //= 14,  //设置来电号码显示
 ZSETMUTE_ACK_Func,  //= 15,  //静音控制
 CIMI_ACK_Func,      //= 16,  //查询国际 ID 号
 CGMR_ACK_Func,      //= 17,  //获取产品版本号
 ECHO_ACK_Func,      //= 18,  //回音消除控制
 GSN_ACK_Func,       //= 19,  //获取当前 IMEI 号
 ZVERS_ACK_Func,     //= 20,  //获取当前软件版本号
 CLCK_ACK_Func,      //= 21,  //功能锁定
 CCFC_ACK_Func,      //= 22,  //呼叫转移号码和条件设置 
 CCWA_ACK_Func,      //= 23,  //呼叫等待控制
 CHLD_ACK_Func,      //= 24,  //呼叫保持和多方会议
 TSIMINS_ACK_Func,   //= 25,  //查询 SIM 卡状态
 CPWD_ACK_Func,      //= 26,  //修改密码
 CGMI_ACK_Func,      //= 27,  //查询模块厂家
 CSCS_ACK_Func,      //= 28,  //字符集种类的选择
 CLCC_ACK_Func,      //= 29,  //呼叫状态查询
//DTMF指令
 VTS_ACK_Func,     //= 30,  //发送 DTMF 音
//网络服务指令 
 CREG_ACK_Func,    //= 31,  //网络注册和漫游
 COPS_ACK_Func,    //= 32,  //网络选择
//移动设备控制和状态报告
 CPAS_ACK_Func,    //= 33,  //模块状态查询
 CFUN_ACK_Func,    //= 34,  //设置模块功能
 CMEE_ACK_Func,    //= 35,  //移动设备错误报告
 ZPWROFF_ACK_Func, //= 36,  //模块关机
 CPIN_ACK_Func,    //= 37,  //输入 PIN 码
 EPIN1_ACK_Func,   //= 38,  //输入 PIN1 码
 EPIN2_ACK_Func,   //= 39,  //输入 PIN2 码
 EPINC_ACK_Func,   //= 40,  //查询 PIN1， PIN2 剩余输入次数
 CSQ_ACK_Func,     //= 41,  //信号强度查询
 CCLK_ACK_Func,    //= 42,  //时钟管理
// 消息服务指令 
 CSCA_ACK_Func,    //= 43,    //短信中心号码
 CNMA_ACK_Func,    //= 44,    //短信确认
 CMGF_ACK_Func,    //= 45,    //设置短消息模式
 CNMI_ACK_Func,    //= 46,    //设置短信指示格式
 CMGR_ACK_Func,    //= 47,    //读短消息
 CMGW_ACK_Func,    //= 48,    //写短消息
 CSMS_ACK_Func,    //= 49,    //选择短信服务
 CMGS_ACK_Func,    //= 50,    //发送短信
 CPMS_ACK_Func,    //= 51,    //首选短消息存储器
 CMGD_ACK_Func,    //= 52,    //删除短信
 CMGL_ACK_Func,    //= 53,    //短消息列表
 CMSS_ACK_Func,    //= 54,    //发送存贮的短信
//电话本指令 
 CPBS_ACK_Func,    //= 55,    //选择电话本存储
 CPBR_ACK_Func,    //= 56,    //读取电话本信息
 CPBW_ACK_Func,    //= 57,    //向电话本写信息
 CPBF_ACK_Func,    //= 58,    //查找电话本信息
 CNUM_ACK_Func,    //= 59,    //获取本机号码
//数据压缩指令 
 IFC_ACK_Func,     //= 60,    //流控制
 DTR_ACK_Func,     //= 61,    //设置 DTR 模式
 DCD_ACK_Func,     //= 62,    //设置 DCD 模式
 IPR_ACK_Func,     //= 63,    //设定模块波特率
 AT_F_ACK_Func,      //= 64,    //恢复出厂设置
 AT_W_ACK_Func,       //= 65,   //保存设置
//GPRS指令
 CGDCONT_ACK_Func, //= 66,    //设置 PDP 格式
 CGACT_ACK_Func,   //= 67,    //激活/解除 PDP 设置
 CGATT_ACK_Func,   //= 68,    //设置 GPRS 业务 
 CGCLASS_ACK_Func, //= 69,    //GPRS 设备等级
//ZTE 特有指令
 ZSTR_ACK_Func,         // = 70,    //查询模块状态信息
 ZGETICCID_ACK_Func,    // = 71,    //获取 ICCID
 ZCSQ_ACK_Func,         // = 72,    //设置自动显示 CSQ 的条件 
 ZEDT_ACK_Func,         // = 73,    //设置 DTR 的检测模式 
 ZDSLEEP_ACK_Func,      // = 74,    //32KHz 深度睡眠模式
 CUSD_ACK_Func,         // = 75,    //发送 USSD 数据
 ZRINGPINMODE_ACK_Func, // = 76,    //设置 RING 引脚的信号模式 
 ZVERSWITCH_ACK_Func,    //= 77,    //定制版本切换
// 网络参数指令 
 CSTT_ACK_Func,         //= 78,    //设置 APN，用户名，密码 
 CIICR_ACK_Func,      //= 79,    //打开 GPRS 数据连接 
 ZPPPCLOSE_ACK_Func,     //= 80,    //关闭 GPRS 数据连接 
 CIFSR_ACK_Func,      //= 81,    //查询当前模块 IP 地址值 
 ZDNSSERV_ACK_Func,     // = 82,    //设置 DNS 服务器的 IP 地址 
 ZDNSGETIP_ACK_Func,    // = 83,    //获取对应 Internet 域名的 IP 地址 
//TCP 连接指令 
 CIPSTART_ACK_Func,     // = 84,   //建立 TCP 服务器链接
 CIPSEND_ACK_Func,       //= 85,   //向目标地址发送 TCP 数据 
 ZPPPSTATUS_ACK_Func,    //= 86,   //查询 GPRS 连接状态 
 CIPCLOSE_ACK_Func,      //= 87,   //关闭 TCP 链接
 ZIPSTATUS_ACK_Func,     //= 88,   //查询当前 TCP 连接状态 
// UDP 连接指令 
 CIPSTARTU_ACK_Func,     //= 89,  //建立 UDP 服务器链接 
 CIPSENDU_ACK_Func,      //= 90,  //向 UDP 服务器发送数据 
 ZIPSTATUSU_ACK_Func,    //= 91,  //查询 UDP 状态
 CIPCLOSEU_ACK_Func,     //= 92,  //关闭 UDP 链接
// SERVER 指令 
 ZTCPLISTEN_ACK_Func,   /// = 93,  //端口监听功能设置 
 ZTCPSENDP_ACK_Func,     //= 94,  //在被动打开的链路上发送数据 
 ZTCPCLOSEP_ACK_Func,    //= 95,  //关闭监听到的连接
 ZTCPSTATUSP_ACK_Func,   //= 96,  //查询被动打开链路
 ZIPTIMEOUT_ACK_Func,    //= 97,  //设置模块连接服务器和发送数据的超时时间
 ZUDPLISTEN_ACK_Func,    //= 98,  //端口监听功能设置
 ZUDPSENDP_ACK_Func,     //= 99,  //在被动打开的链路上发送数据
// FTP 指令
 ZFTPLOGIN_ACK_Func,     //= 100, //登录 FTP 服务器
 ZFTPTYPE_ACK_Func,      //= 101, //设置 FTP 文件类型
 ZFTPUPLOAD_ACK_Func,    //= 102, //上传文件
 ZFTPSIZE_ACK_Func,      //= 103, //获取文件大小
 ZFTPDNLOAD_ACK_Func,    //= 104, //下载文件
 ZFTPDNLOADEX_ACK_Func,  //= 105, //下载 FTP 文件
 ZFTPDEL_ACK_Func,       //= 106, //删除文件
 ZFTPQUIT_ACK_Func,      //= 107, //退出 FTP 
// 透明传输指令 
 ZTRANSFER_ACK_Func,     //= 108, //透明传输
//音频相关指令 
 ZCALLTONE_ACK_Func,     //= 109, //摘机音设置
 ZDTMFTONE_ACK_Func,    // = 110, //ZDTMF 拨号音设置 
 ZKTDSWITCH_ACK_Func,    //= 111, //打开 DTMF 检测功能
 ZKTDIND_ACK_Func,       //= 112, //主动上报检测的 DTMF 按键
 SPEAKER_ACK_Func,       //= 113, //语音通道切换指令 
 ZMICGB_ACK_Func,        //= 114, //设置 MIC 音频参数
//基站相关指令(直放站需求指令) 
 CCED_ACK_Func,          //= 115, //获取小区信息
 ZBCCH_ACK_Func,         //= 116, //锁定 BCCH 信道（暂不支持）
 ZBAND_ACK_Func,         //= 117, //锁定 GSM 频段 
 ZOPT_ACK_Func,          //= 118, //锁定网络运营商
 ZCALIST_ACK_Func,       //= 119, //读取载波列表
// TTS 相关指令
 ZTTS_ACK_Func,          //= 120, //TTS(Text to Speech)语音播报业务 
 ZTTSP_ACK_Func,         //= 121, //设置 TTS(Text to Speech)语音播报参数(暂不支持切换发音人)
// 录音相关指令
 ZAUDREC_ACK_Func,       //= 122, //录音功能
 ZFILEREAD_ACK_Func,     //= 123, //读取文件
// GPS 相关指令 
 ZGPSOPEN_ACK_Func,      //= 124, //开关 GPS 指令
 ZGPSSLEEP_ACK_Func,    // = 125, //GPS 休眠指令 
 ZGPSNMEARPT_ACK_Func,  // = 126, //NMEA 上报控制指令 
 ZGPSLOC_ACK_Func,      // = 127, //获取 GPS 定位经纬度指令 
 ZGPSUTC_ACK_Func,      // = 128, //获取 GPS 时间指令
 ZGPSHS_ACK_Func,       // = 129, //获取角度速度指令
 ZGPSIND_ACK_Func,      // = 130,  //GPS 定位状态上报 
   
 SyncRate_ACK_Func,      //= 131,
 CIPMUX_ACK_Func         //= 132
 //NC_Command_MG2618 = 255 
};
/*********************************************************************************
功能代码定义区
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                // 
 Output:        //
 Return:        //
 Others:        //
                  
*********************************************************************************/
SystemErrName SendCmd_MG2618(CommandDescriptor_MG2618 *Command,
                             GPRS_FinishCallBack CallBack,
                             unsigned char Reps,
                             unsigned short timer)
{
  SystemErrName  err;
  SystemErrName  err2;  
  Command_pin = Command->Cmd;
  if(P_NULL == Command_ACK_FuncList_MG2618[Command->Cmd])
  {
   if(AT_Again != Command->Cmd)
   {return CmdValid;}
  }
  if(OFF == MG2618Infr.ModInf.Status)  
  {return Port_OFF;}     //端口关闭
  if(WaitAck == MG2618Infr.ModInf.Status)  
  {return PortBusy;}     //端口端繁忙
  
  MG2618Infr.Cnt = Reps;
    
  err = CommandSerialization_MG2618(Command);
  if(err > Warning) 
  {return err;} 
  MG2618Infr.Command.Step = 0; 
  //获取命令计时器
  MG2618Infr.TimeOut = SofTimerCreate( timer,             //周期
                                      TimerTB_10MS,       //时基
                                      SofTimer_PERIODIC,  //模式
                                      AckTimeOut_MG2618,  //回调
                                      P_NULL);//回调参数
 if(P_NULL == MG2618Infr.TimeOut)
 {return Nothing;}  //获取命令计时器失败 
 if(AT_Again != Command->Cmd)
 {MG2618Infr.Command.Cmd = Command->Cmd;}
 MG2618Infr.CallBack = CallBack;
 err2 = SendStringToMG2618(MG2618Infr.String); 
 if(NO_ERR == err2)
 {
  //MG2618Infr.Cnt = 1; 
  MG2618Infr.ModInf.Status = WaitAck;
  SofTimerStart(MG2618Infr.TimeOut);
  GprsStop_time = 0;  
 }
 else
 {SofTimerDel(&(MG2618Infr.TimeOut));}
 if(err2 > err)
 {err = err2;}  
 return err;  
}
/*********************************************************************************
 Function:      //
 Description:   // 模块输入数据解析
Input:          // data     输入数据首地址
                // length   输入数据长度
                //
                //   
 Output:        //
 Return:        //
 Others:        //              
*********************************************************************************/
void Decode_MG2618(unsigned char *gprs_ch, DataBuffSpec_Struct *Data)
{
 SystemErrName err = DataValid; 
 unsigned short itemp = 0; 
 DataBuffSpec_Struct Rec;
 //寻找参数开始字节
 *gprs_ch = 255;
 
 if(WaitAck == MG2618Infr.ModInf.Status)
 {
   if(CIPSEND == MG2618Infr.Command.Cmd)
   {
     Command_ACK_FuncList_MG2618[MG2618Infr.Command.Cmd](&((Data->pbuff)[itemp]),Data->length);
     return;
   }
   else
   {
     while (1)
     {
       if(IsLETTER(*(Data->pbuff)) 
          ||Isletter(*(Data->pbuff))
            ||IsDigit(*(Data->pbuff))
              ||('+'== *(Data->pbuff))
                ||('>'==*(Data->pbuff)))
       {break;}
       if(0 == --(Data->length)){return;}
       Data->pbuff ++;
     }
     if(WaitAck == MG2618Infr.ModInf.Status)
     {
       if((('A'==(Data->pbuff)[0])&&('T'==(Data->pbuff)[1]))
          ||(('A'==(Data->pbuff)[0])&&('/'==(Data->pbuff)[1])))
       { //消除回显
         itemp = 0;
         while (1)
         {
           if(('\r'==(Data->pbuff)[itemp])
              ||('\n'==(Data->pbuff)[itemp]))
           {break;}
           if(0 == --(Data->length))
           { return;}
           itemp ++;
         }
         while (1)
         {//寻找参数开始字节
           if('+'==(Data->pbuff)[itemp]
              ||IsLETTER((Data->pbuff)[itemp])
                ||Isletter((Data->pbuff)[itemp])
                  ||IsDigit((Data->pbuff)[itemp])
                    ||('>'==(Data->pbuff)[itemp]))
           {break;}
           if(0 == --(Data->length)){ return;}
           itemp ++;
         }
       }
       if(P_NULL != Command_ACK_FuncList_MG2618[MG2618Infr.Command.Cmd])
       {
         err = Command_ACK_FuncList_MG2618[MG2618Infr.Command.Cmd](&((Data->pbuff)[itemp]),Data->length);
         if(NO_ERR == err){return;} 
       }
     }
   }
 }
 
 const char*  const IndicatorList_MG2618[] = { 
   "+RECEIVE:",        //提示从当前数据链路接收数据 
   "0, CLOSED\r\n",       //提示TCP通道关闭提示 
   "1, CLOSED\r\n",       //提示TCP通道关闭提示
   "2, CLOSED\r\n",       //提示TCP通道关闭提示
   "3, CLOSED\r\n",       //提示TCP通道关闭提示
   "4, CLOSED\r\n",       //提示TCP通道关闭提示
   "5, CLOSED\r\n",       //提示TCP通道关闭提示
 }; 
 while(Data->length)
 {
   if(IsDigit(*(Data->pbuff))||('+'==*(Data->pbuff)))
   {
     itemp = MatchinString(&(Data->pbuff),(const char**)IndicatorList_MG2618, 7);   //
     switch(itemp)
     {
     case 0 :   //提示接收 TCP 数据 
       {
         *gprs_ch = GetData_TCP_UDP(Data, &Rec);      
         if(255 == *gprs_ch)
         {
           Data->pbuff += sizeof("+RECEIVE");
         }
         break;
       }
     case 1 :   //提示TCP通道关闭提示
     case 2 :   //提示TCP通道关闭提示 
     case 3 :   //提示TCP通道关闭提示 
     case 4 :   //提示TCP通道关闭提示 
     case 5 :   //提示TCP通道关闭提示  
     case 6 :   //提示TCP通道关闭提示   
       {
         GprsClose_UEV(itemp-1);
         //Data->pbuff += sizeof(IndicatorList_MG2618[itemp]);
         //Data->length -=sizeof(IndicatorList_MG2618[itemp]);
         break;
       }
     } 
   }
   if(Data->length > 0)
   { 
     Data->length --;
     Data->pbuff ++;
   }
 }
 Data->length = Rec.length;
 Data->pbuff = Rec.pbuff;
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          // 
                // 
                // 
 Output:        //
 Return:        //
 Others:        //
                  
*********************************************************************************/
unsigned short CompString (unsigned char *data,const char *sp,unsigned char **p)
{
 unsigned short length = 0;
 unsigned char *tp; 
 //比对查找参数名
 tp = data;
 while(sp[length] == tp[length])
 {length ++;}
 if('\0'== sp[length])
 {
   if(P_NULL != p)
   {*p = &tp[length];}
   return length;
 }
 else
 {
   if(P_NULL != p)
   {*p = data;}
   return 0;
 }
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                // 
 Output:        //
 Return:        //
 Others:        //
                  
*********************************************************************************/
void StopListen_MG2618(void)
{
 if(P_NULL != MG2618Infr.TimeOut)
 {SofTimerDel(&(MG2618Infr.TimeOut));}
 MG2618Infr.ModInf.Status = Idle; 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
FunctionalState Get_MG2168PowerState (void)
{
 if(0 == MG2618_PowerFlg)
 {return DISABLE;}
 else
 {return ENABLE;}
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void PowerOff_GPRS (void)
{
 unsigned char *p ;
 unsigned short itmp; 
 GPIO_InitTypeDef        GPIO_InitStructure;
 
 GprsClose_UEV(1);
 GprsClose_UEV(2);
 GprsClose_UEV(3);
 GprsClose_UEV(4);
 
 SofTimerDel(&(MG2618Infr.HeartBeatTime));
 SofTimerDel(&(MG2618Infr.TimeOut)) ;
 
 GPIO_SetBits(GPIOA, GPIO_Pin_2);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 USART_Cmd(USART2, DISABLE);
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);
 MG2618_PowerOFF( );
 MG2618Infr.ModInf.Status = OFF;
 
 p = (unsigned char *)&MG2618Infr;
 itmp = 0;
 //模块描述数据清零
 while(itmp < sizeof(MG2618DescriptorStruct))
 {p[itmp++] = 0;}
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                //
                //
                //   
 Output:        //
 Return:        //
 Others:        //
                  
*********************************************************************************/
static SystemErrName CommandSerialization_MG2618 (CommandDescriptor_MG2618 *Command)
{
  SystemErrName err = NO_ERR;
  unsigned short  itemp = 0;
  
  while (itemp < MG2616_ATORDER_LEN)
  {MG2618Infr.String[itemp++]  = '\0';} 
  
  switch(Command->Cmd)
  {
  case  AtData:
  case  AT_Again:  
    {break;}
  default:     //查询 UDP 状态
    {AddString(MG2618Infr.String, "AT");break;}
  }
  AddString(MG2618Infr.String,(unsigned char*)CmdNameList_MG2618[Command->Cmd]); 
  
  switch(Command->Format)
  {
  case  Void:
    {break;}
  case  Query:
    {AddString(MG2618Infr.String,"?");break; }
  case  Help:
    {AddString(MG2618Infr.String,"=?");break; }   
  case  Param:
    {
      if(Param == Command->Format)
      {
        itemp = 0; 
        while(itemp < sizeof(CommandParameter_MG2618))
        {
          ((unsigned char*)&MG2618Infr.Command.CmdPram)[itemp] = ((unsigned char*)&(Command->CmdPram))[itemp];
          itemp++ ;
        }
      }
      switch(Command->Cmd)
      {
      case  CSTT: 
        {err = CSTT_PramFill(Command);break;}
      case  CIPSTART:
      case  CIPSTARTU:
        {err = TcpUdpConnect_PramFill(Command);break;}
      case CIPSEND:         //向目标地址发送 TCP 数据 
        {err = TcpSend_PramFill(Command);break;}
      //case CIPSENDU:      //向 UDP 服务器发送数据 
       // {err = UpdSend_PramFill(Command); break; }
      case CIPCLOSE:      //关闭 TCP 链接
        {err = CIPCLOSE_PramFill(Command);break;}
      case CIPMUX:
        {err = CIPMUX_PramFill(Command);break;}
      default:     //查询 UDP 状态
        {err = CmdValid;break;}
      }
      break;
    }
  default:     //查询 UDP 状态
    {err = CmdValid; break;}
  }  
  if(NO_ERR == err)
  {AddString(MG2618Infr.String, "\r\n");}
  return err;
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                //
                //
                //
 Output:        //
 Return:        //
 Others:        //AT+ZPNUM=<APN>
                //AT+ZPNUM="cmnet"
*********************************************************************************/
static SystemErrName CSTT_PramFill(CommandDescriptor_MG2618 *Command)
{
 unsigned char* p;

 p = MG2618Infr.String;
 while('\0'!= *p)
 { p++;}
 
 *p++ = '=';
 if(P_NULL != Command->CmdPram.ZPNUM.APN)
 { p =  AddString(p, Command->CmdPram.ZPNUM.APN);}
 return NO_ERR; 

}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                //
                //
                //
 Output:        //
 Return:        //
 Others:        //AT+ZPNUM=<APN>,<USER>,<PWD>,<MODE>,<AUTH>
                //AT+ZPNUM="cmnet","user","pwd",1,0
*********************************************************************************/
static SystemErrName CIPMUX_PramFill(CommandDescriptor_MG2618 *Command)
{
  unsigned char* p;
  
  p = MG2618Infr.String;
  while('\0'!= *p)
  { p++;}
  
  *p++ = '=';
  *p++ = Command->CmdPram.CIPMUX.mode;
  return NO_ERR;
  
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                //
                //
                //
 Output:        //
 Return:        //
 Others:        //AT+CIPSTART=<N>,<IP>,<SERVER_PORT>[,<LOCAL_PORT>]
                  AT+CIPSTART=1,61.144.216.219,2332
*********************************************************************************/
static SystemErrName TcpUdpConnect_PramFill(CommandDescriptor_MG2618 *Command)
{
 unsigned char* p;

 if(Command->CmdPram.TcpUdp_Connect.CH > 3)
  {return IllegalParam;}
 p = MG2618Infr.String;
 while('\0'!= *p)
 { p++;}
 *p++ = '='; 
 
 *p++ = Command->CmdPram.TcpUdp_Connect.CH +'0';
 *p++ = ',';
 
 if(TCP == Command->CmdPram.TcpUdp_Connect.Mode)
 { p = AddString(p, "TCP");}
 else{p = AddString(p, "UDP");}
 *p++ = ',';
 
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Connect.IP[0]);
 *p++ = '.'; 
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Connect.IP[1]);
 *p++ = '.'; 
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Connect.IP[2]);
 *p++ = '.'; 
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Connect.IP[3]);
 *p++ = ','; 
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Connect.PORT);
 
 return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                // 
 Output:        //
 Return:        //
 Others:        // AT+CIPSEND= port,length<CR>
                   AT+CIPSEND=1,10
                   >abcdefghij
                   +CIPSEND:OK
                   OK
                   提示'>'后再发送数据
*********************************************************************************/
static SystemErrName TcpSend_PramFill(CommandDescriptor_MG2618 *Command)
{
 unsigned char* p;
 if(Command->CmdPram.TcpUdp_Send.CH >3)
  {return IllegalParam;}
 p = MG2618Infr.String;
 while('\0'!= *p){p++;}
 *p++ = '='; 
 *p++ = Command->CmdPram.TcpUdp_Send.CH +'0';
 *p++ = ',';
 if(Command->CmdPram.TcpUdp_Send.LENGTH > 4096)
 {return IllegalParam;  }
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Send.LENGTH);
 return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                // 
 Output:        //
 Return:        //
 Others:        // AT+CIPSEND= port,length<CR>
                   AT+CIPSEND=1,10
                   >abcdefghij
                   +CIPSEND:OK
                   OK
                   提示'>'后再发送数据
*********************************************************************************/
/*
static SystemErrName UpdSend_PramFill(CommandDescriptor_MG2618 *Command)
{
 unsigned char* p;

 if((Command->CmdPram.TcpUdp_Connect.CH < 1)
     ||(Command->CmdPram.TcpUdp_Connect.CH > 4))
  {return IllegalParam;}
 
 p = MG2618Infr.String;
 while('\0'!= *p){p++;}
  
 *p++ = Command->CmdPram.TcpUdp_Send.CH +'0';
 *p++ = ',';
 if(Command->CmdPram.TcpUdp_Send.LENGTH > 1000)
 {return IllegalParam;  }
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Send.LENGTH);
 return NO_ERR; 
}*/
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                // 
 Output:        //
 Return:        //
 Others:        // AT+CIPCLOSE=<N>
*********************************************************************************/
static SystemErrName CIPCLOSE_PramFill(CommandDescriptor_MG2618 *Command)
{
 unsigned char* p;
 if(Command->CmdPram.CIPCLOSE.CH > 3)
  {return IllegalParam;}
 p = MG2618Infr.String;
 while('\0'!= *p){p++;};
  *p++ = '=';
 *p++ = Command->CmdPram.CIPCLOSE.CH +'0';
 return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void AckTimeOut_MG2618 (void *p_tmr, void *p_arg)
{
  //if(CIPSEND == MG2618Infr.Command.Cmd)
  //{
   // CIPSEND_ACK_Func("> ", 2);
  //}
  //else 
  if(MG2618Infr.Cnt-- > 0)
  {
    MG2618Infr.Command.Step = 0;
    SendStringToMG2618(MG2618Infr.String);
  }
  else
  {
   StopListen_MG2618( ); 
   if(P_NULL != MG2618Infr.CallBack)
   {MG2618Infr.CallBack(MG2618Infr.Command.Cmd,P_NULL);}
  }
}
/*********************************************************************************
 Function:      //
 Description:   //应答呼叫
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATA_Command_EN)
SystemErrName ATA_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //发起呼叫
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATD_Command_EN)
SystemErrName ATD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //拨打最近呼出的电话号码
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATDL_Command_EN)
SystemErrName ATDL_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //使能回馈通道
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATE_Command_EN)
SystemErrName ATE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //断开当前连接
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATH_Command_EN)
SystemErrName ATH_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //提示模块生产厂家信息 
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATI_Command_EN)
SystemErrName ATI_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置是否在终端回显返回值
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATQ_Command_EN)
SystemErrName ATQ_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //从数据模式切换到命令模式
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != AtData_Command_EN)
SystemErrName AtData_ACK_Func(unsigned char *data, unsigned short length) 
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //从命令模式切换到数据模式
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATO_Command_EN)
SystemErrName ATO_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //使用脉冲拨号
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATP_Command_EN)
SystemErrName ATP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //自动应答设置
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATS0_Command_EN)
SystemErrName ATS0_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置来电类型
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != AT_CRC_Command_EN)
SystemErrName AT_CRC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //受话音量
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CLVL_Command_EN)
SystemErrName CLVL_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置来电号码显示
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CLIP_Command_EN)
SystemErrName CLIP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //静音控制
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZSETMUTE_Command_EN)
SystemErrName ZSETMUTE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //查询国际 ID 号
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIMI_Command_EN)
SystemErrName CIMI_ACK_Func(unsigned char *data, unsigned short length)
{ 
 return Default_ACK_Func(data, length);
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取产品版本号
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CGMR_Command_EN)
SystemErrName CGMR_ACK_Func(unsigned char *data, unsigned short length)
{
 unsigned short  itemp; 
 itemp = CompString(data,CmdNameList_MG2618[CGMR],&data);
 if(0 != itemp)
 {
  length -= itemp;  
  return Default_ACK_Func(data, length);
 }
 else
 {return DataValid;}  
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //回音消除控制
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ECHO_Command_EN)
SystemErrName ECHO_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取当前 IMEI 号
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != GSN_Command_EN)
SystemErrName GSN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取当前软件版本号
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZVERS_Command_EN)
SystemErrName ZVERS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //功能锁定
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CLCK_Command_EN)
SystemErrName CLCK_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //呼叫转移号码和条件设置
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CCFC_Command_EN)
SystemErrName CCFC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //呼叫等待控制
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CCWA_Command_EN)
SystemErrName CCWA_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //呼叫保持和多方会议
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CHLD_Command_EN)
SystemErrName CHLD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //查询 SIM 卡状态
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != TSIMINS_Command_EN)
SystemErrName TSIMINS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //修改密码
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPWD_Command_EN)
SystemErrName CPWD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //查询模块厂家
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CGMI_Command_EN)
SystemErrName CGMI_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //字符集种类的选择
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CSCS_Command_EN)
SystemErrName CSCS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //呼叫状态查询
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CLCC_Command_EN)
SystemErrName CLCC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //发送 DTMF 音
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != VTS_Command_EN)
//DTMF指令
SystemErrName VTS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //网络注册和漫游
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CREG_Command_EN)
//网络服务指令 
SystemErrName CREG_ACK_Func(unsigned char *data, unsigned short length)
{
 unsigned short  itemp; 
 itemp = CompString(data,CmdNameList_MG2618[CREG],&data);
 if(0 != itemp)
 {
  length -= itemp;  
  return Default_ACK_Func(data, length);
 }
 else
 {return DataValid;} 
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //网络选择
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != COPS_Command_EN)
SystemErrName COPS_ACK_Func(unsigned char *data, unsigned short length)
{
 //请在这里增加实现代码
 unsigned short  itemp; 
 itemp = CompString(data,CmdNameList_MG2618[COPS],&data);
 if(0 != itemp)
 {
  length -= itemp;  
  return Default_ACK_Func(data, length);
 }
 else
 {return DataValid;} 
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //模块状态查询
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPAS_Command_EN)
//移动设备控制和状态报告
SystemErrName CPAS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置模块功能
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CFUN_Command_EN)
SystemErrName CFUN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //移动设备错误报告
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMEE_Command_EN)
SystemErrName CMEE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //模块关机
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZPWROFF_Command_EN)
SystemErrName ZPWROFF_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //输入 PIN 码
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPIN_Command_EN)
SystemErrName CPIN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //输入 PIN1 码
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != EPIN1_Command_EN)
SystemErrName EPIN1_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //输入 PIN2 码
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != EPIN2_Command_EN)
SystemErrName EPIN2_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //查询 PIN1， PIN2 剩余输入次数
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != EPINC_Command_EN)
SystemErrName EPINC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //信号强度查询
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CSQ_Command_EN)
SystemErrName CSQ_ACK_Func(unsigned char *data, unsigned short length)
{
 DataBuffSpec_Struct AckData ;
 unsigned short  itemp; 
 unsigned short  itemp1;
 itemp = CompString(data,CmdNameList_MG2618[CSQ],&data);
 if(0 != itemp)
 {
  length -= itemp;  
  while (1)
  {
   if(IsDigit(*data)){break;}
   if(0 == --length){ return DataValid;}
   data ++;
  }
  AckData.pbuff = data;
  
  itemp = 0;
  while (1)
  {
   if(',' == *data){break;}
   itemp *= 10;
   itemp += (*data) - '0';
   if((0 == --length)||('\r' == *data)||('\n' == *data)){ return DataValid;}  
   data ++;
  }
  while (1)
  {
   if(IsDigit(*data)){break;}
   if(0 == --length){ return DataValid;}
   data ++;
  }
  itemp1 = 0;
  while (1)
  {
   if(('\r' ==*data)||('\n' == *data)){break;}
   itemp1 *= 10;
   itemp1 += (*data) - '0';
   if(0 == --length){ return DataValid;}  
   data ++;
  }
  MG2618Infr.ModInf.Rssi = itemp;
  MG2618Infr.ModInf.Ber = itemp1;
  
  if(P_NULL !=MG2618Infr.CallBack)
  {
    AckData.length = 0;
    while (--length)
    {
    if(('\r'== AckData.pbuff[AckData.length])||('\n' == AckData.pbuff[AckData.length+1]))
    {AckData.length ++;break;}  
    if(('O'== AckData.pbuff[AckData.length])||('K' == AckData.pbuff[AckData.length+1]))
    {AckData.length ++;break;} 
     AckData.length ++;
    }
    MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);
  }
  else
  {StopListen_MG2618( );}
  return NO_ERR;
 }
 else
 {return DataValid;}   
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //时钟管理
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CCLK_Command_EN)
SystemErrName CCLK_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //短信中心号码
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CSCA_Command_EN)
// 消息服务指令 
SystemErrName CSCA_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //短信确认
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CNMA_Command_EN)
SystemErrName CNMA_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置短消息模式
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMGF_Command_EN)
SystemErrName CMGF_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置短信指示格式
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CNMI_Command_EN)
SystemErrName CNMI_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //读短消息
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMGR_Command_EN)
SystemErrName CMGR_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //写短消息
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMGW_Command_EN)
SystemErrName CMGW_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //选择短信服务
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CSMS_Command_EN)
SystemErrName CSMS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //发送短信
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMGS_Command_EN)
SystemErrName CMGS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //首选短消息存储器
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPMS_Command_EN)
SystemErrName CPMS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //删除短信
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMGD_Command_EN)
SystemErrName CMGD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //短消息列表
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMGL_Command_EN)
SystemErrName CMGL_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //发送存贮的短信
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMSS_Command_EN)
SystemErrName CMSS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //选择电话本存储
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPBS_Command_EN)
//电话本指令 
SystemErrName CPBS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //读取电话本信息
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPBR_Command_EN)
SystemErrName CPBR_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //向电话本写信息
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPBW_Command_EN)
SystemErrName CPBW_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //查找电话本信息
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPBF_Command_EN)
SystemErrName CPBF_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取本机号码
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CNUM_Command_EN)
SystemErrName CNUM_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //流控制
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != IFC_Command_EN)
SystemErrName IFC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 DTR 模式
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != DTR_Command_EN)
SystemErrName DTR_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 DCD 模式
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != DCD_Command_EN)
SystemErrName DCD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设定模块波特率
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != IPR_Command_EN)
SystemErrName IPR_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //恢复出厂设置
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != AT_F_Command_EN)
SystemErrName AT_F_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //保存设置
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != AT_W_Command_EN)
SystemErrName AT_W_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 PDP 格式
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CGDCONT_Command_EN)
SystemErrName CGDCONT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //激活/解除 PDP 设置
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CGACT_Command_EN)
SystemErrName CGACT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 GPRS 业务
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CGATT_Command_EN)
SystemErrName CGATT_ACK_Func(unsigned char *data, unsigned short length)
{
  unsigned short  itemp; 
 itemp = CompString(data,CmdNameList_MG2618[CGATT],&data);
 if(0 != itemp)
 {
  length -= itemp;  
  return Default_ACK_Func(data, length);
 }
 else
 {return DataValid;}

}
#endif
/*********************************************************************************
 Function:      //
 Description:   //GPRS 设备等级
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CGCLASS_Command_EN)
SystemErrName CGCLASS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //查询模块状态信息
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZSTR_Command_EN)
//ZTE 特有指令
SystemErrName ZSTR_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取 ICCID
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGETICCID_Command_EN)
SystemErrName ZGETICCID_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置自动显示 CSQ 的条件
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZCSQ_Command_EN)
SystemErrName ZCSQ_ACK_Func(unsigned char *data, unsigned short length)//设置自动显示 CSQ 的条件
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 DTR 的检测模式
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZEDT_Command_EN)
SystemErrName ZEDT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //32KHz 深度睡眠模式
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZDSLEEP_Command_EN)
SystemErrName ZDSLEEP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //发送 USSD 数据
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CUSD_Command_EN)
SystemErrName CUSD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 RING 引脚的信号模式
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZRINGPINMODE_Command_EN)
SystemErrName ZRINGPINMODE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //定制版本切换
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZVERSWITCH_Command_EN)
SystemErrName ZVERSWITCH_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 APN，用户名，密码
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CSTT_Command_EN)
SystemErrName CSTT_ACK_Func(unsigned char *data, unsigned short length)
{
 DataBuffSpec_Struct AckData ;
 
  while(length)
 {
   if(IsLETTER(*(data))) {break;}
    length--;
   data++;
 }
 if(CompString(data, "OK", P_NULL)||(CompString(data, "ERROR", P_NULL)))
 {
   if(P_NULL !=MG2618Infr.CallBack)
   {
     AckData.length = length;
     AckData.pbuff = data;
     MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);
   } 
 }
 return NO_ERR;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //打开 GPRS 数据连接
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIICR_Command_EN)
SystemErrName CIICR_ACK_Func(unsigned char *data, unsigned short length)
{
  DataBuffSpec_Struct AckData ;
  while(length)
  {
    if(IsLETTER(*(data))) {break;}
    length--;
    data++;
  }
  if(CompString(data, "OK", P_NULL)||(CompString(data, "ERROR", P_NULL)))
  {
    if(P_NULL !=MG2618Infr.CallBack)
    {
      AckData.length = length;
      AckData.pbuff = data;
      MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);
    } 
  }
 return NO_ERR;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //关闭 GPRS 数据连接
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZPPPCLOSE_Command_EN)
SystemErrName ZPPPCLOSE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //查询当前模块 IP 地址值
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIFSR_Command_EN)
SystemErrName CIFSR_ACK_Func(unsigned char *data, unsigned short length)
{
 unsigned short  itemp; 
 unsigned char   IP[4] = {0,0,0,0};      //本地IP地址 
 DataBuffSpec_Struct  AckData; 
  while (1)
  {
   if(IsDigit(*data)){break;}
   if(0 == --length){return DataValid;}
   data ++;
  }
  AckData.pbuff = data;
  AckData.length = length;
  itemp = 0;
  while (1)
  {
   if(('\r'== *data)||('\n' == *data)){break;}  
   if(0 == --length){return DataValid;}
   AckData.length ++;
   if('.'== *data){itemp ++; itemp %= 4;}
   if(IsDigit(*data))
   {IP[itemp] *= 10; IP[itemp] += *data - '0';  }
   data ++;
  }
  
  MG2618Infr.ModInf.LOCAL_IP[0] = IP[0];
  MG2618Infr.ModInf.LOCAL_IP[1] = IP[1];
  MG2618Infr.ModInf.LOCAL_IP[2] = IP[2];
  MG2618Infr.ModInf.LOCAL_IP[3] = IP[3];
  if(P_NULL !=MG2618Infr.CallBack) 
  {MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);}
  return NO_ERR;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 DNS 服务器的 IP 地址
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZDNSSERV_Command_EN)
SystemErrName ZDNSSERV_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取对应 Internet 域名的 IP 地址
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZDNSGETIP_Command_EN)
SystemErrName ZDNSGETIP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //建立 TCP 服务器链接
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIPSTART_Command_EN)
SystemErrName CIPSTART_ACK_Func(unsigned char *data, unsigned short length)
{
  unsigned char ch;
  GprsInfr_Struct GprsInfr;
  DataBuffSpec_Struct AckData ;
  
  while(length)
  {
    if(IsLETTER(*(data))||IsDigit(*(data))) {break;}
    length--;
    data++;
  }
  if(IsLETTER(*(data)))
  {
    if(CompString(data, "OK", &data)||(CompString(data, "ERROR", &data)))
    {
      while(length)
      {
        if(IsDigit(*(data))||IsLETTER(*(data))) {break;}
        length--;
        data++;
      }
      if(length > 0)
      {
        if(P_NULL !=MG2618Infr.CallBack)
        {
          AckData.length = length;
          AckData.pbuff = data;
          MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);
        } 
      }
    }
    return NO_ERR;
  }
  else if(IsDigit(*(data)))
  {
    AckData.length = length;
    AckData.pbuff = data;
    
    ch = *(data++) - '0';
    length -- ;
    if(ch == MG2618Infr.Command.CmdPram.TcpUdp_Connect.CH)
    {
      while(length)
      {
        if(IsLETTER(*(data))) {break;}
        length--;
        data++;
      }
      if(CompString(data,"CONNECT OK",P_NULL))
      {
        GprsInfr.IP[0] = MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[0];
        GprsInfr.IP[1] = MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[1];
        GprsInfr.IP[2] = MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[2];
        GprsInfr.IP[3] = MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[3];
        GprsInfr.PORT = MG2618Infr.Command.CmdPram.TcpUdp_Connect.PORT;
        GprsConnect_OK(MG2618Infr.Command.CmdPram.TcpUdp_Connect.CH, &GprsInfr);
      }
    }
    if(AckData.length > 0)
    {
      if(P_NULL !=MG2618Infr.CallBack)
      {
        AckData.length = length;
        AckData.pbuff = data;
        MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);
      } 
    }
    return NO_ERR;
  }
  else
  {return NO_ERR;}
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //向目标地址发送 TCP 数据
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void CIPSEND_OK_Func(CommPortNumber_Type port)//, void *pbuff)
{
  StopListen_MG2618( );
  if(P_NULL !=MG2618Infr.Command.CmdPram.TcpUdp_Send.callBack)
  {MG2618Infr.Command.CmdPram.TcpUdp_Send.callBack(port,MG2618Infr.Command.CmdPram.TcpUdp_Send.buff);}
}
/*********************************************************************************
 Function:      //
 Description:   //向目标地址发送 TCP 数据
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIPSEND_Command_EN)
SystemErrName CIPSEND_ACK_Func(unsigned char *data, unsigned short length)
{
 CommPortNumber_Type  Port = GPRS_CH0;
 unsigned short itemp  = 0;
 
 switch (MG2618Infr.Command.Step)
 {
 case 0:
   {
     while(length--)
     {
       if('>'== *data)
       {
         if(P_NULL != MG2618Infr.TimeOut)
         {SofTimerRset(MG2618Infr.TimeOut);}  
         SendDataToMG2618(MG2618Infr.Command.CmdPram.TcpUdp_Send.buff,
                          MG2618Infr.Command.CmdPram.TcpUdp_Send.LENGTH,
                          P_NULL);
         MG2618Infr.Command.Step++;
         break; 
       }
       data ++;
     }
     break;     
   }
 case 1:
   {
     if(length == MG2618Infr.Command.CmdPram.TcpUdp_Send.LENGTH)
     {MG2618Infr.Command.Step++;}
     break; 
   }
 case 2:
   {
     while(length--)
     {
       if(IsDigit(*(data)))
       {break; }
       data ++;
     }
     itemp = *(data) - '0';
     while(length--)
     {
       if(IsLETTER(*(data)))
       {break; }
       data ++;
     }  
     if(CompString(data,"SEND OK",P_NULL))
     {
       if(itemp == MG2618Infr.Command.CmdPram.TcpUdp_Send.CH)
       {
         Port += itemp;
         CIPSEND_OK_Func(Port);
       }
     }
     
   }  
 }
 return NO_ERR;  
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //查询 GPRS 连接状态
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZPPPSTATUS_Command_EN)
SystemErrName ZPPPSTATUS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //关闭 TCP 链接
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIPCLOSE_Command_EN)
SystemErrName CIPCLOSE_ACK_Func(unsigned char *data, unsigned short length)
{
  DataBuffSpec_Struct  AckData;
  
  while(length)
  {
    if(IsLETTER(*data))
    {
      if(CompString(data,"CLOSE OK",P_NULL))
      {
        if(P_NULL !=MG2618Infr.CallBack)
        {
          AckData.length = length;
          AckData.pbuff = data; 
          MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);
        }
        GprsClose_UEV(MG2618Infr.Command.CmdPram.CIPCLOSE.CH);
        StopListen_MG2618( );
        return NO_ERR;
      }
      else
      {
        while(length) 
        { 
          if(!IsLETTER(*(data))){ break;}
          data ++;
         length --;
        }
        continue;
      }
    }
    length--;
    data ++;
  }
 return NO_ERR;
} 
#endif
/*********************************************************************************
 Function:      //
 Description:   //查询当前 TCP 连接状态 
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZIPSTATUS_Command_EN)
SystemErrName ZIPSTATUS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //建立 UDP 服务器链接
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIPSTARTU_Command_EN)
// UDP 连接指令 
SystemErrName CIPSTARTU_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //向 UDP 服务器发送数据
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIPSENDU_Command_EN)
SystemErrName CIPSENDU_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //查询 UDP 状态
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZIPSTATUSU_Command_EN)
SystemErrName ZIPSTATUSU_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //关闭 UDP 链接
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIPCLOSEU_Command_EN)
SystemErrName CIPCLOSEU_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //端口监听功能设置
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTCPLISTEN_Command_EN)
SystemErrName ZTCPLISTEN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //在被动打开的链路上发送数据
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTCPSENDP_Command_EN)
SystemErrName ZTCPSENDP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //关闭监听到的连接
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTCPCLOSEP_Command_EN)
SystemErrName ZTCPCLOSEP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //查询被动打开链路
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTCPSTATUSP_Command_EN)
SystemErrName ZTCPSTATUSP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置模块连接服务器和发送数据的超时时间
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZIPTIMEOUT_Command_EN)
SystemErrName ZIPTIMEOUT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //端口监听功能设置
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZUDPLISTEN_Command_EN)
SystemErrName ZUDPLISTEN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //在被动打开的链路上发送数据
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZUDPSENDP_Command_EN)
SystemErrName ZUDPSENDP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //登录 FTP 服务器
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPLOGIN_Command_EN)
// FTP 指令
SystemErrName ZFTPLOGIN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 FTP 文件类型
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPTYPE_Command_EN)
SystemErrName ZFTPTYPE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //上传文件
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPUPLOAD_Command_EN)
SystemErrName ZFTPUPLOAD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取文件大小
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPSIZE_Command_EN)
SystemErrName ZFTPSIZE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //下载文件
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPDNLOAD_Command_EN)
SystemErrName ZFTPDNLOAD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //下载 FTP 文件
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPDNLOADEX_Command_EN)
SystemErrName ZFTPDNLOADEX_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //删除文件
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPDEL_Command_EN)
SystemErrName ZFTPDEL_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //退出 FTP
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPQUIT_Command_EN)
SystemErrName ZFTPQUIT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //透明传输
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTRANSFER_Command_EN)
// 透明传输指令 
SystemErrName ZTRANSFER_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //摘机音设置
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZCALLTONE_Command_EN)
//音频相关指令 
SystemErrName ZCALLTONE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //ZDTMF 拨号音设置
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZDTMFTONE_Command_EN)
SystemErrName ZDTMFTONE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //打开 DTMF 检测功能
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZKTDSWITCH_Command_EN)
SystemErrName ZKTDSWITCH_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //主动上报检测的 DTMF 按键
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZKTDIND_Command_EN)
SystemErrName ZKTDIND_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //语音通道切换指令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != SPEAKER_Command_EN)
SystemErrName SPEAKER_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 MIC 音频参数
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZMICGB_Command_EN)
SystemErrName ZMICGB_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取小区信息
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CCED_Command_EN)
SystemErrName CCED_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //锁定 BCCH 信道（暂不支持）
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZBCCH_Command_EN)
SystemErrName ZBCCH_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //锁定 GSM 频段
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZBAND_Command_EN)
SystemErrName ZBAND_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //锁定网络运营商
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZOPT_Command_EN)
SystemErrName ZOPT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //读取载波列表
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZCALIST_Command_EN)
SystemErrName ZCALIST_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //TTS(Text to Speech)语音播报业务
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTTS_Command_EN)
SystemErrName ZTTS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 TTS(Text to Speech)语音播报参数(暂不支持切换发音人)
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTTSP_Command_EN)
SystemErrName ZTTSP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //录音功能
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZAUDREC_Command_EN)
SystemErrName ZAUDREC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //读取文件
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFILEREAD_Command_EN)
SystemErrName ZFILEREAD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //开关 GPS 指令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSOPEN_Command_EN)
// GPS 相关指令 
SystemErrName ZGPSOPEN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //GPS 休眠指令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSSLEEP_Command_EN)
SystemErrName ZGPSSLEEP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //NMEA 上报控制指令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSNMEARPT_Command_EN)
SystemErrName ZGPSNMEARPT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;

}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取 GPS 定位经纬度指令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSLOC_Command_EN)
SystemErrName ZGPSLOC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取 GPS 时间指令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSUTC_Command_EN)
SystemErrName ZGPSUTC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err; 
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取角度速度指令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSHS_Command_EN)
SystemErrName ZGPSHS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err; 
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //GPS 定位状态上报
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSIND_Command_EN)
SystemErrName ZGPSIND_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != SyncRate_Command_EN)
SystemErrName SyncRate_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR; 
 DataBuffSpec_Struct  AckData;
 
 AckData.length = CompString(data, "AT", &(AckData.pbuff));
 if(0 != AckData.length)
 {
   if(P_NULL !=MG2618Infr.CallBack)
   {MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);} 
 }
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIPMUX_Command_EN)
SystemErrName CIPMUX_ACK_Func(unsigned char *data, unsigned short length)
{
 DataBuffSpec_Struct  AckData;
 while(length)
 {
   if(IsLETTER(*(data))) {break;}
    length--;
   data++;
 }
 if(CompString(data, "OK", P_NULL)||(CompString(data, "ERROR", P_NULL)))
 {
   if(P_NULL !=MG2618Infr.CallBack)
   {
     AckData.length = length;
     AckData.pbuff = data;
     MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);
   } 
 }
 return NO_ERR;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName Default_ACK_Func(unsigned char *data, unsigned short length)
{
 DataBuffSpec_Struct  AckData;
 if(P_NULL !=MG2618Infr.CallBack)
 {
  while (1)
  {
   if(IsDigit(*data) || IsLETTER(*data) || Isletter(*data)){break;}
   if(0 == --length){return DataValid;}
   data ++;
  }
  AckData.pbuff = data;
  AckData.length = length;
  {MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);}
 }
 return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static unsigned char GetData_TCP_UDP(DataBuffSpec_Struct *Data, DataBuffSpec_Struct* Rec)
{
  unsigned char  CH = 255;
  
  while (Data->length)
  {
   if(IsDigit(*(Data->pbuff))){break;}
   Data->pbuff ++;
   Data->length --;
  }
  if(IsDigit(*(Data->pbuff)))
  {
    CH = *(Data->pbuff) - '0';
    if(CH > 6)
    {
      return 255;
    }
    Data->pbuff ++;
    Data->length --;
  }
  
  while (Data->length)
  {
   if(IsDigit(*(Data->pbuff))){break;}
   Data->pbuff ++;
   Data->length --;
  }
  
  Rec->length = 0; 
  while ((Data->length > 0)&&(IsDigit(*(Data->pbuff))))
  {
    Rec->length *= 10; 
    Rec->length += *(Data->pbuff) - '0';
    Data->pbuff ++;
    Data->length --;
  }
  while (Data->length)
  {
    if(('\r' == Data->pbuff[0])&&('\n' == Data->pbuff[1]))
    {
      Rec->pbuff = &Data->pbuff[2];
      Data->length -=2;
      if(Data->length < Rec->length)
      {
        Data->length = 0;
        CH = 255;
      }
      else
      {
       Data->length -= Rec->length;
       Data->pbuff = Rec->pbuff + Rec->length;
      }
      break;
    }
    Data->pbuff ++;
    Data->length --;
  }
  return CH;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
/*static void TcpClose_ACK(DataBuffSpec_Struct *Data)
{
  unsigned char  CH;
  while (1)
  {
   if(IsDigit(*(Data->pbuff))){break;}
   if(0 == --(Data->length)){return ;}
   Data->pbuff ++;
  }
  while (1)
  {
   if(!IsDigit(*(Data->pbuff))){break;}  
   if(0 == --(Data->length)){ return;}
   if(IsDigit(*(Data->pbuff)))
   {CH = *(Data->pbuff) - '0'; break; }
   Data->pbuff ++;
  }
 GprsClose_UEV(CH);
}*/
/***************************************END*****************************************/
