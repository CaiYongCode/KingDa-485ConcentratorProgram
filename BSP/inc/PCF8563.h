
#ifndef __PCF8563_H__
#define __PCF8563_H__


#include "..\..\Projects\inc\SystemDefine.h"

#define PCF8563ADDR_WRITE 	0xA2
#define PCF8563ADDR_READ  	0xA3

/*******RTC 各寄存器  报警寄存器最高位为0 有效**********/
#define  CONTROL2  	 		0x01
#define  SECOND				0x02
#define  MINUTE				0x03
#define  HOUR 				0x04
#define  DAY     			0x05
#define  WEEK     			0x06
#define  MONTH				0x07
#define  YEAR  		 		0x08
#define  WAR_MINUTE 		        0x09
#define  WAR_HOUR			0x0A		
#define  WAR_DAY			0x0B
#define	 WAR_WEEK			0x0C
#define  DTIME_CONTROL          	0x0E
#define  TIME_CONT			0x0F
/*  中断方式  定时 或 报警中断*/
#define WARING_TIME 			0x02
#define DEFINE_TIME 			0x01
#define WARING_DEFINE_TIME 	        0x03

/*  定时器周期选择*/
#define PCF8563_TD_1_4096S 	        0x00
#define PCF8563_TD_1_64S 		0x01
#define PCF8563_TD_1S		        0x02
#define PCF8563_TD_60s 	                0x03

//#define INTTER_MODE 	                WARING_TIME

static void delay_us(unsigned int us);
unsigned char I2C_8563_ByteWrite(unsigned char WriteAddr,unsigned char pBuffer);
unsigned char I2C_8563_byteRead(unsigned char ReadAddr);
void PCF8563_Set_Time(TimerData_Struct* timer);
void PCF8563_Read_time(TimerData_Struct* timer);
void F8563_woring_mode(unsigned char mod);
void F8563_worning_init(unsigned char hour,unsigned char minute);  ///   
void F8563_back_init(unsigned char time, unsigned char TD);  /// 
void F8563_Interrupt(void);  // 
void get_SystemTimer(TimerData_Struct* timer);
#endif 



