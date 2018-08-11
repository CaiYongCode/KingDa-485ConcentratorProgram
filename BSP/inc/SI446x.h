 

#ifndef _SI446X_H_
#define _SI446X_H_


#include "stm32f10x_gpio.h"
#include "SI446X_defs.h"


//-----------------------------------------------------------------------------
//	Commands
//-----------------------------------------------------------------------------

// Boot Commands
#define CMD_POWER_UP					0x02	  //启动选项和晶体频率偏移量
#define CMD_PATCH_IMAGE					0x04 	  //

// Common Commands
#define CMD_NOP							0x00      //空操作	
#define CMD_PART_INFO					0x01 	  //报告设备的基本信息
#define CMD_FUNC_INFO					0x10 	  //返回设备的功能修改信息
#define CMD_SET_PROPERTY			    0x11 	  //设置一个属性的值
#define CMD_GET_PROPERTY				0x12 	  //获取一个属性的值
#define CMD_GPIO_PIN_CFG				0x13 	  //Configures the GPIO pins
#define CMD_FIFO_INFO				    0x15 	  //重置TX和RX FIFO
#define CMD_GET_INT_STATUS				0x20 	  //返回中断状态
#define CMD_REQUEST_DEVICE_STATE		0x33 	  //返回当前设备状态
#define CMD_CHANGE_STATE				0x34 	  //Commands the part to any of the defined states or modes
#define CMD_READ_CMD_BUFF				0x44 	  //
#define CMD_FRR_A_READ					0x50 	  //Fast response registers for faster read access
#define CMD_FRR_B_READ					0x51 	  //Fast response registers for faster read access
#define CMD_FRR_C_READ					0x53 	  //Fast response registers for faster read access
#define CMD_FRR_D_READ					0x57 	  //Fast response registers for faster read access
#define CMD_WRITE_TX_FIFO				0x66 	  //Write data to TX FIFO
#define CMD_READ_RX_FIFO				0x77 	  //Read data from RX FIFO
#define CMD_GET_ADC_READING		    	0x14	  //检索温度传感器、低电池检测器或ADC阅读	
#define CMD_PACKET_INFO					0x16 	  //
#define CMD_IRCAL		    		    0x17 	  //
#define CMD_PROTOCOL_CFG				0x18 	  //
#define CMD_GET_PH_STATUS				0x21 	  //返回数据包处理状态和中断
#define CMD_GET_MODEM_STATUS			0x22 	  //返回调制解调器状态和中断
#define CMD_GET_CHIP_STATUS				0x23 	  //返回芯片状态和中断
#define CMD_RX_HOP						0x36 	  //

// Tx Commands
#define CMD_START_TX					0x31 	  //Changes to TX state and configures common parameters

// Rx 
#define CMD_START_RX					0x32 	  //Changes to RX state and configures common parameters

// Debug Commands
/*
=================================================================================
-----------------------------External IMPORT functions---------------------------
=================================================================================
*/


#define SI_CSN_LOW( )   GPIO_ResetBits(GPIOC, GPIO_Pin_4);
#define SI_CSN_HIGH( )  GPIO_SetBits(GPIOC, GPIO_Pin_4);

#define SI_SDN_LOW( )   GPIO_ResetBits(GPIOB, GPIO_Pin_0);
#define SI_SDN_HIGH( )  GPIO_SetBits(GPIOB, GPIO_Pin_0);

/*
=================================================================================
------------------------------INTERNAL EXPORT APIs-------------------------------
=================================================================================
*/

#define  PACKET_LENGTH      0//0-64, if = 0: variable mode, else: fixed mode


/*================================数据包头格式==================================*/
typedef struct 
{
 unsigned char  net;               //网络号
 unsigned char  versions      : 5;	  //协议号
 unsigned char  excess        : 3;	  //附加信息

}SI446X_PackageHead;
/*================================数据包格式===================================*/
typedef struct
{
 SI446X_PackageHead        head            ; 
 union
 {
  unsigned char  byte;
  struct 
  {
   unsigned char                tll          : 3;    //包生存周期
   unsigned char                dir          : 1;	 //数据传递方向
   unsigned char                mult_type    : 2;    //组播标志
   unsigned char                cmd_type     : 2;    //命令类型
  }	Fields;
 } property;
 unsigned char                  lenthg ;	  	    //
 unsigned char 				    data[14]  ;
 //unsigned short               crc             ;
}SI446X_Package;


/*
=================================================================================
------------------------------INTERNAL EXPORT APIs-------------------------------
=================================================================================
*/

/*Read the PART_INFO of the device, 8 bytes needed*/
void SI446X_PART_INFO( unsigned char *buffer );

/*Read the FUNC_INFO of the device, 7 bytes needed*/
void SI446X_FUNC_INFO( unsigned char *buffer );

/*Send a command to the device*/
void SI446X_CMD(const unsigned char *cmd, unsigned char cmdsize );
void SI446X_WAIT_CTS( void );
/*Read the INT status of the device, 9 bytes needed*/
void SI446X_INT_STATUS( unsigned char *buffer );

void SI446X_SET_PROPERTYS( SI446X_PROPERTY GROUP_NUM, unsigned char NUM_PROPS, unsigned char *PAR_BUFF );
/*Read the PROPERTY of the device*/
void SI446X_GET_PROPERTYS( SI446X_PROPERTY GROUP_NUM, unsigned char NUM_PROPS, unsigned char *buffer  );
/*configuration the device*/
void SI446X_CONFIG_INIT( void );

/*reset the SI446x device*/
void SI446X_RESET( void );

/*write data to TX fifo*/
void SI446X_W_TX_FIFO( unsigned char *txbuffer, unsigned char size );

/*start TX command*/
void SI446X_START_TX( unsigned char channel, unsigned char condition, unsigned short tx_len );

/*read RX fifo*/
unsigned char SI446X_READ_PACKET( unsigned char *buffer );

/*start RX state*/
void SI446X_START_RX( unsigned char channel, unsigned char condition, unsigned short rx_len,
                      unsigned char n_state1, unsigned char n_state2, unsigned char n_state3 );

/*read packet information*/
void SI446X_PKT_INFO( unsigned char *buffer, unsigned char FIELD, unsigned short length, unsigned short diff_len );

/*read fifo information*/
void SI446X_FIFO_INFO( unsigned char *buffer );

/*Power up the device*/
void SI446X_POWER_UP( unsigned long f_xtal );

/*send a packet*/
void SI446X_SEND_PACKET( unsigned char *txbuffer, unsigned char size, unsigned char channel, unsigned char condition );

/*Set the PROPERTY of the device*/
void SI446X_SET_PROPERTY_X( SI446X_PROPERTY GROUP_NUM, unsigned char NUM_PROPS, unsigned char *PAR_BUFF );

/*config the CRC, PROPERTY 0x1200*/
void SI446X_CRC_CONFIG( unsigned char PKT_CRC_CONFIG );

/*Get the PROPERTY of the device, only 1 byte*/
unsigned char SI446X_GET_PROPERTY( SI446X_PROPERTY GROUP_NUM );

/*Set the PROPERTY of the device, only 1 byte*/
void SI446X_SET_PROPERTY( SI446X_PROPERTY GROUP_NUM, unsigned char proirity );

/*config the GPIOs, IRQ, SDO*/
void SI446X_GPIO_CONFIG( unsigned char G0, unsigned char G1, unsigned char G2, unsigned char G3,
                         unsigned char IRQ, unsigned char SDO, unsigned char GEN_CONFIG );

/*reset the RX FIFO of the device*/
void SI446X_RX_FIFO_RESET( void );

/*reset the TX FIFO of the device*/
void SI446X_TX_FIFO_RESET( void );

void SI4463_Config_Init(void);
void SI446X_Senddata(unsigned char * sendbuffer);
void reprocess(void);
void SI446X_GET_MODEM_STATUS(unsigned char * buff);
void SI446X_FRR_A_READ(unsigned char *buff);
void SI446X_FRR_B_READ(unsigned char *buff);
void SI446X_FRR_C_READ(unsigned char *buff);
void SI446X_FRR_D_READ(unsigned char *buff);

void sentPackage ( SI446X_Package *package);


/*
=================================================================================
----------------------------PROPERTY fast setting macros-------------------------
=================================================================================
*/
// GOLBAL(0x00)
#define GLOBAL_XO_TUNE( x )                 SI446X_SET_PROPERTY( 0x0000, x )
#define GLOBAL_CLK_CFG( x )                 SI446X_SET_PROPERTY( 0x0001, x )
#define GLOBAL_LOW_BATT_THRESH( x )         SI446X_SET_PROPERTY( 0x0002, x )
#define GLOBAL_CONFIG( x )                  SI446X_SET_PROPERTY( 0x0003, x )
#define GLOBAL_WUT_CONFIG( x )              SI446X_SET_PROPERTY( 0x0004, x )
#define GLOBAL_WUT_M_15_8( x )              SI446X_SET_PROPERTY( 0x0005, x )
#define GLOBAL_WUT_M_7_0( x )               SI446X_SET_PROPERTY( 0x0006, x )
#define GLOBAL_WUT_R( x )                   SI446X_SET_PROPERTY( 0x0007, x )
#define GLOBAL_WUT_LDC( x )                 SI446X_SET_PROPERTY( 0x0008, x )
#define GLOBAL_WUT_CAL( x )                 SI446X_SET_PROPERTY( 0x0009, x )

// INT_CTL(0x01)
#define INT_CTL_ENABLE( x )                 SI446X_SET_PROPERTY( 0x0100, x )
#define INT_CTL_PH_ENABLE( x )              SI446X_SET_PROPERTY( 0x0101, x )
#define INT_CTL_MODEM_ENABLE( x )           SI446X_SET_PROPERTY( 0x0102, x )
#define INT_CTL_CHIP_ENABLE( x )            SI446X_SET_PROPERTY( 0x0103, x )

//group 0x02, FRR_CTL
#define FRR_CTL_A_MODE( x )                 SI446X_SET_PROPERTY_1( 0x0200, x )
#define FRR_CTL_B_MODE( x )                 SI446X_SET_PROPERTY_1( 0x0201, x )
#define FRR_CTL_C_MODE( x )                 SI446X_SET_PROPERTY_1( 0x0202, x )
#define FRR_CTL_D_MODE( x )                 SI446X_SET_PROPERTY_1( 0x0203, x )

// PREAMBLE (0x10)









#endif //_SI446X_H_

/*
=================================================================================
------------------------------------End of FILE----------------------------------
=================================================================================
*/

