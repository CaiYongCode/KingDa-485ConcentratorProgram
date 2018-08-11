/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
#ifndef  __SERIAL_PORT_H
#define  __SERIAL_PORT_H 
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include ".\MCU_Config.h"
#include ".\PcbDefine.h"
#include "..\..\Projects\inc\SystemDefine.h"
#include "..\..\Projects\inc\Common_API.h"
/*********************************************************************************
�궨����
*********************************************************************************/
#define  Rs232_Number          4
#define  Rs232_BuffLength      64

#define  Rs485_Number          4
#define  Rs485_BuffLength      64

#define  USART2_Number          4
#define  USART2_BuffLength      128

#define  ConfigRs485_RX       set_RS485_DIRpin
#define  ConfigRs485_TX       clr_RS485_DIRpin 

/*********************************************************************************
�������Ͷ���
*********************************************************************************/
typedef struct{
  union {
    unsigned char Byte;
    struct {
      unsigned char flg  :1; //������Чָʾ
      unsigned char idle :1; //����������
      unsigned char org  :1; 
    }Bit;
  }CCR;
  unsigned char length;
  unsigned char data[USART2_BuffLength];
}USART2_BuffStruct;

typedef struct{
  union {
    unsigned char Byte;
    struct {
      unsigned char flg  :1; //������Чָʾ
      unsigned char idle :1; //����������
      unsigned char org  :1; 
    }Bit;
  }CCR;
  unsigned char length;
  unsigned char data[Rs232_BuffLength];
}Rs232_BuffStruct;

typedef struct{
  union {
    unsigned char Byte;
    struct {
      unsigned char flg  :1; //������Чָʾ
      unsigned char idle :1; //����������
      unsigned char org  :1; 
    }Bit;
  }CCR;
  CommPortNumber_Type  port;
  unsigned char length;
  unsigned char data[Rs485_BuffLength];
}Rs485_BuffStruct;

typedef enum {
 Parity_No   =    USART_Parity_No ,  
 Parity_Even =    USART_Parity_Even ,
 Parity_Odd  =    USART_Parity_Odd ,                
}SerialPort_Parity;

typedef enum {
  StopBits_1    =      USART_StopBits_1,         
  StopBits_0_5  =      USART_StopBits_0_5,      
  StopBits_2    =      USART_StopBits_2,       
  StopBits_1_5  =      USART_StopBits_1_5,     
}SerialPort_StopBits;

typedef enum {
 Polling   = 0, 
 ByIT      = 1,
 ByDMA     = 2,
}SerialPort_ReceiveMod; 

typedef void (*SerialPort_SendEndCallBack_Type)(CommPortNumber_Type port, void *pbuff);
/*********************************************************************************
�ⲿ����������
*********************************************************************************/
extern PortStatus  Rs232Flg ;
extern PortStatus  Rs485Flg ;
extern PortStatus  USART2Flg ;
extern unsigned short     RS485_IdleTime  ;
/*********************************************************************************
����������
*********************************************************************************/
void TXE_Interrupt_USART2 (void);
void TC_Interrupt_USART2 (void);
void RXNE_Interrupt_USART2 (void);
void IDLE_Interrupt_USART2 (void);
void ERR_Interrupt_USART2 (void);
void receiveByDMA_TC_Interrupt_USART2 (void); 
void sendByDMA_TC_Interrupt_USART2 (void);
void Launch_USART2 (unsigned long BaudRate, SerialPort_Parity  Parity, 
                     SerialPort_StopBits StopBits, SerialPort_ReceiveMod mod );                            
USART2_BuffStruct* GetDataForBuff_USART2 (void);
void FreeBuff_USART2(USART2_BuffStruct* buff);
SystemErrName sendBuffByIT_USART2 (void * buff, unsigned short length,
                                   SerialPort_SendEndCallBack_Type callBack,
                                   void *callBack_arg );
SystemErrName sendStringByIT_USART2 (unsigned char *buff);
SystemErrName sendBuffByDMA_USART2 (void *buff, unsigned short length,
                                   SerialPort_SendEndCallBack_Type callBack,
                                   void *callBack_arg );
SystemErrName sendStringByDMA_USART2 (unsigned char *buff );

void TXE_Interrupt_Rs485 (void);
void TC_Interrupt_Rs485 (void);
void RXNE_Interrupt_Rs485 (void);
void IDLE_Interrupt_Rs485 (void);
void ERR_Interrupt_Rs485 (void);
void receiveByDMA_TC_Interrupt_Rs485 ( void );
void sendByDMA_TC_Interrupt_Rs485 (void);
SystemErrName Launch_Rs485( CommPortNumber_Type Channel, unsigned long BaudRate,
                            SerialPort_Parity  Parity, SerialPort_StopBits StopBits,
                            SerialPort_ReceiveMod   mod );
CommPortNumber_Type get_RS458ChannelNumber ( void );
SystemErrName PowerConfig_Rs485 (CommPortNumber_Type Channel);
FunctionalState getPowerRs485_State (CommPortNumber_Type channel);
SystemErrName changeChannel_Rs485(CommPortNumber_Type Channel);
CommPortNumber_Type  getChannel_Rs485( void );

Rs485_BuffStruct* GetDataForBuff_Rs485 (void);  
void FreeBuff_Rs485(Rs485_BuffStruct* buff);
SystemErrName sendBuffByIT_Rs485 (void * buff, unsigned short length,
                                  SerialPort_SendEndCallBack_Type callBack,
                                   void *callBack_arg );
SystemErrName sendStringByIT_Rs485 (unsigned char *buff);
SystemErrName sendBuffByDMA_Rs485 (void *buff, unsigned short length,
                                   SerialPort_SendEndCallBack_Type callBack,
                                   void *callBack_arg );
SystemErrName sendStringByDMA_Rs485 (unsigned char *buff );


void TXE_Interrupt_Rs232 (void);
void TC_Interrupt_Rs232 (void);
void RXNE_Interrupt_Rs232 (void);
void IDLE_Interrupt_Rs232 (void);
void ERR_Interrupt_Rs232 (void);
void receiveByDMA_TC_Interrupt_Rs232 (void);
void sendByDMA_TC_Interrupt_Rs232 (void);
void Launch_Rs232 (unsigned long BaudRate, SerialPort_Parity  Parity, 
                     SerialPort_StopBits StopBits, SerialPort_ReceiveMod mod );
Rs232_BuffStruct* GetDataForBuff_Rs232 (void);
void FreeBuff_Rs232(Rs232_BuffStruct* buff);
SystemErrName sendBuffByIT_Rs232 (void * buff, unsigned short length,    
                                   SerialPort_SendEndCallBack_Type callBack,
                                   void *callBack_arg );
SystemErrName sendStringByIT_Rs232 (unsigned char *buff);
SystemErrName sendBuffByDMA_Rs232 (void *buff, unsigned short length,
                                   SerialPort_SendEndCallBack_Type callBack,
                                   void *callBack_arg );
SystemErrName sendStringByDMA_Rs232 (unsigned char *buff );
/********************************************************************************/
#endif

