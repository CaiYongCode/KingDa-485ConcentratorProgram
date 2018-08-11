
/*********************************************************************************
//概述：: Operation for SI446x

//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "si446x.h"
#include "bsp.h"
#include "radio_config_Si4463.h"
#include "si446x_defs.h"
#include "WirelessNet1.0/inc/BaseCourseDefine.h"
#include "WirelessNet1.0/inc/Net1.0Config.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
const static unsigned char config_table[] = RADIO_CONFIGURATION_DATA_ARRAY;
/*********************************************************************************
公共变量定义区
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
私有变量定义区
*********************************************************************************/
//static unsigned  char	revflag ;
//static unsigned  char	traflag ;
 unsigned char   si446x_txcnt = 0;  
/*********************************************************************************
测试变量定义区
*********************************************************************************/
unsigned char   si446x_TimeOut_flg = 0;
/*********************************************************************************
内部数据类型定义区
*********************************************************************************/
/*********************************************************************************
内部函数定义区
*********************************************************************************/
unsigned char WriteReadByte_SI446X ( unsigned char dt);
void SI446X_int  ( void );
void SI446X_CMD(const unsigned char *cmd, unsigned char cmdsize );
void SI446X_POWER_UP( unsigned long f_xtal );
void SI446X_READ_RESPONSE( unsigned char *buffer, unsigned char size );
void SI446X_WAIT_CTS( void );
unsigned char  SI446X_NOP( void );
void SI446X_PART_INFO( unsigned char *buffer );
void SI446X_FUNC_INFO( unsigned char *buffer );
void SI446X_INT_STATUS( unsigned char *buffer );
void SI446X_GET_PROPERTYS( SI446X_PROPERTY GROUP_NUM, unsigned char NUM_PROPS, unsigned char *buffer  );
void SI446X_SET_PROPERTYS( SI446X_PROPERTY GROUP_NUM, unsigned char NUM_PROPS, unsigned char *PAR_BUFF );
void SI446X_SET_PROPERTY( SI446X_PROPERTY GROUP_NUM, unsigned char proirity );
unsigned char SI446X_GET_PROPERTY( SI446X_PROPERTY GROUP_NUM );
void SI446X_RESET( void );
void SI446X_CONFIG_INIT( void );
void SI446X_W_TX_FIFO( unsigned char *txbuffer, unsigned char size );
void SI446X_SEND_PACKET( unsigned char *txbuffer, unsigned char size, unsigned char channel, unsigned char condition );
void SI446X_START_TX( unsigned char channel, unsigned char condition, unsigned short tx_len );
unsigned char SI446X_READ_PACKET( unsigned char *buffer );
void SI446X_START_RX( unsigned char channel, unsigned char condition, unsigned short rx_len,
                      unsigned char n_state1, unsigned char n_state2, unsigned char n_state3 );
void SI446X_RX_FIFO_RESET( void );
void SI446X_PKT_INFO( unsigned char *buffer, unsigned char FIELD, unsigned short length, unsigned short diff_len );
void SI446X_FIFO_INFO( unsigned char *buffer );
void SI446X_GPIO_CONFIG( unsigned char G0, unsigned char G1, unsigned char G2, unsigned char G3,
                         unsigned char IRQ, unsigned char SDO, unsigned char GEN_CONFIG );
void SI446X_REQUEST_DEVICE_STATE( unsigned char *buffer );
void SI446X_Senddata(unsigned char * sendbuffer);

void SI446X_GET_MODEM_STATUS(unsigned char * buff);
void SI446X_FRR_A_READ(unsigned char *buff);
void SI446X_FRR_B_READ(unsigned char *buff);
void SI446X_FRR_C_READ(unsigned char *buff);
void SI446X_FRR_D_READ(unsigned char *buff);
/*********************************************************************************
功能代码定义区
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
unsigned char WriteReadByte_SI446X ( unsigned char dt)
{
 unsigned short i = 0;
 while (!(SPI1 -> SR & (0x0001<< 1)))
 {i ++ ;if(i > 0x4fff){break;}}
 SPI1->DR = dt;//       SPI_I2S_SendData(SPI1,dt);
while (!(SPI1 -> SR & (0x0001<< 0)))
{i ++ ;if(i > 0x4fff){break;}} 
 return SPI1->DR;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_int  ( void )
{
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_CMD(const unsigned char *cmd, unsigned char cmdsize )
{
  SI446X_WAIT_CTS( );
  SI_CSN_LOW( );
  while( cmdsize -- )
  {WriteReadByte_SI446X( *cmd++ );}
  SI_CSN_HIGH( );
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_POWER_UP( unsigned long f_xtal )
{
    unsigned char cmd[7];
    cmd[0] = POWER_UP;
    cmd[1] = 0x01;
    cmd[2] = 0x00;
    cmd[3] = f_xtal>>24;
    cmd[4] = f_xtal>>16;
    cmd[5] = f_xtal>>8;
    cmd[6] = f_xtal;
    SI446X_CMD( cmd, 7 );
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_READ_RESPONSE( unsigned char *buffer, unsigned char size )
{
 SI446X_WAIT_CTS( );
 SI_CSN_LOW( );
 WriteReadByte_SI446X( READ_CMD_BUFF );
 while( size -- )
 {*buffer++ = WriteReadByte_SI446X( 0xFF );}
 SI_CSN_HIGH( );
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_WAIT_CTS( void )
{
 unsigned char  cts;
 unsigned short bErrCnt = 0;
 do
 {
  SI_CSN_LOW( );
  WriteReadByte_SI446X( CMD_READ_CMD_BUFF );
  cts = WriteReadByte_SI446X( 0xFF );
  SI_CSN_HIGH( );
  bErrCnt ++ ;
  if (++bErrCnt > 2500)
  {si446x_TimeOut_flg  = 1;return ;  }
 }while( cts != 0xFF );
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
unsigned char SI446X_NOP( void )
{
 unsigned char cts;
 SI_CSN_LOW( );
 cts = WriteReadByte_SI446X( NOP );
 SI_CSN_HIGH( );
 return cts;
}
/*********************************************************************************
 Function:      //
 Description:   //报告关于设备的基本信息
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_PART_INFO( unsigned char *buffer )
{
 unsigned char cmd = PART_INFO;
 SI446X_CMD( &cmd, 1 );
 SI446X_READ_RESPONSE( buffer, 8 );
}
/*********************************************************************************
 Function:      //
 Description:   //返回设备的功能修改信息
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_FUNC_INFO( unsigned char *buffer )
{
 unsigned char cmd = FUNC_INFO;
 SI446X_CMD( &cmd, 1 );
 SI446X_READ_RESPONSE( buffer, 7 );
}
/*********************************************************************************
 Function:      //
 Description:   //返回中断状态字节
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_INT_STATUS( unsigned char *buffer )
{
 unsigned char cmd[4];
 cmd[0] = GET_INT_STATUS;
 cmd[1] = 0;
 cmd[2] = 0;
 cmd[3] = 0;
 SI446X_CMD( cmd, 4 );
 SI446X_READ_RESPONSE( buffer, 9 );
}
/*********************************************************************************
 Function:      //
 Description:   //获取调制解调状态。
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_GET_MODEM_STATUS(unsigned char * buff)
{
 unsigned char cmd = GET_MODEM_STATUS ;
 SI446X_CMD( &cmd, 1 );
 SI446X_READ_RESPONSE( buff, 9 );
}
/*********************************************************************************
 Function:      //
 Description:   //获取调制解调状态。
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_FRR_A_READ(unsigned char * buff)
{
 SI_CSN_LOW( );
 WriteReadByte_SI446X( CMD_FRR_A_READ);
 buff[0] = WriteReadByte_SI446X( 0xFF);
 buff[1] = WriteReadByte_SI446X( 0xFF);
 buff[2] = WriteReadByte_SI446X( 0xFF);
 buff[3] = WriteReadByte_SI446X( 0xFF);
 SI_CSN_HIGH( );
}
/*********************************************************************************
 Function:      //
 Description:   //获取属性的值。
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_GET_PROPERTYS( SI446X_PROPERTY GROUP_NUM, unsigned char NUM_PROPS, unsigned char *buffer  )
{
 unsigned char cmd[4];
 cmd[0] = GET_PROPERTY;
 cmd[1] = GROUP_NUM>>8;
 cmd[2] = NUM_PROPS;
 cmd[3] = GROUP_NUM;
 SI446X_CMD( cmd, 4 );
 SI446X_READ_RESPONSE( buffer, NUM_PROPS + 1 );
}
/*********************************************************************************
 Function:      //
 Description:   //设置属性的值。
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_SET_PROPERTYS( SI446X_PROPERTY GROUP_NUM, unsigned char NUM_PROPS, unsigned char *PAR_BUFF )
{
    unsigned char cmd[20], i = 0;
    if( NUM_PROPS >= 16 )   { return; }
    cmd[i++] = SET_PROPERTY;
    cmd[i++] = GROUP_NUM>>8;
    cmd[i++] = NUM_PROPS;
    cmd[i++] = GROUP_NUM;
    while( NUM_PROPS-- )
    {cmd[i++] = *PAR_BUFF++;}
    SI446X_CMD( cmd, i );
}
/*********************************************************************************
 Function:      //
 Description:   //设置一个属性的值。
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_SET_PROPERTY( SI446X_PROPERTY GROUP_NUM, unsigned char proirity )
{
    unsigned char cmd[5];

    cmd[0] = SET_PROPERTY;
    cmd[1] = GROUP_NUM>>8;
    cmd[2] = 1;
    cmd[3] = GROUP_NUM;
    cmd[4] = proirity;
    SI446X_CMD( cmd, 5 );
}
/*********************************************************************************
 Function:      //
 Description:   //读取一个属性的值。
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
unsigned char SI446X_GET_PROPERTY( SI446X_PROPERTY GROUP_NUM )
{
    unsigned char cmd[4];

    cmd[0] = GET_PROPERTY;
    cmd[1] = GROUP_NUM>>8;
    cmd[2] = 1;
    cmd[3] = GROUP_NUM;
    SI446X_CMD( cmd, 4 );
    SI446X_READ_RESPONSE( cmd, 2 );
    return cmd[1];
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_RESET( void )
{
    unsigned short x = 30000;
    SI_SDN_HIGH( );
    while( x-- );
    SI_SDN_LOW( );
    SI_CSN_HIGH( );
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_CONFIG_INIT( void )
{
 unsigned char i;
 unsigned short j = 0;
 while( ( i = config_table[j] ) != 0 )
 {
  j += 1;
  SI446X_CMD( config_table + j, i );
  j += i;
 }
#if PACKET_LENGTH > 0          
    SI446X_SET_PROPERTY( PKT_FIELD_1_LENGTH_7_0, PACKET_LENGTH );
#else
    
	SI446X_SET_PROPERTY( MODEM_RSSI_CONTROL, (0x00 << 4 | 0x03 <<0) ); 
	SI446X_SET_PROPERTY( MODEM_RSSI_CONTROL2, (0x01 << 2 | 0x01 <<3 | 0x01 << 4) );
    SI446X_SET_PROPERTY( FRR_CTL_A_MODE, 0x0A );
	SI446X_SET_PROPERTY( FRR_CTL_B_MODE, 0x06 );
	                         
    SI446X_SET_PROPERTY( PKT_CONFIG1, 0x00 );
    SI446X_SET_PROPERTY( PKT_CRC_CONFIG, 0x00 );
    SI446X_SET_PROPERTY( PKT_LEN_FIELD_SOURCE, 0x01 );
    SI446X_SET_PROPERTY( PKT_LEN, 0x2A );
    SI446X_SET_PROPERTY( PKT_LEN_ADJUST, 0x00 );
    SI446X_SET_PROPERTY( PKT_FIELD_1_LENGTH_12_8, 0x00 );
    SI446X_SET_PROPERTY( PKT_FIELD_1_LENGTH_7_0, 0x01 );
    SI446X_SET_PROPERTY( PKT_FIELD_1_CONFIG, 0x00 );
    SI446X_SET_PROPERTY( PKT_FIELD_1_CRC_CONFIG, 0x00 );
    SI446X_SET_PROPERTY( PKT_FIELD_2_LENGTH_12_8, 0x00 );
    SI446X_SET_PROPERTY( PKT_FIELD_2_LENGTH_7_0, 0x10 );
    SI446X_SET_PROPERTY( PKT_FIELD_2_CONFIG, 0x00 );
    SI446X_SET_PROPERTY( PKT_FIELD_2_CRC_CONFIG, 0x00 );
#endif 
    SI446X_GPIO_CONFIG( 0, 0, 0x60, 0x61, 0, 0, 0 ); 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_W_TX_FIFO( unsigned char *txbuffer, unsigned char size )
{
 SI_CSN_LOW( );
 WriteReadByte_SI446X( WRITE_TX_FIFO );
 while( size -- ) 
 { WriteReadByte_SI446X( *txbuffer++ ); }
 SI_CSN_HIGH( );
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_SEND_PACKET( unsigned char *txbuffer, unsigned char size, 
                         unsigned char channel, unsigned char condition )
{
 unsigned char cmd[5];
 unsigned char tx_len = size;
 SI446X_TX_FIFO_RESET( );
 SI_CSN_LOW( );
 WriteReadByte_SI446X( WRITE_TX_FIFO );
 #if PACKET_LENGTH == 0
 tx_len ++;
 WriteReadByte_SI446X( size );
 #endif
 while( size -- )
 { WriteReadByte_SI446X( *txbuffer++ ); }
 SI_CSN_HIGH( );
 cmd[0] = START_TX;
 cmd[1] = channel;
 cmd[2] = condition;
 cmd[3] = 0;
 cmd[4] = tx_len;
 SI446X_CMD( cmd, 5 );
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         // channel : 通道号
                // condition ：
				   condition [bit(7-4)] 当前包传输完成后的状态
				     0 = No change.
                     1 = Sleep state.
                     2 = Spi Active state.
                     3 = Ready state.
                     4 = Another enumeration for Ready state.
                     5 = Tune state for TX.
                     6 = Tune state for RX.
                     7 = TX state.
                     8 = RX state.
				  condition [bit2] 
				     0:发送数据,写入fifo。如果fifo空fifo下溢中断发生
					 1:再次发送最后一个数据包。如果使用这个选项,确保没有新数据写入fifo
				  condition [bit(1-0)]
				     0:立即发送  
					 1:当醒来计时器到发送 
                // rx_len ：如果不是零,数据包将只使用字段1传播。如是零,配置
                            包处理程序的字段。如果设置转播,这一领域将被忽略
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_START_TX( unsigned char channel, unsigned char condition, unsigned short tx_len )
{
 unsigned char cmd[5];

 cmd[0] = START_TX;
 cmd[1] = channel;
 cmd[2] = condition;
 cmd[3] = tx_len>>8;
 cmd[4] = tx_len;
 SI446X_CMD( cmd, 5 );
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
unsigned char SI446X_READ_PACKET( unsigned char *buffer )
{
    unsigned char length, i;
    SI446X_WAIT_CTS( );
    SI_CSN_LOW( );

    WriteReadByte_SI446X( READ_RX_FIFO );
#if PACKET_LENGTH == 0
    length = WriteReadByte_SI446X( 0x00 );
#else
    length = PACKET_LENGTH;
#endif
    i = length;

    while( length -- )
    {
        *buffer++ = WriteReadByte_SI446X( 0x00 );
    }
    SI_CSN_HIGH( );
    return i;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         // channel : 通道号
                // condition ：0:立即接收  1 ：当醒来计时器到期接受 
                // rx_len ： 如果为零则为变长否则定长
				// n_state1：前导字段超时后状态
				// n_state2：接收到数据包后状态
				// n_state3：如果启用了CRC校验,在CRC错误的情况下状态
				     0 = No change
                     1 = Sleep state.
                     2 = Spi Active state.
                     3 = Ready state.
                     4 = Another enumeration for Ready state.
                     5 = Tune state for TX.
                     6 = Tune state for RX.
                     7 = TX state.
                     8 = RX state.
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_START_RX( unsigned char channel, unsigned char condition, unsigned short rx_len,
                      unsigned char n_state1, unsigned char n_state2, unsigned char n_state3 )
{
 unsigned char cmd[8];
 SI446X_RX_FIFO_RESET( );
 SI446X_TX_FIFO_RESET( );
 cmd[0] = START_RX;
 cmd[1] = channel;
 cmd[2] = condition;
 cmd[3] = rx_len>>8;
 cmd[4] = rx_len;
 cmd[5] = n_state1;
 cmd[6] = n_state2;
 cmd[7] = n_state3;
 SI446X_CMD( cmd, 8 );
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_RX_FIFO_RESET( void )
{
 unsigned char cmd[2];
 cmd[0] = FIFO_INFO;
 cmd[1] = 0x02;
 SI446X_CMD( cmd, 2 );
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_TX_FIFO_RESET( void )
{
 unsigned char cmd[2];
 cmd[0] = FIFO_INFO;
 cmd[1] = 0x01;
 SI446X_CMD( cmd, 2 );
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_PKT_INFO( unsigned char *buffer, unsigned char FIELD, unsigned short length, unsigned short diff_len )
{
    unsigned char cmd[6];
    cmd[0] = PACKET_INFO;
    cmd[1] = FIELD;
    cmd[2] = length >> 8;
    cmd[3] = length;
    cmd[4] = diff_len >> 8;
    cmd[5] = diff_len;

    SI446X_CMD( cmd, 6 );
    SI446X_READ_RESPONSE( buffer, 3 );
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_FIFO_INFO( unsigned char *buffer )
{
    unsigned char cmd[2];
    cmd[0] = FIFO_INFO;
    cmd[1] = 0x03;

    SI446X_CMD( cmd, 2 );
    SI446X_READ_RESPONSE( buffer, 3);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_GPIO_CONFIG( unsigned char G0, unsigned char G1, unsigned char G2, unsigned char G3,
                         unsigned char IRQ, unsigned char SDO, unsigned char GEN_CONFIG )
{
    unsigned char cmd[10];
    cmd[0] = GPIO_PIN_CFG;
    cmd[1] = G0;
    cmd[2] = G1;
    cmd[3] = G2;
    cmd[4] = G3;
    cmd[5] = IRQ;
    cmd[6] = SDO;
    cmd[7] = GEN_CONFIG;
    SI446X_CMD( cmd, 8 );
    SI446X_READ_RESPONSE( cmd, 8 );
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_REQUEST_DEVICE_STATE( unsigned char *buffer )
{
    unsigned char cmd[1];
    cmd[0] = REQUEST_DEVICE_STATE;
    SI446X_CMD( cmd, 1 );
    SI446X_READ_RESPONSE( buffer, 3);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI4463_Config_Init(void)
{
	unsigned long cont;
	unsigned char buffer[20];
	SI446X_RESET();        
	for(cont = 0; cont < 0xfffff; cont++);
	SI446X_POWER_UP( 30000000 );

	for(cont = 0; cont < 0x500000; cont++);

	SI446X_CONFIG_INIT( );  
	SI446X_PART_INFO(buffer);
	
	SI446X_INT_STATUS( buffer );
	SI446X_START_RX( 0, 0, PACKET_LENGTH,0,8,8 );
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SI446X_Senddata(unsigned char * sendbuffer)
{
 unsigned char buffer[10];
 #if 0
 u32 cont = 0xfff;
 do
 {
  SI446X_REQUEST_DEVICE_STATE( buffer );
  cont --;
  if(0 == cont)
  {
   	SI4463_Config_Init();
	break;
  }

 }while ( 0x08 != buffer[1] & 0x0f);
#endif

 #if PACKET_LENGTH == 0    
 SI446X_SEND_PACKET( sendbuffer, 9, 0, 0 );
 #else                  
 SI446X_SEND_PACKET( sendbuffer, PACKET_LENGTH, 0, 0 );
 #endif
  while( XL4463_NJRQPin == 1);
	  SI446X_INT_STATUS( buffer );
 #if 0
 cont = 0xfffff;
 while (cont--);
 cont = 0xfff;
 do
 {
  SI446X_REQUEST_DEVICE_STATE( buffer );
  cont --;
  if(0 == cont)
  {
   	SI4463_Config_Init();
	break;
  }
 }while ( 0x08 != buffer[1] & 0x0f);
 SI446X_INT_STATUS( buffer );
 cont = 0xfffff;
 si446x_txcnt ++;
 #endif 
 si446x_txcnt ++;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
unsigned char  GetLink ( void )
{
 return  0;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void sentPackage ( SI446X_Package *package)
{
 unsigned char itemp = 0;
 unsigned char data[2];

 #if (MASTER == IDENTITY)
 package->property.Fields.dir = GotoMaster;
 #else
 package->property.Fields.dir = GotoSlave;
 #endif

 package->property.Fields.tll = DEFAULT_TLL ;

 package -> head.versions = 1;
 package -> head.excess = 1;


 SI446X_SET_PROPERTYS(SYNC_BITS_31_24 , 2, ((unsigned char *)&(package -> head)));

 //SI446X_TX_FIFO_RESET( );

 data[0] = package->property.byte;
 data[1] = package->lenthg ;
 SI446X_W_TX_FIFO(data, 2);
 SI446X_W_TX_FIFO( package->data, package->lenthg );
 itemp = GetLink( );
 SI446X_START_TX(itemp, 0x80, package->lenthg + 2 );
}
/*------------------------------------End of FILE----------------------------------*/
