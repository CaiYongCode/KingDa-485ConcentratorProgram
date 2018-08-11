/**********************************************************************************
//������ 
//���ߣ����������      ʱ�䣺 2011-06-27  16��30   �ص㣺�з���
//CPU�ͺţ�     ϵͳ��Ƶ��
//�汾�ţ�
***********************************************************************************/
#ifndef FILTER_H
#define FILTER_H
/**********************************************************************************
�ļ�������
***********************************************************************************/
/**********************************************************************************
�궨����
***********************************************************************************/

/**********************************************************************************
�������Ͷ�����
***********************************************************************************/
typedef struct 
{
 unsigned char series;
 unsigned char pin ;
 unsigned long sum; 
}MeanFilter_struct; 

typedef struct 
{
  union{
    unsigned char Byte;
    struct{
      unsigned char rst : 1;
    }Bit;
  }CSR;
 unsigned short series;
 unsigned short pin ; 
 unsigned long sum;
 unsigned short* buff;
}SlideFilter_struct ; 
/**********************************************************************************
�ⲿ����������
***********************************************************************************/

/**********************************************************************************
����������
***********************************************************************************/
void MeanFilter_rset (MeanFilter_struct* meanFilter);
void MeanFilter_config (MeanFilter_struct* meanFilter ,unsigned char s);  
unsigned char  MeanFilter (MeanFilter_struct* meanFilter, unsigned short data, unsigned short * value);
void SlideFilter_rset (SlideFilter_struct * slideFilter ,unsigned short data);
void SlideFilter_config (SlideFilter_struct * slideFilter, unsigned char s, unsigned short* buff);
unsigned short  SlideFilter (SlideFilter_struct * slideFilter, unsigned short data);
/**********************************************************************************/
#endif

