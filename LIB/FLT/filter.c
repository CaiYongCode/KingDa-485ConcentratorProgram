/**********************************************************************************
//概述： 
//作者：大胡子书生      时间： 2011-06-27  16：30   地点：研发部
//CPU型号：    系统主频：
//版本号：
***********************************************************************************/
/**********************************************************************************
文件包含区
***********************************************************************************/
#include "filter.h"
/**********************************************************************************
常量定义区
***********************************************************************************/
/**********************************************************************************
公共变量定义区
***********************************************************************************/
/**********************************************************************************
私有变量定义区
***********************************************************************************/
/**********************************************************************************
外部变量声明区
***********************************************************************************/
/**********************************************************************************
测试变量定义区
***********************************************************************************/
/**********************************************************************************
内部函数声明区
***********************************************************************************/
/**********************************************************************************
功能代码定义区
***********************************************************************************/
/**********************************************************************************
  Function:       // 
  Description:    //算术平均值滤波器复位  
  Input:          //meanFilter : 滤波器控制数据块地址 
                  // 
  Output:         // 
  Return:         // 
  Others:         // 
************************************************************************************/
void MeanFilter_rset (MeanFilter_struct* meanFilter)
{
 meanFilter -> sum = 0;
 meanFilter -> pin = 0;
}
/**********************************************************************************
  Function:       // 
  Description:    //算术平均值滤波器复位  
  Input:          //meanFilter : 滤波器控制数据块地址 
                  //s:滤波器阶数 
  Output:         // 
  Return:         // 
  Others:         // 
************************************************************************************/
void MeanFilter_config (MeanFilter_struct* meanFilter ,unsigned char s)
{
 meanFilter -> sum = 0;
 meanFilter -> pin = 0;
 meanFilter -> series = s;
}  
/**********************************************************************************
  Function:       // 
  Description:    //算术平均值滤波 
  Input:          //meanFilter : 滤波器控制数据块地址 
                  //data ：最新采样值 
				  //value : 更新值存放地址
  Output:         //  
  Return:         // 0：没有更新      1：有更新
  Others:         // 
************************************************************************************/
unsigned char  MeanFilter (MeanFilter_struct* meanFilter, unsigned short data, unsigned short * value)
{
 meanFilter -> sum += data;
 meanFilter -> pin ++;
 if(meanFilter -> pin <  meanFilter -> series )
 { return 0;}
 else
 {
  meanFilter -> pin = 0 ;
  *value = meanFilter -> sum / meanFilter -> series; 
  meanFilter -> sum = 0;
  return 1; 
 }
}
/**********************************************************************************
  Function:       // 
  Description:    //平均滑膜滤波器复位  
  Input:          //SlideFilter : 滤波器控制数据块地址 
                  // 
  Output:         // 
  Return:         // 
  Others:         // 
************************************************************************************/
void SlideFilter_rset (SlideFilter_struct * slideFilter ,unsigned short data)
{
 unsigned short itemp = 0;
 slideFilter -> pin = 0;
 slideFilter -> sum = 0;
 while (itemp < slideFilter -> series)
 {
  slideFilter -> buff[itemp ++] = data; 
  slideFilter -> sum += data;
 }
}
/**********************************************************************************
  Function:       // 
  Description:    //平均滑膜滤波器初始化 
  Input:          //SlideFilter : 滤波器控制数据块地址 
                  //s:滤波器阶数 
				  //s:滤波器数据暂存区地址
  Output:         // 
  Return:         // 
  Others:         // 
************************************************************************************/
void SlideFilter_config(SlideFilter_struct * slideFilter, unsigned char s, unsigned short * buff)
{
 slideFilter->CSR.Bit.rst = 0; 
 slideFilter -> series = s;
 slideFilter -> buff = buff;
}
/**********************************************************************************
  Function:     // 
  Description:  //平均滑膜滤波 
  Input:        // SlideFilter : 滤波器控制数据块地址
                // data ：最新采样值
  Output:       //  
  Return:       // 本次更新值
  Others:       // 算法原理 ：更新值为,本次采样值及本次采样值之前的若干次连续采样值的
                              算数平均值。
			      Yn = (Xn + X(n-1) + X(n-2) + ..... + X(n-s)) / s
		    特点：优点与算术品均值滤波法相比，采样值利用高,采样速率和更新速率
			  相等,在采样速率相对较慢的情况下仍可获得较高的数据更新速率却
                          点,与算术平均值滤波法相比占用更多数据存储空间
************************************************************************************/
unsigned short SlideFilter(SlideFilter_struct * slideFilter, unsigned short data)
{
 if(slideFilter -> series <2)
 {return data;}
 if(0 == slideFilter->CSR.Bit.rst)
 {
  SlideFilter_rset (slideFilter,data);
  slideFilter->CSR.Bit.rst = 1;
  return data;
 }
 slideFilter -> pin ++;
 slideFilter -> pin %= slideFilter -> series;
 slideFilter -> sum -= slideFilter -> buff[slideFilter -> pin];
 slideFilter -> buff[slideFilter -> pin] =  data;
 slideFilter -> sum += slideFilter -> buff[slideFilter -> pin];
 return	slideFilter -> sum / slideFilter -> series;
}


