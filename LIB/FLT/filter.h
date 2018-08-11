/**********************************************************************************
//概述： 
//作者：大胡子书生      时间： 2011-06-27  16：30   地点：研发部
//CPU型号：     系统主频：
//版本号：
***********************************************************************************/
#ifndef FILTER_H
#define FILTER_H
/**********************************************************************************
文件包含区
***********************************************************************************/
/**********************************************************************************
宏定义区
***********************************************************************************/

/**********************************************************************************
数据类型定义区
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
外部变量声明区
***********************************************************************************/

/**********************************************************************************
函数声明区
***********************************************************************************/
void MeanFilter_rset (MeanFilter_struct* meanFilter);
void MeanFilter_config (MeanFilter_struct* meanFilter ,unsigned char s);  
unsigned char  MeanFilter (MeanFilter_struct* meanFilter, unsigned short data, unsigned short * value);
void SlideFilter_rset (SlideFilter_struct * slideFilter ,unsigned short data);
void SlideFilter_config (SlideFilter_struct * slideFilter, unsigned char s, unsigned short* buff);
unsigned short  SlideFilter (SlideFilter_struct * slideFilter, unsigned short data);
/**********************************************************************************/
#endif

