/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include ".\DLT 645-1997.h"
/*********************************************************************************
常量定义区
*********************************************************************************/

/*********************************************************************************
公共变量定义区
*********************************************************************************/
//const unsigned char *DLT_645_MeterAddr = (unsigned char *)(DLT_645_MeterAdrr_AddrInRom + RomStartAddr);
//const unsigned char *DLT_645_PassWord  = (unsigned char *)(DLT_645_PassWord_AddrInRom + RomStartAddr);
//const unsigned char DLT_645_MeterCommAddr[MeterAddrLength] = {0x99,0x99,0x99,0x99,0x99,0x99};

/*********************************************************************************
外部变量声明区
*********************************************************************************/
/*********************************************************************************
私有变量定义区
*********************************************************************************/ 
typedef struct{
 unsigned char      flg;
 DLT_645_Frame_Struct   buff;
}DLT_645_SendBuff_Struct;
static DLT_645_SendBuff_Struct  dlt_645_sendbuff ;
static unsigned char  sendbuff_time = 0;

/*********************************************************************************
测试变量定义区
*********************************************************************************/
//unsigned short  iiiiiii = 0x55aa;
/*********************************************************************************
内部函数定义区
*********************************************************************************/
SystemErrName DLT_645_1997_frameVerify (DLT_645_Frame_Struct *frame, unsigned char length);
SystemErrName DLT_ReadData_ACKFunc(DLT_645_Frame_Struct *frame,unsigned short DI,DLT_645_SendBuff_Struct *output);
SystemErrName DLT_WriteAddr_ACKFunc(DLT_645_Frame_Struct *frame,DLT_645_SendBuff_Struct *output);
SystemErrName DLT_ReadAddr_ACKFunc(DLT_645_Frame_Struct *frame,DLT_645_SendBuff_Struct *output);
SystemErrName DLT_SetPassWord_ACKFunc(DLT_645_Frame_Struct *frame,DLT_645_SendBuff_Struct *output);
SystemErrName DLT_ReadPassWord_ACKFunc(DLT_645_Frame_Struct *frame,DLT_645_SendBuff_Struct *output);




SystemErrName send_MeterData_DLT (unsigned char ser,DLT_645_SendBuff_Struct *pbuff);
void BinToBCD_DLT (unsigned short data, unsigned char *pbuff, unsigned char w);
void DLT_645_SendOK_CallBack (unsigned char port, void *pbuff);

SystemErrName DLT_645_SendStart (void *pbuff);
void DLT_645_Send(unsigned char port, void *pbuff );
/*********************************************************************************
功能代码定义区
*********************************************************************************/
void DLT_time ( void )
{
  if(0 == dlt_645_sendbuff.flg)
  {sendbuff_time = 0;}
  else
  {sendbuff_time ++;}
  if(sendbuff_time > 200)
  {dlt_645_sendbuff.flg = 0;}
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
SystemErrName DLT_645_1997_InPut (void *pbuff, unsigned length)
{
 SystemErrName err = NO_ERR;
 unsigned char itemp;
 DLT_645_Frame_Struct *frame;
 unsigned short  di;
 
 itemp = 0;
 while (DLT_645_Preamble ==((unsigned char *)pbuff)[itemp])
 {itemp ++;}
 frame =(DLT_645_Frame_Struct *)(&((unsigned char *)pbuff)[itemp]);
 
 err = DLT_645_1997_frameVerify(frame, length);
 if(NO_ERR != err) 
 {return err;}
 
 itemp = 0;
 while(itemp < frame->Head.Length)
 {frame->Data.Byte[itemp++]  -= 0x33;}
 
 switch (frame->Head.CCR)
 {
  case DLT_CCR_ReadData_ASK:
    {
      if(frame->Data.Super.DI0 == DLT_DI0_ReadData)
      {
        if((frame->Data.Super.DI1 == DLT_DI1_ReadData)
            ||(frame->Data.Super.DI1 == 0x20)
            ||(frame->Data.Super.DI1 == 0x10))        
        {
           di = frame->Data.Super.DI1;
           di <<= 8;
           di |= frame->Data.Super.DI0;
          err = DLT_ReadData_ACKFunc(frame, di, &dlt_645_sendbuff);
        }
      }
      else if((DLT_DI0_ReadAddress == frame->Data.Super.DI0)
                &&(DLT_DI1_ReadAddress == frame->Data.Super.DI1))
      {err = DLT_ReadAddr_ACKFunc(frame,&dlt_645_sendbuff);}
      else if((DLT_DI0_ReadPassWord == frame->Data.Super.DI0)
               &&(DLT_DI1_ReadPassWord == frame->Data.Super.DI1))
      {err = DLT_ReadPassWord_ACKFunc(frame,&dlt_645_sendbuff);}
      break;
     }
  case DLT_CCR_WriteAddress_ASK:
   { 
     err = DLT_WriteAddr_ACKFunc(frame,&dlt_645_sendbuff);
     break;
    }
  case DLT_CCR_SetPassWord_ASK:
   { 
     err = DLT_SetPassWord_ACKFunc(frame,&dlt_645_sendbuff);
     break;
    }
 default:
   {err = OperateCmd_err;}
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
SystemErrName DLT_645_1997_frameVerify (DLT_645_Frame_Struct *frame, unsigned char length)
{
 unsigned char itemp ;
 unsigned char itemp2 ;
 unsigned char itemp3 ;//unsigned char itemp4 ;
 
 itemp = sizeof(DLT_645_FrameHeadStruct) + 2;
 itemp += frame->Head.Length;
 if(length < itemp)
 {return Length_err;} 
 
 if((DLT_645_StartCharacter != frame->Head.StartCharacter1)
    ||(DLT_645_StartCharacter != frame->Head.StartCharacter2))
 {return FrameSync_err;}
 
 itemp = 0;
 while(itemp < MeterAddrLength)
 {
  //if(frame->Head.Addr.MeterAddr[itemp]!= DLT_645_MeterAddr[itemp])
  //{break;}
  itemp ++;
 }
 if(itemp < MeterAddrLength)
 {
  itemp = 0; 
  while(itemp < MeterAddrLength)
  {
  //if(frame->Head.Addr.MeterAddr[itemp]!= DLT_645_MeterCommAddr[itemp])
  //{break;}
  itemp ++;
  }
  if(itemp < MeterAddrLength)
  {return FrameInvalid; }
 }
 itemp = 0;
 itemp2 = frame->Head.Length + sizeof(DLT_645_FrameHeadStruct);
 itemp3 = 0;
 while(itemp < itemp2)
 {
  itemp3 +=  ((unsigned char *)frame)[itemp];
  itemp ++;
 }
 if(itemp3 != ((unsigned char *)frame)[itemp])
 {return Verify_err;}
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
SystemErrName DLT_ReadData_ACKFunc(DLT_645_Frame_Struct *frame,unsigned short DI,DLT_645_SendBuff_Struct *output)
{
// unsigned short data = 0; 
// //unsigned char itemp;
// //unsigned char  pbuff[4];
// 
// if(0 == output->flg)
// {output->flg = 1;}
// else
// {return PortBusy;}
// output->buff.Data.GetValue.Y.Word = 0;
// //if(0 ==getCounterValue_bin (&data, P_NULL))
// {output->buff.Data.GetValue.Y.Bit.S = 1;}  
// output->buff.Data.GetValue.Value[0] = 0x00;
// //data = 1234;
// BinToBCD_DLT (data, &(output->buff.Data.GetValue.Value[1]), 3);
// output->buff.Data.GetValue.DI0 = DI;
// output->buff.Data.GetValue.DI1 = DI >> 8 ;
// output->buff.Head.CCR = DLT_CCR_ReadData_ACK;
// output->buff.Head.Length = sizeof(DLT_645_GetValue_ACK_Struct);
// return DLT_645_SendStart (output); 
  
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
SystemErrName DLT_WriteAddr_ACKFunc(DLT_645_Frame_Struct *frame,DLT_645_SendBuff_Struct *output)
{
// unsigned char itemp;
// itemp = 0;
//
// while (itemp < DLT_PassWordLength)
// {
//  if(frame->Data.WriteAddr.PassWord[itemp] != (DLT_645_PassWord)[itemp])
//  {break;}
//  itemp ++;
// }
// if(itemp < DLT_PassWordLength)
// {return PassWordErr;}
// else
// {
//   if(0 == output->flg)
//   {output->flg = 1;}
//   else
//   {return Busy;}
//   WriteRom (DLT_645_MeterAdrr_AddrInRom,&(frame->Data.WriteAddr.NewAddr), MeterAddrLength);
//   output->buff.Head.CCR = DLT_CCR_WriteAddress_ACK;
//   output->buff.Head.Length = 0;
//   return DLT_645_SendStart (output); 
// }
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
SystemErrName DLT_ReadAddr_ACKFunc(DLT_645_Frame_Struct *frame,DLT_645_SendBuff_Struct *output)
{
// unsigned char itemp;
// itemp = 0;
// 
// if(0 == output->flg){output->flg = 1;}
// else {return Busy;}
// 
// while (itemp < MeterAddrLength)
// {
//   output->buff.Data.ReadAddr.Addr.MeterAddr[itemp] = (DLT_645_MeterAddr)[itemp];
//   itemp ++;
// }
// output->buff.Head.CCR = DLT_CCR_ReadData_ACK;
// output->buff.Head.Length = sizeof (DLT_645_AddresStruct) + 2;  
// output->buff.Data.ReadAddr.DI0 =   DLT_DI0_ReadAddress;
// output->buff.Data.ReadAddr.DI1 =   DLT_DI1_ReadAddress;
 return DLT_645_SendStart (output);  
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
SystemErrName DLT_SetPassWord_ACKFunc(DLT_645_Frame_Struct *frame,DLT_645_SendBuff_Struct *output)
{
// unsigned char itemp;
// if(0 == output->flg)
// {output->flg = 1;}
// else
// {return Busy;}
// itemp = 0;
// while (itemp < DLT_PassWordLength)
// { 
//  if(frame->Data.SetPassWord.CurrentPassWord[itemp] != (DLT_645_PassWord)[itemp])
//  {break;}
//  itemp ++;
// }
// if(itemp < DLT_PassWordLength)
// {
//  output->buff.Data.SetPassWordErr.ErrCode = 0x04; 
//  output->buff.Head.CCR = DLT_CCR_SetPassWordErr_ACK;
//  output->buff.Head.Length = 1;  
// }
// else
// {
//   WriteRom (DLT_645_PassWord_AddrInRom,&(frame->Data.SetPassWord.NewPassWord), DLT_PassWordLength); 
//   itemp = 0;
//   while(itemp < DLT_PassWordLength)
//   {output->buff.Data.SetPassWordOk.CurrentPassWord[itemp] = (DLT_645_PassWord)[itemp];itemp ++;}
//   output->buff.Head.CCR = DLT_CCR_SetPassWordOk_ACK;
//   output->buff.Head.Length = sizeof (DLT_645_SetPassWordOk_ACK_Struct);  
// }
 return DLT_645_SendStart (output); 
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
SystemErrName DLT_ReadPassWord_ACKFunc(DLT_645_Frame_Struct *frame,DLT_645_SendBuff_Struct *output)
{
// unsigned char itemp;
// if(0 == output->flg)
// {output->flg = 1;}
// else
// {return Busy;}
// 
// itemp = 0;
// while(itemp < DLT_PassWordLength)
// {
//  output->buff.Data.ReadPassWord.CurrentPassWord[itemp] = (DLT_645_PassWord)[itemp];
//  itemp ++;
// }
// output->buff.Data.ReadPassWord.DI0 = DLT_DI0_ReadPassWord;
// output->buff.Data.ReadPassWord.DI1 = DLT_DI1_ReadPassWord;
// output->buff.Head.CCR = DLT_CCR_ReadData_ACK;
// output->buff.Head.Length = sizeof(DLT_645_ReadPassWord_ACK_Struct);
 return DLT_645_SendStart (output); 
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
SystemErrName DLT_645_SendStart (void *pbuff)
{
//  static  unsigned char preamble [2] = {DLT_645_Preamble,DLT_645_Preamble};
//  
//  ((DLT_645_SendBuff_Struct *)pbuff)->flg =  Mbus_SEND_CallBack ( preamble, 2, DLT_645_Send,pbuff );
//  if(0 == ((DLT_645_SendBuff_Struct *)pbuff)->flg)
//  {return Busy;}
//  else
//  {return NoErr;}
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
void DLT_645_Send(unsigned char port, void *pbuff )
{
//  DLT_645_SendBuff_Struct *p;
//  unsigned char *p1;
//  unsigned char itemp;
//  unsigned char itemp1;
//  unsigned char sum;
//  
//  p = pbuff;
//  p->buff.Head.StartCharacter1 = DLT_645_StartCharacter;
//  p->buff.Head.StartCharacter2 = DLT_645_StartCharacter;
//  
//  itemp = 0;
//  while (itemp < MeterAddrLength)
//  {
//    //p->buff.Head.Addr.MeterAddr[itemp] = (DLT_645_MeterAddr)[itemp];
//    itemp ++;
//  }
//  itemp = 0;
//  while (itemp <  p->buff.Head.Length)
//  {
//    p->buff.Data.Byte[itemp] += 0x33;
//    itemp ++;
//  }
//  
//  itemp = 0;
//  itemp1 = sizeof(DLT_645_FrameHeadStruct) +  p->buff.Head.Length;
//  sum = 0;  
//  p1 = (unsigned char *)&(p->buff);
//  while (itemp < itemp1)
//  {
//    sum += ((unsigned char *)p1)[itemp];
//    itemp ++;
//  }
//  ((unsigned char *)p1)[itemp ++] = sum;
//  ((unsigned char *)p1)[itemp ++] = DLT_645_Pause;
//  
//  p->flg = Mbus_SEND_CallBack ( &(p->buff), itemp, DLT_645_SendOK_CallBack, pbuff );
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
void DLT_645_SendOK_CallBack (unsigned char port, void *pbuff)
{
  if(P_NULL != pbuff)
  {((DLT_645_SendBuff_Struct *)pbuff)->flg = 0;}
  
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
void BinToBCD_DLT (unsigned short data, unsigned char *pbuff, unsigned char w)
{
  unsigned char itemp;
  itemp = 0;
  while (data > 0)
  {
    pbuff[itemp] =  data %10;
    data /= 10;
    pbuff[itemp] |= (data %10) << 4;
    data /= 10;
    itemp ++;
    if(itemp >= w){break;}
  }
  while (itemp < w)
  {pbuff[itemp] = 0;itemp ++;}
}
/******************************************END********************************************************/

