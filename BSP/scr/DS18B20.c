
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
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;			 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);		
//	BUS_H();
}

static void GPIO_18b20_In_Config(void )
{
	
	GPIO_InitTypeDef GPIO_InitStructure;              // data ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;			 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);	

}

unsigned char init_18b20(void )		//18b20��λ
{
	unsigned int timecount = 0xffff;
	GPIO_18b20_Out_Config();
	delay_us(20);
	BUS_L();
	delay_us(4000);// 500us  ��ʱ
	BUS_H();
	delay_us(20); // 3us ��ʱ
	
	GPIO_18b20_In_Config();
	while((BUS_IN() == 1) && (timecount--) > 0);
	if (timecount == 0)
	{
		return 0;
	}
		
	delay_us(4000);		// 500us  ��ʱ		
	return 1;
}


static unsigned char read_btye(void)				//��һ���ֽ�
{
	
	uchar i,data;
	data = 0;
	
	for(i=0;i<8;i++)
	{
		GPIO_18b20_Out_Config();
		BUS_L();
		delay_us(20);// 3us  ��ʱ
		BUS_H();
		data >>= 1;
		GPIO_18b20_In_Config();
		delay_us(20);
		if(BUS_IN() != 0)
		{
			data |= 0x80;
		}
		delay_us(480); // 64us ��ʱ
					
	}
	return data;
}

static void write_btye(uchar outdata)		   //дһ���ֽ�
{
	uchar i;
	GPIO_18b20_Out_Config();
	for(i = 0; i < 8; i++)
	{
		if(outdata & 0x01)
		{
			BUS_L();
			delay_us(20);	 // 15us ������  
			BUS_H();
			delay_us(480);  // ����������60us 
		
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

unsigned int temp_gether(void)						   //��ȡ�¶�
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
		write_btye(0x82);//�����¶�����Ϊ130��					  
		write_btye(0xa8);//�����¶�����Ϊ-40��
		write_btye(0x3f);//���þ���Ϊ10λ
		init_ok = init_18b20();									     
		if(init_ok)
		{								
			write_btye(0xcc);		        
			write_btye(0x44);		   		//�¶�ת������
			//Delayms(20); //200 ms ��ʱ
			OSTimeDly ( 2, OS_OPT_TIME_DLY, &err);
			init_18b20();
			write_btye(0xcc);	 
			write_btye(0xbe);				//���¶�����
			temperature += (uint)read_btye();		  //�����Ͱ�λ
			temperature <<= 8;
			temperature&=0xff00;
			temperature += (uint)read_btye();		  //�����߰�λ	
			temperature=((temperature >> 8) & 0x00ff) | (temperature << 8);//���ֽڸ��ֽڶԵ�
			if((temperature&0x8000) != 0)	  //�ж��Ƿ�Ϊ����
			{
				temperature = (~temperature) + 1;
				temperature = (temperature >> 4) * 10 + ((temperature & 0x000f) * 10 / 16);  //ת����ʮ�������¶����֣������10
				temperature |= 0x8000;//�����λ��һ��˵���Ǹ���
			}
			else
				temperature=(temperature>>4)*10+((temperature&0x000f)*10/16);  //ת����ʮ�������¶����֣������10			
		}
	}
		return temperature;
}
