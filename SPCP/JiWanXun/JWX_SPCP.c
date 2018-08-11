/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include ".\JWX_SPCP.h"
#include "..\..\BSP\inc\bsp.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
const unsigned char   BroadcastAddress_JWX_SPCP[] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
/*********************************************************************************
公共变量定义区
*********************************************************************************/
unsigned char JWX_TxBuff[DataLengthLimit_JWX_SPCP + sizeof(JWX_SPCP_PackageTailed_Type)];
/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
私有变量定义区
*********************************************************************************/

/*********************************************************************************
测试变量定义区
*********************************************************************************/

/*********************************************************************************
内部函数定义区
*********************************************************************************/
void SendPackage_JWX_SPCP_Func (JWX_SPCP_Package_Type* Package);
SystemErrName JWX_frameVerify (void *pbuff, unsigned char length ,JWX_SPCP_PackageHeader_Type** frame);
static void CTR_ReadData_ACKFunc(unsigned char* addr,JWX_SPCP_ReadData_AckData_Struct* data );
static void CTR_ReadAddress_ACKFunc(unsigned char* newadd);
void CTR_ValveOperation_ACKFunc(unsigned char* addr,JWX_SPCP_ValveOperation_AckData_Struct* data);
void CTR_Disable_ACKFunc(unsigned char* addr,JWX_SPCP_Disable_Enable_AckData_Struct*data);
void CTR_Enable_ACKFunc(unsigned char* addr,JWX_SPCP_Disable_Enable_AckData_Struct*data);
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
SystemErrName JWX_InPut (void *pbuff, unsigned length)
{
 SystemErrName err = NO_ERR;
// unsigned char itemp;
 JWX_SPCP_PackageHeader_Type *Header;
 
 err = JWX_frameVerify(pbuff, length, &Header);
 if(NO_ERR != err) 
 {return err;}
 
 switch (Header->CMD)
 {
  case JWX_ReadData_ACK:
    { 
     CTR_ReadData_ACKFunc(Header->Addr,(JWX_SPCP_ReadData_AckData_Struct*)(Header+1));
      break;
     }

  case JWX_ReadAddress_ACK:
    { 
      CTR_ReadAddress_ACKFunc(Header->Addr);
      break; 
    }

  case JWX_WriteAddress_ACK:
    { 
      //CTR_WriteAddress_ACKFunc(pbuff, &cjt_188_sendbuff);
      break; 
    }
  case JWX_ValveOperation_ACK:
    {
      CTR_ValveOperation_ACKFunc(Header->Addr,(JWX_SPCP_ValveOperation_AckData_Struct*)(Header+1));
      break; 
    }
  case JWX_Disable_ACK :
    {
      CTR_Disable_ACKFunc(Header->Addr,(JWX_SPCP_Disable_Enable_AckData_Struct*)(Header+1));
      break; 
    }
  case JWX_Enable_ACK :
    {
      CTR_Enable_ACKFunc(Header->Addr,(JWX_SPCP_Disable_Enable_AckData_Struct*)(Header+1));
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
SystemErrName JWX_frameVerify (void *pbuff, unsigned char length,JWX_SPCP_PackageHeader_Type** frame)
{
  unsigned char itemp ;
  unsigned char itemp2 ;
  unsigned char itemp3 ;
  //JWX_SPCP_PackageHeader_Type*  Package; 
  
  itemp = 0;
  while (Preamble_JWX_SPCP !=((unsigned char *)pbuff)[itemp])
  {
    itemp ++;
    if(itemp >= length)
    {return FramePreamble_err;}
  }
  itemp2 = 0;
  while (Preamble_JWX_SPCP ==((unsigned char *)pbuff)[itemp])
  {
    ///itemp2 ++;
    itemp ++;
    if(itemp >= length)
    {return FramePreamble_err;}
  }
  //if(itemp2 < 1 )
  //{return FramePreamble_err;}
  
  //itemp2 = itemp -itemp2;
  
  *frame = (JWX_SPCP_PackageHeader_Type *)&(((unsigned char *)pbuff)[itemp]);

  if(SYNC_JWX_SPCP != (*frame)->Sync)
  {return FrameSync_err;}
  
  itemp += (*frame)->DataLength ;
  itemp += sizeof(JWX_SPCP_PackageHeader_Type) + sizeof(JWX_SPCP_PackageTailed_Type);
  if(length < itemp)
  {return Length_err;} 
 
  itemp = 0;
  itemp2 = (*frame)->DataLength + sizeof(JWX_SPCP_PackageHeader_Type);
  itemp3 = 0;
  pbuff = &((*frame)->Sync);
  while(itemp < itemp2)
  {
    itemp3 +=  ((unsigned char *)pbuff)[itemp];
    itemp ++;
  }
  if(itemp3 != ((unsigned char *)pbuff)[itemp])
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
void readVaule_JWX (unsigned char *addr)
{
  unsigned char itemp ;
  JWX_SPCP_Package_Type* Package = (JWX_SPCP_Package_Type*)JWX_TxBuff;
  
  Package->Header.CMD = JWX_ReadData_ASK;
  Package->Header.NodeType = ColdWaterMeter_JWX;
  itemp = 0;
  while (itemp < 7)
  {
    Package->Header.Addr[itemp] = addr[itemp];
    itemp ++;
  }
  Package->Header.DataLength = 3; 
 
 Package->Data[0] = 0x90;
 Package->Data[1] = 0x1F;
 Package->Data[2] = 0x00;
 SendPackage_JWX_SPCP_Func (Package);
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
void ReadAddress__JWX ( void )
{
  unsigned char itemp ;
  JWX_SPCP_Package_Type* Package = (JWX_SPCP_Package_Type*)JWX_TxBuff;
  
  Package->Header.NodeType = ColdWaterMeter_JWX;
  Package->Header.CMD = JWX_ReadAddress_ASK;
  
  itemp = 0;
  while (itemp < 7)
  {
    Package->Header.Addr[itemp] = 0xAA;
    itemp ++;
  }
  Package->Header.DataLength = 3; 
 
 Package->Data[0] = 0x81;
 Package->Data[1] = 0x0A;
 Package->Data[2] = 0x00;
 SendPackage_JWX_SPCP_Func (Package);
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
void WriteAddress_JWX (unsigned char *addr, unsigned char *Newaddr)
{
  unsigned char itemp ;
  JWX_SPCP_Package_Type* Package = (JWX_SPCP_Package_Type*)JWX_TxBuff;
  
  Package->Header.NodeType = ColdWaterMeter_JWX;
  Package->Header.CMD = JWX_WriteAddress_ASK;
  
  itemp = 0;
  while (itemp < 7)
  {
    Package->Header.Addr[itemp] = addr[itemp];
    itemp ++;
  }

 Package->Data[0] = 0x81;
 Package->Data[1] = 0xA0;
 Package->Data[2] = 0x18;
 itemp = 0;
 while (itemp < 7)
 {
   Package->Data[itemp + 3] = Newaddr[itemp];
   itemp ++;
  }
  Package->Header.DataLength = 0x0A;
 SendPackage_JWX_SPCP_Func (Package);
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
void ValveOperation_JWX (unsigned char *addr, JWX_ValveOperationCode_Type  cmd)
{
  void CTR_Disable_ASK_JWX (unsigned char *addr);
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
void ValveOperation_ASK_JWX (unsigned char *addr, JWX_ValveOperationCode_Type  cmd)
{
  unsigned char itemp ;
  JWX_SPCP_Package_Type* Package = (JWX_SPCP_Package_Type*)JWX_TxBuff;
  
  Package->Header.CMD = JWX_ValveOperation_ASK;
  Package->Header.NodeType = ColdWaterMeter_JWX;
  itemp = 0;
  while (itemp < 7)
  {
    Package->Header.Addr[itemp] = addr[itemp];
    itemp ++;
  }
  Package->Header.DataLength = 4; 
 
 Package->Data[0] = 0xA0;
 Package->Data[1] = 0x17;
 Package->Data[2] = 0x00;
 Package->Data[3] = cmd;
 SendPackage_JWX_SPCP_Func (Package);
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
void CTR_Disable_ASK_JWX (unsigned char *addr)
{
  unsigned char itemp ;
  JWX_SPCP_Package_Type* Package = (JWX_SPCP_Package_Type*)JWX_TxBuff;
  
  Package->Header.CMD = JWX_Disable_ASK;
  Package->Header.NodeType = ColdWaterMeter_JWX;
  itemp = 0;
  while (itemp < 7)
  {
    Package->Header.Addr[itemp] = addr[itemp];
    itemp ++;
  }
  Package->Header.DataLength = 3; 
 
 Package->Data[0] = 0xA0;
 Package->Data[1] = 0x09;
 Package->Data[2] = 0x00;
 SendPackage_JWX_SPCP_Func (Package);
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
void CTR_Enable_ASK_JWX (unsigned char *addr)
{
  unsigned char itemp ;
  JWX_SPCP_Package_Type* Package = (JWX_SPCP_Package_Type*)JWX_TxBuff;
  
  Package->Header.CMD = JWX_Enable_ASK;
  Package->Header.NodeType = ColdWaterMeter_JWX;
  itemp = 0;
  while (itemp < 7)
  {
    Package->Header.Addr[itemp] = addr[itemp];
    itemp ++;
  }
  Package->Header.DataLength = 3; 
 
 Package->Data[0] = 0xA0;
 Package->Data[1] = 0x19;
 Package->Data[2] = 0x00;
 SendPackage_JWX_SPCP_Func (Package);
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
void SendPackage_JWX_SPCP_Func (JWX_SPCP_Package_Type* Package)
{
 unsigned char itemp = 0;
 unsigned char L = 0;
 unsigned char cs = 0;
 unsigned char *p ;
 JWX_SPCP_PackageTailed_Type*  Tailed;
 
 Package->Preamble1 = Preamble_JWX_SPCP;
 Package->Preamble2 = Preamble_JWX_SPCP;
 Package->Header.Sync = SYNC_JWX_SPCP; 

 L = Package->Header.DataLength + sizeof(JWX_SPCP_PackageHeader_Type);
 p =  &(Package->Header.Sync); 
 while (itemp < L)
 {cs += p[itemp ++];}
 
 Tailed = (JWX_SPCP_PackageTailed_Type*)&(Package->Data[Package->Header.DataLength]);
 Tailed->CS = cs;
 Tailed->Tailed =  Tailed_JWX_SPCP; 
 
 itemp = Package->Header.DataLength + sizeof(JWX_SPCP_PackageHeader_Type) + sizeof(JWX_SPCP_PackageTailed_Type) + 2;
 JWX_SPCP_TxFunc ( Package, itemp );
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
static void  CTR_ReadData_ACKFunc(unsigned char* addr,JWX_SPCP_ReadData_AckData_Struct* data )
{
  
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
static void CTR_ReadAddress_ACKFunc(unsigned char* newadd)
{
  
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
void CTR_ValveOperation_ACKFunc(unsigned char* addr,JWX_SPCP_ValveOperation_AckData_Struct* data)
{
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
void CTR_Disable_ACKFunc(unsigned char* addr,JWX_SPCP_Disable_Enable_AckData_Struct*data)
{
  
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
void CTR_Enable_ACKFunc(unsigned char* addr,JWX_SPCP_Disable_Enable_AckData_Struct*data)
{
  
}
/**************************************END****************************************/
