/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
#ifndef  DRIVER_PCB_H
#define  DRIVER_PCB_H 
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "..\..\LIB\FLT\filter.h"
#include "..\..\LIB\CMSIS\Device\STM32F10x\Include\stm32f10x.h"
#include "..\..\LIB\CMSIS\Device\STM32F10x\Include\system_stm32f10x.h"
#include "..\..\Projects\inc\Common_API.h"
#include "..\..\Projects\inc\SystemDefine.h"
/*********************************************************************************
�궨����
*********************************************************************************/
#define key_cnt          10

#define Electricity_OutLimit         30      //����������� ����λ��100mA��



#define	 SetState(State)      State = 0x01
#define	 RsetState(State)     State = 0x00


#define UserK_State         Prot_state.Bit.no1
#define UserK_UEV           Prot_DW.Bit.no1
#define Clr_UserK_UEV( )    Prot_DW.Bit.no1 = 0


#define OutA_OpenErr        PCB_ErrState.Bit.no1
#define OutA_CloseErr       PCB_ErrState.Bit.no2

#define OutB_OpenErr        PCB_ErrState.Bit.no3
#define OutB_CloseErr       PCB_ErrState.Bit.no4 

#define OutC_OpenErr        PCB_ErrState.Bit.no5
#define OutC_CloseErr       PCB_ErrState.Bit.no6 

#define ElectricityOverLaodErr       PCB_ErrState.Bit.no7 

#define  APC950_PowerFlg              (IO02pin)            
#define  APC950_PowerON( )            set_IO02pin         
#define  APC950_PowerOFF( )           clr_IO02pin

#define  GET_APC950_RSET_PIN         ~(IO00pin)  
#define  SET_APC950_RSET_PIN          clr_IO00pin   
#define  RSET_APC950_RSET_PIN         set_IO00pin  

/*********************************************************************************
�������Ͷ���
*********************************************************************************/
typedef void (*APC950_Launch_OK)(void); 
/*********************************************************************************
�ⲿ����������
*********************************************************************************/
extern unsigned short  VIN_Ubf;    //��λ 100mV
extern unsigned short  OUT_Ibf;    //��λ 100mA
//extern unsigned short  OUTA_Ubf;   //��λ 100mV 
//extern unsigned short  OUTB_Ubf;   //��λ 100mV 
//extern unsigned short  OUTC_Ubf;   //��λ 100mV 
extern BIT_8W Prot_state;   //
extern BIT_8W Prot_UP;
extern BIT_8W Prot_DW;
extern BIT_16W  PCB_ErrState;
/*********************************************************************************
����������
*********************************************************************************/
void ADC1_TCIE_IRQ (void);
void AnalogComput (void);
void Port_sacn (void);

SystemErrName Launch_APC950 (APC950_Launch_OK FinishFunc);
/********************************************************************************/
#endif













