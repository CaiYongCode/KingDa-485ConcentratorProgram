/*********************************************************************************
//���������豸����
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "..\inc\EquipMgmt.h"
#include "..\..\LIB\FAT\ff.h"
/*********************************************************************************
����������
*********************************************************************************/
#define  EquipList_file   "FLASH:/Equip"
/*********************************************************************************
��������������
*********************************************************************************/
FATFS  flash;
/*********************************************************************************
�ⲿ����������
*********************************************************************************/

/*********************************************************************************
˽�б���������
*********************************************************************************/
static FIL    File;
/*********************************************************************************
���Ա���������
*********************************************************************************/

/*********************************************************************************
�ڲ�����������
*********************************************************************************/

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
SystemErrName MountMemory(void)
{
  FRESULT rse;
  rse = f_mount (&flash, "FLASH:", 1);
  if(FR_OK != rse)
  {
   rse = f_mkfs ("FLASH:", 0 ,16*1024*1024);
   if(FR_OK != rse)
   {return Unsuccessful;}
   else
   {
    rse = f_mount (&flash, "FLASH:", 1);
    if(FR_OK != rse)
    {return Unsuccessful;}  
   }
  }
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
SystemErrName  CreateBlank_Equip( unsigned long  *number)
{
 FRESULT rse; 
 UINT  itemp;
 unsigned char *tp;
 EquipRecord_struct  equip;

 equip.Spcp = UnFind;
 equip.Channel1 = 0xff;
 equip.Channel2 = 0xff;
 itemp = 0;
 tp = (unsigned char *)&equip.Data ;
 while( itemp < sizeof(EquipRecordData_struct))
 {tp[itemp++] = 0xff;}

 equip.CSR.Byte = 0;
 equip.CSR.Bit.Idel = 1;
 
 rse = f_open(&File, EquipList_file, FA_WRITE | FA_READ );
 if(FR_OK != rse)
 {
  if(P_NULL != number)
  {*number = 0;}
  return DataFileLost;
 }
 rse = f_lseek (&File , File.fsize);
 if(FR_OK != rse)
 {
  if(P_NULL != number)
  {*number = 0;}
  return Unsuccessful;
 }
 rse = f_write (&File, &equip, sizeof(EquipRecord_struct), &itemp); 
 if(FR_OK != rse)
 {
  if(P_NULL != number)
  {*number = 0;}
  return Unsuccessful;
 } 
 rse = f_close (&File);
 if(FR_OK != rse)
 {
  if(P_NULL != number)
  {*number = 0;}
  return Unsuccessful;
 }
 *number = File.fsize / sizeof(EquipRecord_struct) - 1;
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
SystemErrName  GetBlank_Equip( unsigned long  *number)
{
 FRESULT rse; 
 UINT  itemp;
 EquipRecord_struct  equip;
  
 if(P_NULL == number)
 { return IllegalParam;}

 rse = f_open(&File, EquipList_file,FA_READ );
 if(FR_OK != rse)
 {
  if(P_NULL != number)
  {*number = 0;}
  return DataFileLost;
 }
 
 rse = f_lseek (&File , 0);
 if(FR_OK != rse)
 {
  if(P_NULL != number)
  {*number = 0;}
  return Unsuccessful;
 }
 *number = 0;
 do
 {
  rse = f_read (&File,  &equip, sizeof(EquipRecord_struct),  &itemp); 
  if(FR_OK != rse)
  { return Unsuccessful;}
  if(itemp == sizeof(EquipRecord_struct))
  {
    if(0 != equip.CSR.Bit.Idel)
    { return NO_ERR; }
  }
  *number ++;
 }while(itemp == sizeof(EquipRecord_struct));
 f_close (&File);
 *number = 0xffffffff;
 return Unsuccessful;
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
SystemErrName  Config_Equip(unsigned long  number,     //�豸������¼�����
                            Equip_SPCP_type  spcp_tpye, //�豸��ͨ��Э���
                            unsigned char  channel1,  //�豸һ��ͨ���� 
                            unsigned char  channel2,  //�豸����ͨ����
                            void*           data,      //�豸��ϸ��Ϣ
                            unsigned char  length)    //��Ϣ����
{
 FRESULT rse; 
 UINT  itemp;
 unsigned long  i;
 unsigned char* cp;
 unsigned char* tp;
 EquipRecord_struct  equip; 
 
 rse = f_open(&File, EquipList_file, FA_WRITE | FA_READ );
 if(FR_OK != rse)
 {return DataFileLost;}
 
 i = (number + 1) * sizeof(EquipRecord_struct);
 if(File.fsize < i )
 {return IllegalParam;}
 
 rse = f_lseek (&File , i);
 if(FR_OK != rse)
 {return Unsuccessful;}
 
 equip.CSR.Bit.Idel = 0;
 equip.Spcp = spcp_tpye;
 equip.Channel1 = channel1;
 equip.Channel2 = channel2; 
 
 cp = data;
 tp = (unsigned char*)&equip.Data;
 while(length --)
 {
   *tp = *cp;
   tp++;
   cp++;
 }
 rse = f_write (&File, &equip, sizeof(EquipRecord_struct), &itemp); 
 if((FR_OK != rse)||(itemp != sizeof(EquipRecord_struct)))
 {return Unsuccessful;} 
 rse = f_close (&File);
 if(FR_OK != rse)
 {return Unsuccessful;}
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
SystemErrName  ReadForNumber_Equip(unsigned long  number, EquipRecord_struct* equip)
{
 FRESULT rse; 
 UINT  itemp;
 unsigned long  i; 
 
 rse = f_open(&File, EquipList_file, FA_WRITE | FA_READ );
 if(FR_OK != rse)
 {return DataFileLost;}
 
 i = (number + 1) * sizeof(EquipRecord_struct);
 if(File.fsize < i )
 {return IllegalParam;}
 
 rse = f_lseek (&File , i);
 if(FR_OK != rse)
 {return Unsuccessful;}
 
 rse =  f_read (&File, equip, sizeof(EquipRecord_struct), &itemp);
 if((FR_OK != rse)||(itemp != sizeof(EquipRecord_struct)))
 {return Unsuccessful;}
 
  rse = f_close (&File);
 if(FR_OK != rse)
 {return Unsuccessful;}  
 
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
SystemErrName  ReadnextOChannel_Equip( unsigned char channel1, 
                                       unsigned char channel2,
                                       unsigned char opt,
                                       EquipRecord_struct* equip) 
{
 SystemErrName err = NO_ERR; 
 FRESULT rse; 
 UINT  itemp;
 static unsigned long Number = 0;
 
 if(0 != opt)
 {Number = 0;}
 
 rse = f_open(&File, EquipList_file, FA_WRITE | FA_READ );
 if(FR_OK != rse)
 {return DataFileLost;}
 rse = f_lseek (&File ,Number );
 if(FR_OK != rse)
 {return Unsuccessful;}
 do
 {
  rse = f_read (&File,  &equip, sizeof(EquipRecord_struct),  &itemp); 
  if(FR_OK != rse)
  { err =  Unsuccessful; break;}
  else
  {
   if(itemp == sizeof(EquipRecord_struct))
   {
    Number = File.fptr; 
    if((equip->CSR.Bit.Idel == 0)&&(equip->Spcp == KingDa) 
       && (equip->Channel1 == channel1)
       && (equip->Channel2 == channel2)) 
    { 
     err =  NO_ERR;
     break;
    }
   }
   else
   {
     Number = 0;
     err =  Eend; break;
   }
  }
 }while(itemp == sizeof(EquipRecord_struct));
 rse = f_close (&File);
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
SystemErrName  Add_KingDaWaterMeter_Equip(KingDaEquip_type type,
                                          unsigned char*  id,
                                          unsigned char*  addr) 

{
  SystemErrName err = NO_ERR;
  unsigned long  number = 0;
  KingDaEquipInfor_struct  WaterMeter;
  
  err = CreateBlank_Equip(&number);
  if(NO_ERR != err)
    
  WaterMeter.Type = KingDa_WaterMeter;
  WaterMeter.ID[0] = id[0];
  WaterMeter.ID[1] = id[1];
  WaterMeter.ID[2] = id[2];
  WaterMeter.ID[3] = id[3];
    
  WaterMeter.ADDR[0] = addr[0];
  WaterMeter.ADDR[1] = addr[1];
  WaterMeter.ADDR[2] = addr[2];  
  
  err = Config_Equip(number,     //�豸������¼�����
                     KingDa, //�豸��ͨ��Э���
                     addr[0],  //�豸һ��ͨ���� 
                     addr[1],  //�豸����ͨ����
                     &WaterMeter,        //�豸��ϸ��Ϣ
                     sizeof (KingDaEquipInfor_struct));//��Ϣ����
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
SystemErrName  Get_KingDaAddrForId_Equip(unsigned char*  id, unsigned char*  addr) 
{
 SystemErrName err = NO_ERR; 
 FRESULT rse; 
 UINT  itemp;
 EquipRecord_struct equip;
 
 if(P_NULL == id)
 {return IllegalParam;}
 
 rse = f_open(&File, EquipList_file, FA_WRITE | FA_READ );
 if(FR_OK != rse)
 {return Unsuccessful;}
 rse = f_lseek (&File ,0);
 if(FR_OK != rse)
 {return Unsuccessful;}
 do
 {
  rse = f_read (&File,  &equip, sizeof(EquipRecord_struct),  &itemp); 
  if(FR_OK != rse)
  { err =  Unsuccessful; break;}
  else
  {
   if(itemp == sizeof(EquipRecord_struct))
   {
    if((equip.CSR.Bit.Idel == 0)&&(equip.Spcp == KingDa) 
       && (equip.Data.KingDaNode.ID[0] == id[0])
       && (equip.Data.KingDaNode.ID[1] == id[1])
       && (equip.Data.KingDaNode.ID[2] == id[2])
       && (equip.Data.KingDaNode.ID[3] == id[3])) 
    { 
     err =  NO_ERR;
     if(P_NULL != addr )
     addr [0] = equip.Data.KingDaNode.ADDR[0];
     addr [1] = equip.Data.KingDaNode.ADDR[1];
     addr [2] = equip.Data.KingDaNode.ADDR[2];
     break;
    }
   }
   else
   {err =  Unsuccessful; break;}
  }
 }while(itemp == sizeof(EquipRecord_struct));
 rse = f_close (&File);
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
SystemErrName  Get_KingDaIdForAddr_Equip(unsigned char* addr, unsigned char* id) 
{
 SystemErrName err = NO_ERR; 
 FRESULT rse; 
 UINT  itemp;
 EquipRecord_struct equip;
 
 if(P_NULL == addr)
 {return IllegalParam;}
 
 rse = f_open(&File, EquipList_file, FA_WRITE | FA_READ );
 if(FR_OK != rse)
 {return Unsuccessful;}
 rse = f_lseek (&File ,0);
 if(FR_OK != rse)
 {return Unsuccessful;}
 do
 {
  rse = f_read (&File,  &equip, sizeof(EquipRecord_struct),  &itemp); 
  if(FR_OK != rse)
  { err =  Unsuccessful; break;}
  else
  {
   if(itemp == sizeof(EquipRecord_struct))
   {
    if((equip.CSR.Bit.Idel == 0)&&(equip.Spcp == KingDa) 
       && (equip.Data.KingDaNode.ADDR[0] == addr[0])
       && (equip.Data.KingDaNode.ADDR[1] == addr[1])
       && (equip.Data.KingDaNode.ADDR[2] == addr[2])) 
    { 
     err =  NO_ERR;
     if(P_NULL != addr )
     id [0] = equip.Data.KingDaNode.ID[0];
     id [1] = equip.Data.KingDaNode.ID[1];
     id [2] = equip.Data.KingDaNode.ID[2];
     id [3] = equip.Data.KingDaNode.ID[3];
     break;
    }
   }
   else
   {err =  Unsuccessful; break;}
  }
 }while(itemp == sizeof(EquipRecord_struct));
 rse = f_close (&File);
 return err;
}





