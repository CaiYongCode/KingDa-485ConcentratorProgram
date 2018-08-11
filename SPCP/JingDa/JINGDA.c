/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include ".\JINGDA.h"
#include "..\CJT-188-2004\CJT-188-2004.h"
#include "..\..\LIB\FAT\ff.h"
/*********************************************************************************
����������
*********************************************************************************/

/*********************************************************************************
��������������
*********************************************************************************/
JD_ConfigData_Stuct     JD_ConfigData;
JD_Gprs_Stuct           JD_GprsData;
unsigned char           ID[5];
/*********************************************************************************
�ⲿ����������
*********************************************************************************/
/*********************************************************************************
˽�б���������
*********************************************************************************/ 
static CommPortNumber_Type  CurrentPort = Null_Port; 
static unsigned char LocalAddr[5] = {0,0,0,0,0}; 
//static struct {
//  union{
//    unsigned char Byte;
//    struct{
//      unsigned char idle :1; //����������
 ///   }Bit;
//  }CSR; //����״̬�Ĵ���
// JingDa_Frame_Struct   buff; 
//}OutPutBuff[JingDa_OutPutBuff_Number];


static JingDa_OutPutBuff_Struct  JingDa_OutPutBuff[JingDa_OutPutBuff_Number];
static FIL    File;
static  unsigned  char   State = 0;    // 0 ����ͨ������; 1�����ڳ���; 2: ����Ѳ����ַ; 
/*********************************************************************************
���Ա���������
*********************************************************************************/

/*********************************************************************************
�ڲ�����������
*********************************************************************************/
static SystemErrName JINGDA_frameVerify (JingDa_FrameDatas_Struct *frame, unsigned char length);
//���ü�����ʱ��
static SystemErrName TimeWcmd_Func(JD_TimeWcmd_askParam_Stuct* Indata, void* outData);
//��ȡ������ʱ��
static SystemErrName TimeRcmd_Func(JD_TimeRcmd_askParam_Stuct* Indata, void* outData);
//���ü������������
static SystemErrName ConfigDataWcmd_Func(JD_ConfigDataWcmd_askParam_Stuct* Indata, void* outData);
//��ȡ�������������
static SystemErrName ConfigDataRcmd_Func(JD_ConfigDataRcmd_askParam_Stuct* Indata, void* outData);	
//���ü�����GPRS����
static SystemErrName GprsWcmd_Func(JD_GprsWcmd_askParam_Stuct* Indata, void* outData);	
//��ȡ������GPRS����
static SystemErrName GprsRcmd_Func(JD_GprsRcmd_askParam_Stuct* Indata, void* outData);	
//���ر�Ƶ�ַ	
static SystemErrName MeterWcmd_Func(JD_MeterWcmd_askParam_Stuct* Indata, void* outData);		
//��ȡ���سɹ���Ƶ�ַ
static SystemErrName MeterRcmd_Func(JD_MeterRcmd_askParam_Stuct* Indata, void* outData);		
//��ȡ����ʧ�ܱ�Ƶ�ַ
static SystemErrName MeterErrRcmd_Func(JD_MeterErrRcmd_askParam_Stuct* Indata, void* outData);	
//�´��������
static SystemErrName OperateWcmd_Func(JD_OperateWcmd_askParam_Stuct* Indata, void* outData);	
//��ȡ��������
static SystemErrName OperateRcmd_Func(JD_OperateRcmd_askParam_Stuct* Indata, void* outData);		
//������������
static SystemErrName ReadingWcmd_Func(JD_ReadingWcmd_askParam_Stuct* Indata, void* outData);	
//��ȡĳһ�ɼ��������б�Ƶ�ǰ����
static SystemErrName AllCurrentDataRcmd_Func(JD_AllCurrentDataRcmd_askParam_Stuct* Indata, void* outData);	
//��ȡĳһ�ɼ��������б���ն�������	
static SystemErrName AllFreezingDataRcmd_Func(JD_AllFreezingDataRcmd_askParam_Stuct* Indata, void* outData);	
//��ȡ����ĵ�ǰ����
static SystemErrName SingCurrentDataRcmd_Func(JD_SingCurrentDataRcmd_askParam_Stuct* Indata, void* outData);	
//��ȡ������ն�������
static SystemErrName SingFreezingDataRcmd_Func(JD_SingFreezingDataRcmd_askParam_Stuct* Indata, void* outData);	
//�滻���
static SystemErrName AlterMeterWcmd_Func(JD_AlterMeterWcmd_askParam_Stuct* Indata, void* outData);
//ɾ�����
static SystemErrName DeleteMeterWcmd_Func(JD_DeleteMeterWcmd_askParam_Stuct* Indata, void* outData);

//static SystemErrName JingDa_Send(CommPortNumber_Type Port, JingDa_Frame_Struct* farame);

/*********************************************************************************
���ܴ��붨����
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
void JingDaSPCP_init(void)
{
 unsigned char itemp = 0;
 while (itemp < JingDa_OutPutBuff_Number)
 {
   JingDa_OutPutBuff[itemp].CSR.Bit.idle = 1;
   itemp++;
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
void Frame_init(JingDa_Frame_Struct* frame)
{
 if(P_NULL != frame )
 {
  frame->Preamble = JINGDA_Preamble;
  frame->Datas.Head.StartCharacter = JINGDA_StartCharacter;
  frame->Datas.Head.LAI = JINGDA_AreaCode;
  
  frame->Datas.Head.Concentrator[0]  = LocalAddr[0];
  frame->Datas.Head.Concentrator[1]  = LocalAddr[1];
  frame->Datas.Head.Concentrator[2]  = LocalAddr[2];
  frame->Datas.Head.Concentrator[3]  = LocalAddr[3];
  frame->Datas.Head.Concentrator[4]  = LocalAddr[4];
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
JingDa_OutPutBuff_Struct* getJingDaSendBuff(void)
{
 unsigned char itemp = 0;
 while (itemp < JingDa_OutPutBuff_Number)
 {
   if(1 == JingDa_OutPutBuff[itemp].CSR.Bit.idle)
   {
     JingDa_OutPutBuff[itemp].CSR.Bit.idle = 0;
     Frame_init(&(JingDa_OutPutBuff[itemp].Frame));
     return (&(JingDa_OutPutBuff[itemp]));
   }
   itemp ++;
 }
 return (P_NULL);
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
void freeJingDaSendBuff(void* frame )
{
 unsigned char itemp = 0;
 while (itemp < JingDa_OutPutBuff_Number)
 {
   if(frame == &(JingDa_OutPutBuff[itemp])
      ||frame == &(JingDa_OutPutBuff[itemp].Frame))
   {
     FreeOutputBuffForData_MXPC (&(JingDa_OutPutBuff[itemp].Frame));
     JingDa_OutPutBuff[itemp].CSR.Bit.idle = 1;
     break;
   }
   itemp ++;
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
SystemErrName JingDa_InPut (void *pbuff, unsigned length,CommPortNumber_Type port)
{
 SystemErrName err = NO_ERR;
 unsigned char itemp;
 JingDa_FrameDatas_Struct *package;
 JingDa_OutPutBuff_Struct* ack;
 
 itemp = 0;
 //while (JINGDA_Preamble ==((unsigned char *)pbuff)[itemp])
 //{itemp ++;}
 package =(JingDa_FrameDatas_Struct *)(&((unsigned char *)pbuff)[itemp]);
 
 err = JINGDA_frameVerify(package, length);
 if(NO_ERR != err) 
 {return err;}
 
 CurrentPort = port;
 
 ack =  getJingDaSendBuff( );
 if(P_NULL == ack)
 {return Nothing;}
 
 switch (package->Head.Code)
 {
  case JD_Time_Wcmd:  //���ü�����ʱ��
    {
      err = TimeWcmd_Func(&(package->Param.TimeWcmd_ask), ack->Frame.Datas.Param.Data);
      ack->Frame.Datas.Head.DataLength.Word = sizeof(JD_TimeWcmd_ackParam_Stuct);
      break;
    }
  case JD_Time_Rcmd:  //��ȡ������ʱ��
   { 
     err = TimeRcmd_Func(&(package->Param.TimeRcmd_ask), ack->Frame.Datas.Param.Data);
     ack->Frame.Datas.Head.DataLength.Word = sizeof(JD_TimeRcmd_ackParam_Stuct);
     break;
    }
 case JD_ConfigData_Wcmd:  //���ü������������
   { 
     err = ConfigDataWcmd_Func(&(package->Param.ConfigDataWcmd_ask), ack->Frame.Datas.Param.Data);
     ack->Frame.Datas.Head.DataLength.Word = sizeof(JD_ConfigDataWcmd_ackParam_Stuct);
     break;
    }
 case JD_ConfigData_Rcmd:  //��ȡ�������������
   { 
     err = ConfigDataRcmd_Func((void*)&(package->Param.Data), ack->Frame.Datas.Param.Data);
     ack->Frame.Datas.Head.DataLength.Word = sizeof(JD_ConfigDataRcmd_ackParam_Stuct);
     break;
    } 
 case JD_Gprs_Wcmd:  //���ü�����GPRS����
   { 
     err = GprsWcmd_Func((void*)(package->Param.Data), ack->Frame.Datas.Param.Data);
     ack->Frame.Datas.Head.DataLength.Word = sizeof(JD_GprsWcmd_ackParam_Stuct);
     break;
    }     
 case JD_Gprs_Rcmd:  //��ȡ������GPRS����
   { 
     err = GprsRcmd_Func((void*)(package->Param.Data), ack->Frame.Datas.Param.Data);
     ack->Frame.Datas.Head.DataLength.Word = sizeof(JD_GprsRcmd_ackParam_Stuct);
     break;
    } 
 case JD_Meter_Wcmd:  //���ر�Ƶ�ַ
   { 
     err = MeterWcmd_Func((void*)&(package->Param.Data), ack->Frame.Datas.Param.Data);
     ack->Frame.Datas.Head.DataLength.Word = sizeof(JD_MeterWcmd_ackParam_Stuct);
     break;
    }
 case JD_Meter_Rcmd:  //��ȡ���سɹ���Ƶ�ַ
   { 
     err = MeterRcmd_Func((void*)&(package->Param.Data), ack->Frame.Datas.Param.Data);
     ack->Frame.Datas.Head.DataLength.Word = ((JD_MeterRcmd_ackParam_Stuct*)&(ack->Frame.Datas.Param))->MetNumber * sizeof (JD_Meter_Stuct);
     ack->Frame.Datas.Head.DataLength.Word += 7;
     break;
    }    
 case JD_MeterErr_Rcmd:  //��ȡ����ʧ�ܱ�Ƶ�ַ
   { 
     err = MeterErrRcmd_Func((void*)&(package->Param.Data), ack->Frame.Datas.Param.Data);
     ack->Frame.Datas.Head.DataLength.Word = ((JD_MeterErrRcmd_ackParam_Stuct*)&(ack->Frame.Datas.Param))->MetNumber * sizeof (JD_Meter_Stuct);
     ack->Frame.Datas.Head.DataLength.Word += 7;
     break;
    }  	
 case JD_Operate_Wcmd:  //�´��������
   { 
     err = OperateWcmd_Func((void*)&(package->Param.Data), ack->Frame.Datas.Param.Data);
     ack->Frame.Datas.Head.DataLength.Word = sizeof(JD_OperateWcmd_ackParam_Stuct);
     break;
    } 
 case JD_Operate_Rcmd:  //��ȡ��������
   { 
     err = OperateRcmd_Func((void*)&(package->Param.Data), ack->Frame.Datas.Param.Data);
     ack->Frame.Datas.Head.DataLength.Word = sizeof(JD_OperateRcmd_ackParam_Stuct);
     break;
    } 
 case JD_Reading_Wcmd:  //������������
   { 
     err = ReadingWcmd_Func((void*)&(package->Param.Data), ack->Frame.Datas.Param.Data);
     ack->Frame.Datas.Head.DataLength.Word = sizeof(JD_ReadingWcmd_ackParam_Stuct);
     break;
    } 

 case JD_AllCurrentData_Rcmd:  //��ȡĳһ�ɼ��������б�Ƶ�ǰ����
   { 
     err = AllCurrentDataRcmd_Func((void*)&(package->Param.Data), ack->Frame.Datas.Param.Data);
     ack->Frame.Datas.Head.DataLength.Word = ((JD_AllCurrentDataRcmd_ackParam_Stuct*)&(ack->Frame.Datas.Param))->RecordNum;
     ack->Frame.Datas.Head.DataLength.Word *= sizeof (JD_CJT188_MeterValue_Stuct);
     ack->Frame.Datas.Head.DataLength.Word += 5;
     break;
    }     

 case JD_AllFreezingData_Rcmd:  //��ȡĳһ�ɼ��������б���ն�������
   { 
     err = AllFreezingDataRcmd_Func((void*)&(package->Param.Data), ack->Frame.Datas.Param.Data);
     //ack->buff.buff.Head.DataLength.Word = ((JD_AllFreezingDataRcmd_ackParam_Stuct*)&(ack->buff.Data))->RecordNum;
     //ack->Frame.Datas.Head.DataLength.Word *= sizeof (JD_CJT188_MeterValue_Stuct);
     //ack->Frame.Datas.Head.DataLength.Word += 5;
     break;
    } 

 case JD_SingCurrentData_Rcmd:  //��ȡ����ĵ�ǰ����
   { 
     err = SingCurrentDataRcmd_Func((void*)&(package->Param.Data), ack->Frame.Datas.Param.Data);
     ack->Frame.Datas.Head.DataLength.Word = sizeof(JD_SingCurrentDataRcmd_CJT188_ackParam_Stuct);
     break;
    }     
 case JD_SingFreezingData_Rcmd:  //��ȡ������ն�������
   { 
     err = SingFreezingDataRcmd_Func((void*)&(package->Param.Data), ack->Frame.Datas.Param.Data);
     ack->Frame.Datas.Head.DataLength.Word = sizeof(JD_SingFreezingDataRcmd_ackParam_Stuct);
     break;
    }  
 case JD_AlterMeter_Wcmd:  //�滻���
   { 
     err = AlterMeterWcmd_Func((void*)&(package->Param.Data), ack->Frame.Datas.Param.Data);
     ack->Frame.Datas.Head.DataLength.Word = sizeof(JD_AlterMeterWcmd_ackParam_Stuct);
     break;
    }
 case JD_DeleteMeter_Wcmd:  //ɾ�����
   { 
     err = DeleteMeterWcmd_Func((void*)(package->Param.Data), ack->Frame.Datas.Param.Data);
     ack->Frame.Datas.Head.DataLength.Word = ((JD_DeleteMeterWcmd_ackParam_Stuct*)&(ack->Frame.Datas.Param))->TotalNumber;
     ack->Frame.Datas.Head.DataLength.Word += 3;
     break;
    }      
 default:
   {err =  OperateCmd_err;}
 }
 if(NO_ERR != err) 
 {
   freeJingDaSendBuff(ack);
   return err;
 }
 ack->Frame.Datas.Head.Code = package->Head.Code;
 err = JingDa_Send(port, &(ack->Frame));
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
//void JingDa_Send(CommPortNumber_Type Port,  JingDa_Frame_Struct* farame)
//{
  
//}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName JINGDA_frameVerify (JingDa_FrameDatas_Struct *frame, unsigned char length)
{
 unsigned short itemp ;
 unsigned char  itemp2 ;
 unsigned char  sum;
 
 if(JINGDA_StartCharacter != frame->Head.StartCharacter)
 {return FrameSync_err;}
 
 itemp = sizeof(JingDa_FrameHead_Struct);
 itemp += frame->Head.DataLength.Word;
 itemp += 2;    //У��� + ������
 if(length < itemp)
 {return Length_err;}
 
 itemp2 = 0;
 while(itemp2 < ConcentratorAddr_Length)
 {
  if(frame->Head.Concentrator[itemp2]!= LocalAddr[itemp2])
  {break;}
  itemp2 ++;
 }
 
 itemp2 = 0;
 sum = 0;  
 while(itemp2 < itemp)
 {
  sum += ((unsigned char *)frame)[itemp2];
  itemp2 ++;
 }
 if(sum != ((unsigned char *)frame)[itemp])
 {return Verify_err;}
 return NO_ERR; 
}
/*********************************************************************************
 Function:      //
 Description:   //���ü�����ʱ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName TimeWcmd_Func(JD_TimeWcmd_askParam_Stuct* Indata, void* outData)
{
  JD_TimeWcmd_ackParam_Stuct* ack;
  TimerData_Struct timer;

  timer.Year = 2000 + BCDToInt8 (&(Indata->Year),2);
  timer.Month = BCDToInt8 (&(Indata->Month),2);
  timer.Day = BCDToInt8 (&(Indata->Day),2);
  timer.Hour = BCDToInt8 (&(Indata->Hour),2);
  timer.Minute = BCDToInt8 (&(Indata->Minute),2);
  timer.Second = BCDToInt8 (&(Indata->Second),2);
  timer.WeekDay = BCDToInt8 (&(Indata->WeekDay),2);

  PCF8563_Set_Time(&timer);
  
  ack =  outData;
  ack->ReplyFlag = 0x00;
  ack->SER = Indata->SER;
  return NO_ERR;
}
/*********************************************************************************
 Function:      //��ȡ������ʱ��
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName TimeRcmd_Func(JD_TimeRcmd_askParam_Stuct* Indata, void* outData)
{
  JD_TimeRcmd_ackParam_Stuct* ack;
  TimerData_Struct timer;

  get_SystemTimer(&timer);
  
  ack = outData;
  Int8ToBCD (timer.Year, &ack->Year, 2);
  Int8ToBCD (timer.Month, &ack->Month, 2);
  Int8ToBCD (timer.Day, &ack->Day, 2);
  Int8ToBCD (timer.Hour, &ack->Hour, 2);
  Int8ToBCD (timer.Minute, &ack->Minute, 2);
  Int8ToBCD (timer.Second, &ack->Second, 2);
  Int8ToBCD (timer.WeekDay, &ack->WeekDay, 2);

  ack->ReplyFlag = 0x00;
  ack->SER = Indata->SER;
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //���ü������������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName ConfigDataWcmd_Func(JD_ConfigDataWcmd_askParam_Stuct* Indata, void* outData)
{
  unsigned short itemp;
  unsigned char* tp; 
  unsigned char* cp;
  JD_ConfigDataWcmd_ackParam_Stuct* ack;
  SystemErrName err;
  JD_ConfigData_Stuct data;
  
  itemp = 0;
  tp = (unsigned char*)&JD_ConfigData;
  cp = (unsigned char*)&(Indata->data);
  while(itemp < sizeof (JD_ConfigData_Stuct))
  { 
    tp[itemp] = cp[itemp];
    itemp ++;
  }
  //���泭������
  err =  EEPROM_I2C_BufferWrite(&(Indata->data), 
                               JD_ConfigData_AddrEeprom, 
                               sizeof (JD_ConfigData_Stuct)); 
  //У��洢
  if(NO_ERR == err )
  {
    err =  EEPROM_I2C_MultipleRead(&data,
                                  JD_ConfigData_AddrEeprom,
                                  sizeof (JD_ConfigData_Stuct)); 
    if(NO_ERR == err )
    {
     itemp = 0; 
     tp = (unsigned char*)&data;
     cp = (unsigned char*)&(Indata->data);
     while(itemp < sizeof (JD_ConfigData_Stuct))
     {
      if(tp[itemp] != cp[itemp])
      {break;}
      itemp ++;
     }
     if(itemp < sizeof (JD_ConfigData_Stuct))
     {err = Unsuccessful; } 
    }
  }
  //����Ӧ������  
  ack = outData; 
  if(NO_ERR == err)
  {ack->ReplyFlag = 0x00;}    //���� 
  else
  {ack->ReplyFlag = 0xCC;} //����д��eeprom����ȷ
  ack->ReplyFlag = 0;
  ack->SER = Indata->SER;  
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ�������������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName ConfigDataRcmd_Func(JD_ConfigDataRcmd_askParam_Stuct* Indata, void* outData)	
{
  unsigned short itemp = 0;
  unsigned char* tp; 
  unsigned char* cp;
  JD_ConfigDataRcmd_ackParam_Stuct* ack;
  
  ack = outData;
  tp = (unsigned char*)&(ack->Data);
  cp = (unsigned char*)&JD_ConfigData;
  while(itemp < sizeof (JD_ConfigData_Stuct))
  {
    tp[itemp] = cp[itemp];
    itemp ++;
  }
  ack->ReplyFlag = 0;
   ack->SER = Indata->SER;  
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //���ü�����GPRS����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName GprsWcmd_Func(JD_GprsWcmd_askParam_Stuct* Indata, void* outData)
{
  unsigned short itemp;
  unsigned char* tp; 
  unsigned char* cp;
  JD_GprsWcmd_ackParam_Stuct* ack;
  SystemErrName err;
  JD_Gprs_Stuct data;
  
  itemp = 0;
  tp = (unsigned char*)&JD_GprsData;
  cp = (unsigned char*)&(Indata->data);
  while(itemp < sizeof (JD_Gprs_Stuct))
  {
    tp[itemp] = cp[itemp];
    itemp ++;
  }
  //���泭������
  err =  EEPROM_I2C_BufferWrite(&(Indata->data), 
                               JD_GprsData_AddrEeprom, 
                               sizeof (JD_Gprs_Stuct)); 
  //У��洢
  if(NO_ERR == err )
  {
    err =  EEPROM_I2C_MultipleRead(&data,
                                  JD_GprsData_AddrEeprom,
                                  sizeof (JD_Gprs_Stuct)); 
    if(NO_ERR == err )
    {
     itemp = 0; 
     tp = (unsigned char*)&data;
     cp = (unsigned char*)&(Indata->data);
     while(itemp < sizeof (JD_Gprs_Stuct))
     {
      if(tp[itemp] != cp[itemp])
      {break;}
      itemp ++;
     }
     if(itemp < sizeof (JD_Gprs_Stuct))
     {err = Unsuccessful; } 
    }
  }
  //����Ӧ������  
  ack = outData; 
  if(NO_ERR == err)
  {ack->ReplyFlag = 0x00;}    //���� 
  else
  {ack->ReplyFlag = 0xCC;} //����д��eeprom����ȷ
  ack->ReplyFlag = 0;
  ack->SER = Indata->SER;  
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ������GPRS����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName GprsRcmd_Func(JD_GprsRcmd_askParam_Stuct* Indata, void* outData)
{
 unsigned short itemp = 0;
 unsigned char* tp; 
 unsigned char* cp;
 JD_GprsRcmd_ackParam_Stuct* ack;
  
  ack = outData;
  tp = (unsigned char*)&(ack->Data);
  cp = (unsigned char*)&JD_GprsData;
  while(itemp < sizeof (JD_Gprs_Stuct))
  {
    tp[itemp] = cp[itemp];
    itemp ++;
  }
  ack->ReplyFlag = 0;
  ack->SER = Indata->SER;  
  return NO_ERR; 
  
}
/*********************************************************************************
 Function:      //
 Description:   //���ر�Ƶ�ַ	
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName MeterWcmd_Func(JD_MeterWcmd_askParam_Stuct* Indata, void* outData)		
{
 FRESULT rse;
 unsigned char itemp;
 UINT number;
 JD_MeterWcmd_ackParam_Stuct* ack;
 

 if(1 == Indata->PackageNumber)
 {f_unlink(JD_MeterConfigFile_PathName);}
 rse = f_open(&File,
               JD_MeterConfigFile_PathName, 
               FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
 if(FR_OK == rse)
 {
  rse = f_lseek (&File , File.fsize);
  itemp = 0;
  while(itemp < Indata->MetNumber)
  {
   f_write (&File, &Indata->Meter[itemp], sizeof(JD_Meter_Stuct), &number);
   itemp ++;
  }
 }
 f_close (&File);
 
 ack = outData;
 ack->ReplyFlag =  0x00;
 ack->SER = Indata->SER;
 
 return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ���سɹ���Ƶ�ַ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName MeterRcmd_Func(JD_MeterRcmd_askParam_Stuct* Indata, void* outData)		
{
 FRESULT rse;
 unsigned short itemp;
 UINT number;
 JD_MeterRcmd_ackParam_Stuct* ack;
 JD_Meter_Stuct    Meter;
 
 ack = outData;
 rse = f_open(&File, JD_MeterConfigFile_PathName, FA_READ);
 if(FR_OK == rse)
 {
   f_lseek (&File , 0);
   itemp = 0;
   while(1)
   {
     rse = f_read (&File, &Meter,sizeof(JD_Meter_Stuct), &number);
     if(FR_OK == rse)
     {
      if(number == sizeof(JD_Meter_Stuct))
      {
       if(Meter.CR1.Bit.Channel == Indata->Channel)
       {itemp ++;}
      }
      else
      {break;}
     }
     else
     {return DataFileLost;}
   }
 }
 else
 {return DataFileLost;}
 
  ack->MeterSum.Word = itemp;
 
  ack->PackageSum = itemp / 25;
  if(0 != itemp % 25)
  {ack->PackageSum ++;}
 
  ack->PackageNumber = Indata->PackageNumber;
 
  itemp  = ack->PackageNumber - 1; 
  itemp  *= 25;
  
  ack->MetNumber = 0;
  
  f_lseek (&File , 0);
  
  while(ack->MetNumber < 25)
   {
    rse = f_read (&File, &ack[ack->MetNumber],sizeof(JD_Meter_Stuct), &number);
    if(FR_OK != rse)
    {
     if(number == sizeof(JD_Meter_Stuct))
     {
      if(Meter.CR1.Bit.Channel == Indata->Channel)
      {
       if(itemp > 0)
       {itemp --;}
       else
       {ack->MetNumber ++;}
      }
      else
      {break;}
     }
     else
     {return DataFileLost;}
    }
    else
    {return DataFileLost;}
   }
  f_close (&File);
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ����ʧ�ܱ�Ƶ�ַ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName MeterErrRcmd_Func(JD_MeterErrRcmd_askParam_Stuct* Indata, void* outData)	
{
  JD_MeterErrRcmd_ackParam_Stuct* ack;
  
  ack = outData;
  ack->PackageSum = 0;
  ack->MeterSum.Word = 0;
  ack->PackageNumber = Indata->PackageNumber;
  ack->MetNumber = 0;
  return DataFileLost;
}
/*********************************************************************************
 Function:      //
 Description:   //�´��������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void CJT188_OperateAckFunc(CJT188_SendBuff_Struct* SendData, CJT188_Frame_Struct* AckData)
{
  JD_OperateWcmd_ackParam_Stuct*  ack;
  JingDa_OutPutBuff_Struct*  outbuff;   
  
 outbuff =  getJingDaSendBuff( );
 if(P_NULL == outbuff)
 {return;}
 
 ack = (JD_OperateWcmd_ackParam_Stuct *) outbuff->Frame.Datas.Param.Data;
 
 ack->MeterType = SendData->buff.Normal.Head.Type;
 
 CopyBuff (&(SendData->buff.Head.Addr), &(ack->Addr), sizeof(JD_MeterAddr_Stuct));
 
 if(P_NULL == AckData) 
 {
  ack->MetState = 0x00;
  ack->ReplyFlag =  0xCE;
 }
 else
 {
  if(0 == AckData->Head.CTR.Bit.CommFlg) 
  {
    //ack->MetState = AckData->Normal.Data.ValveState;
    ack->ReplyFlag =  0x00;
  }
  else
  {
    ack->MetState = 0x00;
    ack->ReplyFlag = 0x05; //MBUSͨ���쳣 
  }
 }
 ack->SER = SendData->buff.Normal.DataHead.SER;
 outbuff->Frame.Datas.Head.Code = JD_Operate_Wcmd;
 JingDa_Send(SendData->SourcePort, &(outbuff->Frame));   
}
/*********************************************************************************
 Function:      //
 Description:   //�´��������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName OperateWcmd_Func(JD_OperateWcmd_askParam_Stuct* Indata, void* outData)	
{
  SystemErrName err;
  FRESULT rse;
  UINT number;
  CommPortNumber_Type  Port;
  JD_OperateWcmd_ackParam_Stuct*  ack;
  ack = outData; 
  
  switch (Indata->Operate.CR1.Bit.Channel)
  {
   case 1:  //M-BUSͨ��1
     {Port = Rs485_PortA; break;}
   case 2:  //M-BUSͨ��2
     {Port = Rs485_PortA; break;}
   case 3:  //M-BUSͨ��3
     {Port = Rs485_PortA; break;}
   case 4:  //M-BUSͨ��4
     {Port = Rs485_PortA; break;}
   default:
     {return DataValid;}
  }
 
  if(0x00 == Indata->Operate.CR1.Bit.Protocol)
  {
   //��ˮ���Ͳ���ָ��  
   err =  CJT188_OperateNodeValve(Indata->Operate.MeterType,
                                  (CJT188_AddresStruct*)&(Indata->Operate.Addr),
                                  Indata->Operate.MeterState, 
                                  CurrentPort,
                                  Port,
                                  CJT188_OperateAckFunc);
   if(NO_ERR != err)
   {ack->ReplyFlag =  0xCB;}   //����æ;
   else
   { ack->ReplyFlag =  0x00;  }
  }
  else
  {return DataValid;}
  //��¼����
  rse = f_open(&File, JD_OperateRecordFile_PathName, FA_CREATE_ALWAYS|FA_WRITE|FA_READ);
  if(FR_OK == rse)
  {
   f_lseek (&File , File.fsize);
   f_write (&File, &(Indata->Operate), sizeof(JD_OperateRecord_Stuct), &number);
   f_close (&File);
  }
  
  ack->MetState =  0x00; //�ط��ɹ�
  ack->MeterType = Indata->Operate.MeterType;
  CopyBuff (&(Indata->Operate.Addr), &(ack->Addr), sizeof(JD_MeterAddr_Stuct));
  return NO_ERR;
}

/*********************************************************************************
 Function:      //
 Description:   //��ȡ��������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName OperateRcmd_Func(JD_OperateRcmd_askParam_Stuct* Indata, void* outData)		
{
 FRESULT rse;
 unsigned short itemp;
 UINT number;
 JD_OperateRcmd_ackParam_Stuct* ack;
 JD_OperateRecord_Stuct    Record;
 
 ack = outData;
 rse = f_open(&File, JD_OperateRecordFile_PathName, FA_READ);
 if(FR_OK == rse)
 {
   f_lseek (&File , 0);
   itemp = 0;
   while(1)
   {
     rse = f_read (&File, &Record,sizeof(JD_OperateRecord_Stuct), &number);
     if(FR_OK == rse)
     {
      if(number == sizeof(JD_Meter_Stuct))
      {itemp ++;}
      else
      {break;}
     }
     else
     {return DataFileLost;}
   }
 }
 else
 {return DataFileLost;}
  ack->PackageSum = itemp / 15;
  if(0 != itemp % 15)
  {ack->PackageSum ++;}
  ack->PackageNumber = Indata->PackageNumber;
  itemp  = ack->PackageNumber - 1; 
  itemp  *= itemp;
  ack->ConNumber = 0;
  f_lseek (&File , 0);
  while( ack->ConNumber < 25)
   {
    rse = f_read (&File, &ack[ack->ConNumber],sizeof(JD_OperateRecord_Stuct), &number);
    if(FR_OK != rse)
    {
     if(number == sizeof(JD_OperateRecord_Stuct))
     {
       if(itemp > 0)
       {itemp --;}
       else
       {ack->ConNumber ++;}
     }
     else
     {break;}
    }
    else
    {return DataFileLost;}
   }
  f_close (&File);
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //������������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName ReadingSteup_Func( void )
{
  
  
  
  
  
  
  
   return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //������������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName ReadingWcmd_Func(JD_ReadingWcmd_askParam_Stuct* Indata, void* outData)	
{
 JD_ReadingWcmd_ackParam_Stuct* ack;

 ack = outData;
 if((0 == State)||(State > 2 ))          // ����ͨ������
 {
   ack ->ReplyFlag =0;   // ������������
   State = 1; 
   ReadingSteup_Func( );
 }
 else if(1 == State)     // ���ڳ���; 
 {ack ->ReplyFlag =1; }  // ����æ�Ѿ��ڳ�����,�����ٴ�����
 else if(2 == State)     // ����Ѳ����ַ;
 {ack ->ReplyFlag = 2; } // ��������Ѳ����ַ,������������
 return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡĳһ�ɼ��������б�Ƶ�ǰ����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName AllCurrentDataRcmd_Func(JD_AllCurrentDataRcmd_askParam_Stuct* Indata, void* outData)	
{
 FRESULT rse;
 unsigned short itemp;
 UINT number;
 JD_AllCurrentDataRcmd_ackParam_Stuct* ack;
 JD_Time_Stuct   Time;
 
 ack = outData;
 rse = f_open(&File, JD_AllMeterCurrentValueFile_PathName, FA_READ);
 if(FR_OK == rse)
 {
   f_lseek (&File,0);
   rse = f_read (&File, &Time,sizeof(JD_Time_Stuct), &number);
   if((FR_OK == rse)&&(sizeof(JD_Time_Stuct) == number))
   {
    Int8ToBCD (Time.Year, &(ack->Time.Year), 2);
    Int8ToBCD (Time.Month, &(ack->Time.Month), 2);
    Int8ToBCD (Time.Day, &(ack->Time.Day), 2); 
    Int8ToBCD (Time.Hour, &(ack->Time.Hour), 2);
    Int8ToBCD (Time.Minute, &(ack->Time.Minute), 2);
    Int8ToBCD (Time.Second, &(ack->Time.Second), 2);
   }
   else
   {return DataFileLost;}
 }
 else
 {return DataFileLost;}
   
 itemp = File.fsize - sizeof(JD_Time_Stuct);
 itemp /= sizeof(JD_CJT188_MeterValue_Stuct);
 ack->FileSum = itemp / 17;
 if(0 != itemp % 17)
 { ack->FileSum ++;}
 ack->Filecount = Indata->PackageNumber;
 itemp  = ack->Filecount - 1; 
 itemp  *= 17;
 ack->RecordNum = 0;
 f_lseek (&File , sizeof(JD_Time_Stuct));
 while( ack->RecordNum < 17)
 {
  rse = f_read (&File, &(ack->Value.CJT188[ack->RecordNum]),sizeof(JD_CJT188_MeterValue_Stuct), &number);
  if(FR_OK != rse)
  {
   if(number == sizeof(JD_CJT188_MeterValue_Stuct))
   {
    if(itemp > 0)
    {itemp --;}
    else
    {ack->RecordNum ++;}
   }
   else
   {break;}
  }
  else
  {return DataFileLost;}
 }
 f_close (&File);
 return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡĳһ�ɼ��������б���ն�������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName AllFreezingDataRcmd_Func(JD_AllFreezingDataRcmd_askParam_Stuct* Indata, void* outData)	
{
 FRESULT rse;
 unsigned long itemp;
 UINT number;
 JD_AllFreezingDataRcmd_ackParam_Stuct* ack;
 JD_Time_Stuct   Time;
 
 ack = outData;
 rse = f_open(&File,JD_AllMeterFreezingValueFile_PathName, FA_READ);
 if(FR_OK == rse)
 {
   f_lseek (&File,0);
   rse = f_read (&File, &Time,sizeof(JD_Time_Stuct), &number);
   if((FR_OK == rse)&&(sizeof(JD_Time_Stuct) == number))
   {
    Int8ToBCD (Time.Year, &(ack->Time.Year), 2);
    Int8ToBCD (Time.Month, &(ack->Time.Month), 2);
    Int8ToBCD (Time.Day, &(ack->Time.Day), 2); 
    Int8ToBCD (Time.Hour, &(ack->Time.Hour), 2);
    Int8ToBCD (Time.Minute, &(ack->Time.Minute), 2);
    Int8ToBCD (Time.Second, &(ack->Time.Second), 2);
   }
   else
   {return DataFileLost;}
 }
 else
 {return DataFileLost;}
 
 itemp = File.fsize - sizeof(JD_Time_Stuct);
 itemp /= sizeof(JD_CJT188_MeterValue_Stuct);
 ack->FileSum = itemp / 17;
 if(0 != itemp % 17)
 { ack->FileSum ++;}
 ack->Filecount = Indata->PackageNumber;
 itemp  = ack->Filecount - 1; 
 itemp  *= 17;
 ack->RecordNum = 0;
 f_lseek (&File , sizeof(JD_Time_Stuct));
 while( ack->RecordNum < 17)
 {
  rse = f_read (&File, &(ack->Value.CJT188[ack->RecordNum]),sizeof(JD_CJT188_MeterValue_Stuct), &number);
  if(FR_OK != rse)
  {
   if(number == sizeof(JD_CJT188_MeterValue_Stuct))
   {
    if(itemp > 0)
    {itemp --;}
    else
    {ack->RecordNum ++;}
   }
   else
   {break;}
  }
  else
  {return DataFileLost;}
 }
 f_close (&File);
 return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ����ĵ�ǰ����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName SingCurrentDataRcmd_Func(JD_SingCurrentDataRcmd_askParam_Stuct* Indata, void* outData)	
{
 SystemErrName  err = NO_ERR; 
 FRESULT rse;
 UINT number;
 JD_SingCurrentDataRcmd_CJT188_ackParam_Stuct* ack;
 
 ack = outData;
 if(0 ==Indata->Meter.CR1.Bit.Protocol) 
 {
  rse = f_open(&File,JD_AllMeterCurrentValueFile_PathName, FA_READ);
  if(FR_OK == rse)
  {
   f_lseek (&File , sizeof(JD_Time_Stuct));
   while( 1 )
   {
    rse = f_read (&File, &(ack->Data),sizeof(JD_CJT188_MeterValue_Stuct), &number);
    if((FR_OK == rse)&&(sizeof(JD_CJT188_MeterValue_Stuct) == number))
    {
     if((ack->Data.Meter.Addr.FactoryCode.Word = Indata->Meter.Addr.FactoryCode.Word)
        &&(ack->Data.Meter.Addr.ID[0] = Indata->Meter.Addr.ID[0])
        &&(ack->Data.Meter.Addr.ID[1] = Indata->Meter.Addr.ID[1])
        &&(ack->Data.Meter.Addr.ID[2] = Indata->Meter.Addr.ID[2])
        &&(ack->Data.Meter.Addr.ID[3] = Indata->Meter.Addr.ID[3])
        &&(ack->Data.Meter.Addr.ID[4] = Indata->Meter.Addr.ID[4]))
     {
      break; 
     }  
    }
    else
    {err = Nodata ;break;}
   }
  }
  else
  {err= DataFileLost;} 
 }
 
 f_close (&File);
 if(NO_ERR == err)
 {ack->ReplyFlag = 0x00; } //0x00:������
 else
 {ack->ReplyFlag = 0xCF; }  //0xCF:δ�ҵ��� */
 return err;
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ������ն�������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName SingFreezingDataRcmd_Func(JD_SingFreezingDataRcmd_askParam_Stuct* Indata, void* outData)	
{
 SystemErrName  err = NO_ERR; 
 FRESULT rse;
 UINT number;
 JD_SingFreezingDataRcmd_ackParam_Stuct* ack;
 
 ack = outData;
 if(0 ==Indata->Meter.CR1.Bit.Protocol) 
 {
  rse = f_open(&File,JD_AllMeterFreezingValueFile_PathName, FA_READ);
  if(FR_OK == rse)
  {
   f_lseek (&File , sizeof(JD_Time_Stuct));
   while( 1 )
   {
    rse = f_read (&File, &(ack->Value.CJT188),sizeof(JD_CJT188_MeterValue_Stuct), &number);
    if((FR_OK == rse)&&(sizeof(JD_CJT188_MeterValue_Stuct) == number))
    {
     if((ack->Value.CJT188.Meter.Addr.FactoryCode.Word = Indata->Meter.Addr.FactoryCode.Word)
        &&(ack->Value.CJT188.Meter.Addr.ID[0] = Indata->Meter.Addr.ID[0])
        &&(ack->Value.CJT188.Meter.Addr.ID[1] = Indata->Meter.Addr.ID[1])
        &&(ack->Value.CJT188.Meter.Addr.ID[2] = Indata->Meter.Addr.ID[2])
        &&(ack->Value.CJT188.Meter.Addr.ID[3] = Indata->Meter.Addr.ID[3])
        &&(ack->Value.CJT188.Meter.Addr.ID[4] = Indata->Meter.Addr.ID[4]))
     {
      break; 
     }  
    }
    else
    {err = Nodata ;break;}
   }
  }
  else
  {err= DataFileLost;} 
 }
 
 f_close (&File);
 if(NO_ERR == err)
 {ack->ReplyFlag = 0x00; } //0x00:������
 else
 {ack->ReplyFlag = 0xCF; }  //0xCF:δ�ҵ��� */
 return err; 
}
/*********************************************************************************
 Function:      //
 Description:   //�滻���
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName AlterMeterWcmd_Func(JD_AlterMeterWcmd_askParam_Stuct* Indata, void* outData)
{
 SystemErrName  err = NO_ERR; 
 FRESULT rse;
 UINT number;
 JD_AlterMeterWcmd_ackParam_Stuct* ack;
 JD_Meter_Stuct Meter;
 
 ack = outData;
 rse = f_open(&File,JD_MeterConfigFile_PathName, FA_READ | FA_WRITE);
 if(FR_OK == rse)
 {
  f_lseek (&File, 0);
  while( 1 )
  {
   rse = f_read (&File, &(Meter),sizeof(JD_Meter_Stuct), &number);
   if((FR_OK == rse)&&(sizeof(JD_Meter_Stuct) == number))
   {
    if((Meter.Addr.FactoryCode.Word = Indata->OldMeter.Addr.FactoryCode.Word)
        &&(Meter.Addr.ID[0] = Indata->OldMeter.Addr.ID[0])
        &&(Meter.Addr.ID[1] = Indata->OldMeter.Addr.ID[1])
        &&(Meter.Addr.ID[2] = Indata->OldMeter.Addr.ID[2])
        &&(Meter.Addr.ID[3] = Indata->OldMeter.Addr.ID[3])
        &&(Meter.Addr.ID[4] = Indata->OldMeter.Addr.ID[4]))
     {
       rse = f_lseek (&File, File.fptr - sizeof(JD_Meter_Stuct)); 
       if(FR_OK == rse)
       {
         rse = f_write (&File, &Indata->NewMeter, sizeof(JD_Meter_Stuct), &number);
         if(FR_OK == rse)
         {ack->ReplyFlag = 0x00;}  //�滻�ɹ�
         else
         {}
       }
       
      break; 
     }  
   }
   else
   {
     ack->ReplyFlag = 0x01;  //: δ��ѯ�����
     err= Nodata;
   }  
  }
 }
 else
 {
   ack->ReplyFlag = 0x01;  //: δ��ѯ�����
   err= DataFileLost;
 } 
 f_close (&File);
 return err; 
}
/*********************************************************************************
 Function:      //
 Description:   //ɾ�����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName DeleteMeterWcmd_Func(JD_DeleteMeterWcmd_askParam_Stuct* Indata, void* outData)
{
 SystemErrName  err = NO_ERR;
 unsigned char itemp; 
 FRESULT rse;
 UINT number;
 JD_DeleteMeterWcmd_ackParam_Stuct* ack;
 JD_Meter_Stuct Meter;
 
 ack = outData;
 itemp = 0;
 while (itemp < Indata->TotalNumber)
 {ack->DeleteResult[itemp++] = 1; }
 
 rse = f_open(&File,JD_MeterConfigFile_PathName, FA_READ | FA_WRITE);
 if(FR_OK == rse)
 {
  f_lseek (&File, 0); 
  while( 1 )
  {
    rse = f_read (&File, &(Meter),sizeof(JD_Meter_Stuct), &number);
    if((FR_OK == rse)&&(sizeof(JD_Meter_Stuct) == number))
    {
     itemp = 0;
     while (itemp < Indata->TotalNumber)
     {
      if((Meter.Addr.FactoryCode.Word = Indata->Meter[itemp].Addr.FactoryCode.Word)
         &&(Meter.Addr.ID[0] = Indata->Meter[itemp].Addr.ID[0])
         &&(Meter.Addr.ID[1] = Indata->Meter[itemp].Addr.ID[1])
         &&(Meter.Addr.ID[2] = Indata->Meter[itemp].Addr.ID[2])
         &&(Meter.Addr.ID[3] = Indata->Meter[itemp].Addr.ID[3])
         &&(Meter.Addr.ID[4] = Indata->Meter[itemp].Addr.ID[4]))
      {
        Meter.CR1.Byte = 0xff;
        rse = f_lseek (&File, File.fptr - sizeof(JD_Meter_Stuct)); 
        if(FR_OK == rse)
        {
         rse = f_write (&File, &Meter, sizeof(JD_Meter_Stuct), &number);
         if(FR_OK == rse)
         {ack->DeleteResult[itemp] = 0;}
         break;
        } 
      }
      itemp ++;
     }
    }
    else
    {break;}
  }
 }
 f_close (&File);
 ack->TotalNumber = Indata->TotalNumber;
 ack->ReplyFlag = 0x00;
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
SystemErrName sendJingDa_WLMQSY ( CommPortNumber_Type Port)
{
  static unsigned char buff[11];
  
  buff[0] = 'W';
  buff[1] = 'L';
  buff[2] = 'M';
  buff[3] = 'Q';
  buff[4] = 'S';
  buff[5] = 'Y';
  
  buff[6] = LocalAddr[0];
  buff[7] = LocalAddr[1];
  buff[8] = LocalAddr[2];
  buff[9] = LocalAddr[3];
  buff[10] = LocalAddr[4];
  
  return MXPC_Send_AutoRetrans ( Port,             //Ŀ�Ķ˿�
                                buff,              //���ݻ�����
                                11,                //���ݳ���
                                0,                 //�ش�����
                                0,                 //�ش�������
                                TimerTB_100MS,     //�ش�����
                                P_NULL,            //������ɻص�
                                P_NULL);           //�ط���ɻص�   
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
SystemErrName sendJingDaCmd ( CommPortNumber_Type Port,
                               unsigned char Code,
                               unsigned short ParamLength,
                               void *Param)
{
  SystemErrName err;
  JingDa_OutPutBuff_Struct *frame;
  frame = getJingDaSendBuff( );
  if(P_NULL ==frame)
  {return Nothing;}
  
  
  frame->Frame.Datas.Head.Code = Code;
  frame->Frame.Datas.Head.DataLength.Word = ParamLength;
    
  CopyBuff (frame->Frame.Datas.Param.Data,Param,ParamLength);
  
  err = JingDa_Send(Port,&(frame->Frame));
  
  if(NO_ERR != err)
  {freeJingDaSendBuff(frame);}
  
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
static void Send_OK(CommPortNumber_Type port, void *pbuff)
{
  freeJingDaSendBuff(pbuff);
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
SystemErrName JingDa_Send(CommPortNumber_Type Port,  JingDa_Frame_Struct* farame)
{
 unsigned char  sum;
 unsigned short itemp;
 unsigned long itemp2;  
 unsigned char *pin;
 
 //����У���
 itemp = 0;
 sum = 0;
 itemp2 = sizeof(JingDa_FrameHead_Struct) + farame->Datas.Head.DataLength.Word;
 pin = &farame->Datas.Head.StartCharacter; 
 while(itemp < itemp2)
 {
  sum += *pin;
  pin ++;
  itemp ++;
 }
 //���֡β
 ((JingDa_FrameStern_Struct*)pin)->CS = sum;
 ((JingDa_FrameStern_Struct*)pin)->Pause = JINGDA_Pause;

 return MXPC_Send_AutoRetrans ( Port,      //Ŀ�Ķ˿�
                                farame,           //���ݻ�����
                                sizeof(JingDa_Preamble) 
                                + sizeof(JingDa_FrameHead_Struct)
                                + sizeof(JingDa_FrameStern_Struct)
                                + farame->Datas.Head.DataLength.Word,     //���ݳ���
                                0,                //�ش�����
                                0,                //�ش�������
                                TimerTB_100MS,    //�ش�����
                                Send_OK,          //������ɻص�
                                P_NULL);          //�ط���ɻص�
  
}
/******************************************END********************************************************/

