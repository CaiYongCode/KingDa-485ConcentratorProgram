/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "..\inc\SystemDefine.h"
#include "..\inc\Common_API.h"
/*********************************************************************************
����������
*********************************************************************************/
const unsigned char  Integer[10] ={0,1,2,3,4,5,6,7,8,9};
/*********************************************************************************
��������������
*********************************************************************************/

/*********************************************************************************
�ⲿ����������
*********************************************************************************/

/*********************************************************************************
˽�б���������
*********************************************************************************/
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
void DelayuS(unsigned long us)
{
 while ( us -- );
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
void DelymS (unsigned short ms)
{
  unsigned long itemp ;
  while(ms--)
  {
    itemp = 8000;
    while (itemp--);
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
void Int8ToBCD (unsigned char data, unsigned char *pbuff, unsigned char w)
{
  unsigned char itemp;
  if( 0 == w )
  {return;}
  else if(w & 0x01)
  {w ++;}
  w >>= 1;
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
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void Int16ToBCD (unsigned short data, unsigned char *pbuff, unsigned char w)
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
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void Int32ToBCD (unsigned long data, unsigned char *pbuff, unsigned char w)
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
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
unsigned char BCDToInt8 (unsigned char *pbuff, unsigned char w)
{
  unsigned char itemp = 0;
  unsigned char data = 0;
  unsigned char cnt = 0;

  while (itemp < 2)
  {
    if(cnt >= w)
    {break;}
    else
    {
     data *= 10;
     data += pbuff[itemp]& 0x0f;
     cnt++;
    }
    if(cnt >= w)
    {break;}
    else
    {
     data *= 10;
     data += pbuff[itemp] >>= 4;;
     cnt++;
    }
    itemp ++;
  }
  return data;
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
unsigned short BCDToInt16 (unsigned char *pbuff, unsigned char w)
{
  unsigned char itemp = 0;
  unsigned short data = 0;
  unsigned char cnt = 0;

  while (itemp < 3)
  {
    if(cnt >= w)
    {break;}
    else
    {
     data *= 10;
     data += pbuff[itemp]& 0x0f;
     cnt++;
    }
    if(cnt >= w)
    {break;}
    else
    {
     data *= 10;
     data += pbuff[itemp] >>= 4;;
     cnt++;
    }
    itemp ++;
  }
  return data;
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
unsigned long BCDToInt32 (unsigned char *pbuff, unsigned char w)
{
  unsigned char itemp = 0;
  unsigned long data = 0;
  unsigned char cnt = 0;

  while (itemp < 5)
  {
    if(cnt >= w)
    {break;}
    else
    {
     data *= 10;
     data += pbuff[itemp]& 0x0f;
     cnt++;
    }
    if(cnt >= w)
    {break;}
    else
    {
     data *= 10;
     data += pbuff[itemp] >>= 4;;
     cnt++;
    }
    itemp ++;
  }
  return data;
}
/*********************************************************************************
 Function:      //
 Description:   //ASCII�봮��ʾ��16���Ʊ任Ϊʵ��
 Input:         //hex�� ASCII�봮�׵�ַ
                //length��ASCII�봮����
                  width��������ʵ��16������Ԫ��
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void AsciiHexToBCD ( unsigned char* hex,
                     unsigned char length,
                     void* bcd, 
                     unsigned char width )   
{
  unsigned char* pdata;
  pdata = bcd;
  do{
     *pdata = 0;
    if((0 == length)||(0 ==width))
    {break;}
    if(IsDigit(*hex))
    {*pdata = *hex - '0';}
    else if(IsABCDEF(*hex))
    {*pdata = *hex - 'A';}
    else if(Isabcdef(*hex))
    {*pdata = *hex - 'a';}
    length --;
    width --;
    if((0 == length)||(0 ==width))
    {break;}
    *pdata <<= 4;
    hex ++;
    if(IsDigit(*hex))
    {*pdata |= *hex - '0';}
    else if(IsABCDEF(*hex))
    {*pdata |= *hex - 'A';}
    else if(Isabcdef(*hex))
    {*pdata |= *hex - 'a';}
    length --;
    width --;
    hex ++;
    pdata ++;
  }while( 1 );
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
void CopyBuff (void* tp, void* sp, unsigned long length)
{
  while(length-- > 0)
  {
   ((unsigned char*)tp)[length] = ((unsigned char*)sp)[length];
  }
}
/*********************************************************************************
 Function:      //AddString
 Description:   //��һ���ַ������ӵ���һ���ַ���ĩβ
 Input:         // tp��ָ��Ŀ���ַ���
                // cp��ָ��Դ���ַ���
 Output:        //
 Return:        // ָ�����һ�����ӵ��ַ�����һ���ַ���ַ
 Others:        //
*********************************************************************************/
unsigned char*  AddString(unsigned char* tp, unsigned char* cp)
{
  while('\0' != *tp)
  {tp++;}
  while('\0' != * cp )
  {
   *tp = *cp; 
   tp++;
   cp++;
  }
  return tp;
}
/*********************************************************************************
 Function:      //AddIntToString
 Description:   //��һ������������ʮ�����ַ�����ʽ���ӵ���һ���ַ���ĩβ
 Input:         // tp��ָ��Ŀ���ַ���
                // cp��ָ��Դ���ַ���
 Output:        //
 Return:        // ָ�����һ�����ӵ��ַ�����һ���ַ���ַ
 Others:        //
*********************************************************************************/
unsigned char* AddIntToString(unsigned char* tp, unsigned long vlue)
{
  unsigned char buff[20];
  unsigned char itemp =0;
  do
  {
    buff[itemp++] = (vlue % 10)+'0';
    vlue /= 10;
  }while(0 != vlue);
  
  while('\0' != *tp)
  {tp++;}
  
  while(0 != itemp)
  {
   *tp = buff[--itemp]; 
   tp++;
  }
  return tp;
}
/*********************************************************************************
 Function:      //AddIntToString
 Description:   //��һ������������ASCII���ʽ���ӵ���һ���ַ���ĩβ
 Input:         // tp��ָ��Ŀ���ַ���
                // cp��ָ��Դ���ַ���
 Output:        //
 Return:        // ָ�����һ�����ӵ��ַ�����һ���ַ���ַ
 Others:        //
*********************************************************************************/
unsigned char* AddBinToAsciiString(unsigned char* tp, unsigned long vlue)
{
  unsigned char buff[20];
  unsigned char itemp =0;
  unsigned char itemp1 = 0;
  do
  {
    itemp1 = vlue & 0x0000000f;
    if(itemp1 < 0x0A)
    {itemp1 += '0';}
    else
    {itemp1 += 'A'- 0x0A;}
    buff[itemp++] = itemp1;
    vlue >>= 4;
  }while(0 != vlue);
  
  while('\0' != *tp)
  {tp++;}
 
  while(0 != itemp)
  {
   *tp = buff[--itemp]; 
   tp++;
  }
  return tp;
}
/*********************************************************************************
 Function:      //
 Description:   // �ȶ������ַ����Ƿ����
Input:          // 
                //    
                // 
 Return:        //  
 Others:        //
                  
*********************************************************************************/
unsigned char ContrastString (unsigned char*String1,unsigned char*String2)
{
  unsigned char i =0;  
  while((String1[i]==String2[i])
        && (String1[i]!='\0')
        && (String2[i]!='\0'))
  { i++;}
  
 if(String1[i]==String2[i])
 {i =1;}
 else
 {i = 0;}
 return i;
}
/*********************************************************************************
 Function:      //
 Description:   //Ѱ�Ҳ��б�ָʾ�������ָʾ���
Input:          // data     ���������׵�ַ
                // length   �������ݳ���
                // indicator �洢ָʾ��������
 Output:        //
 Return:        //ָ��ָʾ��Ϣ�ı���   
 Others:        //
                  
*********************************************************************************/
unsigned short MatchinString (unsigned char **data,const char**StringList, unsigned short ListSize)
{
 const char *sp; 
 unsigned char *tp; 
 unsigned short itemp = 0; 
 do 
 {
  sp = StringList[itemp];
  tp = *data;
  while((*sp != '\0')&&(*sp++ == *tp++));
  if(*sp == '\0')
  { 
   *data = tp;
   break;
  }
 }while(++itemp < ListSize);
 return itemp;
}
