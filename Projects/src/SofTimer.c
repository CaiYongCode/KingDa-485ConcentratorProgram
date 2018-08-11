/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\inc\SofTimer.h"
#include "..\inc\SystemDefine.h"
#include "..\..\LIB\CMSIS\Device\STM32F10x\Include\stm32f10x.h"
#include "..\..\APP\inc\MCPX.h"
#include "..\..\APP\inc\GPRS.h"
#include "..\..\APP\inc\ReadEquipData.h"
#include "..\..\APP\inc\Appcon_API.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
/*********************************************************************************
公共变量定义区
*********************************************************************************/
/*********************************************************************************
外部变量声明区
*********************************************************************************/
unsigned char _1mS_cnt =0;

extern SofTimer_Struct*  RTC_Timer;   //RTC 守护时钟 防止RTC停转
/*********************************************************************************
私有变量定义区
*********************************************************************************/ 
static  SofTimer_Struct tmr[ SofTimer_Number ];
/*********************************************************************************
测试变量定义区
*********************************************************************************/
void WriteCH376Cmd( unsigned char mCmd ) ;
/*********************************************************************************
内部函数定义区
*********************************************************************************/
/*********************************************************************************
功能代码定义区
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void  SofTimer_DeInit(void)
{
  unsigned char i;
  i = 0;
  while (i < SofTimer_Number)
  {
    tmr[i].CR.bit.EN = SofTimer_IDLE;
    i ++;
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
SofTimer_Struct* getSofTimer(void)
{
 unsigned char itemp = 0;
 while (itemp < SofTimer_Number)
 {
   if(SofTimer_IDLE == tmr[itemp].CR.bit.EN)
   {
     tmr[itemp].CR.bit.EN = SofTimer_USE;
     return &(tmr[itemp]);
   }
   itemp ++;
 }
 return P_NULL;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
static SystemErrName SofTimerVerify (SofTimer_Struct *p_tmr) 
{
 unsigned char itemp = 0;
 while (itemp < SofTimer_Number)
 {
   if(p_tmr == &(tmr[itemp]))
   {
     return NO_ERR;
   }
   itemp ++;
 }
 return IllegalParam;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
SofTimer_Struct* SofTimerCreate (unsigned short Period,     //周期
                                 SofTimer_TB tb,              //时基
                                 unsigned char opt,          //模式
                                 SofTimer_CALLBACK p_callback, //回调
                                 void *p_callback_arg)        //回调参数
{
  SofTimer_Struct*  p_tmr;
  
  p_tmr = getSofTimer( );
  if(P_NULL != p_tmr )
  {
   p_tmr->CR.byte = 0x00; 
   p_tmr->CR.bit.EN = SofTimer_USE;
   p_tmr->CR.bit.Opt = opt;
   p_tmr->CR.bit.TB = tb;
   p_tmr->ARR = Period;
   p_tmr->CNTR = p_tmr->ARR;
   p_tmr->CallbackPtr = p_callback;
   p_tmr->CallbackPtrArg = p_callback_arg; 
  }
  else
  {while(1);}//IWDG -> KR = 0xBBBB;}  //复位
 return p_tmr;
}
/*********************************************************************************
 Function:      //
 Description:   //改变定时周期
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
SystemErrName SofTimer_ChangePeriod (SofTimer_Struct* p_tmr,       //操纵的定时器
                                     unsigned short NewPeriod,   //新的周期
                                     unsigned char opt)          //是否复位定时器 1:重新计时  0:继续计时
{
  SystemErrName err;
  err = SofTimerVerify (p_tmr);
  if(NO_ERR == err)
  {
    p_tmr->ARR = NewPeriod;
    if(opt)
   {p_tmr->CNTR = p_tmr->ARR;}
  }
  return err;  
}
/*********************************************************************************
 Function:      //
 Description:   //强制定时立即发生一次定时溢出事件
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
SystemErrName SofTimer_Compelling(SofTimer_Struct* p_tmr)
{
  SystemErrName err;
  err = SofTimerVerify (p_tmr);
  if(NO_ERR == err)
  {
    if((SofTimer_USE == p_tmr->CR.bit.EN ) && (SofTimer_RUN == p_tmr->CR.bit.State))
    {
      if(P_NULL != p_tmr->CallbackPtr)
      { p_tmr->CallbackPtr(p_tmr,p_tmr->CallbackPtrArg);}
      if(SofTimer_ONE_SHOT == p_tmr->CR.bit.Opt) //定时器为单次触发式工作
      {p_tmr->CR.bit.EN = SofTimer_IDLE;}
      else
      {p_tmr->CNTR = p_tmr->ARR;}
    } 
  }
  return err;
}
/*********************************************************************************
 Function:      //
 Description:   //启动定时器
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
SystemErrName SofTimerStart(SofTimer_Struct* p_tmr) 
{
  SystemErrName err;
  err = SofTimerVerify (p_tmr);
  if(NO_ERR == err)
  {p_tmr->CR.bit.State = SofTimer_RUN;}
  return err;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
SystemErrName SofTimerStop(SofTimer_Struct* p_tmr) 
{
  SystemErrName err;
  err = SofTimerVerify (p_tmr);
  if(NO_ERR == err)
  {p_tmr->CR.bit.State = SofTimer_STOP;}
  return err;
}
/*********************************************************************************
 Function:      //
 Description:   //删除定时器
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
SystemErrName SofTimerDel(SofTimer_Struct** p_tmr) 
{
  SystemErrName err;
  err = SofTimerVerify (*p_tmr);
  if(NO_ERR == err)
  {(*p_tmr)->CR.bit.EN = SofTimer_IDLE;}
  *p_tmr = P_NULL;
  return err;
}
/*********************************************************************************
 Function:      //
 Description:   //定时器计时复位
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
SystemErrName SofTimerRset(SofTimer_Struct*  p_tmr)          
{
  SystemErrName err;
  err = SofTimerVerify (p_tmr);
  if(NO_ERR == err)
  {p_tmr->CNTR = p_tmr->ARR;  }
  return err;
}
/*********************************************************************************
 Function:      //
 Description:   //获取定时剩余时间 (单位毫秒)
 Input:         //
                //
 Output:        //
 Return:	//   
 Others:        //
*********************************************************************************/
unsigned long  SofTimerRemainGet(SofTimer_Struct* p_tmr, SystemErrName *err)
{
  unsigned long timer = 0;
  unsigned long tb =0;
  *err = SofTimerVerify (p_tmr);
  if(NO_ERR == err)
  {
   switch (p_tmr->CR.bit.TB) 
   {
    case TimerTB_10MS:
     {tb = 10;break;}
    case TimerTB_100MS:
     {tb = 100;break;}
    case TimerTB_1S:
     {tb = 1000;break;}
    case TimerTB_1M:
     {tb = 60000;break;}
    case TimerTB_1H:
     {tb = 3600000;break;} 
    default :
     {break;}
   }
   timer = p_tmr->CNTR * tb; 
  }
  return timer;
}
/*********************************************************************************
 Function:      //
 Description:   //获取定时器状态
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
unsigned char get_SofTimerState(SofTimer_Struct* p_tmr)
{
  SystemErrName err;
  err = SofTimerVerify (p_tmr);
  if(NO_ERR == err)
  {
    if(p_tmr->CR.bit.State)
    { return 1; }
    else
    {return 0;}
  }
  return 0;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void  SofTimer_Process(void)
{
 static unsigned char _10mS_cnt    = 0;
 static unsigned char _100mS_cnt   = 0;
 static unsigned char _1S_cnt      = 0; 
 static unsigned char _1M_cnt      = 0;
 static unsigned char _1H_1H       = 0;
 
 static unsigned char  Second = 0; // SystemTimer.Second
 static unsigned char  Delayed = 3; // SystemTimer.Second
 
 
 SofTimer_CCR1_struct flg;
 unsigned char itemp;
 TimerData_Struct timer;
 
 flg.byte = 0;
 
 if(_1mS_cnt >= 10)
 {
   _1mS_cnt -= 10;
   _10mS_cnt ++;
   flg.bit.TB = TimerTB_10MS;
   ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //使能或者失能指定的 ADC 的软件转换启动功能
   Port_sacn( );   
 }
 else if(_10mS_cnt >= 10)
 {
   _10mS_cnt -= 10;
   _100mS_cnt ++;
   flg.bit.TB = TimerTB_100MS;
   GPIOB->ODR  |= (0x00000001 << 1);
 }
 else if(_100mS_cnt >= 10)
 {
   _100mS_cnt -=10;
   _1S_cnt ++;
   flg.bit.TB = TimerTB_1S;
   if(Concentrator_DebugTimer > 0)
   {Concentrator_DebugTimer --;}
   else
   {Concentrator.CSR.Bit.Debug = 0;}
   if(RS485_IdleTime < 100)
   {
    if(60 == RS485_IdleTime++)
    {
     Concentrator.CSR.Bit.ReadAll = 0x00; 
     PowerOut_Cmd (Rs485_ALL, DISABLE);  
     PowerConfig_Rs485 (Null_Port);
     
    }  
   }
  GprsStop_time ++;
  LinkErr_time ++;
  LinkLed_time ++;
  IWDG -> KR	 = 0xAAAA;//复位看门狗
 }
 else  if(_1S_cnt >= 60)
 {
   _1S_cnt -= 60;
   _1M_cnt ++;
   if(Delayed > 0)
   {Delayed --;}
   flg.bit.TB = TimerTB_1M;   
 }
 else if(_1M_cnt >= 60)
 {
   _1M_cnt -= 60;
   _1H_1H ++;
   flg.bit.TB = TimerTB_1H;
 }
 else 
 {return;}
 
 itemp = 0;
 while (itemp < SofTimer_Number)
 {
  if(SofTimer_USE == (tmr[itemp].CR.bit.EN )
       &&(SofTimer_RUN == tmr[itemp].CR.bit.State) 
       && (flg.bit.TB == tmr[itemp].CR.bit.TB))
  {
   if(tmr[itemp].CNTR <=1)
   { //定时时间到达；
    if(P_NULL != tmr[itemp].CallbackPtr)
    { tmr[itemp].CallbackPtr(&tmr[itemp],tmr[itemp].CallbackPtrArg);}
    if(SofTimer_ONE_SHOT == tmr[itemp].CR.bit.Opt) //定时器为单次触发式工作
    {tmr[itemp].CR.bit.EN = SofTimer_IDLE;}
    else
    {tmr[itemp].CNTR = tmr[itemp].ARR;}
   }
   else
   { tmr[itemp].CNTR -- ; }
  }
  itemp ++;
 }
 
 if(TimerTB_1S == flg.bit.TB )
 {
   get_SystemTimer (&timer);
   if(Second != timer.Second)
   {
     Second = timer.Second;
     GPIOB->ODR  &= ~(0x00000001 << 1);
     SofTimerRset(RTC_Timer);  
   }
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
       if((ConcentratorUpdataTimer.day < 1 )
          ||(ConcentratorUpdataTimer.day > 31 ))
       {ConcentratorUpdataTimer.day = timer.Day;}
       break;
     }
   case 4:
   case 6:
   case 9:
   case 11:
     {
       if((ConcentratorUpdataTimer.day < 1 )
          ||(ConcentratorUpdataTimer.day > 30 ))
       {ConcentratorUpdataTimer.day = timer.Day;}
       break;
     }
   case 2:
     {
       if(((0 == timer.Year % 4)
           &&(0 != timer.Year % 100))
          ||((0 == timer.Year % 100)
             &&(0 == timer.Year % 400)))
       {
         if((ConcentratorUpdataTimer.day < 1 )
            ||(ConcentratorUpdataTimer.day > 29 ))
         {ConcentratorUpdataTimer.day = timer.Day;}
       }
       else
       {
         if((ConcentratorUpdataTimer.day < 1 )
            ||(ConcentratorUpdataTimer.day > 28 ))
         {ConcentratorUpdataTimer.day = timer.Day;}
        
       }
       break;
     }
   default:
     {
       if((ConcentratorUpdataTimer.day < 1 )
          ||(ConcentratorUpdataTimer.day > 28 ))
       {ConcentratorUpdataTimer.day = timer.Day;}
       break;
     }
   }
   
   if((ConcentratorUpdataTimer.day == timer.Day)&&(0 == Delayed))
   {
     if((timer.Hour == Concentrator.cycle.hour)
        &&(timer.Minute == Concentrator.cycle.minute))
     {
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
       Concentrator.CSR.Bit.Updata_flg = 1;
       LaunchReadAllValue_KindgDa( );
    } 
   }

 }
}
/**********************************END*****************************************/