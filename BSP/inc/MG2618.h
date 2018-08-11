/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
#ifndef  __BSP_MG2618_H
#define  __BSP_MG2618_H 

#ifndef  IDLE 
#define  IDLE  0
#endif

#ifndef  INUSE
#define  INUSE  1
#endif
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "PowerConfig.h"
#include "..\..\Projects\inc\SofTimer.h"
/*********************************************************************************
�궨����
*********************************************************************************/
#define  MG2618_PowerFlg              (GPRS_ENpin)            
#define  MG2618_PowerON( )            set_GPRS_ENpin         
#define  MG2618_PowerOFF( )           clr_GPRS_ENpin

#define  GET_MG2616_ONOF_PIN         ~(GPRS_ONOFFpin) 
#define  SET_MG2616_ONOF_PIN          clr_GPRS_ONOFFpin
#define  RSET_MG2616_ONOF_PIN         set_GPRS_ONOFFpin      

#define  GET_MG2616_RSET_PIN         ~(GPRS_RESTpin)  
#define  SET_MG2616_RSET_PIN          clr_GPRS_RESTpin  
#define  RSET_MG2616_RSET_PIN         set_GPRS_RESTpin  

#define  ConfigUsart_MG2616(BaudRate,Parity,StopBits,mod)           Launch_USART2(BaudRate, Parity, StopBits, mod)                           
#define  GetDataForUsart_MG261( )                                   GetDataForBuff_USART2( )
#define  FreeUsartInputBuff_MG261(buff)                             FreeBuff_USART2(buff)
#define  SendDataToMG261_ByIT(buff,length,callBack,callBack_arg)    sendBuffByIT_USART2(buff,length,callBack,callBack_arg)
#define  SendDataToMG261_ByDMA(buff,length,callBack,callBack_arg)   sendBuffByDMA_USART2(buff, length,callBack,callBack_arg)
#define  SendStringToMG261_ByIT(String)                             sendStringByIT_USART2(String)
#define  SendStringToMG261_ByDMA(String)                            sendStringByDMA_USART2(String)
#define  SendDataToMG2618(buff,length,callBack)                     SendDataToMG261_ByDMA(buff,length,callBack,P_NULL)
#define  SendStringToMG2618(String)                                 SendStringToMG261_ByDMA(String)    

#define  MG2616_ATORDER_LEN   64

#define AT_Again_Command_EN  0   //��ָ�������ظ���һ��ָ�
#define ATA_Command_EN       0   //Ӧ�����
#define ATD_Command_EN       0   //�������
#define ATDL_Command_EN      0   //������������ĵ绰���� 
#define ATE_Command_EN       0   //ʹ�ܻ���ͨ��
#define ATH_Command_EN       0   //�Ͽ���ǰ����
#define ATI_Command_EN       0   //��ʾģ������������Ϣ 
#define ATQ_Command_EN       0   //�����Ƿ����ն˻��Է���ֵ
#define AtData_Command_EN    0   //������ģʽ�л�������ģʽ 
#define ATO_Command_EN       0   //������ģʽ�л�������ģʽ 
#define ATP_Command_EN       0  //ʹ�����岦��
#define ATS0_Command_EN      0  //�Զ�Ӧ������
#define AT_CRC_Command_EN    0  //������������
#define CLVL_Command_EN      0  //�ܻ�����
#define CLIP_Command_EN      0  //�������������ʾ
#define ZSETMUTE_Command_EN  0  //��������
#define CIMI_Command_EN      0  //��ѯ���� ID ��
#define CGMR_Command_EN      0  //��ȡ��Ʒ�汾��
#define ECHO_Command_EN      0  //������������
#define GSN_Command_EN       0  //��ȡ��ǰ IMEI ��
#define ZVERS_Command_EN     0  //��ȡ��ǰ����汾��
#define CLCK_Command_EN      0  //��������
#define CCFC_Command_EN      0  //����ת�ƺ������������ 
#define CCWA_Command_EN      0  //���еȴ�����
#define CHLD_Command_EN      0  //���б��ֺͶ෽����
#define TSIMINS_Command_EN   0  //��ѯ SIM ��״̬
#define CPWD_Command_EN      0  //�޸�����
#define CGMI_Command_EN      0  //��ѯģ�鳧��
#define CSCS_Command_EN      0  //�ַ��������ѡ��
#define CLCC_Command_EN      0  //����״̬��ѯ
//DTMFָ��
#define VTS_Command_EN       0  //���� DTMF ��
//�������ָ�� 
#define CREG_Command_EN      0  //����ע�������
#define COPS_Command_EN      1  //����ѡ��
//�ƶ��豸���ƺ�״̬����
#define CPAS_Command_EN      0  //ģ��״̬��ѯ
#define CFUN_Command_EN      0  //����ģ�鹦��
#define CMEE_Command_EN      0  //�ƶ��豸���󱨸�
#define ZPWROFF_Command_EN   0  //ģ��ػ�
#define CPIN_Command_EN      0  //���� PIN ��
#define EPIN1_Command_EN     0  //���� PIN1 ��
#define EPIN2_Command_EN     0  //���� PIN2 ��
#define EPINC_Command_EN     0  //��ѯ PIN1�� PIN2 ʣ���������
#define CSQ_Command_EN       1  //�ź�ǿ�Ȳ�ѯ
#define CCLK_Command_EN      0  //ʱ�ӹ���
// ��Ϣ����ָ�� 
#define CSCA_Command_EN    0   //�������ĺ���
#define CNMA_Command_EN    0    //����ȷ��
#define CMGF_Command_EN    0   //���ö���Ϣģʽ
#define CNMI_Command_EN    0    //���ö���ָʾ��ʽ
#define CMGR_Command_EN    0    //������Ϣ
#define CMGW_Command_EN    0    //д����Ϣ
#define CSMS_Command_EN    0    //ѡ����ŷ���
#define CMGS_Command_EN    0    //���Ͷ���
#define CPMS_Command_EN    0    //��ѡ����Ϣ�洢��
#define CMGD_Command_EN    0    //ɾ������
#define CMGL_Command_EN    0    //����Ϣ�б�
#define CMSS_Command_EN    0    //���ʹ����Ķ���
//�绰��ָ�� 
#define CPBS_Command_EN    0    //ѡ��绰���洢
#define CPBR_Command_EN    0    //��ȡ�绰����Ϣ
#define CPBW_Command_EN    0    //��绰��д��Ϣ
#define CPBF_Command_EN    0    //���ҵ绰����Ϣ
#define CNUM_Command_EN    0    //��ȡ��������
//����ѹ��ָ�� 
#define IFC_Command_EN        0    //������
#define DTR_Command_EN        0    //���� DTR ģʽ
#define DCD_Command_EN        0    //���� DCD ģʽ
#define IPR_Command_EN        0    //�趨ģ�鲨����
#define AT_F_Command_EN       0    //�ָ���������
#define AT_W_Command_EN       0    //��������
//GPRSָ��
#define CGDCONT_Command_EN     0    //���� PDP ��ʽ
#define CGACT_Command_EN       0    //����/��� PDP ����
#define CGATT_Command_EN       1    //���� GPRS ҵ�� 
#define CGCLASS_Command_EN     0    //GPRS �豸�ȼ�
//ZTE ����ָ��
#define ZSTR_Command_EN          0    //��ѯģ��״̬��Ϣ
#define ZGETICCID_Command_EN     0    //��ȡ ICCID
#define ZCSQ_Command_EN          0    //�����Զ���ʾ CSQ ������ 
#define ZEDT_Command_EN          0    //���� DTR �ļ��ģʽ 
#define ZDSLEEP_Command_EN       0   //32KHz ���˯��ģʽ
#define CUSD_Command_EN          0    //���� USSD ����
#define ZRINGPINMODE_Command_EN  0    //���� RING ���ŵ��ź�ģʽ 
#define ZVERSWITCH_Command_EN    0    //���ư汾�л�
// �������ָ�� 
#define CSTT_Command_EN          1    //���� APN���û��������� 
#define CIICR_Command_EN         1    //�� GPRS �������� 
#define ZPPPCLOSE_Command_EN     0   //�ر� GPRS �������� 
#define CIFSR_Command_EN         1    //��ѯ��ǰģ�� IP ��ֵַ 
#define ZDNSSERV_Command_EN      0    //���� DNS �������� IP ��ַ 
#define ZDNSGETIP_Command_EN     0   //��ȡ��Ӧ Internet ������ IP ��ַ 
//TCP ����ָ�� 
#define CIPSTART_Command_EN      1   //���� TCP ����������
#define CIPSEND_Command_EN       1  //��Ŀ���ַ���� TCP ���� 
#define ZPPPSTATUS_Command_EN    0   //��ѯ GPRS ����״̬ 
#define CIPCLOSE_Command_EN      1   //�ر� TCP ����
#define ZIPSTATUS_Command_EN     0   //��ѯ��ǰ TCP ����״̬ 
// UDP ����ָ�� 
#define ZIPSETUPU_Command_EN     0  //���� UDP ���������� 
#define ZIPSENDU_Command_EN      0 //�� UDP �������������� 
#define ZIPSTATUSU_Command_EN    0 //��ѯ UDP ״̬
#define ZIPCLOSEU_Command_EN     0 //�ر� UDP ����
// SERVER ָ�� 
#define ZTCPLISTEN_Command_EN    0  //�˿ڼ����������� 
#define ZTCPSENDP_Command_EN     0  //�ڱ����򿪵���·�Ϸ������� 
#define ZTCPCLOSEP_Command_EN    0 //�رռ�����������
#define ZTCPSTATUSP_Command_EN   0  //��ѯ��������·
#define ZIPTIMEOUT_Command_EN    0  //����ģ�����ӷ������ͷ������ݵĳ�ʱʱ��
#define ZUDPLISTEN_Command_EN    0  //�˿ڼ�����������
#define ZUDPSENDP_Command_EN     0  //�ڱ����򿪵���·�Ϸ�������
// FTP ָ��
#define ZFTPLOGIN_Command_EN     0 //��¼ FTP ������
#define ZFTPTYPE_Command_EN      0 //���� FTP �ļ�����
#define ZFTPUPLOAD_Command_EN    0 //�ϴ��ļ�
#define ZFTPSIZE_Command_EN      0 //��ȡ�ļ���С
#define ZFTPDNLOAD_Command_EN    0 //�����ļ�
#define ZFTPDNLOADEX_Command_EN  0 //���� FTP �ļ�
#define ZFTPDEL_Command_EN       0 //ɾ���ļ�
#define ZFTPQUIT_Command_EN      0 //�˳� FTP 
// ͸������ָ�� 
#define ZTRANSFER_Command_EN     0 //͸������
//��Ƶ���ָ�� 
#define ZCALLTONE_Command_EN     0 //ժ��������
#define ZDTMFTONE_Command_EN     0 //ZDTMF ���������� 
#define ZKTDSWITCH_Command_EN    0 //�� DTMF ��⹦��
#define ZKTDIND_Command_EN       0 //�����ϱ����� DTMF ����
#define SPEAKER_Command_EN       0 //����ͨ���л�ָ�� 
#define ZMICGB_Command_EN        0 //���� MIC ��Ƶ����
//��վ���ָ��(ֱ��վ����ָ��) 
#define CCED_Command_EN          0 //��ȡС����Ϣ
#define ZBCCH_Command_EN         0 //���� BCCH �ŵ����ݲ�֧�֣�
#define ZBAND_Command_EN         0 //���� GSM Ƶ�� 
#define ZOPT_Command_EN          0 //����������Ӫ��
#define ZCALIST_Command_EN       0 //��ȡ�ز��б�
// TTS ���ָ��
#define ZTTS_Command_EN          0 //TTS(Text to Speech)��������ҵ�� 
#define ZTTSP_Command_EN         0 //���� TTS(Text to Speech)������������(�ݲ�֧���л�������)
// ¼�����ָ��
#define ZAUDREC_Command_EN       0 //¼������
#define ZFILEREAD_Command_EN     0 //��ȡ�ļ�
// GPS ���ָ�� 
#define ZGPSOPEN_Command_EN      0 //���� GPS ָ��
#define ZGPSSLEEP_Command_EN     0 //GPS ����ָ�� 
#define ZGPSNMEARPT_Command_EN   0 //NMEA �ϱ�����ָ�� 
#define ZGPSLOC_Command_EN       0 //��ȡ GPS ��λ��γ��ָ�� 
#define ZGPSUTC_Command_EN       0 //��ȡ GPS ʱ��ָ��
#define ZGPSHS_Command_EN        0 //��ȡ�Ƕ��ٶ�ָ��
#define ZGPSIND_Command_EN       0 //GPS ��λ״̬�ϱ� 

#define SyncRate_Command_EN      1
#define CIPMUX_Command_EN        1
  
#define TCP_LinkType       1   //TCP�ͻ��� 
#define UDP_LinkType       2   //UDP 
#define SERVER_LinkType    3   //����SERVER
/*********************************************************************************
�������Ͷ���
*********************************************************************************/
typedef enum
{
  Void      = 0,        //�޲���ָ��
  Param     = 1,        //�� �� �� ָ �� 
  Query     = 2,        //��ѯָ�� ��ѯ��ָ�ǰ���õ�ֵ
  Help      = 3,        //����ָ� �г���ָ��Ŀ��ܲ�����
}AT_Format;


typedef enum{ 
//2.1 ��ָͨ��
 AT_Again  = 0,   //��ָ�������ظ���һ��ָ��
 ATA       = 1,   //Ӧ�����
 ATD       = 2,   //�������
 ATDL      = 3,   //������������ĵ绰���� 
 ATE       = 4,   //ʹ�ܻ���ͨ��
 ATH       = 5,   //�Ͽ���ǰ����
 ATI       = 6,   //��ʾģ������������Ϣ 
 ATQ       = 7,   //�����Ƿ����ն˻��Է���ֵ
 AtData    = 8,   //������ģʽ�л�������ģʽ 
 ATO       = 9,   //������ģʽ�л�������ģʽ 
 ATP       = 10,  //ʹ�����岦��
 ATS0      = 11,  //�Զ�Ӧ������
 AT_CRC    = 12,  //������������
 CLVL      = 13,  //�ܻ�����
 CLIP      = 14,  //�������������ʾ
 ZSETMUTE  = 15,  //��������
 CIMI      = 16,  //��ѯ���� ID ��
 CGMR      = 17,  //��ȡ��Ʒ�汾��
 ECHO      = 18,  //������������
 GSN       = 19,  //��ȡ��ǰ IMEI ��
 ZVERS     = 20,  //��ȡ��ǰ����汾��
 CLCK      = 21,  //��������
 CCFC      = 22,  //����ת�ƺ������������ 
 CCWA      = 23,  //���еȴ�����
 CHLD      = 24,  //���б��ֺͶ෽����
 TSIMINS   = 25,  //��ѯ SIM ��״̬
 CPWD      = 26,  //�޸�����
 CGMI      = 27,  //��ѯģ�鳧��
 CSCS      = 28,  //�ַ��������ѡ��
 CLCC      = 29,  //����״̬��ѯ
//DTMFָ��
 VTS     = 30,  //���� DTMF ��
//�������ָ�� 
 CREG    = 31,  //�������ע��״̬ 
 COPS    = 32,  //����ѡ��
//�ƶ��豸���ƺ�״̬����
 CPAS    = 33,  //ģ��״̬��ѯ
 CFUN    = 34,  //����ģ�鹦��
 CMEE    = 35,  //�ƶ��豸���󱨸�
 ZPWROFF = 36,  //ģ��ػ�
 CPIN    = 37,  //���� PIN ��
 EPIN1   = 38,  //���� PIN1 ��
 EPIN2   = 39,  //���� PIN2 ��
 EPINC   = 40,  //��ѯ PIN1�� PIN2 ʣ���������
 CSQ     = 41,  //�ź�ǿ�Ȳ�ѯ
 CCLK    = 42,  //ʱ�ӹ���
// ��Ϣ����ָ�� 
 CSCA    = 43,    //�������ĺ���
 CNMA    = 44,    //����ȷ��
 CMGF    = 45,    //���ö���Ϣģʽ
 CNMI    = 46,    //���ö���ָʾ��ʽ
 CMGR    = 47,    //������Ϣ
 CMGW    = 48,    //д����Ϣ
 CSMS    = 49,    //ѡ����ŷ���
 CMGS    = 50,    //���Ͷ���
 CPMS    = 51,    //��ѡ����Ϣ�洢��
 CMGD    = 52,    //ɾ������
 CMGL    = 53,    //����Ϣ�б�
 CMSS    = 54,    //���ʹ����Ķ���
//�绰��ָ�� 
 CPBS    = 55,    //ѡ��绰���洢
 CPBR    = 56,    //��ȡ�绰����Ϣ
 CPBW    = 57,    //��绰��д��Ϣ
 CPBF    = 58,    //���ҵ绰����Ϣ
 CNUM    = 59,    //��ȡ��������
//����ѹ��ָ�� 
 IFC     = 60,    //������
 DTR     = 61,    //���� DTR ģʽ
 DCD     = 62,    //���� DCD ģʽ
 IPR     = 63,    //�趨ģ�鲨����
 AT_F      = 64,    //�ָ���������
 AT_W       = 65,   //��������
//GPRSָ��
 CGDCONT = 66,    //���� PDP ��ʽ
 CGACT   = 67,    //����/��� PDP ����
 CGATT   = 68,    //���GPRS ����״̬ 
 CGCLASS = 69,    //GPRS �豸�ȼ�
//ZTE ����ָ��
 ZSTR          = 70,    //��ѯģ��״̬��Ϣ
 ZGETICCID     = 71,    //��ȡ ICCID
 ZCSQ          = 72,    //�����Զ���ʾ CSQ ������ 
 ZEDT          = 73,    //���� DTR �ļ��ģʽ 
 ZDSLEEP       = 74,    //32KHz ���˯��ģʽ
 CUSD          = 75,    //���� USSD ����
 ZRINGPINMODE  = 76,    //���� RING ���ŵ��ź�ģʽ 
 ZVERSWITCH    = 77,    //���ư汾�л�
// �������ָ�� 
 CSTT         = 78,    //���� APN��
 CIICR        = 79,    //����������·��GPRS����CSD�� 
 ZPPPCLOSE     = 80,    //�ر� GPRS �������� 
 CIFSR      = 81,    //��ѯ��ǰģ�� IP ��ֵַ 
 ZDNSSERV      = 82,    //���� DNS �������� IP ��ַ 
 ZDNSGETIP     = 83,    //��ȡ��Ӧ Internet ������ IP ��ַ 
//TP ����ָ�� 
 CIPSTART      = 84,   //���� TCP ����������
 CIPSEND       = 85,   //��Ŀ���ַ���� TCP ���� 
 ZPPPSTATUS    = 86,   //��ѯ GPRS ����״̬ 
 CIPCLOSE      = 87,   //�ر� TCP ����
 ZIPSTATUS     = 88,   //��ѯ��ǰ TCP ����״̬ 
// UDP ����ָ�� 
 CIPSTARTU     = 89,  //���� UDP ���������� 
 ZIPSENDU      = 90,  //�� UDP �������������� 
 ZIPSTATUSU    = 91,  //��ѯ UDP ״̬
 ZIPCLOSEU     = 92,  //�ر� UDP ����
// SERVER ָ�� 
 ZTCPLISTEN    = 93,  //�˿ڼ����������� 
 ZTCPSENDP     = 94,  //�ڱ����򿪵���·�Ϸ������� 
 ZTCPCLOSEP    = 95,  //�رռ�����������
 ZTCPSTATUSP   = 96,  //��ѯ��������·
 ZIPTIMEOUT    = 97,  //����ģ�����ӷ������ͷ������ݵĳ�ʱʱ��
 ZUDPLISTEN    = 98,  //�˿ڼ�����������
 ZUDPSENDP     = 99,  //�ڱ����򿪵���·�Ϸ�������
// FTP ָ��
 ZFTPLOGIN     = 100, //��¼ FTP ������
 ZFTPTYPE      = 101, //���� FTP �ļ�����
 ZFTPUPLOAD    = 102, //�ϴ��ļ�
 ZFTPSIZE      = 103, //��ȡ�ļ���С
 ZFTPDNLOAD    = 104, //�����ļ�
 ZFTPDNLOADEX  = 105, //���� FTP �ļ�
 ZFTPDEL       = 106, //ɾ���ļ�
 ZFTPQUIT      = 107, //�˳� FTP 
// ͸������ָ�� 
 ZTRANSFER     = 108, //͸������
//��Ƶ���ָ�� 
 ZCALLTONE     = 109, //ժ��������
 ZDTMFTONE     = 110, //ZDTMF ���������� 
 ZKTDSWITCH    = 111, //�� DTMF ��⹦��
 ZKTDIND       = 112, //�����ϱ����� DTMF ����
 SPEAKER       = 113, //����ͨ���л�ָ�� 
 ZMICGB        = 114, //���� MIC ��Ƶ����
//��վ���ָ��(ֱ��վ����ָ��) 
 CCED          = 115, //��ȡС����Ϣ
 ZBCCH         = 116, //���� BCCH �ŵ����ݲ�֧�֣�
 ZBAND         = 117, //���� GSM Ƶ�� 
 ZOPT          = 118, //����������Ӫ��
 ZCALIST       = 119, //��ȡ�ز��б�
// TTS ���ָ��
 ZTTS          = 120, //TTS(Text to Speech)��������ҵ�� 
 ZTTSP         = 121, //���� TTS(Text to Speech)������������(�ݲ�֧���л�������)
// ¼�����ָ��
 ZAUDREC       = 122, //¼������
 ZFILEREAD     = 123, //��ȡ�ļ�
// GPS ���ָ�� 
 ZGPSOPEN      = 124, //���� GPS ָ��
 ZGPSSLEEP     = 125, //GPS ����ָ�� 
 ZGPSNMEARPT   = 126, //NMEA �ϱ�����ָ�� 
 ZGPSLOC       = 127, //��ȡ GPS ��λ��γ��ָ�� 
 ZGPSUTC       = 128, //��ȡ GPS ʱ��ָ��
 ZGPSHS        = 129, //��ȡ�Ƕ��ٶ�ָ��
 ZGPSIND       = 130,  //GPS ��λ״̬�ϱ� 
 
 SyncRate      = 131,
 CIPMUX        = 132,
 NC_Command_MG2618 = 255 
}Command_MG2618; 

//��ָ�����ڽ�������ѡ��
//�����������
typedef struct {
  enum{
    Auto          = '0',  //�Զ�����ѡ�񣬺��Բ���<format><oper>
    Manual        = '1',  //�˹�ѡ�����磬��Ҫ����<format><oper>
    Repeal        = '2',  //��������ע�ᣨ MTK��ƽ̨��֧�ִ˹��ܣ�
    SetingFormat  = '3',  //��ǣ������ע��Ķ���������������������<format>�ġ���ʱ�����в���<format>��
    Auto_Manual   = '4',  //4 manual/auto.�˹�ע�����粻�ɹ��󣬾��Զ�ע�����硣
  } mode;    
 enum{ 
   LongChar       = '0',  //���ַ���ʽ
   ShortChar      = '1',  //���ַ���ʽ
   Digit          = '2'   //���ָ�ʽ
 }format;   //��ֵΪ�趨��Ӫ��<oper>��ʾ�ĸ�ʽ�� 
unsigned char* oper;   //��Ӫ�����ƣ����� format ����
}COPS_PramStructure_MG2618;

//��ָ�����ڽ�������ѡ��
//�����������
typedef struct {
 unsigned char mode;   //��Ӫ�����ƣ����� format ����
}CIPMUX_PramStructure_MG2618;


//���� APN���û���������
//�����������
typedef struct { 
 unsigned char* APN;   //GPRS ��Ӫ���ṩ�� GPRS ���ʽڵ�(APN)��
 //unsigned char* user;   //�û���½��
 //unsigned char* pwd;   //  �û���½����
 //enum{
  // PS = '0',  //PS������
   //CS =  '1'  //CS������
 //}MODE;  //ѡ��������ʽ
 //enum{
  // pap  = '0', 
  // chap = '1'
 //}AUTH;  //��Ȩ��ʽ  
}ZPNUM_PramStructure_MG2618;

//�� GPRS ��������  
//�����������
typedef struct {
unsigned char* cmne;     //GPRS��Ӫ���ṩ�� GPRS ���ʽڵ�(APN)��
unsigned char* USER;    //�û���½����
unsigned char* PWD;     //�û���½���롣
}ZPPPOPEN_PramStructure_MG2618;

//���� DNS �������� IP ��ַ 
//�����������
typedef struct {
 unsigned char IP1[4];          //��DNS��������ַ
 unsigned char IP2[4];          //��DNS��������ַ
}IZDNSSERV_PramStructure_MG2618; 

//��ȡ��Ӧ Internet ������ IP ��ַ
//�����������
typedef struct {
 unsigned char* domainname;     //Internet����
}ZDNSGETIP_PramStructure_MG2618; 

//���� TCP/UDP ���������� 
//�����������
typedef struct {
  enum{
   TCP  = 0, 
   UDP  = 1
 }Mode;  //��Ȩ��ʽ  
 unsigned char  CH;             //TCP ���ӵ�ͨ���ţ�ȡֵ��Χ:0-4
 unsigned char  IP[4];          //Ϊһ��Ŀ���ַ IP ֵ    
 unsigned short PORT;           //�������˿�
 //unsigned char* DomainName;
}IP_Connect_PramStructure_MG2618; 
//��Ŀ���ַ���� TCP / UDP ���� 
//�����������
typedef struct {
 unsigned char  CH;            //TCP ����ͨ����  ȡֵ��Χ:0-4
 unsigned short LENGTH;        //���ݳ���(���֧�� 4K ���ַ���֧�� 0x00~0xff ����)
 unsigned char  *buff;         //ָ�����ݻ�����
 SerialPort_SendEndCallBack_Type callBack;
 void *callBack_arg;
}IP_Send_PramStructure_MG2618; 
//��ѯ��ǰ TCP/UDP ����״̬ �� �ر� TCP/UDP ����
//�����������
typedef struct {
 unsigned char CH;             //TCP ����ͨ����  ȡֵ��Χ:0-4 
}CIPCLOSE_PramStructure_MG2618; 

//�˿ڼ�����������
//�����������
typedef struct {
 unsigned char State;          //'1':start listening '2':stop listening
 unsigned short PortNum;       //�������Ķ˿�   
}ZTCPLISTEN_ZUDPLISTEN_PramStructure_MG2618;
//�ڱ����򿪵���·�Ϸ�������
//�����������
typedef struct {
 unsigned char CH;             //���ӽ����Ŀͻ��˵ı�ʶ
 unsigned short Length;        //Ҫ�������ݵĳ��� 
 unsigned char *buff;          //ָ�����ݻ�����
}ZTCPSENDP_PramStructure_MG2618;
//�رռ�����������
//�����������
typedef struct {
 unsigned char CH;             //���ӽ����Ŀͻ��˵ı�ʶ
}ZTCPCLOSEP_PramStructure_MG2618;
//��ѯ��������·
//�����������
typedef struct {
 unsigned char CH;             //��ѯ��ͨ����
}ZTCPSTATUSP_PramStructure_MG2618;
//����ģ�����ӷ������ͷ������ݵĳ�ʱʱ��
//�����������
typedef struct {
 unsigned short connect_timeout;      //ģ����Ϊ�ͻ��˵����ӳ�ʱʱ�䣬��λ s��
 unsigned short send_data_timeout;    //ģ�鷢�����ݵĳ�ʱʱ�䣬ģ���ڴ�ʱ�������ݻ�û�з��ͳ�ȥ������Ϊ���������������쳣���رմ�����
}ZIPTIMEOUT_PramStructure_MG2618;
//�ڱ����򿪵���·�Ϸ������� UDP
//�����������
typedef struct {
unsigned char IP[4];     //Ŀ�ķ��� IP ��ַ�����ݽ��������ϱ��� 
unsigned short PORT;     //Ŀ�ķ��Ķ˿ڣ����ݽ��������ϱ���
unsigned short LEN;      //Ҫ�������ݵĳ��� 
unsigned char *buff;     //ָ�����ݻ�����
}ZUDPSENDP_PramStructure_MG2618;

//�������������
typedef  union{
   unsigned char                    CH_No;
   ZPNUM_PramStructure_MG2618        ZPNUM;            //���� APN���û��������� 
   IP_Connect_PramStructure_MG2618   TcpUdp_Connect;
   IP_Send_PramStructure_MG2618      TcpUdp_Send; 
   CIPCLOSE_PramStructure_MG2618     CIPCLOSE; 
   COPS_PramStructure_MG2618         COPS;             //��ָ�����ڽ�������ѡ��
   CIPMUX_PramStructure_MG2618       CIPMUX;           //���ö���·ģʽ
  }CommandParameter_MG2618; 
//����������
typedef struct{                   
  Command_MG2618              Cmd;       //����������
  unsigned char              Step;
  AT_Format                   Format;
  CommandParameter_MG2618     CmdPram;   //�������
}CommandDescriptor_MG2618;

typedef struct{
  unsigned char  IP[4];          //Ϊһ��Ŀ���ַ IP ֵ    
  unsigned short PORT;           //�������˿�
  //unsigned short LOCAL_PORT;    //���ذ󶨶˿ڣ�����ʹ�� 1024 ���ϵĶ˿ڰ� ��û������ LOCAL_PORT ʱ���˿�������� 
}GprsInfr_Struct;  



typedef void (*GPRS_FinishCallBack)(Command_MG2618 cmd, DataBuffSpec_Struct *AckData);

typedef struct {
  
 // struct{
   // unsigned char RTEN:1;   //Ӧ��ʱ�ط�ʹ�� 
  //}CSR; 
  
  union
  {
    unsigned char Byte;
    struct{
      unsigned char Flg :1;  //ͨ��ռ��״̬ 0 ���� 1 ռ��  
      unsigned char CN :1;   //ͨ������״̬ 0 û������ 1  ������ 
    }Bit;
  }CH_SR[4];
    
  GprsInfr_Struct GprsInfr[4];
    
  CommandDescriptor_MG2618   Command;    //��ǰ���������� 

  GPRS_FinishCallBack        CallBack;   //������ɻص� 
  unsigned char             Cnt;        //���ʹ���������
  SofTimer_Struct            *TimeOut;   //��ʱ��ʱ��
  SofTimer_Struct            *HeartBeatTime;  //GPRS������ʱ��
  //SofTimer_Struct            *CH_Time[4];  //

  struct{
   PortStatus      Status;           //�˿�״ָ̬ʾ 
   unsigned char   LOCAL_IP[4];      //����IP��ַ   
   unsigned char   Rssi;             //�����ź�ǿ��
   unsigned char   Ber;              //�ŵ�λ������
  }ModInf;  
  unsigned char  String[MG2616_ATORDER_LEN];    //�洢ASCII��������
}MG2618DescriptorStruct;

typedef struct {
  struct {
    unsigned char  Valid:1;    //�������Ƿ���Ч
    unsigned char  Step :1;    //�������Ƿ���Ҫ�ಽA���� 
  }SR;                         //��״̬�Ĵ���
 unsigned char  PramLen;       //������������ֽ���
}CmdInformationStructure_MG2618;       
/*********************************************************************************
�ⲿ����������
*********************************************************************************/
extern MG2618DescriptorStruct MG2618Infr;
/*********************************************************************************
����������
*********************************************************************************/
FunctionalState Get_MG2168PowerState (void);

void PowerOff_GPRS (void);

void Decode_MG2618(unsigned char *gprs_ch, DataBuffSpec_Struct *Data);

unsigned short CompString (unsigned char *data,const char *sp,unsigned char **p);

void StopListen_MG2618(void);

SystemErrName SendCmd_MG2618(CommandDescriptor_MG2618 *Command,
                             GPRS_FinishCallBack CallBack,
                             unsigned char Reps,
                             unsigned short timer);


/********************************************************************************/
#endif
