/*********************************************************************************
//�����������豸����
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "..\inc\ReadEquipData.h"
#include "..\..\Projects\inc\SystemDefine.h"
#include "..\inc\EquipMgmt.h"
#include "..\..\LIB\FAT\ff.h"
#include "..\inc\Appcon_API.h"
/*********************************************************************************
����������
*********************************************************************************/

/*********************************************************************************
��������������
*********************************************************************************/
 CmdCode_KindgDaSPCP   CmdCodePV111; 
/*********************************************************************************
�ⲿ����������
*********************************************************************************/

/*********************************************************************************
˽�б���������
*********************************************************************************/
static unsigned char  RepeatersId;
static unsigned char  ChannelNumber;
static unsigned char  NodeNumberPV;  
static unsigned char  RepeatersNodes[4];
static CmdCode_KindgDaSPCP   CmdCodePV; 

static NodeValue_RecordType NodeValuelist[NodeValuelist_Length];

static FIL    File;
/*********************************************************************************
���Ա���������
*********************************************************************************/
unsigned char addrr11[3];
unsigned long cnt11 = 0;
/*********************************************************************************
�ڲ�����������
*********************************************************************************/
static void ReadValueAll_KindgDa_ErrFunc(void *p_tmr, void *p_arg);
//static void DayChange_KindgDa (void);
//static void SendMeasureValue_ToKingDaServer(KingDaServer_PackageStruct* SendBuff, void* AckData);
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
unsigned short GetTotalNum_Repeaters(void)
{
  unsigned char itemp;
  itemp = Concentrator.channel[0].max - Concentrator.channel[0].min;
  itemp ++;
  
  itemp = Concentrator.channel[1].max - Concentrator.channel[1].min;
  itemp ++;
  
  itemp = Concentrator.channel[2].max - Concentrator.channel[2].min;
  itemp ++;
  
  itemp = Concentrator.channel[3].max - Concentrator.channel[3].min;
  itemp ++;
 return itemp; 
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
unsigned char GetNextID_Repeaters(unsigned char id )
{
  unsigned char itemp = 0;
  unsigned char next;
  next = id;
  if(( 0 == Concentrator.channel[0].max)
     &&(0 == Concentrator.channel[1].max)
       &&(0 == Concentrator.channel[2].max))
  {return 0;}
    
  while (itemp < 4)
  {
   if(id < Concentrator.channel[itemp].min)
   {
     next = Concentrator.channel[itemp].min;
     break;
   }
   if((id >= Concentrator.channel[itemp].min)
      &&(id < Concentrator.channel[itemp].max))
   {
     next = id +1 ;
     break;
   } 
   itemp ++;
  }
  return next;
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
SystemErrName write_NodeValueRecord_KindgDa (unsigned char* adrr,
                                             unsigned long value,
                                             unsigned char adrrWidth)
{
 unsigned short  itemp =0;
 unsigned char   w =0;
 TimerData_Struct timer;
 get_SystemTimer (&timer); 
 do
 {
   if(0 ==NodeValuelist[itemp].CSR.bit.FL)
   {
     
     while (w < adrrWidth)
     {
      NodeValuelist[itemp].datas.KingDa.NodeId[w] = adrr[w];
      w++;
     }
     NodeValuelist[itemp].datas.KingDa.NodeValue = value;

     NodeValuelist[itemp].datas.KingDa.Timer.second = timer.Second;
     NodeValuelist[itemp].datas.KingDa.Timer.minute = timer.Minute;
     NodeValuelist[itemp].datas.KingDa.Timer.hour = timer.Hour;
     NodeValuelist[itemp].CSR.bit.FL = 1;
     NodeValuelist[itemp].CSR.bit.news1 = 0x01;
     NodeValuelist[itemp].CSR.bit.news2 = 0x01;
     NodeValuelist[itemp].CSR.bit.Type = KingDaRecord_Type;
     return NO_ERR;
   }
   else
   {itemp ++ ;}
 }while (itemp < NodeValuelist_Length);
 return Nothing;
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
SystemErrName write_NodeValueRecord_Appcon (APPCON_RDNODE_AckData_Struct* Data)
{
 static unsigned short  write_NodeValueRecordcnt  = 0;
 unsigned short  itemp = 0;
 unsigned long   itemp32 ;
 NodeValue_RecordType* Record;
 
 itemp32 = APPCON_AsciiHexToDigit(Data->id,8,8);
 do
 {
   if((1 == NodeValuelist[itemp].CSR.bit.FL)
      &&(AppconDaRecord_Type == NodeValuelist[itemp].CSR.bit.Type))
   {
    if(itemp32 == NodeValuelist[itemp].datas.Appcon.id)
    {
      Record = &NodeValuelist[itemp];
      break;
    }
   }
   itemp ++ ;
 }while (itemp < NodeValuelist_Length);
 if(itemp >= NodeValuelist_Length)
 {
   itemp = 0;
   do
   {
     if(0 == NodeValuelist[itemp].CSR.bit.FL)
     {
       Record =  &NodeValuelist[itemp];
       break;
     }
     itemp ++ ;
   }while (itemp < NodeValuelist_Length);
   if(itemp >= NodeValuelist_Length)
   {return Nothing;}
 }
 Record->datas.Appcon.id = itemp32;
 Record->datas.Appcon.value = APPCON_AsciiHexToDigit(Data->value,12,8);
 Record->datas.Appcon.vbat = APPCON_AsciiHexToDigit(Data->bat,2,2);
 Record->datas.Appcon.link = Data->link;
 Record->datas.Appcon.route = Data->route;
 Record->CSR.bit.Type = AppconDaRecord_Type;
 NodeValuelist[itemp].CSR.bit.FL = 0x01;
 NodeValuelist[itemp].CSR.bit.news1 = 0x01;
 NodeValuelist[itemp].CSR.bit.news2 = 0x01;
 write_NodeValueRecordcnt ++;
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
/*void rset_NodeValueRecord_Appcon ( void )
{
  unsigned short  itemp;
  itemp = 0;
  while( itemp < NodeValuelist_Length) 
  {
    if(0x01 == NodeValuelist[itemp].CSR.bit.FL)
     {
       NodeValuelist[itemp].CSR.bit.news1 = 0x01;
       NodeValuelist[itemp].CSR.bit.news2 = 0x01;
     }
     itemp ++;
   }
}*/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName WriteToRom_NodeValueRecord_KindgDa ( void )
{
 unsigned short  itemp1;
 //FRESULT rse;  
 //UINT itemp;
 //TimerData_Struct timer1;
 //TimerData_Struct timer2;
 
 Concentrator.SR1.Bit.NewData_flg = 0;
 /*
 get_SystemTimer (&timer1);
 rse = f_open(&File,"FLASH:/Today", FA_WRITE | FA_READ);  
 if(FR_OK != rse)  
 {
  f_close (&File);
  rse = f_open(&File,"FLASH:/Today", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);  
  rse = f_lseek (&File , 0);
  rse = f_write (&File, &timer1, sizeof(TimerData_Struct), &itemp);
 }
 else
 {
  rse = f_read (&File, &timer2, sizeof(TimerData_Struct), &itemp); 
  if(itemp < sizeof(TimerData_Struct))
  {
   rse = f_lseek (&File , 0);
   rse = f_write (&File, &timer1, sizeof(TimerData_Struct), &itemp); 
  }
  else if((timer1.Day != timer2.Day)||(timer1.Month != timer2.Month))
  {
   f_close (&File);
   DayChange_KindgDa( );
   rse = f_open(&File,"FLASH:/Today",FA_WRITE | FA_READ);
  } 
 }
 rse = f_lseek (&File , File.fsize);
 */
 
 itemp1 = 0;
 while (itemp1 < NodeValuelist_Length)
 {
   if(1 ==NodeValuelist[itemp1].CSR.bit.FL)
   {  
    //rse = f_write (&File, &NodeValuelist[itemp1], sizeof(NodeValue_RecordType), &itemp); 
    //if(FR_OK != rse)
    //{return Unsuccessful;}  
    NodeValuelist[itemp1].CSR.bit.FL = 0;  
   }
   itemp1 ++;
 }
 //rse = f_close (&File);
 //if(FR_OK != rse)
 //{return Unsuccessful;}
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
SystemErrName read_TodayNodeValueRecord_KindgDa (unsigned char* adrr, NodeValue_RecordType *record)
{
 unsigned char  flg = 0;
 FRESULT rse;  
 UINT itemp;
 rse = f_open(&File,"FLASH:/Today", FA_READ);
 if(FR_OK != rse)
 {return DataFileLost;}
 rse = f_lseek (&File ,sizeof(NodeValue_RecordDescType));
 do
 {
   rse = f_read (&File, record, sizeof(NodeValue_RecordType), &itemp);
   if(FR_OK == rse)
   {
     if (itemp >=  sizeof(NodeValue_RecordType))
     {
       if((record->datas.KingDa.NodeId[0] == adrr[0])
          &&(record->datas.KingDa.NodeId[1] == adrr[1])
          &&(record->datas.KingDa.NodeId[2] == adrr[2]))
       {
         flg = 1;
       }
     }
   }
 }while(itemp >=  sizeof(NodeValue_RecordType));
 
 f_close (&File);

 if(1 == flg)
 {return NO_ERR;}
 else
 {return NoData;}
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
/*
void DayChange_KindgDa (void)
{
 FRESULT rse; 
 UINT itemp;
 unsigned char old_day[] = {'F','L','A','S','H',':','/','D', 0, 0, 'M', 0, 0, 0, 0, 0 }; 
                           //0   1   2   3   4   5   6   7   8  9   10  11 12
 TimerData_Struct timer;
 
 rse = f_open(&File,"FLASH:/Today", FA_READ);
 if(FR_OK != rse)
 {return ;}
 rse = f_read (&File, &timer, sizeof(TimerData_Struct), &itemp); 
 f_close (&File);
 
 if((FR_OK == rse)&&(itemp >= sizeof(TimerData_Struct)))
 {
   
  itemp = timer.Month % 10;
  old_day[12] = itemp + '0';
  timer.Month /= 10 ;
  itemp = timer.Month % 10;
  old_day[11] = itemp + '0';
   
  itemp = timer.Day % 10;
  old_day[9] = itemp + '0';
  timer.Day /= 10 ;
  itemp = timer.Day % 10;
  old_day[8] = itemp + '0';
 }
 
 f_unlink ((const TCHAR*) old_day);
 f_rename ("FLASH:/Today",(const TCHAR*) old_day);
 
 rse = f_open(&File,"FLASH:/Today", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);  //FA_OPEN_ALWAYS

 rse = f_lseek (&File , 0);
 get_SystemTimer (&timer);
 
 rse = f_write (&File, &timer, sizeof(TimerData_Struct), &itemp);
 f_close (&File);
}*/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        // 
*********************************************************************************/
SystemErrName ReadValueAll_Appcon_Func ( )
{
  SystemErrName  err;
  err = Appcon_Reading_Func(VOID_Format,P_NULL,P_NULL);
  if(err != NO_ERR)
  {
   if(0 != ServerState[0].SR.Bit.CN)
   {
     Concentrator.CSR.Bit.ReadAll = 0;
     LaunchSendMeasureValue_ToKingDaServer1( );
     Concentrator.SR1.Bit.NewData_flg = 0;
   }
   else
   {
     Concentrator.SR1.Bit.NewData_flg = 1; 
     if(0 != ServerState[1].SR.Bit.CN)
     {LaunchSendMeasureValue_ToKingDaServer2( );
     }
   }
  }
 return err;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        // �������� ��1,��ȡ��Ӧ�м����ĸ�ͨ��ˮ������
                              2,���м����Ķ�Ӧͨ��
                              3�����ζ���
*********************************************************************************/
void ReadValueAll_KindgDa_AckFunc (PProtocolSendBuff_Struct* SendBuff, void* AckData)
{
 //static unsigned char  state  = 0;
 SystemErrName err = NO_ERR;
 unsigned char RepeatersNextId;
 unsigned short rtime = 20; 
 unsigned char NoderAddr[3]; 
 unsigned short itemp ;
 KindgDaSPCP_PackageDataStruct*  data;
 SofTimer_Struct* p_tmr;

 if(Concentrator.CSR.Bit.Debug)
 {
  rtime = 6000;  
  if(P_NULL == SendBuff)     //�״β���
  { 
   if(0 == Concentrator.CSR.Bit.ReadAll)
   {Concentrator.CSR.Bit.ReadAll = 0x01;}
   else
   {return;}
   WriteToRom_NodeValueRecord_KindgDa ( );
   RepeatersId = 1;
   ChannelNumber = 1;
   NodeNumberPV = 0;
   CmdCodePV = ReadChannelNodeNumber_Repeaters;
  }
  else
  {
   data =  AckData;
   CmdCodePV =  SendBuff->buff.Data.TypeA.head.Cmd;
  }
  err = Nothing;
 }
 else
 { 
  if(P_NULL == SendBuff)     //�״β���
  {
   if(0 == Concentrator.CSR.Bit.ReadAll)
   {Concentrator.CSR.Bit.ReadAll = 0x01;}
   else
   {return;}
   WriteToRom_NodeValueRecord_KindgDa ( );
   RepeatersId = Concentrator.channel[0].min;
   ChannelNumber = 1;
   NodeNumberPV = 0;
   CmdCodePV = ReadChannelNodeNumber_Repeaters;
   
   RepeatersNextId =  GetNextID_Repeaters(0);
   if(0 != RepeatersNextId)
   {err = ReadRepeatersNodeNumber_KindDa ( RepeatersNextId ,Null_Port, ReadValueAll_KindgDa_AckFunc);}
   else
   {
     if(0x01 == Concentrator.CSR.Bit.Appcon_EN)
     {ReadValueAll_Appcon_Func( );}
   }
  }
  else
  {
   data =  AckData;
   CmdCodePV =  SendBuff->buff.Data.TypeA.head.Cmd;
   switch (CmdCodePV)
   {
    case OPen_Repeaters:     //��ͨ����Ӧ
     {
      if(P_NULL != AckData)  //��ȷ��Ӧ��ͨ��
      {
       //��ȡͨ��
       NodeNumberPV = 1;
       NoderAddr[0] = RepeatersId;
       NoderAddr[1] = ChannelNumber;
       NoderAddr[2] = NodeNumberPV;
       CmdCodePV = ReadValue;
       err = ReadNodeValue_KindDa( NoderAddr, AutomatchPort, Null_Port, ReadValueAll_KindgDa_AckFunc);
      }
      else //�޻�Ӧ
      {
       while (++ ChannelNumber <= 4)
       {
        if(RepeatersNodes[ChannelNumber]> 0){break;}
       }
       if(ChannelNumber <= 4 )//�Ƿ������һͨ��
       {
         CmdCodePV = OPen_Repeaters;
         err = OpenNETChannel_KindDa(RepeatersId, ChannelNumber, Null_Port, ReadValueAll_KindgDa_AckFunc); 
       }
       else
       {
         CmdCodePV = Close_Repeaters;
         err = CloseRepeatersId_KindDa(RepeatersId, Null_Port, ReadValueAll_KindgDa_AckFunc);
       }  
      }
      break;
     }
    case  Close_Repeaters:     //�ر�ͨ����Ӧ
     {
      if(P_NULL != AckData)    //�м���ͨ���ر���ȷ
      {
        RepeatersNextId = GetNextID_Repeaters(RepeatersId);
        if(RepeatersNextId <= RepeatersId)
        {
         //ɨ�����
         Concentrator.CSR.Bit.ReadAll = 0x00;
         PowerOut_Cmd (Rs485_ALL, DISABLE);  
         PowerConfig_Rs485 (Null_Port);
         if(0x01 == Concentrator.CSR.Bit.Appcon_EN)
         {ReadValueAll_Appcon_Func( );}
         else
         { 
           if(0 != ServerState[0].SR.Bit.CN)
           {
             LaunchSendMeasureValue_ToKingDaServer1( );
             Concentrator.SR1.Bit.NewData_flg = 0;
           }
           else
           {
            Concentrator.SR1.Bit.NewData_flg = 1; 
            if(0 != ServerState[1].SR.Bit.CN)
            {
              LaunchSendMeasureValue_ToKingDaServer2( );
            }
           }
         }
        }
        else  //�������м���δɨ�����ȡ��һ���м�����Ϣ
        { 
          CmdCodePV = ReadChannelNodeNumber_Repeaters;
          RepeatersId = RepeatersNextId;
          err = ReadRepeatersNodeNumber_KindDa(RepeatersId,  Null_Port,  ReadValueAll_KindgDa_AckFunc);
         }
      }
      else   //�м���ͨ���رմ���
      {
       RepeatersNextId = GetNextID_Repeaters(RepeatersId);
       if(RepeatersNextId <= RepeatersId)
       {
         //ɨ�����
         Concentrator.CSR.Bit.ReadAll = 0x00;
         PowerOut_Cmd (Rs485_ALL, DISABLE);  
         PowerConfig_Rs485 (Null_Port);
         if(0x01 == Concentrator.CSR.Bit.Appcon_EN)
         {ReadValueAll_Appcon_Func( );}
         else
         { 
           if(0 != ServerState[0].SR.Bit.CN)
           {
             LaunchSendMeasureValue_ToKingDaServer1( );
             Concentrator.SR1.Bit.NewData_flg = 0;
           }
           else
           {
            Concentrator.SR1.Bit.NewData_flg = 1; 
            if(0 != ServerState[1].SR.Bit.CN)
            {
              LaunchSendMeasureValue_ToKingDaServer2( );
            }
           }
         }
       }
       else   //ɨ��δ����
       {
         RepeatersId = RepeatersNextId;
         if((RepeatersId == Concentrator.channel[0].min)    //����ͨ����Ȼ�ر�ǰһ���м���
            ||(RepeatersId == Concentrator.channel[1].min)
            ||(RepeatersId == Concentrator.channel[2].min))
         {
          CmdCodePV = ReadChannelNodeNumber_Repeaters;
          err = ReadRepeatersNodeNumber_KindDa( RepeatersId,  Null_Port,  ReadValueAll_KindgDa_AckFunc);
         }
         else
         {
          if(Concentrator.channel[0].max < Concentrator.channel[1].max) 
          {itemp = Concentrator.channel[1].max;}
          else
          {itemp = Concentrator.channel[0].max;}
          if(itemp < Concentrator.channel[2].max)
          {itemp = Concentrator.channel[2].max;}
          if(RepeatersId < itemp)  //����ͨ�� ��Ҫ����������
          {
           //�ֶ�ǿ�ƹر��м���ͨ��
           PowerOut_Cmd (Rs485_ALL, DISABLE);  
           CmdCodePV = ReadChannelNodeNumber_Repeaters;
           rtime = 300;
           err = Nothing; 
          }
          else
          {
           CmdCodePV = ReadChannelNodeNumber_Repeaters; 
           err = ReadRepeatersNodeNumber_KindDa( RepeatersId, Null_Port, ReadValueAll_KindgDa_AckFunc);
          }
         }
       }
      }
      break;  
    }
   case ReadChannelNodeNumber_Repeaters:     //��ȡ�м���N�¸���ͨ��ˮ������
     {
      if(P_NULL != AckData)   //��ȡ�ɹ�
      {
       //���ؽ������Ҫ��ֵ ��¼����ֵ
       RepeatersNodes[0] = data->TypeA.pram.ReadChannelNodeNumber_Repeaters_ack.NodeNumber_channel[0];
       RepeatersNodes[1] = data->TypeA.pram.ReadChannelNodeNumber_Repeaters_ack.NodeNumber_channel[1];
       RepeatersNodes[2] = data->TypeA.pram.ReadChannelNodeNumber_Repeaters_ack.NodeNumber_channel[2];
       RepeatersNodes[3] = data->TypeA.pram.ReadChannelNodeNumber_Repeaters_ack.NodeNumber_channel[3];  
       //���Ҹ��м����е�һ���ڵ�������Ϊ���ͨ��
       ChannelNumber = 0;
       while ( 0 == RepeatersNodes[ChannelNumber])
       {if(++ChannelNumber > 3){break;}}
       if(++ ChannelNumber <= 4)
       { 
        CmdCodePV = OPen_Repeaters; 
        err = OpenNETChannel_KindDa( RepeatersId, ChannelNumber, Null_Port, ReadValueAll_KindgDa_AckFunc); 
       }
       else //��ǰ�м������ҵĽڵ�����Ϊ�� �رյ�ǰ�м���
       {
         CmdCodePV = Close_Repeaters;
         err = CloseRepeatersId_KindDa ( RepeatersId, Null_Port, ReadValueAll_KindgDa_AckFunc );
       } 
      }
      else  //δ��ȡ����ǰ�м����ڵ���Ϣ �رյ�ǰ�м���
      {
        CmdCodePV = Close_Repeaters;
        err = CloseRepeatersId_KindDa ( RepeatersId, Null_Port, ReadValueAll_KindgDa_AckFunc );
      }
     break;
    }
   case ReadValue:     //��ȡ�ڵ�����
     {
      if(P_NULL != AckData)   //��ȡ�ɹ�
      {
        itemp = data->TypeA.pram.ReadValue_ack.value & 0x00ff;
        itemp <<= 8;
        itemp |= data->TypeA.pram.ReadValue_ack.value >> 8;
        write_NodeValueRecord_KindgDa(data->TypeA.pram.ReadValue_ack.addr, itemp, 3);
      }
      else  //��ȡʧ��
      {
        NoderAddr[0] = RepeatersId;
        NoderAddr[1] = ChannelNumber;
        NoderAddr[2] = NodeNumberPV; 
        write_NodeValueRecord_KindgDa(NoderAddr, 0xffffffff, 3);
      }  //��ȡʧ�� 
      if(++NodeNumberPV > RepeatersNodes[ChannelNumber-1])     //��ǰ�ڵ��ǵ�ǰͨ�������һ���ڵ� �رյ�ǰͨ��
      {
       ChannelNumber ++;  
       while (ChannelNumber <= 4)
       {
         if(RepeatersNodes[ChannelNumber-1]> 0)
         {break;}
         ChannelNumber++;
       }
       if (ChannelNumber > 4)  //��ǰ�м�������ͨ���Զ���
       {
         CmdCodePV = Close_Repeaters;
         err = CloseRepeatersId_KindDa ( RepeatersId, Null_Port, ReadValueAll_KindgDa_AckFunc );
       }
       else
       {
        CmdCodePV = OPen_Repeaters; 
        err = OpenNETChannel_KindDa( RepeatersId,  ChannelNumber, Null_Port, ReadValueAll_KindgDa_AckFunc);
       }
      }
      else //�������һ���ڵ�
      {
       NoderAddr[0] = RepeatersId;
       NoderAddr[1] = ChannelNumber;
       NoderAddr[2] = NodeNumberPV; 
       err =  ReadNodeValue_KindDa( NoderAddr, AutomatchPort, Null_Port, ReadValueAll_KindgDa_AckFunc);  
      }
      break;
    } 
   } 
  }
 }
 if(Nothing ==  err)
 {
  p_tmr = SofTimerCreate ( rtime,                          //����
                           TimerTB_100MS,                  //ʱ��
                           SofTimer_PERIODIC,              //ģʽ
                           ReadValueAll_KindgDa_ErrFunc,   //�ص�
                           P_NULL);                      //�ص�����
  if(P_NULL != p_tmr );
  { SofTimerStart(p_tmr);}
 }
 else
 {
   CmdCodePV111 = CmdCodePV; 
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
void ReadValueAll_KindgDa_ErrFunc(void *p_tmr, void *p_arg)
{
  SystemErrName err = NO_ERR;
  unsigned char NoderAddr[3];
  if(Concentrator.CSR.Bit.Debug)
  {return;}
  else
  {
   SofTimer_ChangePeriod (p_tmr,       //���ݵĶ�ʱ��
                          20,   //�µ�����
                          0);  
  }
  switch (CmdCodePV)
  {
   case  OPen_Repeaters:     //��ͨ����Ӧ
    { 
     err = OpenNETChannel_KindDa( RepeatersId, ChannelNumber, Null_Port, ReadValueAll_KindgDa_AckFunc);
     break;
     }
   case  Close_Repeaters:     //�ر�ͨ����Ӧ
    {  
     err = CloseRepeatersId_KindDa ( RepeatersId, Null_Port, ReadValueAll_KindgDa_AckFunc );
     break;  
    }
  case ReadChannelNodeNumber_Repeaters:     //��ȡ�м���N�¸���ͨ��ˮ������
    {
     err = ReadRepeatersNodeNumber_KindDa( RepeatersId, Null_Port, ReadValueAll_KindgDa_AckFunc);
     break;
    }
  case ReadValue:     //��ȡ�ڵ�����
    {
     NoderAddr[0] = RepeatersId;
     NoderAddr[1] = ChannelNumber;
     NoderAddr[2] = NodeNumberPV;  
     err = ReadNodeValue_KindDa( NoderAddr,  AutomatchPort,  Null_Port,ReadValueAll_KindgDa_AckFunc);
     break;
    } 
  } 
if(NO_ERR ==  err)
  {SofTimerDel ((SofTimer_Struct**)&p_tmr); } 
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
SystemErrName ReadNodeVlue(EquipRecord_struct* equip, KindgDaSPCPCmdCode_AckFunc AckFunc)
{
  SystemErrName err= NO_ERR;
  
  if(P_NULL == equip)
  {return IllegalParam;}
  
  switch(equip->Spcp)
  {
   case KingDa :
     {
       err = ReadNodeValue_KindDa(equip->Data.KingDaNode.ADDR,  AutomatchPort,  Null_Port, AckFunc);
       break;
     }
  }
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
unsigned char CheckChannel_state(unsigned char channel)
{
  CommPortNumber_Type PortNumber;
  PortNumber =  getPortForAddr (channel);
  
  if(PortNumber == get_RS458ChannelNumber ( ))
  { return 1;}
  else
  { return 0;}
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
void MeasureValue_ToKingDaServer(Repeaters_DescrType* repeaters, unsigned char *pbuff)
{
  unsigned char  ch;
  unsigned char  p;
  unsigned short  itemp;
  unsigned short  data[100] ;
  ch = 1;
  while (ch < 5)
  {
   itemp = 0;
   while (itemp < 100)
   {data[itemp ++] = 0xffff;}
   itemp = 0;
   p = ch-1;
   repeaters->CH_NO[p] = 0;
   while( itemp < NodeValuelist_Length) 
   {
     if((1 == NodeValuelist[itemp].CSR.bit.FL)
        &&(KingDaRecord_Type == NodeValuelist[itemp].CSR.bit.Type))  ///�޸�
     {
       if((NodeValuelist[itemp].datas.KingDa.NodeId[0] == repeaters->ID)
          &&(NodeValuelist[itemp].datas.KingDa.NodeId[1] == ch)
          &&(NodeValuelist[itemp].datas.KingDa.NodeId[2] > 0))
       {
         if(NodeValuelist[itemp].datas.KingDa.NodeId[2])
         data[NodeValuelist[itemp].datas.KingDa.NodeId[2] - 1] = NodeValuelist[itemp].datas.KingDa.NodeValue;
         if(NodeValuelist[itemp].datas.KingDa.NodeId[2] > repeaters->CH_NO[p])
         {repeaters->CH_NO[p] = NodeValuelist[itemp].datas.KingDa.NodeId[2];}
       }  
     }
     itemp ++;
   }
  itemp = 0;
  while( itemp < repeaters->CH_NO[p])
  {
   *pbuff = data[itemp]>>8;
   pbuff ++; 
   *pbuff = data[itemp] & 0x00ff;
   pbuff ++; 
   itemp ++;
  }
  ch ++;  
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
void AppconMeasureValue_ToKingDaServer1(unsigned char *Number, void* pbuff)
{
  unsigned short  itemp;
  unsigned char   cnt ;
  NodeDatas_APPCON_Struct* NodeDatas; 
  itemp = 0;
  cnt = 0;
  NodeDatas = (NodeDatas_APPCON_Struct*)pbuff;
  while( itemp < NodeValuelist_Length) 
  {
    if((0x01 == NodeValuelist[itemp].CSR.bit.FL)
       &&(0x01 == NodeValuelist[itemp].CSR.bit.news1)
       &&(AppconDaRecord_Type == NodeValuelist[itemp].CSR.bit.Type))
     {
       if(0 == (*Number) --)
       {break;}
       cnt ++; 
       NodeDatas->id[0] = NodeValuelist[itemp].datas.Appcon.id >>24; 
       NodeDatas->id[1] = NodeValuelist[itemp].datas.Appcon.id >>16;
       NodeDatas->id[2] = NodeValuelist[itemp].datas.Appcon.id >>8;
       NodeDatas->id[3] = NodeValuelist[itemp].datas.Appcon.id ;
         
       NodeDatas->value[0] = NodeValuelist[itemp].datas.Appcon.value >>24;
       NodeDatas->value[1] = NodeValuelist[itemp].datas.Appcon.value >>16;
       NodeDatas->value[2] = NodeValuelist[itemp].datas.Appcon.value >>8;
       NodeDatas->value[3] = NodeValuelist[itemp].datas.Appcon.value;
         
       NodeDatas->vbat = NodeValuelist[itemp].datas.Appcon.vbat;
       NodeDatas->link = NodeValuelist[itemp].datas.Appcon.link;
       NodeDatas->route = NodeValuelist[itemp].datas.Appcon.route;
       NodeDatas ++;
       NodeValuelist[itemp].CSR.bit.news1 = 0x00;
     }
     itemp ++;
   }
  *Number = cnt; 
}
/*********************************************************************************
 Function:      //
 Description:   // �����س��������ϴ���������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void AppconMeasureValue_ToKingDaServer2(unsigned char *Number, void* pbuff)
{
  unsigned short  itemp;
  unsigned char   cnt ;
  NodeDatas_APPCON_Struct* NodeDatas; 
  itemp = 0;
  cnt = 0;
  NodeDatas = (NodeDatas_APPCON_Struct*)pbuff;
  while( itemp < NodeValuelist_Length) 
  {
    if((0x01 == NodeValuelist[itemp].CSR.bit.FL)
       &&(0x01 == NodeValuelist[itemp].CSR.bit.news2)
       &&(AppconDaRecord_Type == NodeValuelist[itemp].CSR.bit.Type))
     {
       if(0 == (*Number) --)
       {break;}
       cnt ++; 
       NodeDatas->id[0] = NodeValuelist[itemp].datas.Appcon.id >>24; 
       NodeDatas->id[1] = NodeValuelist[itemp].datas.Appcon.id >>16;
       NodeDatas->id[2] = NodeValuelist[itemp].datas.Appcon.id >>8;
       NodeDatas->id[3] = NodeValuelist[itemp].datas.Appcon.id ;
         
       NodeDatas->value[0] = NodeValuelist[itemp].datas.Appcon.value >>24;
       NodeDatas->value[1] = NodeValuelist[itemp].datas.Appcon.value >>16;
       NodeDatas->value[2] = NodeValuelist[itemp].datas.Appcon.value >>8;
       NodeDatas->value[3] = NodeValuelist[itemp].datas.Appcon.value;
         
       NodeDatas->vbat = NodeValuelist[itemp].datas.Appcon.vbat;
       NodeDatas->link = NodeValuelist[itemp].datas.Appcon.link;
       NodeDatas->route = NodeValuelist[itemp].datas.Appcon.route;
       NodeDatas ++;
       NodeValuelist[itemp].CSR.bit.news2 = 0x00;
     }
     itemp ++;
   }
  *Number = cnt; 
}
/*********************************************************************************
 Function:      //
 Description:   // �����س��������ϴ���������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void SendMeasureValue_ToKingDaServer1(KingDaServer_PackageStruct* SendBuff, void* AckData)
{ 
  static unsigned  char err_cnt = 0;
  static unsigned  char DataFlg = KingDaRecord_Type; 
  KingDaServer_PackageStruct *pbuff;
  unsigned char RepeatersNextId; 

 if(P_NULL == SendBuff)
 {
   err_cnt = 0;
   RepeatersNextId = GetNextID_Repeaters(0);
   if(0 != RepeatersNextId )
   {
     UpdatMeasureValue_ToKingDaServer(RepeatersNextId, ServerState[0].Port,SendMeasureValue_ToKingDaServer1); 
     DataFlg = KingDaRecord_Type;
   } 
   else
   {
    UpdatAppconMeasureValue_ToKingDaServer(1, SendMeasureValue_ToKingDaServer1);
    DataFlg = AppconDaRecord_Type;
   }
 }
 else
 {
  if(P_NULL !=  AckData)
  {
   pbuff = AckData; 
   if((Concentrator.id[0] != pbuff->pram.ReadALLMeasureValue_Receive.OK.Concentrator_ID[0])
      ||(Concentrator.id[1] != pbuff->pram.ReadALLMeasureValue_Receive.OK.Concentrator_ID[1])
        ||(Concentrator.id[2] != pbuff->pram.ReadALLMeasureValue_Receive.OK.Concentrator_ID[2])
          ||('O' != pbuff->pram.ReadALLMeasureValue_Receive.OK.DIO[0])
            ||('K' != pbuff->pram.ReadALLMeasureValue_Receive.OK.DIO[1]))
   {
    if(KingDaRecord_Type == DataFlg)
    {
      err_cnt ++;
      if(err_cnt < 3)
      {
        RepeatersNextId =  SendBuff->pram.ReadALLMeasureValue_Send.Repeaters_ID;
        UpdatMeasureValue_ToKingDaServer(RepeatersNextId, ServerState[0].Port,SendMeasureValue_ToKingDaServer1);
        DataFlg = KingDaRecord_Type;
        return ;
      }
      else if(AppconDaRecord_Type == DataFlg)
      {err_cnt = 0; }
      else
      {err_cnt = 0;}
    }
    else
    {err_cnt = 0;}
   }
  }
  if(KingDaRecord_Type == DataFlg)
  {
    RepeatersNextId = GetNextID_Repeaters(SendBuff->pram.ReadALLMeasureValue_Send.Repeaters_ID);
    if(RepeatersNextId > SendBuff->pram.ReadALLMeasureValue_Send.Repeaters_ID) 
    { 
     UpdatMeasureValue_ToKingDaServer(RepeatersNextId, ServerState[0].Port,SendMeasureValue_ToKingDaServer1);
     DataFlg = KingDaRecord_Type; 
    }
    else
    {
     UpdatAppconMeasureValue_ToKingDaServer(1, SendMeasureValue_ToKingDaServer1); 
     DataFlg = AppconDaRecord_Type;
    }
  }
  else
  {
    UpdatAppconMeasureValue_ToKingDaServer(1, SendMeasureValue_ToKingDaServer1); 
    DataFlg = AppconDaRecord_Type; 
  }
 }
}
/*********************************************************************************
 Function:      //
 Description:   // �����س��������ϴ���������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void SendMeasureValue_ToKingDaServer2(KingDaServer_PackageStruct* SendBuff, void* AckData)
{ 
  static unsigned  char DataFlg = KingDaRecord_Type;
  static unsigned  char err_cnt = 0;
  KingDaServer_PackageStruct *pbuff;
  unsigned char RepeatersNextId;
  
 if(P_NULL == SendBuff)
 {
   err_cnt = 0;
   RepeatersNextId = GetNextID_Repeaters(0);
   if(0 != RepeatersNextId )
   {
     UpdatMeasureValue_ToKingDaServer(RepeatersNextId, ServerState[1].Port, SendMeasureValue_ToKingDaServer2); 
     DataFlg = KingDaRecord_Type;
   } 
   else
   {
     UpdatAppconMeasureValue_ToKingDaServer(1, SendMeasureValue_ToKingDaServer2);
     DataFlg = AppconDaRecord_Type; 
   }
 }
 else
 {
  if(P_NULL !=  AckData)
  {
   pbuff = AckData; 
   if((Concentrator.id[0] != pbuff->pram.ReadALLMeasureValue_Receive.OK.Concentrator_ID[0])
      ||(Concentrator.id[1] != pbuff->pram.ReadALLMeasureValue_Receive.OK.Concentrator_ID[1])
        ||(Concentrator.id[2] != pbuff->pram.ReadALLMeasureValue_Receive.OK.Concentrator_ID[2])
          ||('O' != pbuff->pram.ReadALLMeasureValue_Receive.OK.DIO[0])
            ||('K' != pbuff->pram.ReadALLMeasureValue_Receive.OK.DIO[1]))
   {
     if(KingDaRecord_Type == DataFlg)
     {
       err_cnt ++;
       if(err_cnt < 3)
       {
         RepeatersNextId = SendBuff->pram.ReadALLMeasureValue_Send.Repeaters_ID;  
         UpdatMeasureValue_ToKingDaServer(RepeatersNextId, ServerState[1].Port, SendMeasureValue_ToKingDaServer2);
         DataFlg = KingDaRecord_Type;
         return ;
       }
       else if(AppconDaRecord_Type == DataFlg)
       {err_cnt = 0; }
       else
       {err_cnt = 0;}
     }
     else{err_cnt = 0;}
   }
  }
  if(KingDaRecord_Type == DataFlg)
  {
    RepeatersNextId = GetNextID_Repeaters(SendBuff->pram.ReadALLMeasureValue_Send.Repeaters_ID);
    if(RepeatersNextId > SendBuff->pram.ReadALLMeasureValue_Send.Repeaters_ID) 
    {
      UpdatMeasureValue_ToKingDaServer(RepeatersNextId, ServerState[1].Port,SendMeasureValue_ToKingDaServer2);
      DataFlg = KingDaRecord_Type;
    }
    else
    {
      UpdatAppconMeasureValue_ToKingDaServer(2, SendMeasureValue_ToKingDaServer2);
      DataFlg = AppconDaRecord_Type; 
    }
  }
  else
  {
   UpdatAppconMeasureValue_ToKingDaServer(2, SendMeasureValue_ToKingDaServer2);
   DataFlg = AppconDaRecord_Type;  
  }
 }
}
/*********************************************************************************
 Function:      //
 Description:   // �����س��������ϴ���������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void SendMeasureValue_ToKingDaServer1_ACK(KingDaServer_PackageStruct* SendBuff, void* AckData)
{ 
  static unsigned  char DataFlg = KingDaRecord_Type;
  static unsigned  char err_cnt = 0;
  SystemErrName    err;
  KingDaServer_PackageStruct *pbuff;
  unsigned char RepeatersNextId;
  
 if(P_NULL == SendBuff)
 {
   err_cnt = 0;
   RepeatersNextId = GetNextID_Repeaters(0);
   if(0 != RepeatersNextId )
   {
     ReadALLMeasureValue_ToKingDaServer_ACK(1, ServerState[0].Port,SendMeasureValue_ToKingDaServer1_ACK); 
     DataFlg = KingDaRecord_Type;
   } 
   else
   {
     if(0x01 == Concentrator.CSR.Bit.Appcon_EN)
     {
       err = ReadAllAppconMeasureValue_ToKingDaServer_ACK(ServerState[0].Port,SendMeasureValue_ToKingDaServer1_ACK);
       if(NO_ERR == err)
       {Concentrator.SR1.Bit.App_RdVal_end_01 = 0;}  
       else
       {Concentrator.SR1.Bit.App_RdVal_end_01 = 1;}  
       DataFlg = AppconDaRecord_Type; 
     }
   }
 }
 else
 {
  if(P_NULL !=  AckData)
  {
   pbuff = AckData; 
   if((Concentrator.id[0] != pbuff->pram.ReadALLMeasureValue_Receive.OK.Concentrator_ID[0])
      ||(Concentrator.id[1] != pbuff->pram.ReadALLMeasureValue_Receive.OK.Concentrator_ID[1])
        ||(Concentrator.id[2] != pbuff->pram.ReadALLMeasureValue_Receive.OK.Concentrator_ID[2])
          ||('O' != pbuff->pram.ReadALLMeasureValue_Receive.OK.DIO[0])
            ||('K' != pbuff->pram.ReadALLMeasureValue_Receive.OK.DIO[1]))
   {

    if(KingDaRecord_Type == DataFlg) 
    { 
      err_cnt ++;
      if(err_cnt < 3)
      {
        RepeatersNextId =  SendBuff->pram.ReadALLMeasureValue_Send.Repeaters_ID;
        ReadALLMeasureValue_ToKingDaServer_ACK(RepeatersNextId, ServerState[0].Port,SendMeasureValue_ToKingDaServer1_ACK);
        DataFlg = KingDaRecord_Type;
        return ;
      }
      else
      {err_cnt = 0;}
    }
    else if(AppconDaRecord_Type == DataFlg)
    {err_cnt = 0; }
    else
    {err_cnt = 0;}
   }
  }
  if(KingDaRecord_Type == DataFlg) 
  { 
    RepeatersNextId = GetNextID_Repeaters(SendBuff->pram.ReadALLMeasureValue_Send.Repeaters_ID);
    if(RepeatersNextId > SendBuff->pram.ReadALLMeasureValue_Send.Repeaters_ID) 
    {
      ReadALLMeasureValue_ToKingDaServer_ACK(RepeatersNextId, ServerState[0].Port,SendMeasureValue_ToKingDaServer1_ACK);
      DataFlg = KingDaRecord_Type; 
    }
    else
    {
     if(0x01 == Concentrator.CSR.Bit.Appcon_EN)
     {
       Concentrator.SR1.Bit.App_RdVal_end_01 = 0;
       ReadAllAppconMeasureValue_ToKingDaServer_ACK(ServerState[0].Port,SendMeasureValue_ToKingDaServer1_ACK);
       DataFlg = AppconDaRecord_Type; 
     }
    }
  }
  else if(0x01 == Concentrator.CSR.Bit.Appcon_EN)
  {
   if(0 == Concentrator.SR1.Bit.App_RdVal_end_01)
   {
     ReadAllAppconMeasureValue_ToKingDaServer_ACK(ServerState[0].Port,SendMeasureValue_ToKingDaServer1_ACK);
     DataFlg = AppconDaRecord_Type;
   }
  }
 }
}
/*********************************************************************************
 Function:      //
 Description:   // �����س��������ϴ���������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void SendMeasureValue_ToKingDaServer2_ACK(KingDaServer_PackageStruct* SendBuff, void* AckData)
{ 
  static unsigned  char DataFlg = KingDaRecord_Type;
  static unsigned char  err_cnt = 0;
  SystemErrName    err;
  KingDaServer_PackageStruct *pbuff;
  unsigned char RepeatersNextId;

 if(P_NULL == SendBuff)
 {
   err_cnt = 0;
   RepeatersNextId = GetNextID_Repeaters(0);
   if(0 != RepeatersNextId )
   {
     ReadALLMeasureValue_ToKingDaServer_ACK(1, ServerState[1].Port,SendMeasureValue_ToKingDaServer2_ACK);
     DataFlg = KingDaRecord_Type;
   } 
   else
   {
     if(0x01 == Concentrator.CSR.Bit.Appcon_EN)
     {
       err = ReadAllAppconMeasureValue_ToKingDaServer_ACK(ServerState[1].Port,SendMeasureValue_ToKingDaServer2_ACK);
       if(NO_ERR == err)
       {Concentrator.SR1.Bit.App_RdVal_end_02 = 0;}
       else
       {Concentrator.SR1.Bit.App_RdVal_end_02 = 1;}
       DataFlg = AppconDaRecord_Type; 
     }
   }                          
 }
 else
 {
  if(P_NULL !=  AckData)
  {
    pbuff = AckData; 
    if((Concentrator.id[0] != pbuff->pram.ReadALLMeasureValue_Receive.OK.Concentrator_ID[0])
       ||(Concentrator.id[1] != pbuff->pram.ReadALLMeasureValue_Receive.OK.Concentrator_ID[1])
         ||(Concentrator.id[2] != pbuff->pram.ReadALLMeasureValue_Receive.OK.Concentrator_ID[2])
           ||('O' != pbuff->pram.ReadALLMeasureValue_Receive.OK.DIO[0])
             ||('K' != pbuff->pram.ReadALLMeasureValue_Receive.OK.DIO[1]))
    {
      if(KingDaRecord_Type == DataFlg)
      {
        err_cnt ++;
        if(err_cnt < 3)
        {
          RepeatersNextId =  SendBuff->pram.ReadALLMeasureValue_Send.Repeaters_ID;
          ReadALLMeasureValue_ToKingDaServer_ACK(RepeatersNextId, ServerState[1].Port,SendMeasureValue_ToKingDaServer2_ACK);
          DataFlg = KingDaRecord_Type;
          return ;
        }
        else
        {err_cnt = 0;}
      }
      else if(AppconDaRecord_Type == DataFlg)
      {err_cnt = 0; }
      else
      {err_cnt = 0;}
    }
  }
  if(KingDaRecord_Type == DataFlg)
  {
    RepeatersNextId = GetNextID_Repeaters(SendBuff->pram.ReadALLMeasureValue_Send.Repeaters_ID);
    if(RepeatersNextId > SendBuff->pram.ReadALLMeasureValue_Send.Repeaters_ID) 
    {
      ReadALLMeasureValue_ToKingDaServer_ACK(RepeatersNextId, ServerState[1].Port,SendMeasureValue_ToKingDaServer2_ACK);
      DataFlg = KingDaRecord_Type;
    }
    else
    {
     if(0x01 == Concentrator.CSR.Bit.Appcon_EN)
     {
       Concentrator.SR1.Bit.App_RdVal_end_02 = 0;
       ReadAllAppconMeasureValue_ToKingDaServer_ACK(ServerState[1].Port,SendMeasureValue_ToKingDaServer2_ACK);
       DataFlg = AppconDaRecord_Type; 
     }
    }
  }
  else if(0x01 == Concentrator.CSR.Bit.Appcon_EN)
  {
   if(0 == Concentrator.SR1.Bit.App_RdVal_end_02) 
   ReadAllAppconMeasureValue_ToKingDaServer_ACK(ServerState[1].Port,SendMeasureValue_ToKingDaServer2_ACK);
   DataFlg = AppconDaRecord_Type; 
  }
 }
}
/*********************************************************************************/
/************************************END*****************************************/
/*********************************************************************************/