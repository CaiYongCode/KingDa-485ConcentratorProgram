/*********************************************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************************************/
#ifndef  __WSN_TIMER_H
#define  __WSN_TIMER_H
/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include  ".\SystemDefine.h"
/*********************************************************************************************************
�궨����
*********************************************************************************************************/

#define SofTimer_Number                64

#define  SofTimer_IDLE                  0    //��ʱ������
#define  SofTimer_USE                   1    //��ʱ��ռ��

#define  SofTimer_STOP                  0    //��ʱ��ֹͣ
#define  SofTimer_RUN                   1    //��ʱ��������

#define  SofTimer_ONE_SHOT              0    /* Timer will not auto restart when it expires        */
#define  SofTimer_PERIODIC              1    /* Timer will     auto restart when it expires        */
/*********************************************************************************************************
�������Ͷ���
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
    unsigned char EN    :1;   //���ñ�־ 
    unsigned char Opt   :1;   //����ģʽ  0�����δ���   1�����ڴ���
    unsigned char State :1;   //����״̬  0��ֹͣ       1������
    unsigned char TB    :3;   //����
    unsigned char NC    :2;   //����
  }bit;
 }SofTimer_CCR1_struct;
 
 
struct sof_timer{
 SofTimer_CCR1_struct     CR;
 unsigned short           CNTR;   //��ʱ��
 unsigned short           ARR;   //�Զ���װֵ
 void                     *CallbackPtrArg;
 SofTimer_CALLBACK        CallbackPtr;// Fun;
};
/*********************************************************************************************************
�ⲿ����������
*********************************************************************************************************/

/*********************************************************************************************************
����������
*********************************************************************************************************/
void  SofTimer_DeInit(void);
SofTimer_Struct* getSofTimer(void);
SofTimer_Struct* SofTimerCreate (unsigned short Period,        //����
                                 SofTimer_TB tb,                 //ʱ��
                                 unsigned char opt,            //ģʽ
                                 SofTimer_CALLBACK p_callback,   //�ص�
                                  void *p_callback_arg);         //�ص�����
SystemErrName SofTimer_ChangePeriod (SofTimer_Struct* p_tmr,     //���ݵĶ�ʱ��
                                     unsigned short NewPeriod,  //�µ�����
                                     unsigned char opt);        //�Ƿ�λ��ʱ�� 1:���¼�ʱ  0:������ʱ
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










