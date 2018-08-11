
#include "bsp.h"


#define uchar unsigned char
#define uint unsigned int

//extern void Delayms(__IO uint32_t nTime);

static void delay_us(unsigned int us)
{
	
	while (us--);
	
}

static void GPIO_18b20_Out_Config(void )
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;			 //口线翻转速度为50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);		
//	BUS_H();
}

static void GPIO_18b20_In_Config(void )
{
	
	GPIO_InitTypeDef GPIO_InitStructure;              // data 脚

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;			 //口线翻转速度为50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);	

}

unsigned char init_18b20(void )		//18b20复位
{
	unsigned int timecount = 0xffff;
	GPIO_18b20_Out_Config();
	delay_us(20);
	BUS_L();
	delay_us(4000);// 500us  延时
	BUS_H();
	delay_us(20); // 3us 延时
	
	GPIO_18b20_In_Config();
	while((BUS_IN() == 1) && (timecount--) > 0);
	if (timecount == 0)
	{
		return 0;
	}
		
	delay_us(4000);		// 500us  延时		
	return 1;
}


static unsigned char read_btye(void)				//读一个字节
{
	
	uchar i,data;
	data = 0;
	
	for(i=0;i<8;i++)
	{
		GPIO_18b20_Out_Config();
		BUS_L();
		delay_us(20);// 3us  延时
		BUS_H();
		data >>= 1;
		GPIO_18b20_In_Config();
		delay_us(20);
		if(BUS_IN() != 0)
		{
			data |= 0x80;
		}
		delay_us(480); // 64us 延时
					
	}
	return data;
}

static void write_btye(uchar outdata)		   //写一个字节
{
	uchar i;
	GPIO_18b20_Out_Config();
	for(i = 0; i < 8; i++)
	{
		if(outdata & 0x01)
		{
			BUS_L();
			delay_us(20);	 // 15us 内拉高  
			BUS_H();
			delay_us(480);  // 整个不少于60us 
		
		}
		else
		{
			BUS_L();
			delay_us(480); // 60us
			BUS_H();
			delay_us(20);
		}
			
		outdata >>= 1;
	}
}

unsigned int temp_gether(void)						   //读取温度
{
	unsigned char init_ok;
	unsigned short temperature;
	static  OS_ERR  err;
	
	temperature = 0xffff;
	
	init_ok = init_18b20();	
	if(init_ok)
	{
		write_btye(0xcc);
		write_btye(0x4e);
		write_btye(0x82);//设置温度上限为130°					  
		write_btye(0xa8);//设置温度下限为-40°
		write_btye(0x3f);//设置精度为10位
		init_ok = init_18b20();									     
		if(init_ok)
		{								
			write_btye(0xcc);		        
			write_btye(0x44);		   		//温度转换命令
			//Delayms(20); //200 ms 延时
			OSTimeDly ( 2, OS_OPT_TIME_DLY, &err);
			init_18b20();
			write_btye(0xcc);	 
			write_btye(0xbe);				//读温度命令
			temperature += (uint)read_btye();		  //读出低八位
			temperature <<= 8;
			temperature&=0xff00;
			temperature += (uint)read_btye();		  //读出高八位	
			temperature=((temperature >> 8) & 0x00ff) | (temperature << 8);//低字节高字节对调
			if((temperature&0x8000) != 0)	  //判断是否为负数
			{
				temperature = (~temperature) + 1;
				temperature = (temperature >> 4) * 10 + ((temperature & 0x000f) * 10 / 16);  //转换成十六进制温度数字，需除以10
				temperature |= 0x8000;//将最高位置一，说明是负数
			}
			else
				temperature=(temperature>>4)*10+((temperature&0x000f)*10/16);  //转换成十六进制温度数字，需除以10			
		}
	}
		return temperature;
}
