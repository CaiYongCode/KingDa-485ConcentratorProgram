/**********************************************************************************
//������ 
//���ߣ����������      ʱ�䣺 2011-06-27  16��30   �ص㣺�����з���
//CPU�ͺţ�    ϵͳ��Ƶ��
//�汾�ţ�
***********************************************************************************/
#ifndef  MODBUS_MASTER_H
#define  MODBUS_MASTER_H
/**********************************************************************************
�ļ�������
***********************************************************************************/
#define Modbus_TXRegister_M  USART1->DR      //���ڷ������ݼĴ���UART2->DR
#define Modbus_RXRegister_M  USART1->DR      //���ڽ������ݼĴ���
/**********************************************************************************
�궨����
***********************************************************************************/
#define	ClrIORBit(Reg, ClrBitMap)	Reg &= ~ClrBitMap
#define	SetIORBit(Reg, SetBitMap)	Reg |= SetBitMap

#define	UART_BAUDRATE_M6	0x0
#define	UART_BAUDRATE_K6	0xd0

#define Mastermodbus_sendBuf_Len  20
#define Mastermodbus_receBuf_Len  40

#define SendFunction_again_N_MAX  5



#ifndef  NG                        
#define  NG      1   
#endif

#ifndef TURE
#define TURE   1
#endif

#ifndef FLASE
#define FLASE  0
#endif

#ifndef FULL
#define FULL  1
#endif

#ifndef EMPTY
#define EMPTY  0
#endif
  
#define  com_err 0x01

/**********************************************************************************
�������Ͷ�����
***********************************************************************************/

/**********************************************************************************
�ⲿ����������
***********************************************************************************/
//extern unsigned short  buff_1;
extern unsigned short  message_name ;
extern unsigned short slave_err_modbus; 
/**********************************************************************************
����������
***********************************************************************************/
void masterModbus__SRinterrupt( void );
void masterModbus_Init( void );
void masterModbus__TXinterrupt (void);
unsigned char masterSendFunction(  unsigned char  addr, 
                                   unsigned char  function,
                                   unsigned char  data1,
                                   unsigned char  data2,
                                   unsigned char  data3,
                                   unsigned char  data4     );
void checkMasterModbus(void);
void mastermodbus_10ms(void); 
unsigned char mastermodbus_send(unsigned char* sendBuf, unsigned char sendLen);
void Slave_scan(void);
void SetSlave_oneRegister ( unsigned short addr_slave, unsigned short addr_reg, unsigned short data );
/***********************************************************************************/
#endif

