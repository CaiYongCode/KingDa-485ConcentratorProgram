/*********************************************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************************************/
#ifndef  __WSN_TIMER_H
#define  __WSN_TIMER_H
/*********************************************************************************************************
文件包含区
*********************************************************************************************************/
#include  ".\SystemDefine.h"
/*********************************************************************************************************
宏定义区
*********************************************************************************************************/

#define SofTimer_Number                64

#define  SofTimer_IDLE                  0    //定时器空闲
#define  SofTimer_USE                   1    //定时器占用

#define  SofTimer_STOP                  0    //定时器停止
#define  SofTimer_RUN                   1    //定时器运行中

#define  SofTimer_ONE_SHOT              0    /* Timer will not auto restart when it expires        */
#define  SofTimer_PERIODIC              1    /* Timer will     auto restart when it expires        */
/*********************************************************************************************************
数据类型定义
*********************************************************************************************************/
typedef void (*SofTimer_CALLBACK)(void *p_tmr, void *p_arg);

typedef struct sof_timer   SofTimer_Struct;

typedef enum {
 TimerTB_10MS    = 1,
 TimerTB_100MS   = 2,
 TimerTB_1S      = 3, 
 TimerTB_1M      = 4,
 TimerTB_1H      = 5,
}SofTimer_TB;

typedef union 
 {
  unsigned char byte;
  struct{
    unsigned char EN    :1;   //启用标志 
    unsigned char Opt   :1;   //工作模式  0：单次触发   1：周期触发
    unsigned char State :1;   //允许状态  0：停止       1：运行
    unsigned char TB    :3;   //保留
    unsigned char NC    :2;   //保留
  }bit;
 }SofTimer_CCR1_struct;
 
 
struct sof_timer{
 SofTimer_CCR1_struct     CR;
 unsigned short           CNTR;   //计时器
 unsigned short           ARR;   //自动重装值
 void                     *CallbackPtrArg;
 SofTimer_CALLBACK        CallbackPtr;// Fun;
};
/*********************************************************************************************************
外部变量声明区
*********************************************************************************************************/

/*********************************************************************************************************
函数声明区
*********************************************************************************************************/
void  SofTimer_DeInit(void);
SofTimer_Struct* getSofTimer(void);
SofTimer_Struct* SofTimerCreate (unsigned short Period,        //周期
                                 SofTimer_TB tb,                 //时基
                                 unsigned char opt,            //模式
                                 SofTimer_CALLBACK p_callback,   //回调
                                  void *p_callback_arg);         //回调参数
SystemErrName SofTimer_ChangePeriod (SofTimer_Struct* p_tmr,     //操纵的定时器
                                     unsigned short NewPeriod,  //新的周期
                                     unsigned char opt);        //是否复位定时器 1:重新计时  0:继续计时
SystemErrName SofTimer_Compelling(SofTimer_Struct* p_tmr);
SystemErrName SofTimerStart(SofTimer_Struct* p_tmr);
SystemErrName SofTimerStop(SofTimer_Struct* p_tmr) ;
SystemErrName SofTimerDel(SofTimer_Struct** p_tmr) ;
SystemErrName SofTimerRset(SofTimer_Struct*  p_tmr);
unsigned long  SofTimerRemainGet(SofTimer_Struct* p_tmr, SystemErrName *err);
unsigned char get_SofTimerState(SofTimer_Struct* p_tmr);
void  SofTimer_Process(void);
/********************************************************************************************************/
#endif










