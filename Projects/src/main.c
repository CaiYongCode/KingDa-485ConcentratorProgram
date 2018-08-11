/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\..\BSP\inc\bsp.h"
#include "..\..\SPCP\JiWanXun\JWX_SPCP.h"
#include "..\..\LIB\FAT\ff.h"
#include "..\..\APP\inc\MCPX.h"
#include "..\..\APP\inc\GPRS.h"
#include "..\..\SPCP\SPCP_Config.h"
#include "..\..\APP\inc\ReadEquipData.h"
#include "..\..\APP\inc\Appcon_API.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
const unsigned char eeprom_flg[]= "KD_V2.2.6";
/*********************************************************************************
公共变量定义区
*********************************************************************************/
 SofTimer_Struct*  RTC_Timer;   //RTC 守护时钟 防止RTC停转
/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
私有变量定义区
*********************************************************************************/

/*********************************************************************************
测试变量定义区
*********************************************************************************/
 unsigned char iddfffff[100] ={1,2,3,4,5,6,7,8};
/*********************************************************************************
内部函数定义区
*********************************************************************************/
 unsigned char Init_CH376Host( void );
/*********************************************************************************
功能代码定义区
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   // 
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName Eeprom_DataInit(void)
{
  SystemErrName err = NO_ERR;
  unsigned char* p;
  unsigned itemp = 0;

  p = (unsigned char*)&Concentrator;
  while (itemp <sizeof(ConcentratorParameterStruct))
  {p[itemp++] = 0;}
  
  Concentrator.cycle.day = 1; 
  Concentrator.cycle.hour = 0;
  Concentrator.cycle.minute = 0;
  Concentrator.server1.ip[0] =112;
  Concentrator.server1.ip[1] =25;
  Concentrator.server1.ip[2] =204;
  Concentrator.server1.ip[3] =3;
  Concentrator.server1.port = 50000;
 
  err = EEPROM_I2C_MultipleWrite(&Concentrator,
                                 ConcentratorParameter_AddrEeprom, 
                                 sizeof(ConcentratorParameterStruct));
  if(NO_ERR != err){return err;}  
  
  err = EEPROM_I2C_MultipleWrite((unsigned char*)eeprom_flg,0, 9);
  if(NO_ERR != err){return err;} 
  
  return err;
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
SystemErrName Eeprom_DataLoad(void)
{
  SystemErrName err = NO_ERR;
  
  err =  EEPROM_I2C_MultipleRead(&Concentrator, 
                                 ConcentratorParameter_AddrEeprom, 
                                 sizeof(ConcentratorParameterStruct));
  if(0 == Concentrator.cycle.day)
  {Concentrator.cycle.day = 1;}
  if(23 < Concentrator.cycle.hour)
  {Concentrator.cycle.hour = 0;}
  if(59 < Concentrator.cycle.minute)
  {Concentrator.cycle.minute = 0;}
  if(NO_ERR != err)
  {return err;}
  return err;
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
SystemErrName SystermData_Load(void)
{
  SystemErrName err;
  unsigned char itemp =0;
  unsigned char cnt =0;
  unsigned char eeprom[10]= {0,0,0,0,0,0,0,0,0,0}; 
  Launch_24CXX ( EEPROM_ADDRESS);
  while (1)
  {
    err = EEPROM_I2C_BufferRead(eeprom, 0, 9);//将EEPROM的数据读入缓冲器
    if(NO_ERR != err)
    { 
      EepromPort_rset(EEPROM_ADDRESS) ;
      if(++cnt > 3){return err;}
    }
    else
    {cnt = 0; break; }
  }
  while (itemp < 9)
  {
   if(eeprom[itemp] != eeprom_flg[itemp])
   {break;}
    itemp ++;
  }
  if(itemp < 9)
  {err =  Eeprom_DataInit( );}
  else
  {err =  Eeprom_DataLoad( );} 
  

  Concentrator.SR1.Bit.App_RdVal_end_01 = 1;
  Concentrator.SR1.Bit.App_RdVal_end_02 = 1;
  
  return err;
}

/*********************************************************************************
 Function:      //INIT_PCB
 Description:   //PCB 板初始配置 
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/ 
void RTC_reset(void *p_tmr, void *p_arg)
{
  I2C1_Rset( );
  DelymS (100);
  F8563_woring_mode(DEFINE_TIME);
  F8563_back_init(64,PCF8563_TD_1_64S);  ///  
}
/*********************************************************************************
 Function:      //INIT_PCB
 Description:   //PCB 板初始配置 
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/ 
void INIT_PCB (void)
{
  
 F8563_woring_mode(DEFINE_TIME);
 F8563_back_init(64,PCF8563_TD_1_64S);  ///  
 //MountMemory( ); 
 
 RTC_Timer = SofTimerCreate( 20,        //周期
                              TimerTB_1S,//时基
                              SofTimer_PERIODIC,  //模式
                              RTC_reset,  //回调
                              P_NULL);//回调参数
  SofTimerStart(RTC_Timer);
}
/*********************************************************************************
 Function:      //
 Description:   //上电延时执行程序 （只执行一次） 
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/ 
void SystermInitDelayed(void *p_tmr, void *p_arg)
{
  TimerData_Struct timer;
  unsigned char  Edata = 0; 
  get_SystemTimer (&timer);
  if(Concentrator.cycle.day < 1)
  {Concentrator.cycle.day = 1;}
  if(Concentrator.cycle.hour > 23)
  {Concentrator.cycle.hour = 0;}
  if(Concentrator.cycle.minute > 59)
  {Concentrator.cycle.minute = 0;}
  ConcentratorUpdataTimer.day = timer.Day + Concentrator.cycle.day;
  ConcentratorUpdataTimer.hour = Concentrator.cycle.hour;
  ConcentratorUpdataTimer.minute = Concentrator.cycle.minute;
  switch (timer.Month)
  {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
      {
        if(ConcentratorUpdataTimer.day > 31)
        {ConcentratorUpdataTimer.day %= 31;}
        break;
      }
    case 4:
    case 6:
    case 9:
    case 11:
      { 
        if(ConcentratorUpdataTimer.day > 30) 
          {ConcentratorUpdataTimer.day %= 30;}
        break;
      }
    case 2:
      {
        if(((0 == timer.Year % 4)
            &&(0 != timer.Year % 100))
           ||((0 == timer.Year % 100)
              &&(0 == timer.Year % 400)))
        {
          if(ConcentratorUpdataTimer.day > 29)
          {ConcentratorUpdataTimer.day %= 29;}
        }
        else
        {
          if(ConcentratorUpdataTimer.day > 28)
          {ConcentratorUpdataTimer.day %= 28;}
        } 
        break;
      }
    default:
      {
        if(ConcentratorUpdataTimer.day > 28)
        {ConcentratorUpdataTimer.day %= 28;}
        break;
      }
  }
  EEPROM_I2C_MultipleRead(&Edata, 511, 1);
  if(Edata != 1)
  {
    Concentrator.CSR.Bit.Updata_flg = 1;
    LaunchReadAllValue_KindgDa( );    //全网抄表并上传
  }
  else
  {
    Edata = 0;
    EEPROM_I2C_MultipleWrite(&Edata, 511, 1);
  }
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
void Systerm_INIT(void)
{
  TimerData_Struct Timer;
  SofTimer_Struct* timer;
  SofTimer_DeInit( );
  SystermData_Load( );
  
  ServerState[0].SR.Byte = 0;
  ServerState[0].ServerConnect_CNT = 0;
  ServerState[0].Port = Null_Port;
  
  ServerState[1].SR.Byte = 0;
  ServerState[1].ServerConnect_CNT = 0;
  ServerState[1].Port = Null_Port;

 
  KingDaV1Protocol_DeInit( );
  KingDaServer_DeInit( );
  //CJT188_SPCP_init( );
  APPCON_SPCP_init( );
  //JingDaSPCP_init( );
  MCPX_DeInit( );
  
  get_SystemTimer (&Timer);
  ConcentratorUpdataTimer.day = Timer.Day;
 
  timer = SofTimerCreate( 10,        //周期
                          TimerTB_1M,//时基
                          SofTimer_ONE_SHOT,  //模式
                          SystermInitDelayed,  //回调
                          P_NULL);//回调参数
  SofTimerStart(timer);
}
/********************************************************************************
 Function:      //
 Description:   // 
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void Reboot_launch(void *p_tmr, void *p_arg)
{
  SofTimer_Struct* timer;
  unsigned char  Edata = 1; 
  if(Concentrator.CSR.Bit.ReadAll != 0x01 ) //判断条件 不为抄表状态
  {
    EEPROM_I2C_MultipleWrite(&Edata, 511, 1);
    NVIC_SystemReset();
  }
  else
  {
    timer = SofTimerCreate( 2,        //周期
                            TimerTB_1H,//时基
                            SofTimer_ONE_SHOT,  //模式
                            Reboot_launch,  //回调
                            P_NULL);//回调参数
    SofTimerStart(timer);
  }
}
/*********************************************************************************
 Function:      //
 Description:   //  重启单片机 初始化
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void Reboot_init(void)
{
  SofTimer_Struct* timer;
  timer = SofTimerCreate( 24,        //周期
                          TimerTB_1H,//时基
                          SofTimer_ONE_SHOT,  //模式
                          Reboot_launch,  //回调
                          P_NULL);//回调参数
  SofTimerStart(timer);
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
int main(void)
{
 unsigned char ch =0;
 
 DelymS (2000); 
 MCU_init ( );
 SysTick_Config(72000);
 Systerm_INIT( );
 INIT_PCB ( );
 Launch_Rs232(2400, Parity_No, StopBits_2,ByIT);  
 Reboot_MG2618( P_NULL );
 Launch_APC950( P_NULL );
 //IWDG -> KR = 0x5555;   //表示允许访问IWDG_PR和IWDG_RLR寄存器
 //IWDG -> PR = 0x00004;  //预分频因子=64       
 //IWDG -> RLR = 0x753 ;  //1900
 //IWDG -> KR  = 0xCCCC;  //启动看门狗工作
 Reboot_init(); //定时重启标识
 while (1)
 {
  MXPC_Thread ( );
  SofTimer_Process( );
  AnalogComput ( );
  GprsGuarde_Process ( );
  IWDG_ReloadCounter();
  if(0 != Concentrator.SR1.Bit.NewData_flg )
  {
    if((0 != ServerState[0].SR.Bit.CN)&&(0 != ServerState[0].SR.Bit.ACK))
   {
     LaunchSendMeasureValue_ToKingDaServer1( );
     Concentrator.SR1.Bit.NewData_flg = 0;
   }
  }
  //多功能按键 处理程序
  if(UserK_UEV)    
  {
   Clr_UserK_UEV();
   LaunchReadAllValue_KindgDa( );    //全网抄表并上传
  }
  if(ch)
  {
   freeChannel_MG2618(ch);
  }
 }  
}
#ifdef  USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/**************************************END****************************************/



