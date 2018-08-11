/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "..\inc\DriverPcb.h"
#include "..\inc\PowerConfig.h"
#include "..\..\Projects\inc\SofTimer.h"
#include "..\..\SPCP\APPCON\APPCON.h"
/*********************************************************************************
����������
*********************************************************************************/
/*********************************************************************************
��������������
*********************************************************************************/
unsigned short  VIN_Ubf = 0;    //��λ 100mV
unsigned short  OUT_Ibf = 0;    //��λ 100mA
BIT_8W Prot_state = {0};   //
BIT_8W Prot_UP = {0};
BIT_8W Prot_DW = {0};

unsigned short  ADC1_DataBuff[8];
BIT_16W  PCB_ErrState = {0};
/*********************************************************************************
�ⲿ����������
*********************************************************************************/
/*********************************************************************************
˽�б���������
*********************************************************************************/
static unsigned char   ADC1_DataBuff_Updata = 0;
static unsigned short  VIN_Ubf_BUF[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static unsigned short  OUT_Ibf_BUF[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                                     					
static SlideFilter_struct VIN_Ubf_SlideFilter = {0,16,0,0,VIN_Ubf_BUF};
static SlideFilter_struct OUT_Ibf_SlideFilter = {0,16,0,0,OUT_Ibf_BUF};

/*********************************************************************************
���Ա���������
*********************************************************************************/

/*********************************************************************************
�ڲ�����������
*********************************************************************************/
unsigned short Voltage_Formul (unsigned short ad);
unsigned short Electricity_Formul(unsigned short ad);
/*********************************************************************************
���ܴ��붨����
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   // ģ��������
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void AnalogComput (void)
{
 unsigned short itemp =0;
 
 if(0 == ADC1_DataBuff_Updata)
 {
   itemp = 1;
   return;
 }
 ADC1_DataBuff_Updata = 0;
 
 itemp = SlideFilter (&OUT_Ibf_SlideFilter, ADC1_DataBuff[0]);
 OUT_Ibf =  Electricity_Formul (itemp);
 
 itemp = SlideFilter (&VIN_Ubf_SlideFilter, ADC1_DataBuff[1]);
 VIN_Ubf =  Voltage_Formul (itemp);
 
 //��� ���ϼ��
 //if(OUT_Ibf > Electricity_OutLimit)
 //{
 //  PowerOut_Cmd(Rs485_ALL,DISABLE);
 //  OUT_I_Cnt = 200;
 //  SetState(ElectricityOverLaodErr);
 //}
 //else
 //{
 // if(OUT_I_Cnt > 0)
 // {OUT_I_Cnt --;}
 // RsetState(ElectricityOverLaodErr);
 //}
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void ADC1_TCIE_IRQ (void)
{
  ADC1_DataBuff_Updata = 1;
}
/*********************************************************************************
 Function:      //
 Description:   // ģ��������
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
unsigned short Voltage_Formul (unsigned short ad)
{
 unsigned long itemp =0;
 itemp = ad * 33000;
 itemp /= 4096;
 itemp *= 537;
 itemp /= 27000;  
 return itemp; 
}
/*********************************************************************************
 Function:      //
 Description:   // ģ��������
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
unsigned short Electricity_Formul(unsigned short ad)
{
 unsigned long itemp =0;
 itemp = ad * 33000;
 itemp /= 4096000;
 return itemp; 
}
/**********************************************************************************
  Function:       // 
  Description:    // 
  Input:          // 
                  // 
  Output:         // 
  Return:         // 
  Others:         // 
**********************************************************************************/
unsigned char get_portstate ( void )
{
 unsigned char portstate ;
 portstate = GPIOD->IDR &  GPIO_Pin_6;                       
 portstate >>= 6;
 return ~portstate; 
}
/**********************************************************************************
  Function:       // 
  Description:    // 
  Input:          // 
                  // 
  Output:         // 
  Return:         // 
  Others:         // 
**********************************************************************************/
void Port_sacn (void)
{
  unsigned short itemp1;
  unsigned char  itemp2;
  static unsigned char count = 0;
  static unsigned char prot_buff[key_cnt];
  static unsigned char last_state= 0;

  if(count >= key_cnt)
  {count =0;}
  prot_buff[count] = get_portstate( ) ;
  count ++ ;

  itemp2 = key_cnt;
  itemp1 = 0xffff;
  while (itemp2)
  {
   itemp2--;
   itemp1 &=  prot_buff[itemp2]; 
  }
  Prot_state.Byte |= itemp1;

  itemp2 = key_cnt;
  itemp1 = 0x0000;
  while (itemp2)
  {
   itemp2--;
   itemp1 |=  prot_buff[itemp2]; 
  }

  Prot_state.Byte &= itemp1;

  Prot_UP.Byte |= (last_state ^ Prot_state.Byte) & Prot_state.Byte;
  Prot_DW.Byte |= (last_state ^ Prot_state.Byte) & (~Prot_state.Byte);  
  last_state = Prot_state.Byte;
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
static void APPCON_CommTsetAckFunc(APPCON_OutPutBuff_Struct *cmd, 
                                    APPCON_AckType AckType, 
                                    DataBuffSpec_Struct* AckData)
{
 unsigned char itemp;

 if(NETID != cmd->Cmd)
 {return ;}
 
 if(OK_APPCON == AckType)
 {
   itemp = 0;
   while(itemp < AckData->length)
   {
     if(('\r' == AckData->pbuff[itemp])
        ||('\n' == AckData->pbuff[itemp]))
     {break;}
     else
     {Concentrator.AppconNetID[itemp] = AckData->pbuff[itemp];}
     itemp ++;
   }
   Concentrator.CSR.Bit.Appcon_EN = 1; 
 }
 else
 {Concentrator.CSR.Bit.Appcon_EN = 0; }

 freeSendBuff_APPCON(cmd);
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
static void Launch_APC950_STEP (void *p_tmr, void* p_arg)
{
 static unsigned char step = 0;
 
 if(0 == APC950_PowerFlg)
 {step = 1;}

 switch(step)   //
 {
  case 1:    //ģ���ϵ�
    {
     APC950_PowerON( );
     RSET_APC950_RSET_PIN; 
     SofTimer_ChangePeriod((SofTimer_Struct*) p_tmr,10,1);
     step ++;
     break;
    }
  case 2:    //ģ�鸴λ
    {
     APC950_PowerON( );
     SET_APC950_RSET_PIN; 
     SofTimer_ChangePeriod((SofTimer_Struct*) p_tmr,100,1); 
     step ++;
     break; 
    }
  case 3:   //ģ���������  
    {
     if(P_NULL != p_arg)  //�ж��Ƿ���Ҫ���Ϳ��������Ϣ
     {((APC950_Launch_OK)p_arg)( );} 
     SofTimer_ChangePeriod((SofTimer_Struct*) p_tmr,50,1); 
     step ++;
      ///Concentrator.CSR.Bit.Appcon_EN = 1;
     break; 
    }
  case 4:  
  case 5:  
  case 6:  
  case 7:  
  case 8:   
  case 9:
  case 10:  
    {
     if(0 != Concentrator.CSR.Bit.Appcon_EN)
     {
       SofTimerDel((SofTimer_Struct**)&p_tmr);
     } 
     else
     {
       if(NO_ERR ==  APPCON_NETID_SendFunc( Null_Port,
                                            P_NULL,
                                            p_tmr,
                                            APPCON_CommTsetAckFunc))
       {step ++;}
     }
     break; 
    }
  default:
   {
     APC950_PowerOFF( );                            
     SET_APC950_RSET_PIN;
     step = 0;
     SofTimerDel((SofTimer_Struct**)&p_tmr);
     break; 
   }   
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
SystemErrName Launch_APC950 (APC950_Launch_OK FinishFunc)
{
  SofTimer_Struct *Timer;
  APC950_PowerOFF( );                            
  SET_APC950_RSET_PIN;                             //ģ�����
  Timer =  SofTimerCreate( 30,                    //����
                           TimerTB_100MS,           //ʱ��
                           SofTimer_PERIODIC,      //ģʽ
                           Launch_APC950_STEP,     //�ص�
                           (void*)FinishFunc);          //�ص�����
  if(P_NULL == Timer)
  {return Nothing;}
  else
  {
   SofTimerStart(Timer);
   return NO_ERR; 
  }
}
/**************************************END****************************************/