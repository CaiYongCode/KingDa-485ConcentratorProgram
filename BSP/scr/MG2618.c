/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "..\inc\MG2618.h"
#include "..\..\Projects\inc\Common_API.h"
#include "..\..\Projects\inc\SofTimer.h"
//#include "..\..\APP\inc\MCPX.h"
#include "..\..\APP\inc\GPRS.h"
/*********************************************************************************
����������
*********************************************************************************/
//AT ָ���б�
const char*  const CmdNameList_MG2618[] = { 
//2.1 ��ָͨ��
/*000*/ "A/",        //�ظ���һ��ָ��
/*001*/ "A",       //Ӧ�����
/*002*/ "D" ,      //�������
/*003*/ "DL",      //������������ĵ绰���� 
/*004*/ "E",      //ʹ�ܻ���ͨ��
/*005*/ "H",      //�Ͽ���ǰ����
/*006*/ "I",       //��ʾģ������������Ϣ 
/*007*/ "Q",       //�����Ƿ����ն˻��Է���ֵ
/*008*/ "+++",       //������ģʽ�л�������ģʽ 
/*009*/ "O",       //������ģʽ�л�������ģʽ 
/*010*/ "P",       //ʹ�����岦��
/*011*/ "S0",      //�Զ�Ӧ������
/*012*/ "+CRC",     //������������
/*013*/ "+CLVL",     //�ܻ�����
/*014*/ "+CLIP",     //�������������ʾ
/*015*/ "+ZSETMUTE", //��������
/*016*/ "+CIMI",    //��ѯ���� ID ��
/*017*/ "+CGMR",     //��ȡ��Ʒ�汾��
/*018*/ "+ECHO",     //������������
/*019*/ "+(C)GSN",   //��ȡ��ǰ IMEI ��
/*020*/ "+ZVERS",    //��ȡ��ǰ����汾��
/*021*/ "+CLCK" ,    //��������
/*022*/ "+CCFC",     //����ת�ƺ������������ 
/*023*/ "+CCWA",     //���еȴ�����
/*024*/ "+CHLD",     //���б��ֺͶ෽����
/*025*/ "*TSIMINS",  //��ѯ SIM ��״̬
/*026*/ "+CPWD",     //�޸�����
/*027*/ "+CGMI",     //��ѯģ�鳧��
/*028*/ "+CSCS",     //�ַ��������ѡ��
/*029*/ "+CLCC",     //����״̬��ѯ
//DTMFָ��
/*030*/ "+VTS",     //���� DTMF ��
//�������ָ�� 
/*031*/ "+CREG",    //����ע�������
/*032*/ "+COPS",    //����ѡ��
//�ƶ��豸���ƺ�״̬����
/*033*/ "+CPAS",    //ģ��״̬��ѯ
/*034*/ "+CFUN",    //����ģ�鹦��
/*035*/ "+CMEE",    //�ƶ��豸���󱨸�
/*036*/ "+ZPWROFF", //ģ��ػ�
/*037*/ "+CPIN",    //���� PIN ��
/*038*/ "+EPIN1",   //���� PIN1 ��
/*039*/ "+EPIN2",   //���� PIN2 ��
/*040*/ "+EPINC",   //��ѯ PIN1�� PIN2 ʣ���������
/*041*/ "+CSQ",     //�ź�ǿ�Ȳ�ѯ
/*042*/ "+CCLK",    //ʱ�ӹ���
// ��Ϣ����ָ�� 
/*043*/ "+CSCA",    //�������ĺ���
/*044*/ "+CNMA",    //����ȷ��
/*045*/ "+CMGF",    //���ö���Ϣģʽ
/*046*/ "+CNMI",    //���ö���ָʾ��ʽ
/*047*/ "+CMGR",    //������Ϣ
/*048*/ "+CMGW",    //д����Ϣ
/*049*/ "+CSMS",    //ѡ����ŷ���
/*050*/ "+CMGS",    //���Ͷ���
/*051*/ "+CPMS",    //��ѡ����Ϣ�洢��
/*052*/ "+CMGD",    //ɾ������
/*053*/ "+CMGL",    //����Ϣ�б�
/*054*/ "+CMSS",    //���ʹ����Ķ���
//�绰��ָ�� 
/*055*/ "+CPBS",    //ѡ��绰���洢
/*056*/ "+CPBR",    //��ȡ�绰����Ϣ
/*057*/ "+CPBW",    //��绰��д��Ϣ
/*058*/ "+CPBF",    //���ҵ绰����Ϣ
/*059*/ "+CNUM",    //��ȡ��������
//����ѹ��ָ�� 
/*060*/ "+IFC",       //������
/*061*/ "&D",           //���� DTR ģʽ
/*062*/ "&C",           //���� DCD ģʽ
/*063*/ "+IPR",       //�趨ģ�鲨����
/*064*/ "&F",           //�ָ���������
/*065*/ "&W",           //��������
//GPRS//ָ��
/*066*/ "+CGDCONT",       //���� PDP ��ʽ
/*067*/ "+CGACT",         //����/��� PDP ����
/*068*/ "+CGATT",         //���� GPRS ҵ�� 
/*069*/ "+CGCLASS",       //GPRS �豸�ȼ�
//ZTE ����ָ��
/*070*/ "+ZSTR",           //��ѯģ��״̬��Ϣ
/*071*/ "+ZGETICCID",      //��ȡ ICCID
/*072*/ "+ZCSQ",           //�����Զ���ʾ CSQ ������ 
/*073*/ "+ZEDT" ,          //���� DTR �ļ��ģʽ 
/*074*/ "+ZDSLEEP",        //32KHz ���˯��ģʽ
/*075*/ "+CUSD",           //���� USSD ����
/*076*/ "+ZRINGPINMODE",   //���� RING ���ŵ��ź�ģʽ 
/*077*/ "+ZVERSWITCH",     //���ư汾�л�
// �������ָ�� 
/*078*/ "+CSTT",           //���� APN���û��������� 
/*079*/ "+CIICR",        //�� GPRS �������� 
/*080*/ "+ZPPPCLOSE",       //�ر� GPRS �������� 
/*081*/ "+CIFSR",       //��ѯ��ǰģ�� IP ��ֵַ 
/*082*/ "+ZDNSSERV",        //���� DNS �������� IP ��ַ 
/*083*/ "+ZDNSGETIP",       //��ȡ��Ӧ Internet ������ IP ��ַ 
//TCP ����ָ�� 
/*084*/ "+CIPSTART",        //���� TCP ����������
/*085*/ "+CIPSEND" ,        //��Ŀ���ַ���� TCP ���� 
/*086*/ "+ZPPPSTATUS",      //��ѯ GPRS ����״̬ 
/*087*/ "+CIPCLOSE" ,       //�ر� TCP ����
/*088*/ "+ZIPSTATUS",       //��ѯ��ǰ TCP ����״̬ 
// UDP ����ָ�� 
/*089*/ "+CIPSTARTU",       //���� UDP ���������� 
/*090*/ "+CIPSENDU",        //�� UDP �������������� 
/*091*/ "+ZIPSTATUSU",      //��ѯ UDP ״̬
/*092*/ "+CIPCLOSEU",       //�ر� UDP ����
// SERVER ָ�� 
/*093*/ "+ZTCPLISTEN",  //�˿ڼ����������� 
/*094*/ "+ZTCPSENDP",   //�ڱ����򿪵���·�Ϸ������� 
/*095*/ "+ZTCPCLOSEP",  //�رռ�����������
/*096*/ "+ZTCPSTATUSP", //��ѯ��������·
/*097*/ "+ZIPTIMEOUT",  //����ģ�����ӷ������ͷ������ݵĳ�ʱʱ��
/*098*/ "+ZUDPLISTEN",  //�˿ڼ�����������
/*099*/ "+ZUDPSENDP" ,  //�ڱ����򿪵���·�Ϸ�������
// FTP ָ��
/*100*/ "+ZFTPLOGIN",     //��¼ FTP ������
/*101*/ "+ZFTPTYPE",      //���� FTP �ļ�����
/*102*/ "+ZFTPUPLOAD",    //�ϴ��ļ�
/*103*/ "+ZFTPSIZE",      //��ȡ�ļ���С
/*104*/ "+ZFTPDNLOAD",    //�����ļ�
/*105*/ "+ZFTPDNLOADEX",  //���� FTP �ļ�
/*106*/ "+ZFTPDEL",       //ɾ���ļ�
/*107*/ "+ZFTPQUIT",      //�˳� FTP 
// ͸������ָ�� 
/*108*/ "+ZTRANSFER",  //͸������
//��Ƶ���ָ�� 
/*109*/ "+ZCALLTONE",  //ժ��������
/*110*/ "+ZDTMFTONE",  //ZDTMF ���������� 
/*111*/ "+ZKTDSWITCH", //�� DTMF ��⹦��
/*112*/ "+ZKTDIND",    //�����ϱ����� DTMF ����
/*113*/ "+SPEAKER",    //����ͨ���л�ָ�� 
/*114*/ "+ZMICGB",     //���� MIC ��Ƶ����
//��վ���ָ��(ֱ��վ����ָ��) ...................................................... 78
/*115*/ "+CCED",       //��ȡС����Ϣ
/*116*/ "+ZBCCH",      //���� BCCH �ŵ����ݲ�֧�֣�
/*117*/ "+ZBAND",      //���� GSM Ƶ�� 
/*118*/ "+ZOPT",       //����������Ӫ��
/*119*/ "+ZCALIST",    //��ȡ�ز��б�
// TTS ���ָ��
/*120*/ "+ZTTS" ,      //TTS(Text to Speech)��������ҵ�� 
/*121*/ "+ZTTSP",      //���� TTS(Text to Speech)������������(�ݲ�֧���л�������)
// ¼�����ָ��
/*122*/ "+ZAUDREC",     //¼������
/*123*/ "+ZFILEREAD",   //��ȡ�ļ�
// GPS ���ָ�� 
/*124*/ "+ZGPSOPEN",     //���� GPS ָ��
/*125*/ "+ZGPSSLEEP",    //GPS ����ָ�� 
/*126*/ "+ZGPSNMEARPT",  //NMEA �ϱ�����ָ�� 
/*127*/ "+ZGPSLOC",      //��ȡ GPS ��λ��γ��ָ�� 
/*128*/ "+ZGPSUTC",      //��ȡ GPS ʱ��ָ��
/*139*/ "+ZGPSHS",       //��ȡ�Ƕ��ٶ�ָ��
/*130*/ "+ZGPSIND",       //GPS ��λ״̬�ϱ� 

/*131*/ "ATATATATATAT",       //

/*132*/ "+CIPMUX"       //GPS ��λ״̬�ϱ�
};   
/*********************************************************************************
��������������
*********************************************************************************/

/*********************************************************************************
�ⲿ����������
*********************************************************************************/

/*********************************************************************************
˽�б���������
*********************************************************************************/
MG2618DescriptorStruct MG2618Infr;
/*********************************************************************************
���Ա���������
*********************************************************************************/
Command_MG2618 Command_pin  = AT_Again;
/*********************************************************************************
�ڲ�����������
*********************************************************************************/
static SystemErrName CommandSerialization_MG2618(CommandDescriptor_MG2618 *Command);
static SystemErrName CSTT_PramFill(CommandDescriptor_MG2618 *Command);
static SystemErrName CIPMUX_PramFill(CommandDescriptor_MG2618 *Command);
static SystemErrName TcpUdpConnect_PramFill(CommandDescriptor_MG2618 *buff);
//static SystemErrName UpdSend_PramFill(CommandDescriptor_MG2618 *buff);
static SystemErrName TcpSend_PramFill(CommandDescriptor_MG2618 *buff);
static SystemErrName CIPCLOSE_PramFill(CommandDescriptor_MG2618 *buff);
static void AckTimeOut_MG2618 (void *p_tmr, void *p_arg);
static SystemErrName Default_ACK_Func(unsigned char *data,unsigned short length);
//static void TcpClose_ACK(DataBuffSpec_Struct *Data);
//static unsigned short MatchinString (unsigned char **data, const char **StringList, unsigned short ListSize);
//static unsigned char* AddIntToString(unsigned char* tp, unsigned long vlue);
static unsigned char GetData_TCP_UDP(DataBuffSpec_Struct *Data, DataBuffSpec_Struct* Rec);
//Ӧ�����
#if (0 == ATA_Command_EN )
  #define AT_Again_ACK_Func   P_NULL
# else
  #define AT_Again_ACK_Func   P_NULL   
  //SystemErrName AT_Again_ACK_Func_ACK_Func(unsigned char *data, unsigned short length);
#endif
//Ӧ�����
#if (0 == ATA_Command_EN )
  #define ATA_ACK_Func   P_NULL
# else
  SystemErrName ATA_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�������
#if (0 == ATD_Command_EN )
#define ATD_ACK_Func   P_NULL
# else
SystemErrName ATD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//������������ĵ绰���� 
#if (0 == ATDL_Command_EN )
#define ATDL_ACK_Func   P_NULL
# else
SystemErrName ATDL_ACK_Func(unsigned char *data, unsigned short length);
#endif
//ʹ�ܻ���ͨ��
#if (0 == ATE_Command_EN )
#define ATE_ACK_Func   P_NULL
# else
SystemErrName ATE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�Ͽ���ǰ����
#if (0 == ATH_Command_EN )
#define ATH_ACK_Func   P_NULL
# else
SystemErrName ATH_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ʾģ������������Ϣ
#if (0 == ATI_Command_EN )
#define ATI_ACK_Func   P_NULL
# else
SystemErrName ATI_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�����Ƿ����ն˻��Է���ֵ
#if (0 == ATQ_Command_EN )
#define ATQ_ACK_Func   P_NULL
# else
SystemErrName ATQ_ACK_Func(unsigned char *data, unsigned short length);
#endif
//������ģʽ�л�������ģʽ 
#if (0 == AtData_Command_EN )
#define AtData_ACK_Func   P_NULL
# else
SystemErrName AtData_ACK_Func(unsigned char *data, unsigned short length);
#endif
//������ģʽ�л�������ģʽ
#if (0 == ATO_Command_EN )
#define ATO_ACK_Func   P_NULL
# else
SystemErrName ATO_ACK_Func(unsigned char *data, unsigned short length); 
#endif
//ʹ�����岦��
#if (0 == ATP_Command_EN )
#define ATP_ACK_Func   P_NULL
# else
SystemErrName ATP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�Զ�Ӧ������
#if (0 == ATS0_Command_EN )
#define ATS0_ACK_Func   P_NULL
# else
SystemErrName ATS0_ACK_Func(unsigned char *data, unsigned short length);
#endif
//������������
#if (0 == AT_CRC_Command_EN )
#define AT_CRC_ACK_Func   P_NULL
# else
SystemErrName AT_CRC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�ܻ�����
#if (0 == CLVL_Command_EN )
#define CLVL_ACK_Func   P_NULL
# else
SystemErrName CLVL_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�������������ʾ
#if (0 == CLIP_Command_EN )
#define CLIP_ACK_Func   P_NULL
# else
SystemErrName CLIP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��������
#if (0 == ZSETMUTE_Command_EN )
#define ZSETMUTE_ACK_Func   P_NULL
# else
SystemErrName ZSETMUTE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ѯ���� ID ��
#if (0 == CIMI_Command_EN )
#define CIMI_ACK_Func   P_NULL
# else
SystemErrName CIMI_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ȡ��Ʒ�汾��
#if (0 == CGMR_Command_EN )
#define CGMR_ACK_Func   P_NULL
# else
SystemErrName CGMR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//������������
#if (0 == ECHO_Command_EN)
#define ECHO_ACK_Func   P_NULL
# else
SystemErrName ECHO_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ȡ��ǰ IMEI ��
#if (0 == GSN_Command_EN)
#define GSN_ACK_Func   P_NULL
# else
SystemErrName GSN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ȡ��ǰ����汾��
#if (0 == ZVERS_Command_EN)
#define ZVERS_ACK_Func   P_NULL
# else
SystemErrName ZVERS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��������
#if (0 == CLCK_Command_EN)
#define CLCK_ACK_Func   P_NULL
# else
SystemErrName CLCK_ACK_Func(unsigned char *data, unsigned short length);
#endif
//����ת�ƺ������������

#if (0 == CCFC_Command_EN)
#define CCFC_ACK_Func   P_NULL
# else
SystemErrName CCFC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���еȴ�����
#if (0 == CCWA_Command_EN)
#define CCWA_ACK_Func   P_NULL
# else
SystemErrName CCWA_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���б��ֺͶ෽����
#if (0 == CHLD_Command_EN)
#define CHLD_ACK_Func   P_NULL
# else
SystemErrName CHLD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ѯ SIM ��״̬
#if (0 == TSIMINS_Command_EN)
#define TSIMINS_ACK_Func   P_NULL
# else
SystemErrName TSIMINS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�޸�����
#if (0 == CPWD_Command_EN)
#define CPWD_ACK_Func   P_NULL
# else
SystemErrName CPWD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ѯģ�鳧��
#if (0 == CGMI_Command_EN)
#define CGMI_ACK_Func   P_NULL
# else
SystemErrName CGMI_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�ַ��������ѡ��
#if (0 == CSCS_Command_EN)
#define CSCS_ACK_Func   P_NULL
# else
SystemErrName CSCS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//����״̬��ѯ
#if (0 == CLCC_Command_EN)
#define CLCC_ACK_Func   P_NULL
# else
SystemErrName CLCC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� DTMF ��
#if (0 == VTS_Command_EN)
#define VTS_ACK_Func   P_NULL
# else
SystemErrName VTS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//����ע�������
#if (0 == CREG_Command_EN)
#define CREG_ACK_Func   P_NULL
# else
//�������ָ�� 
SystemErrName CREG_ACK_Func(unsigned char *data, unsigned short length);
#endif
//����ѡ��
#if (0 == COPS_Command_EN)
#define COPS_ACK_Func   P_NULL
# else
SystemErrName COPS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//ģ��״̬��ѯ
#if (0 == CPAS_Command_EN)
#define CPAS_ACK_Func   P_NULL
# else
SystemErrName CPAS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//����ģ�鹦��
#if (0 == CFUN_Command_EN)
#define CFUN_ACK_Func   P_NULL
# else
SystemErrName CFUN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�ƶ��豸���󱨸�
#if (0 == CMEE_Command_EN)
#define CMEE_ACK_Func   P_NULL
# else
SystemErrName CMEE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//ģ��ػ�
#if (0 == ZPWROFF_Command_EN)
#define ZPWROFF_ACK_Func   P_NULL
# else
SystemErrName ZPWROFF_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� PIN ��
#if (0 == CPIN_Command_EN)
#define CPIN_ACK_Func   P_NULL
# else
SystemErrName CPIN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� PIN1 ��
#if (0 == EPIN1_Command_EN)
#define EPIN1_ACK_Func   P_NULL
# else
SystemErrName EPIN1_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� PIN2 ��
#if (0 == EPIN2_Command_EN)
#define EPIN2_ACK_Func   P_NULL
# else
SystemErrName EPIN2_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ѯ PIN1�� PIN2 ʣ���������
#if (0 == EPINC_Command_EN)
#define EPINC_ACK_Func   P_NULL
# else
SystemErrName EPINC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�ź�ǿ�Ȳ�ѯ
#if (0 == CSQ_Command_EN)
#define CSQ_ACK_Func   P_NULL
# else
SystemErrName CSQ_ACK_Func(unsigned char *data, unsigned short length);
#endif
//ʱ�ӹ���
#if (0 == CCLK_Command_EN)
#define CCLK_ACK_Func   P_NULL
# else
SystemErrName CCLK_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�������ĺ���
#if (0 == CSCA_Command_EN)
#define CSCA_ACK_Func   P_NULL
# else
SystemErrName CSCA_ACK_Func(unsigned char *data, unsigned short length);
#endif
//����ȷ��
#if (0 == CNMA_Command_EN)
#define CNMA_ACK_Func   P_NULL
# else
SystemErrName CNMA_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���ö���Ϣģʽ
#if (0 == CMGF_Command_EN)
#define CMGF_ACK_Func   P_NULL
# else
SystemErrName CMGF_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���ö���ָʾ��ʽ
#if (0 == CNMI_Command_EN)
#define CNMI_ACK_Func   P_NULL
# else
SystemErrName CNMI_ACK_Func(unsigned char *data, unsigned short length);
#endif
//������Ϣ
#if (0 == CMGW_Command_EN)
#define CMGR_ACK_Func   P_NULL
# else
SystemErrName CMGR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//д����Ϣ
#if (0 == CMGW_Command_EN)
#define CMGW_ACK_Func   P_NULL
# else
SystemErrName CMGW_ACK_Func(unsigned char *data, unsigned short length);
#endif
//ѡ����ŷ���
#if (0 == CSMS_Command_EN)
#define CSMS_ACK_Func   P_NULL
# else
SystemErrName CSMS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���Ͷ���
#if (0 == CMGS_Command_EN)
#define CMGS_ACK_Func   P_NULL
# else
SystemErrName CMGS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ѡ����Ϣ�洢��
#if (0 == CPMS_Command_EN)
#define CPMS_ACK_Func   P_NULL
# else
SystemErrName CPMS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//ɾ������
#if (0 == CMGD_Command_EN)
#define CMGD_ACK_Func   P_NULL
# else
SystemErrName CMGD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//����Ϣ�б�
#if (0 == CMGL_Command_EN)
#define CMGL_ACK_Func   P_NULL
# else
SystemErrName CMGL_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���ʹ����Ķ���
#if (0 == CMSS_Command_EN)
#define CMSS_ACK_Func   P_NULL
# else
SystemErrName CMSS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//ѡ��绰���洢
#if (0 == CPBS_Command_EN)
#define CPBS_ACK_Func   P_NULL
# else 
SystemErrName CPBS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ȡ�绰����Ϣ
#if (0 == CPBR_Command_EN)
#define CPBR_ACK_Func   P_NULL
# else
SystemErrName CPBR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��绰��д��Ϣ
#if (0 == CPBW_Command_EN)
#define CPBW_ACK_Func   P_NULL
# else
SystemErrName CPBW_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���ҵ绰����Ϣ
#if (0 == CPBF_Command_EN)
#define CPBF_ACK_Func   P_NULL
# else
SystemErrName CPBF_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ȡ��������
#if (0 == CNUM_Command_EN)
#define CNUM_ACK_Func   P_NULL
# else
SystemErrName CNUM_ACK_Func(unsigned char *data, unsigned short length);
#endif
//������
#if (0 == IFC_Command_EN)
#define IFC_ACK_Func   P_NULL
# else
SystemErrName IFC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� DTR ģʽ
#if (0 == DTR_Command_EN)
#define DTR_ACK_Func   P_NULL
# else
SystemErrName DTR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� DCD ģʽ
#if (0 == DCD_Command_EN)
#define DCD_ACK_Func   P_NULL
# else
SystemErrName DCD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�趨ģ�鲨����
#if (0 == IPR_Command_EN)
#define IPR_ACK_Func   P_NULL
# else
SystemErrName IPR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�ָ���������
#if (0 == AT_F_Command_EN)
#define AT_F_ACK_Func   P_NULL
# else
SystemErrName AT_F_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��������
#if (0 == AT_W_Command_EN)
#define AT_W_ACK_Func   P_NULL
# else
SystemErrName AT_W_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� PDP ��ʽ
#if (0 == CGDCONT_Command_EN)
#define CGDCONT_ACK_Func   P_NULL
# else
SystemErrName CGDCONT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//����/��� PDP ����
#if (0 == CGACT_Command_EN)
#define CGACT_ACK_Func   P_NULL
# else
SystemErrName CGACT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� GPRS ҵ��
#if (0 == CGATT_Command_EN)
#define CGATT_ACK_Func   P_NULL
# else
SystemErrName CGATT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//GPRS �豸�ȼ�
#if (0 == CGCLASS_Command_EN)
#define CGCLASS_ACK_Func   P_NULL
# else
SystemErrName CGCLASS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ѯģ��״̬��Ϣ
#if (0 == ZSTR_Command_EN)
#define ZSTR_ACK_Func   P_NULL
# else
SystemErrName ZSTR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ȡ ICCID
#if (0 == ZGETICCID_Command_EN)
#define ZGETICCID_ACK_Func   P_NULL
# else
SystemErrName ZGETICCID_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�����Զ���ʾ CSQ ������
#if (0 == ZCSQ_Command_EN)
#define ZCSQ_ACK_Func   P_NULL
# else
SystemErrName ZCSQ_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� DTR �ļ��ģʽ
#if (0 == ZEDT_Command_EN)
#define ZEDT_ACK_Func   P_NULL
# else
SystemErrName ZEDT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//32KHz ���˯��ģʽ
#if (0 == ZDSLEEP_Command_EN)
#define ZDSLEEP_ACK_Func   P_NULL
# else
SystemErrName ZDSLEEP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� USSD ����
#if (0 == CUSD_Command_EN)
#define CUSD_ACK_Func   P_NULL
# else
SystemErrName CUSD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� RING ���ŵ��ź�ģʽ
#if (0 == ZRINGPINMODE_Command_EN)
#define ZRINGPINMODE_ACK_Func   P_NULL
# else
SystemErrName ZRINGPINMODE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���ư汾�л�
#if (0 == ZVERSWITCH_Command_EN)
#define ZVERSWITCH_ACK_Func   P_NULL
# else
SystemErrName ZVERSWITCH_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� APN���û��������� 
#if (0 == CSTT_Command_EN)
#define CSTT_ACK_Func   P_NULL
# else
SystemErrName CSTT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�� GPRS �������� 
#if (0 == CIICR_Command_EN)  
#define CIICR_ACK_Func   P_NULL
# else
SystemErrName CIICR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�ر� GPRS ��������
#if (0 == ZPPPCLOSE_Command_EN)
#define ZPPPCLOSE_ACK_Func   P_NULL
# else
SystemErrName ZPPPCLOSE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ѯ��ǰģ�� IP ��ֵַ
#if (0 == CIFSR_Command_EN)
#define CIFSR_ACK_Func   P_NULL
# else
SystemErrName CIFSR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� DNS �������� IP ��ַ 
#if (0 == ZDNSSERV_Command_EN)
#define ZDNSSERV_ACK_Func   P_NULL
# else
SystemErrName ZDNSSERV_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ȡ��Ӧ Internet ������ IP ��ַ
#if (0 == ZDNSGETIP_Command_EN)
#define ZDNSGETIP_ACK_Func   P_NULL
# else
SystemErrName ZDNSGETIP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� TCP ����������
#if (0 == CIPSTART_Command_EN)
#define CIPSTART_ACK_Func   P_NULL
# else 
SystemErrName CIPSTART_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��Ŀ���ַ���� TCP ���� 
#if (0 == CIPSEND_Command_EN)
#define CIPSEND_ACK_Func   P_NULL
# else
SystemErrName CIPSEND_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ѯ GPRS ����״̬ 
#if (0 == ZPPPSTATUS_Command_EN)
#define ZPPPSTATUS_ACK_Func   P_NULL
# else
SystemErrName ZPPPSTATUS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�ر� TCP ����
#if (0 == CIPCLOSE_Command_EN)
#define CIPCLOSE_ACK_Func   P_NULL
# else
SystemErrName CIPCLOSE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ѯ��ǰ TCP ����״̬
#if (0 == ZIPSTATUS_Command_EN)
#define ZIPSTATUS_ACK_Func   P_NULL
# else
SystemErrName ZIPSTATUS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� UDP ���������� 
#if (0 == CIPSTARTU_Command_EN)
#define CIPSTARTU_ACK_Func   P_NULL
# else
SystemErrName CIPSTARTU_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�� UDP ��������������
#if (0 == CIPSENDU_Command_EN)
#define CIPSENDU_ACK_Func   P_NULL
# else
SystemErrName CIPSENDU_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ѯ UDP ״̬
#if (0 == ZIPSTATUSU_Command_EN)
#define ZIPSTATUSU_ACK_Func   P_NULL
# else
SystemErrName ZIPSTATUSU_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�ر� UDP ����
#if (0 == CIPCLOSEU_Command_EN)
#define CIPCLOSEU_ACK_Func   P_NULL
# else
SystemErrName CIPCLOSEU_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�˿ڼ����������� 
#if (0 == ZTCPLISTEN_Command_EN)
#define ZTCPLISTEN_ACK_Func   P_NULL
# else
SystemErrName ZTCPLISTEN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�ڱ����򿪵���·�Ϸ�������
#if (0 == ZTCPSENDP_Command_EN)
#define ZTCPSENDP_ACK_Func   P_NULL
# else
SystemErrName ZTCPSENDP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�رռ�����������
#if (0 == ZTCPCLOSEP_Command_EN)
#define ZTCPCLOSEP_ACK_Func   P_NULL
# else
SystemErrName ZTCPCLOSEP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ѯ��������·
#if (0 == ZTCPSTATUSP_Command_EN)
#define ZTCPSTATUSP_ACK_Func   P_NULL
# else
SystemErrName ZTCPSTATUSP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//����ģ�����ӷ������ͷ������ݵĳ�ʱʱ��
#if (0 == ZIPTIMEOUT_Command_EN)
#define ZIPTIMEOUT_ACK_Func   P_NULL
# else
SystemErrName ZIPTIMEOUT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�˿ڼ�����������
#if (0 == ZUDPLISTEN_Command_EN)
#define ZUDPLISTEN_ACK_Func   P_NULL
# else
SystemErrName ZUDPLISTEN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�ڱ����򿪵���·�Ϸ�������
#if (0 == ZUDPSENDP_Command_EN)
#define ZUDPSENDP_ACK_Func   P_NULL
# else
SystemErrName ZUDPSENDP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��¼ FTP ������
#if (0 == ZFTPLOGIN_Command_EN)
#define ZFTPLOGIN_ACK_Func   P_NULL
# else
SystemErrName ZFTPLOGIN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� FTP �ļ�����
#if (0 == ZFTPTYPE_Command_EN)
#define ZFTPTYPE_ACK_Func   P_NULL
# else
SystemErrName ZFTPTYPE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�ϴ��ļ�
#if (0 == ZFTPUPLOAD_Command_EN)
#define ZFTPUPLOAD_ACK_Func   P_NULL
# else
SystemErrName ZFTPUPLOAD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ȡ�ļ���С
#if (0 == ZFTPSIZE_Command_EN)
#define ZFTPSIZE_ACK_Func   P_NULL
# else
SystemErrName ZFTPSIZE_ACK_Func(unsigned char *data, unsigned short length); 
#endif
//�����ļ�
#if (0 == ZFTPDNLOAD_Command_EN)
#define ZFTPDNLOAD_ACK_Func   P_NULL
# else
SystemErrName ZFTPDNLOAD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� FTP �ļ�
#if (0 == ZFTPDNLOADEX_Command_EN)
#define ZFTPDNLOADEX_ACK_Func   P_NULL
# else
SystemErrName ZFTPDNLOADEX_ACK_Func(unsigned char *data, unsigned short length);
#endif
//ɾ���ļ�
#if (0 == ZFTPDEL_Command_EN)
#define ZFTPDEL_ACK_Func   P_NULL
# else
SystemErrName ZFTPDEL_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�˳� FTP
#if (0 == ZFTPQUIT_Command_EN)
#define ZFTPQUIT_ACK_Func   P_NULL
# else
SystemErrName ZFTPQUIT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//͸������
#if (0 == ZTRANSFER_Command_EN)
#define ZTRANSFER_ACK_Func   P_NULL
# else
SystemErrName ZTRANSFER_ACK_Func(unsigned char *data, unsigned short length);
#endif
//ժ��������
#if (0 == ZCALLTONE_Command_EN)
#define ZCALLTONE_ACK_Func   P_NULL
# else
SystemErrName ZCALLTONE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//ZDTMF ���������� 
#if (0 == ZDTMFTONE_Command_EN)
#define ZDTMFTONE_ACK_Func   P_NULL
# else
SystemErrName ZDTMFTONE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�� DTMF ��⹦��
#if (0 == ZKTDSWITCH_Command_EN)
#define ZKTDSWITCH_ACK_Func   P_NULL
# else
SystemErrName ZKTDSWITCH_ACK_Func(unsigned char *data, unsigned short length);
#endif
//�����ϱ����� DTMF ����
#if (0 == ZKTDIND_Command_EN)
#define ZKTDIND_ACK_Func   P_NULL
# else
SystemErrName ZKTDIND_ACK_Func(unsigned char *data, unsigned short length);
#endif
//����ͨ���л�ָ�� 
#if (0 == SPEAKER_Command_EN)
#define SPEAKER_ACK_Func   P_NULL
# else
SystemErrName SPEAKER_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� MIC ��Ƶ����
#if (0 == ZMICGB_Command_EN)
#define ZMICGB_ACK_Func   P_NULL
# else
SystemErrName ZMICGB_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ȡС����Ϣ
#if (0 == CCED_Command_EN)
#define CCED_ACK_Func   P_NULL
# else
SystemErrName CCED_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� BCCH �ŵ����ݲ�֧�֣�
#if (0 == ZBCCH_Command_EN)
#define ZBCCH_ACK_Func   P_NULL
# else
SystemErrName ZBCCH_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� GSM Ƶ�� 
#if (0 == ZBAND_Command_EN)
#define ZBAND_ACK_Func   P_NULL
# else
SystemErrName ZBAND_ACK_Func(unsigned char *data, unsigned short length);
#endif
//����������Ӫ��
#if (0 == ZOPT_Command_EN)
#define ZOPT_ACK_Func   P_NULL
# else
SystemErrName ZOPT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ȡ�ز��б�
#if (0 == ZCALIST_Command_EN)
#define ZCALIST_ACK_Func   P_NULL
# else
SystemErrName ZCALIST_ACK_Func(unsigned char *data, unsigned short length);
#endif
//TTS(Text to Speech)��������ҵ��
#if (0 == ZTTS_Command_EN)
#define ZTTS_ACK_Func   P_NULL
# else
SystemErrName ZTTS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� TTS(Text to Speech)������������(�ݲ�֧���л�������)
#if (0 == ZTTSP_Command_EN)
#define ZTTSP_ACK_Func   P_NULL
# else
SystemErrName ZTTSP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//¼������
#if (0 == ZAUDREC_Command_EN)
#define ZAUDREC_ACK_Func   P_NULL
# else
SystemErrName ZAUDREC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ȡ�ļ�
#if (0 == ZFILEREAD_Command_EN)
#define ZFILEREAD_ACK_Func   P_NULL
# else
SystemErrName ZFILEREAD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//���� GPS ָ��
#if (0 == ZGPSOPEN_Command_EN)
#define ZGPSOPEN_ACK_Func   P_NULL
# else
SystemErrName ZGPSOPEN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//GPS ����ָ�� 
#if (0 == ZGPSSLEEP_Command_EN)
#define ZGPSSLEEP_ACK_Func   P_NULL
# else
SystemErrName ZGPSSLEEP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//NMEA �ϱ�����ָ��
#if (0 == ZGPSNMEARPT_Command_EN)
#define ZGPSNMEARPT_ACK_Func   P_NULL
# else
SystemErrName ZGPSNMEARPT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ȡ GPS ��λ��γ��ָ��
#if (0 == ZGPSLOC_Command_EN)
#define ZGPSLOC_ACK_Func   P_NULL
# else
SystemErrName ZGPSLOC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ȡ GPS ʱ��ָ��
#if (0 == ZGPSUTC_Command_EN)
#define ZGPSUTC_ACK_Func   P_NULL
# else
SystemErrName ZGPSUTC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//��ȡ�Ƕ��ٶ�ָ��
#if (0 == ZGPSHS_Command_EN)
#define ZGPSHS_ACK_Func   P_NULL
# else
SystemErrName ZGPSHS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//GPS ��λ״̬�ϱ�
#if (0 == ZGPSIND_Command_EN)
#define ZGPSIND_ACK_Func   P_NULL
# else
SystemErrName ZGPSIND_ACK_Func(unsigned char *data, unsigned short length);
#endif

//
#if (0 == SyncRate_Command_EN)
#define SyncRate_ACK_Func   P_NULL
# else
SystemErrName SyncRate_ACK_Func(unsigned char *data, unsigned short length);
#endif

//
#if (0 == CIPMUX_Command_EN)
#define CIPMUX_ACK_Func   P_NULL
# else
SystemErrName CIPMUX_ACK_Func(unsigned char *data, unsigned short length);
#endif

typedef SystemErrName (*Command_ACK_Func) (unsigned char *data, unsigned short length);
const Command_ACK_Func Command_ACK_FuncList_MG2618[] = { 
//2.1 ��ָͨ��
 AT_Again_ACK_Func,  //= 0,   //��ָ�������ظ���һ��ָ��
 ATA_ACK_Func,       //= 1,   //Ӧ�����
 ATD_ACK_Func,       //= 2,   //�������
 ATDL_ACK_Func,      //= 3,   //������������ĵ绰���� 
 ATE_ACK_Func,       //= 4,   //ʹ�ܻ���ͨ��
 ATH_ACK_Func,       //= 5,   //�Ͽ���ǰ����
 ATI_ACK_Func,       //= 6,   //��ʾģ������������Ϣ 
 ATQ_ACK_Func,       //= 7,   //�����Ƿ����ն˻��Է���ֵ
 AtData_ACK_Func,    //= 8,   //������ģʽ�л�������ģʽ 
 ATO_ACK_Func,       //= 9,   //������ģʽ�л�������ģʽ 
 ATP_ACK_Func,       //= 10,  //ʹ�����岦��
 ATS0_ACK_Func,      //= 11,  //�Զ�Ӧ������
 AT_CRC_ACK_Func,    //= 12,  //������������
 CLVL_ACK_Func,      //= 13,  //�ܻ�����
 CLIP_ACK_Func,      //= 14,  //�������������ʾ
 ZSETMUTE_ACK_Func,  //= 15,  //��������
 CIMI_ACK_Func,      //= 16,  //��ѯ���� ID ��
 CGMR_ACK_Func,      //= 17,  //��ȡ��Ʒ�汾��
 ECHO_ACK_Func,      //= 18,  //������������
 GSN_ACK_Func,       //= 19,  //��ȡ��ǰ IMEI ��
 ZVERS_ACK_Func,     //= 20,  //��ȡ��ǰ����汾��
 CLCK_ACK_Func,      //= 21,  //��������
 CCFC_ACK_Func,      //= 22,  //����ת�ƺ������������ 
 CCWA_ACK_Func,      //= 23,  //���еȴ�����
 CHLD_ACK_Func,      //= 24,  //���б��ֺͶ෽����
 TSIMINS_ACK_Func,   //= 25,  //��ѯ SIM ��״̬
 CPWD_ACK_Func,      //= 26,  //�޸�����
 CGMI_ACK_Func,      //= 27,  //��ѯģ�鳧��
 CSCS_ACK_Func,      //= 28,  //�ַ��������ѡ��
 CLCC_ACK_Func,      //= 29,  //����״̬��ѯ
//DTMFָ��
 VTS_ACK_Func,     //= 30,  //���� DTMF ��
//�������ָ�� 
 CREG_ACK_Func,    //= 31,  //����ע�������
 COPS_ACK_Func,    //= 32,  //����ѡ��
//�ƶ��豸���ƺ�״̬����
 CPAS_ACK_Func,    //= 33,  //ģ��״̬��ѯ
 CFUN_ACK_Func,    //= 34,  //����ģ�鹦��
 CMEE_ACK_Func,    //= 35,  //�ƶ��豸���󱨸�
 ZPWROFF_ACK_Func, //= 36,  //ģ��ػ�
 CPIN_ACK_Func,    //= 37,  //���� PIN ��
 EPIN1_ACK_Func,   //= 38,  //���� PIN1 ��
 EPIN2_ACK_Func,   //= 39,  //���� PIN2 ��
 EPINC_ACK_Func,   //= 40,  //��ѯ PIN1�� PIN2 ʣ���������
 CSQ_ACK_Func,     //= 41,  //�ź�ǿ�Ȳ�ѯ
 CCLK_ACK_Func,    //= 42,  //ʱ�ӹ���
// ��Ϣ����ָ�� 
 CSCA_ACK_Func,    //= 43,    //�������ĺ���
 CNMA_ACK_Func,    //= 44,    //����ȷ��
 CMGF_ACK_Func,    //= 45,    //���ö���Ϣģʽ
 CNMI_ACK_Func,    //= 46,    //���ö���ָʾ��ʽ
 CMGR_ACK_Func,    //= 47,    //������Ϣ
 CMGW_ACK_Func,    //= 48,    //д����Ϣ
 CSMS_ACK_Func,    //= 49,    //ѡ����ŷ���
 CMGS_ACK_Func,    //= 50,    //���Ͷ���
 CPMS_ACK_Func,    //= 51,    //��ѡ����Ϣ�洢��
 CMGD_ACK_Func,    //= 52,    //ɾ������
 CMGL_ACK_Func,    //= 53,    //����Ϣ�б�
 CMSS_ACK_Func,    //= 54,    //���ʹ����Ķ���
//�绰��ָ�� 
 CPBS_ACK_Func,    //= 55,    //ѡ��绰���洢
 CPBR_ACK_Func,    //= 56,    //��ȡ�绰����Ϣ
 CPBW_ACK_Func,    //= 57,    //��绰��д��Ϣ
 CPBF_ACK_Func,    //= 58,    //���ҵ绰����Ϣ
 CNUM_ACK_Func,    //= 59,    //��ȡ��������
//����ѹ��ָ�� 
 IFC_ACK_Func,     //= 60,    //������
 DTR_ACK_Func,     //= 61,    //���� DTR ģʽ
 DCD_ACK_Func,     //= 62,    //���� DCD ģʽ
 IPR_ACK_Func,     //= 63,    //�趨ģ�鲨����
 AT_F_ACK_Func,      //= 64,    //�ָ���������
 AT_W_ACK_Func,       //= 65,   //��������
//GPRSָ��
 CGDCONT_ACK_Func, //= 66,    //���� PDP ��ʽ
 CGACT_ACK_Func,   //= 67,    //����/��� PDP ����
 CGATT_ACK_Func,   //= 68,    //���� GPRS ҵ�� 
 CGCLASS_ACK_Func, //= 69,    //GPRS �豸�ȼ�
//ZTE ����ָ��
 ZSTR_ACK_Func,         // = 70,    //��ѯģ��״̬��Ϣ
 ZGETICCID_ACK_Func,    // = 71,    //��ȡ ICCID
 ZCSQ_ACK_Func,         // = 72,    //�����Զ���ʾ CSQ ������ 
 ZEDT_ACK_Func,         // = 73,    //���� DTR �ļ��ģʽ 
 ZDSLEEP_ACK_Func,      // = 74,    //32KHz ���˯��ģʽ
 CUSD_ACK_Func,         // = 75,    //���� USSD ����
 ZRINGPINMODE_ACK_Func, // = 76,    //���� RING ���ŵ��ź�ģʽ 
 ZVERSWITCH_ACK_Func,    //= 77,    //���ư汾�л�
// �������ָ�� 
 CSTT_ACK_Func,         //= 78,    //���� APN���û��������� 
 CIICR_ACK_Func,      //= 79,    //�� GPRS �������� 
 ZPPPCLOSE_ACK_Func,     //= 80,    //�ر� GPRS �������� 
 CIFSR_ACK_Func,      //= 81,    //��ѯ��ǰģ�� IP ��ֵַ 
 ZDNSSERV_ACK_Func,     // = 82,    //���� DNS �������� IP ��ַ 
 ZDNSGETIP_ACK_Func,    // = 83,    //��ȡ��Ӧ Internet ������ IP ��ַ 
//TCP ����ָ�� 
 CIPSTART_ACK_Func,     // = 84,   //���� TCP ����������
 CIPSEND_ACK_Func,       //= 85,   //��Ŀ���ַ���� TCP ���� 
 ZPPPSTATUS_ACK_Func,    //= 86,   //��ѯ GPRS ����״̬ 
 CIPCLOSE_ACK_Func,      //= 87,   //�ر� TCP ����
 ZIPSTATUS_ACK_Func,     //= 88,   //��ѯ��ǰ TCP ����״̬ 
// UDP ����ָ�� 
 CIPSTARTU_ACK_Func,     //= 89,  //���� UDP ���������� 
 CIPSENDU_ACK_Func,      //= 90,  //�� UDP �������������� 
 ZIPSTATUSU_ACK_Func,    //= 91,  //��ѯ UDP ״̬
 CIPCLOSEU_ACK_Func,     //= 92,  //�ر� UDP ����
// SERVER ָ�� 
 ZTCPLISTEN_ACK_Func,   /// = 93,  //�˿ڼ����������� 
 ZTCPSENDP_ACK_Func,     //= 94,  //�ڱ����򿪵���·�Ϸ������� 
 ZTCPCLOSEP_ACK_Func,    //= 95,  //�رռ�����������
 ZTCPSTATUSP_ACK_Func,   //= 96,  //��ѯ��������·
 ZIPTIMEOUT_ACK_Func,    //= 97,  //����ģ�����ӷ������ͷ������ݵĳ�ʱʱ��
 ZUDPLISTEN_ACK_Func,    //= 98,  //�˿ڼ�����������
 ZUDPSENDP_ACK_Func,     //= 99,  //�ڱ����򿪵���·�Ϸ�������
// FTP ָ��
 ZFTPLOGIN_ACK_Func,     //= 100, //��¼ FTP ������
 ZFTPTYPE_ACK_Func,      //= 101, //���� FTP �ļ�����
 ZFTPUPLOAD_ACK_Func,    //= 102, //�ϴ��ļ�
 ZFTPSIZE_ACK_Func,      //= 103, //��ȡ�ļ���С
 ZFTPDNLOAD_ACK_Func,    //= 104, //�����ļ�
 ZFTPDNLOADEX_ACK_Func,  //= 105, //���� FTP �ļ�
 ZFTPDEL_ACK_Func,       //= 106, //ɾ���ļ�
 ZFTPQUIT_ACK_Func,      //= 107, //�˳� FTP 
// ͸������ָ�� 
 ZTRANSFER_ACK_Func,     //= 108, //͸������
//��Ƶ���ָ�� 
 ZCALLTONE_ACK_Func,     //= 109, //ժ��������
 ZDTMFTONE_ACK_Func,    // = 110, //ZDTMF ���������� 
 ZKTDSWITCH_ACK_Func,    //= 111, //�� DTMF ��⹦��
 ZKTDIND_ACK_Func,       //= 112, //�����ϱ����� DTMF ����
 SPEAKER_ACK_Func,       //= 113, //����ͨ���л�ָ�� 
 ZMICGB_ACK_Func,        //= 114, //���� MIC ��Ƶ����
//��վ���ָ��(ֱ��վ����ָ��) 
 CCED_ACK_Func,          //= 115, //��ȡС����Ϣ
 ZBCCH_ACK_Func,         //= 116, //���� BCCH �ŵ����ݲ�֧�֣�
 ZBAND_ACK_Func,         //= 117, //���� GSM Ƶ�� 
 ZOPT_ACK_Func,          //= 118, //����������Ӫ��
 ZCALIST_ACK_Func,       //= 119, //��ȡ�ز��б�
// TTS ���ָ��
 ZTTS_ACK_Func,          //= 120, //TTS(Text to Speech)��������ҵ�� 
 ZTTSP_ACK_Func,         //= 121, //���� TTS(Text to Speech)������������(�ݲ�֧���л�������)
// ¼�����ָ��
 ZAUDREC_ACK_Func,       //= 122, //¼������
 ZFILEREAD_ACK_Func,     //= 123, //��ȡ�ļ�
// GPS ���ָ�� 
 ZGPSOPEN_ACK_Func,      //= 124, //���� GPS ָ��
 ZGPSSLEEP_ACK_Func,    // = 125, //GPS ����ָ�� 
 ZGPSNMEARPT_ACK_Func,  // = 126, //NMEA �ϱ�����ָ�� 
 ZGPSLOC_ACK_Func,      // = 127, //��ȡ GPS ��λ��γ��ָ�� 
 ZGPSUTC_ACK_Func,      // = 128, //��ȡ GPS ʱ��ָ��
 ZGPSHS_ACK_Func,       // = 129, //��ȡ�Ƕ��ٶ�ָ��
 ZGPSIND_ACK_Func,      // = 130,  //GPS ��λ״̬�ϱ� 
   
 SyncRate_ACK_Func,      //= 131,
 CIPMUX_ACK_Func         //= 132
 //NC_Command_MG2618 = 255 
};
/*********************************************************************************
���ܴ��붨����
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                // 
 Output:        //
 Return:        //
 Others:        //
                  
*********************************************************************************/
SystemErrName SendCmd_MG2618(CommandDescriptor_MG2618 *Command,
                             GPRS_FinishCallBack CallBack,
                             unsigned char Reps,
                             unsigned short timer)
{
  SystemErrName  err;
  SystemErrName  err2;  
  Command_pin = Command->Cmd;
  if(P_NULL == Command_ACK_FuncList_MG2618[Command->Cmd])
  {
   if(AT_Again != Command->Cmd)
   {return CmdValid;}
  }
  if(OFF == MG2618Infr.ModInf.Status)  
  {return Port_OFF;}     //�˿ڹر�
  if(WaitAck == MG2618Infr.ModInf.Status)  
  {return PortBusy;}     //�˿ڶ˷�æ
  
  MG2618Infr.Cnt = Reps;
    
  err = CommandSerialization_MG2618(Command);
  if(err > Warning) 
  {return err;} 
  MG2618Infr.Command.Step = 0; 
  //��ȡ�����ʱ��
  MG2618Infr.TimeOut = SofTimerCreate( timer,             //����
                                      TimerTB_10MS,       //ʱ��
                                      SofTimer_PERIODIC,  //ģʽ
                                      AckTimeOut_MG2618,  //�ص�
                                      P_NULL);//�ص�����
 if(P_NULL == MG2618Infr.TimeOut)
 {return Nothing;}  //��ȡ�����ʱ��ʧ�� 
 if(AT_Again != Command->Cmd)
 {MG2618Infr.Command.Cmd = Command->Cmd;}
 MG2618Infr.CallBack = CallBack;
 err2 = SendStringToMG2618(MG2618Infr.String); 
 if(NO_ERR == err2)
 {
  //MG2618Infr.Cnt = 1; 
  MG2618Infr.ModInf.Status = WaitAck;
  SofTimerStart(MG2618Infr.TimeOut);
  GprsStop_time = 0;  
 }
 else
 {SofTimerDel(&(MG2618Infr.TimeOut));}
 if(err2 > err)
 {err = err2;}  
 return err;  
}
/*********************************************************************************
 Function:      //
 Description:   // ģ���������ݽ���
Input:          // data     ���������׵�ַ
                // length   �������ݳ���
                //
                //   
 Output:        //
 Return:        //
 Others:        //              
*********************************************************************************/
void Decode_MG2618(unsigned char *gprs_ch, DataBuffSpec_Struct *Data)
{
 SystemErrName err = DataValid; 
 unsigned short itemp = 0; 
 DataBuffSpec_Struct Rec;
 //Ѱ�Ҳ�����ʼ�ֽ�
 *gprs_ch = 255;
 
 if(WaitAck == MG2618Infr.ModInf.Status)
 {
   if(CIPSEND == MG2618Infr.Command.Cmd)
   {
     Command_ACK_FuncList_MG2618[MG2618Infr.Command.Cmd](&((Data->pbuff)[itemp]),Data->length);
     return;
   }
   else
   {
     while (1)
     {
       if(IsLETTER(*(Data->pbuff)) 
          ||Isletter(*(Data->pbuff))
            ||IsDigit(*(Data->pbuff))
              ||('+'== *(Data->pbuff))
                ||('>'==*(Data->pbuff)))
       {break;}
       if(0 == --(Data->length)){return;}
       Data->pbuff ++;
     }
     if(WaitAck == MG2618Infr.ModInf.Status)
     {
       if((('A'==(Data->pbuff)[0])&&('T'==(Data->pbuff)[1]))
          ||(('A'==(Data->pbuff)[0])&&('/'==(Data->pbuff)[1])))
       { //��������
         itemp = 0;
         while (1)
         {
           if(('\r'==(Data->pbuff)[itemp])
              ||('\n'==(Data->pbuff)[itemp]))
           {break;}
           if(0 == --(Data->length))
           { return;}
           itemp ++;
         }
         while (1)
         {//Ѱ�Ҳ�����ʼ�ֽ�
           if('+'==(Data->pbuff)[itemp]
              ||IsLETTER((Data->pbuff)[itemp])
                ||Isletter((Data->pbuff)[itemp])
                  ||IsDigit((Data->pbuff)[itemp])
                    ||('>'==(Data->pbuff)[itemp]))
           {break;}
           if(0 == --(Data->length)){ return;}
           itemp ++;
         }
       }
       if(P_NULL != Command_ACK_FuncList_MG2618[MG2618Infr.Command.Cmd])
       {
         err = Command_ACK_FuncList_MG2618[MG2618Infr.Command.Cmd](&((Data->pbuff)[itemp]),Data->length);
         if(NO_ERR == err){return;} 
       }
     }
   }
 }
 
 const char*  const IndicatorList_MG2618[] = { 
   "+RECEIVE:",        //��ʾ�ӵ�ǰ������·�������� 
   "0, CLOSED\r\n",       //��ʾTCPͨ���ر���ʾ 
   "1, CLOSED\r\n",       //��ʾTCPͨ���ر���ʾ
   "2, CLOSED\r\n",       //��ʾTCPͨ���ر���ʾ
   "3, CLOSED\r\n",       //��ʾTCPͨ���ر���ʾ
   "4, CLOSED\r\n",       //��ʾTCPͨ���ر���ʾ
   "5, CLOSED\r\n",       //��ʾTCPͨ���ر���ʾ
 }; 
 while(Data->length)
 {
   if(IsDigit(*(Data->pbuff))||('+'==*(Data->pbuff)))
   {
     itemp = MatchinString(&(Data->pbuff),(const char**)IndicatorList_MG2618, 7);   //
     switch(itemp)
     {
     case 0 :   //��ʾ���� TCP ���� 
       {
         *gprs_ch = GetData_TCP_UDP(Data, &Rec);      
         if(255 == *gprs_ch)
         {
           Data->pbuff += sizeof("+RECEIVE");
         }
         break;
       }
     case 1 :   //��ʾTCPͨ���ر���ʾ
     case 2 :   //��ʾTCPͨ���ر���ʾ 
     case 3 :   //��ʾTCPͨ���ر���ʾ 
     case 4 :   //��ʾTCPͨ���ر���ʾ 
     case 5 :   //��ʾTCPͨ���ر���ʾ  
     case 6 :   //��ʾTCPͨ���ر���ʾ   
       {
         GprsClose_UEV(itemp-1);
         //Data->pbuff += sizeof(IndicatorList_MG2618[itemp]);
         //Data->length -=sizeof(IndicatorList_MG2618[itemp]);
         break;
       }
     } 
   }
   if(Data->length > 0)
   { 
     Data->length --;
     Data->pbuff ++;
   }
 }
 Data->length = Rec.length;
 Data->pbuff = Rec.pbuff;
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          // 
                // 
                // 
 Output:        //
 Return:        //
 Others:        //
                  
*********************************************************************************/
unsigned short CompString (unsigned char *data,const char *sp,unsigned char **p)
{
 unsigned short length = 0;
 unsigned char *tp; 
 //�ȶԲ��Ҳ�����
 tp = data;
 while(sp[length] == tp[length])
 {length ++;}
 if('\0'== sp[length])
 {
   if(P_NULL != p)
   {*p = &tp[length];}
   return length;
 }
 else
 {
   if(P_NULL != p)
   {*p = data;}
   return 0;
 }
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                // 
 Output:        //
 Return:        //
 Others:        //
                  
*********************************************************************************/
void StopListen_MG2618(void)
{
 if(P_NULL != MG2618Infr.TimeOut)
 {SofTimerDel(&(MG2618Infr.TimeOut));}
 MG2618Infr.ModInf.Status = Idle; 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
FunctionalState Get_MG2168PowerState (void)
{
 if(0 == MG2618_PowerFlg)
 {return DISABLE;}
 else
 {return ENABLE;}
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void PowerOff_GPRS (void)
{
 unsigned char *p ;
 unsigned short itmp; 
 GPIO_InitTypeDef        GPIO_InitStructure;
 
 GprsClose_UEV(1);
 GprsClose_UEV(2);
 GprsClose_UEV(3);
 GprsClose_UEV(4);
 
 SofTimerDel(&(MG2618Infr.HeartBeatTime));
 SofTimerDel(&(MG2618Infr.TimeOut)) ;
 
 GPIO_SetBits(GPIOA, GPIO_Pin_2);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 USART_Cmd(USART2, DISABLE);
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);
 MG2618_PowerOFF( );
 MG2618Infr.ModInf.Status = OFF;
 
 p = (unsigned char *)&MG2618Infr;
 itmp = 0;
 //ģ��������������
 while(itmp < sizeof(MG2618DescriptorStruct))
 {p[itmp++] = 0;}
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                //
                //
                //   
 Output:        //
 Return:        //
 Others:        //
                  
*********************************************************************************/
static SystemErrName CommandSerialization_MG2618 (CommandDescriptor_MG2618 *Command)
{
  SystemErrName err = NO_ERR;
  unsigned short  itemp = 0;
  
  while (itemp < MG2616_ATORDER_LEN)
  {MG2618Infr.String[itemp++]  = '\0';} 
  
  switch(Command->Cmd)
  {
  case  AtData:
  case  AT_Again:  
    {break;}
  default:     //��ѯ UDP ״̬
    {AddString(MG2618Infr.String, "AT");break;}
  }
  AddString(MG2618Infr.String,(unsigned char*)CmdNameList_MG2618[Command->Cmd]); 
  
  switch(Command->Format)
  {
  case  Void:
    {break;}
  case  Query:
    {AddString(MG2618Infr.String,"?");break; }
  case  Help:
    {AddString(MG2618Infr.String,"=?");break; }   
  case  Param:
    {
      if(Param == Command->Format)
      {
        itemp = 0; 
        while(itemp < sizeof(CommandParameter_MG2618))
        {
          ((unsigned char*)&MG2618Infr.Command.CmdPram)[itemp] = ((unsigned char*)&(Command->CmdPram))[itemp];
          itemp++ ;
        }
      }
      switch(Command->Cmd)
      {
      case  CSTT: 
        {err = CSTT_PramFill(Command);break;}
      case  CIPSTART:
      case  CIPSTARTU:
        {err = TcpUdpConnect_PramFill(Command);break;}
      case CIPSEND:         //��Ŀ���ַ���� TCP ���� 
        {err = TcpSend_PramFill(Command);break;}
      //case CIPSENDU:      //�� UDP �������������� 
       // {err = UpdSend_PramFill(Command); break; }
      case CIPCLOSE:      //�ر� TCP ����
        {err = CIPCLOSE_PramFill(Command);break;}
      case CIPMUX:
        {err = CIPMUX_PramFill(Command);break;}
      default:     //��ѯ UDP ״̬
        {err = CmdValid;break;}
      }
      break;
    }
  default:     //��ѯ UDP ״̬
    {err = CmdValid; break;}
  }  
  if(NO_ERR == err)
  {AddString(MG2618Infr.String, "\r\n");}
  return err;
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                //
                //
                //
 Output:        //
 Return:        //
 Others:        //AT+ZPNUM=<APN>
                //AT+ZPNUM="cmnet"
*********************************************************************************/
static SystemErrName CSTT_PramFill(CommandDescriptor_MG2618 *Command)
{
 unsigned char* p;

 p = MG2618Infr.String;
 while('\0'!= *p)
 { p++;}
 
 *p++ = '=';
 if(P_NULL != Command->CmdPram.ZPNUM.APN)
 { p =  AddString(p, Command->CmdPram.ZPNUM.APN);}
 return NO_ERR; 

}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                //
                //
                //
 Output:        //
 Return:        //
 Others:        //AT+ZPNUM=<APN>,<USER>,<PWD>,<MODE>,<AUTH>
                //AT+ZPNUM="cmnet","user","pwd",1,0
*********************************************************************************/
static SystemErrName CIPMUX_PramFill(CommandDescriptor_MG2618 *Command)
{
  unsigned char* p;
  
  p = MG2618Infr.String;
  while('\0'!= *p)
  { p++;}
  
  *p++ = '=';
  *p++ = Command->CmdPram.CIPMUX.mode;
  return NO_ERR;
  
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                //
                //
                //
 Output:        //
 Return:        //
 Others:        //AT+CIPSTART=<N>,<IP>,<SERVER_PORT>[,<LOCAL_PORT>]
                  AT+CIPSTART=1,61.144.216.219,2332
*********************************************************************************/
static SystemErrName TcpUdpConnect_PramFill(CommandDescriptor_MG2618 *Command)
{
 unsigned char* p;

 if(Command->CmdPram.TcpUdp_Connect.CH > 3)
  {return IllegalParam;}
 p = MG2618Infr.String;
 while('\0'!= *p)
 { p++;}
 *p++ = '='; 
 
 *p++ = Command->CmdPram.TcpUdp_Connect.CH +'0';
 *p++ = ',';
 
 if(TCP == Command->CmdPram.TcpUdp_Connect.Mode)
 { p = AddString(p, "TCP");}
 else{p = AddString(p, "UDP");}
 *p++ = ',';
 
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Connect.IP[0]);
 *p++ = '.'; 
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Connect.IP[1]);
 *p++ = '.'; 
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Connect.IP[2]);
 *p++ = '.'; 
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Connect.IP[3]);
 *p++ = ','; 
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Connect.PORT);
 
 return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                // 
 Output:        //
 Return:        //
 Others:        // AT+CIPSEND= port,length<CR>
                   AT+CIPSEND=1,10
                   >abcdefghij
                   +CIPSEND:OK
                   OK
                   ��ʾ'>'���ٷ�������
*********************************************************************************/
static SystemErrName TcpSend_PramFill(CommandDescriptor_MG2618 *Command)
{
 unsigned char* p;
 if(Command->CmdPram.TcpUdp_Send.CH >3)
  {return IllegalParam;}
 p = MG2618Infr.String;
 while('\0'!= *p){p++;}
 *p++ = '='; 
 *p++ = Command->CmdPram.TcpUdp_Send.CH +'0';
 *p++ = ',';
 if(Command->CmdPram.TcpUdp_Send.LENGTH > 4096)
 {return IllegalParam;  }
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Send.LENGTH);
 return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                // 
 Output:        //
 Return:        //
 Others:        // AT+CIPSEND= port,length<CR>
                   AT+CIPSEND=1,10
                   >abcdefghij
                   +CIPSEND:OK
                   OK
                   ��ʾ'>'���ٷ�������
*********************************************************************************/
/*
static SystemErrName UpdSend_PramFill(CommandDescriptor_MG2618 *Command)
{
 unsigned char* p;

 if((Command->CmdPram.TcpUdp_Connect.CH < 1)
     ||(Command->CmdPram.TcpUdp_Connect.CH > 4))
  {return IllegalParam;}
 
 p = MG2618Infr.String;
 while('\0'!= *p){p++;}
  
 *p++ = Command->CmdPram.TcpUdp_Send.CH +'0';
 *p++ = ',';
 if(Command->CmdPram.TcpUdp_Send.LENGTH > 1000)
 {return IllegalParam;  }
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Send.LENGTH);
 return NO_ERR; 
}*/
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                // 
 Output:        //
 Return:        //
 Others:        // AT+CIPCLOSE=<N>
*********************************************************************************/
static SystemErrName CIPCLOSE_PramFill(CommandDescriptor_MG2618 *Command)
{
 unsigned char* p;
 if(Command->CmdPram.CIPCLOSE.CH > 3)
  {return IllegalParam;}
 p = MG2618Infr.String;
 while('\0'!= *p){p++;};
  *p++ = '=';
 *p++ = Command->CmdPram.CIPCLOSE.CH +'0';
 return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void AckTimeOut_MG2618 (void *p_tmr, void *p_arg)
{
  //if(CIPSEND == MG2618Infr.Command.Cmd)
  //{
   // CIPSEND_ACK_Func("> ", 2);
  //}
  //else 
  if(MG2618Infr.Cnt-- > 0)
  {
    MG2618Infr.Command.Step = 0;
    SendStringToMG2618(MG2618Infr.String);
  }
  else
  {
   StopListen_MG2618( ); 
   if(P_NULL != MG2618Infr.CallBack)
   {MG2618Infr.CallBack(MG2618Infr.Command.Cmd,P_NULL);}
  }
}
/*********************************************************************************
 Function:      //
 Description:   //Ӧ�����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATA_Command_EN)
SystemErrName ATA_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATD_Command_EN)
SystemErrName ATD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //������������ĵ绰����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATDL_Command_EN)
SystemErrName ATDL_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //ʹ�ܻ���ͨ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATE_Command_EN)
SystemErrName ATE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�Ͽ���ǰ����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATH_Command_EN)
SystemErrName ATH_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ʾģ������������Ϣ 
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATI_Command_EN)
SystemErrName ATI_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�����Ƿ����ն˻��Է���ֵ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATQ_Command_EN)
SystemErrName ATQ_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //������ģʽ�л�������ģʽ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != AtData_Command_EN)
SystemErrName AtData_ACK_Func(unsigned char *data, unsigned short length) 
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //������ģʽ�л�������ģʽ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATO_Command_EN)
SystemErrName ATO_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //ʹ�����岦��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATP_Command_EN)
SystemErrName ATP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�Զ�Ӧ������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATS0_Command_EN)
SystemErrName ATS0_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //������������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != AT_CRC_Command_EN)
SystemErrName AT_CRC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�ܻ�����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CLVL_Command_EN)
SystemErrName CLVL_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�������������ʾ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CLIP_Command_EN)
SystemErrName CLIP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZSETMUTE_Command_EN)
SystemErrName ZSETMUTE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ѯ���� ID ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIMI_Command_EN)
SystemErrName CIMI_ACK_Func(unsigned char *data, unsigned short length)
{ 
 return Default_ACK_Func(data, length);
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ȡ��Ʒ�汾��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CGMR_Command_EN)
SystemErrName CGMR_ACK_Func(unsigned char *data, unsigned short length)
{
 unsigned short  itemp; 
 itemp = CompString(data,CmdNameList_MG2618[CGMR],&data);
 if(0 != itemp)
 {
  length -= itemp;  
  return Default_ACK_Func(data, length);
 }
 else
 {return DataValid;}  
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //������������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ECHO_Command_EN)
SystemErrName ECHO_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ȡ��ǰ IMEI ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != GSN_Command_EN)
SystemErrName GSN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ȡ��ǰ����汾��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZVERS_Command_EN)
SystemErrName ZVERS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CLCK_Command_EN)
SystemErrName CLCK_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //����ת�ƺ������������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CCFC_Command_EN)
SystemErrName CCFC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���еȴ�����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CCWA_Command_EN)
SystemErrName CCWA_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���б��ֺͶ෽����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CHLD_Command_EN)
SystemErrName CHLD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ѯ SIM ��״̬
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != TSIMINS_Command_EN)
SystemErrName TSIMINS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�޸�����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPWD_Command_EN)
SystemErrName CPWD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ѯģ�鳧��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CGMI_Command_EN)
SystemErrName CGMI_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�ַ��������ѡ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CSCS_Command_EN)
SystemErrName CSCS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //����״̬��ѯ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CLCC_Command_EN)
SystemErrName CLCC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� DTMF ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != VTS_Command_EN)
//DTMFָ��
SystemErrName VTS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //����ע�������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CREG_Command_EN)
//�������ָ�� 
SystemErrName CREG_ACK_Func(unsigned char *data, unsigned short length)
{
 unsigned short  itemp; 
 itemp = CompString(data,CmdNameList_MG2618[CREG],&data);
 if(0 != itemp)
 {
  length -= itemp;  
  return Default_ACK_Func(data, length);
 }
 else
 {return DataValid;} 
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //����ѡ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != COPS_Command_EN)
SystemErrName COPS_ACK_Func(unsigned char *data, unsigned short length)
{
 //������������ʵ�ִ���
 unsigned short  itemp; 
 itemp = CompString(data,CmdNameList_MG2618[COPS],&data);
 if(0 != itemp)
 {
  length -= itemp;  
  return Default_ACK_Func(data, length);
 }
 else
 {return DataValid;} 
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //ģ��״̬��ѯ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPAS_Command_EN)
//�ƶ��豸���ƺ�״̬����
SystemErrName CPAS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //����ģ�鹦��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CFUN_Command_EN)
SystemErrName CFUN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�ƶ��豸���󱨸�
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMEE_Command_EN)
SystemErrName CMEE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //ģ��ػ�
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZPWROFF_Command_EN)
SystemErrName ZPWROFF_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� PIN ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPIN_Command_EN)
SystemErrName CPIN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� PIN1 ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != EPIN1_Command_EN)
SystemErrName EPIN1_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� PIN2 ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != EPIN2_Command_EN)
SystemErrName EPIN2_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ѯ PIN1�� PIN2 ʣ���������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != EPINC_Command_EN)
SystemErrName EPINC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�ź�ǿ�Ȳ�ѯ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CSQ_Command_EN)
SystemErrName CSQ_ACK_Func(unsigned char *data, unsigned short length)
{
 DataBuffSpec_Struct AckData ;
 unsigned short  itemp; 
 unsigned short  itemp1;
 itemp = CompString(data,CmdNameList_MG2618[CSQ],&data);
 if(0 != itemp)
 {
  length -= itemp;  
  while (1)
  {
   if(IsDigit(*data)){break;}
   if(0 == --length){ return DataValid;}
   data ++;
  }
  AckData.pbuff = data;
  
  itemp = 0;
  while (1)
  {
   if(',' == *data){break;}
   itemp *= 10;
   itemp += (*data) - '0';
   if((0 == --length)||('\r' == *data)||('\n' == *data)){ return DataValid;}  
   data ++;
  }
  while (1)
  {
   if(IsDigit(*data)){break;}
   if(0 == --length){ return DataValid;}
   data ++;
  }
  itemp1 = 0;
  while (1)
  {
   if(('\r' ==*data)||('\n' == *data)){break;}
   itemp1 *= 10;
   itemp1 += (*data) - '0';
   if(0 == --length){ return DataValid;}  
   data ++;
  }
  MG2618Infr.ModInf.Rssi = itemp;
  MG2618Infr.ModInf.Ber = itemp1;
  
  if(P_NULL !=MG2618Infr.CallBack)
  {
    AckData.length = 0;
    while (--length)
    {
    if(('\r'== AckData.pbuff[AckData.length])||('\n' == AckData.pbuff[AckData.length+1]))
    {AckData.length ++;break;}  
    if(('O'== AckData.pbuff[AckData.length])||('K' == AckData.pbuff[AckData.length+1]))
    {AckData.length ++;break;} 
     AckData.length ++;
    }
    MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);
  }
  else
  {StopListen_MG2618( );}
  return NO_ERR;
 }
 else
 {return DataValid;}   
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //ʱ�ӹ���
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CCLK_Command_EN)
SystemErrName CCLK_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�������ĺ���
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CSCA_Command_EN)
// ��Ϣ����ָ�� 
SystemErrName CSCA_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //����ȷ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CNMA_Command_EN)
SystemErrName CNMA_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���ö���Ϣģʽ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMGF_Command_EN)
SystemErrName CMGF_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���ö���ָʾ��ʽ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CNMI_Command_EN)
SystemErrName CNMI_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //������Ϣ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMGR_Command_EN)
SystemErrName CMGR_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //д����Ϣ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMGW_Command_EN)
SystemErrName CMGW_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //ѡ����ŷ���
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CSMS_Command_EN)
SystemErrName CSMS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���Ͷ���
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMGS_Command_EN)
SystemErrName CMGS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ѡ����Ϣ�洢��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPMS_Command_EN)
SystemErrName CPMS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //ɾ������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMGD_Command_EN)
SystemErrName CMGD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //����Ϣ�б�
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMGL_Command_EN)
SystemErrName CMGL_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���ʹ����Ķ���
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMSS_Command_EN)
SystemErrName CMSS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //ѡ��绰���洢
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPBS_Command_EN)
//�绰��ָ�� 
SystemErrName CPBS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ȡ�绰����Ϣ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPBR_Command_EN)
SystemErrName CPBR_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��绰��д��Ϣ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPBW_Command_EN)
SystemErrName CPBW_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���ҵ绰����Ϣ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPBF_Command_EN)
SystemErrName CPBF_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ȡ��������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CNUM_Command_EN)
SystemErrName CNUM_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != IFC_Command_EN)
SystemErrName IFC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� DTR ģʽ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != DTR_Command_EN)
SystemErrName DTR_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� DCD ģʽ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != DCD_Command_EN)
SystemErrName DCD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�趨ģ�鲨����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != IPR_Command_EN)
SystemErrName IPR_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�ָ���������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != AT_F_Command_EN)
SystemErrName AT_F_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != AT_W_Command_EN)
SystemErrName AT_W_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� PDP ��ʽ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CGDCONT_Command_EN)
SystemErrName CGDCONT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //����/��� PDP ����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CGACT_Command_EN)
SystemErrName CGACT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� GPRS ҵ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CGATT_Command_EN)
SystemErrName CGATT_ACK_Func(unsigned char *data, unsigned short length)
{
  unsigned short  itemp; 
 itemp = CompString(data,CmdNameList_MG2618[CGATT],&data);
 if(0 != itemp)
 {
  length -= itemp;  
  return Default_ACK_Func(data, length);
 }
 else
 {return DataValid;}

}
#endif
/*********************************************************************************
 Function:      //
 Description:   //GPRS �豸�ȼ�
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CGCLASS_Command_EN)
SystemErrName CGCLASS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ѯģ��״̬��Ϣ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZSTR_Command_EN)
//ZTE ����ָ��
SystemErrName ZSTR_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ȡ ICCID
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGETICCID_Command_EN)
SystemErrName ZGETICCID_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�����Զ���ʾ CSQ ������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZCSQ_Command_EN)
SystemErrName ZCSQ_ACK_Func(unsigned char *data, unsigned short length)//�����Զ���ʾ CSQ ������
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� DTR �ļ��ģʽ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZEDT_Command_EN)
SystemErrName ZEDT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //32KHz ���˯��ģʽ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZDSLEEP_Command_EN)
SystemErrName ZDSLEEP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� USSD ����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CUSD_Command_EN)
SystemErrName CUSD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� RING ���ŵ��ź�ģʽ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZRINGPINMODE_Command_EN)
SystemErrName ZRINGPINMODE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���ư汾�л�
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZVERSWITCH_Command_EN)
SystemErrName ZVERSWITCH_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� APN���û���������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CSTT_Command_EN)
SystemErrName CSTT_ACK_Func(unsigned char *data, unsigned short length)
{
 DataBuffSpec_Struct AckData ;
 
  while(length)
 {
   if(IsLETTER(*(data))) {break;}
    length--;
   data++;
 }
 if(CompString(data, "OK", P_NULL)||(CompString(data, "ERROR", P_NULL)))
 {
   if(P_NULL !=MG2618Infr.CallBack)
   {
     AckData.length = length;
     AckData.pbuff = data;
     MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);
   } 
 }
 return NO_ERR;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�� GPRS ��������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIICR_Command_EN)
SystemErrName CIICR_ACK_Func(unsigned char *data, unsigned short length)
{
  DataBuffSpec_Struct AckData ;
  while(length)
  {
    if(IsLETTER(*(data))) {break;}
    length--;
    data++;
  }
  if(CompString(data, "OK", P_NULL)||(CompString(data, "ERROR", P_NULL)))
  {
    if(P_NULL !=MG2618Infr.CallBack)
    {
      AckData.length = length;
      AckData.pbuff = data;
      MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);
    } 
  }
 return NO_ERR;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�ر� GPRS ��������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZPPPCLOSE_Command_EN)
SystemErrName ZPPPCLOSE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ѯ��ǰģ�� IP ��ֵַ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIFSR_Command_EN)
SystemErrName CIFSR_ACK_Func(unsigned char *data, unsigned short length)
{
 unsigned short  itemp; 
 unsigned char   IP[4] = {0,0,0,0};      //����IP��ַ 
 DataBuffSpec_Struct  AckData; 
  while (1)
  {
   if(IsDigit(*data)){break;}
   if(0 == --length){return DataValid;}
   data ++;
  }
  AckData.pbuff = data;
  AckData.length = length;
  itemp = 0;
  while (1)
  {
   if(('\r'== *data)||('\n' == *data)){break;}  
   if(0 == --length){return DataValid;}
   AckData.length ++;
   if('.'== *data){itemp ++; itemp %= 4;}
   if(IsDigit(*data))
   {IP[itemp] *= 10; IP[itemp] += *data - '0';  }
   data ++;
  }
  
  MG2618Infr.ModInf.LOCAL_IP[0] = IP[0];
  MG2618Infr.ModInf.LOCAL_IP[1] = IP[1];
  MG2618Infr.ModInf.LOCAL_IP[2] = IP[2];
  MG2618Infr.ModInf.LOCAL_IP[3] = IP[3];
  if(P_NULL !=MG2618Infr.CallBack) 
  {MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);}
  return NO_ERR;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� DNS �������� IP ��ַ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZDNSSERV_Command_EN)
SystemErrName ZDNSSERV_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ȡ��Ӧ Internet ������ IP ��ַ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZDNSGETIP_Command_EN)
SystemErrName ZDNSGETIP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� TCP ����������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIPSTART_Command_EN)
SystemErrName CIPSTART_ACK_Func(unsigned char *data, unsigned short length)
{
  unsigned char ch;
  GprsInfr_Struct GprsInfr;
  DataBuffSpec_Struct AckData ;
  
  while(length)
  {
    if(IsLETTER(*(data))||IsDigit(*(data))) {break;}
    length--;
    data++;
  }
  if(IsLETTER(*(data)))
  {
    if(CompString(data, "OK", &data)||(CompString(data, "ERROR", &data)))
    {
      while(length)
      {
        if(IsDigit(*(data))||IsLETTER(*(data))) {break;}
        length--;
        data++;
      }
      if(length > 0)
      {
        if(P_NULL !=MG2618Infr.CallBack)
        {
          AckData.length = length;
          AckData.pbuff = data;
          MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);
        } 
      }
    }
    return NO_ERR;
  }
  else if(IsDigit(*(data)))
  {
    AckData.length = length;
    AckData.pbuff = data;
    
    ch = *(data++) - '0';
    length -- ;
    if(ch == MG2618Infr.Command.CmdPram.TcpUdp_Connect.CH)
    {
      while(length)
      {
        if(IsLETTER(*(data))) {break;}
        length--;
        data++;
      }
      if(CompString(data,"CONNECT OK",P_NULL))
      {
        GprsInfr.IP[0] = MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[0];
        GprsInfr.IP[1] = MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[1];
        GprsInfr.IP[2] = MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[2];
        GprsInfr.IP[3] = MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[3];
        GprsInfr.PORT = MG2618Infr.Command.CmdPram.TcpUdp_Connect.PORT;
        GprsConnect_OK(MG2618Infr.Command.CmdPram.TcpUdp_Connect.CH, &GprsInfr);
      }
    }
    if(AckData.length > 0)
    {
      if(P_NULL !=MG2618Infr.CallBack)
      {
        AckData.length = length;
        AckData.pbuff = data;
        MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);
      } 
    }
    return NO_ERR;
  }
  else
  {return NO_ERR;}
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��Ŀ���ַ���� TCP ����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void CIPSEND_OK_Func(CommPortNumber_Type port)//, void *pbuff)
{
  StopListen_MG2618( );
  if(P_NULL !=MG2618Infr.Command.CmdPram.TcpUdp_Send.callBack)
  {MG2618Infr.Command.CmdPram.TcpUdp_Send.callBack(port,MG2618Infr.Command.CmdPram.TcpUdp_Send.buff);}
}
/*********************************************************************************
 Function:      //
 Description:   //��Ŀ���ַ���� TCP ����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIPSEND_Command_EN)
SystemErrName CIPSEND_ACK_Func(unsigned char *data, unsigned short length)
{
 CommPortNumber_Type  Port = GPRS_CH0;
 unsigned short itemp  = 0;
 
 switch (MG2618Infr.Command.Step)
 {
 case 0:
   {
     while(length--)
     {
       if('>'== *data)
       {
         if(P_NULL != MG2618Infr.TimeOut)
         {SofTimerRset(MG2618Infr.TimeOut);}  
         SendDataToMG2618(MG2618Infr.Command.CmdPram.TcpUdp_Send.buff,
                          MG2618Infr.Command.CmdPram.TcpUdp_Send.LENGTH,
                          P_NULL);
         MG2618Infr.Command.Step++;
         break; 
       }
       data ++;
     }
     break;     
   }
 case 1:
   {
     if(length == MG2618Infr.Command.CmdPram.TcpUdp_Send.LENGTH)
     {MG2618Infr.Command.Step++;}
     break; 
   }
 case 2:
   {
     while(length--)
     {
       if(IsDigit(*(data)))
       {break; }
       data ++;
     }
     itemp = *(data) - '0';
     while(length--)
     {
       if(IsLETTER(*(data)))
       {break; }
       data ++;
     }  
     if(CompString(data,"SEND OK",P_NULL))
     {
       if(itemp == MG2618Infr.Command.CmdPram.TcpUdp_Send.CH)
       {
         Port += itemp;
         CIPSEND_OK_Func(Port);
       }
     }
     
   }  
 }
 return NO_ERR;  
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ѯ GPRS ����״̬
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZPPPSTATUS_Command_EN)
SystemErrName ZPPPSTATUS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�ر� TCP ����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIPCLOSE_Command_EN)
SystemErrName CIPCLOSE_ACK_Func(unsigned char *data, unsigned short length)
{
  DataBuffSpec_Struct  AckData;
  
  while(length)
  {
    if(IsLETTER(*data))
    {
      if(CompString(data,"CLOSE OK",P_NULL))
      {
        if(P_NULL !=MG2618Infr.CallBack)
        {
          AckData.length = length;
          AckData.pbuff = data; 
          MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);
        }
        GprsClose_UEV(MG2618Infr.Command.CmdPram.CIPCLOSE.CH);
        StopListen_MG2618( );
        return NO_ERR;
      }
      else
      {
        while(length) 
        { 
          if(!IsLETTER(*(data))){ break;}
          data ++;
         length --;
        }
        continue;
      }
    }
    length--;
    data ++;
  }
 return NO_ERR;
} 
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ѯ��ǰ TCP ����״̬ 
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZIPSTATUS_Command_EN)
SystemErrName ZIPSTATUS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� UDP ����������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIPSTARTU_Command_EN)
// UDP ����ָ�� 
SystemErrName CIPSTARTU_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�� UDP ��������������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIPSENDU_Command_EN)
SystemErrName CIPSENDU_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ѯ UDP ״̬
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZIPSTATUSU_Command_EN)
SystemErrName ZIPSTATUSU_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�ر� UDP ����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIPCLOSEU_Command_EN)
SystemErrName CIPCLOSEU_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�˿ڼ�����������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTCPLISTEN_Command_EN)
SystemErrName ZTCPLISTEN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�ڱ����򿪵���·�Ϸ�������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTCPSENDP_Command_EN)
SystemErrName ZTCPSENDP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�رռ�����������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTCPCLOSEP_Command_EN)
SystemErrName ZTCPCLOSEP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ѯ��������·
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTCPSTATUSP_Command_EN)
SystemErrName ZTCPSTATUSP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //����ģ�����ӷ������ͷ������ݵĳ�ʱʱ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZIPTIMEOUT_Command_EN)
SystemErrName ZIPTIMEOUT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�˿ڼ�����������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZUDPLISTEN_Command_EN)
SystemErrName ZUDPLISTEN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�ڱ����򿪵���·�Ϸ�������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZUDPSENDP_Command_EN)
SystemErrName ZUDPSENDP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��¼ FTP ������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPLOGIN_Command_EN)
// FTP ָ��
SystemErrName ZFTPLOGIN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� FTP �ļ�����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPTYPE_Command_EN)
SystemErrName ZFTPTYPE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�ϴ��ļ�
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPUPLOAD_Command_EN)
SystemErrName ZFTPUPLOAD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ȡ�ļ���С
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPSIZE_Command_EN)
SystemErrName ZFTPSIZE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�����ļ�
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPDNLOAD_Command_EN)
SystemErrName ZFTPDNLOAD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� FTP �ļ�
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPDNLOADEX_Command_EN)
SystemErrName ZFTPDNLOADEX_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //ɾ���ļ�
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPDEL_Command_EN)
SystemErrName ZFTPDEL_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�˳� FTP
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPQUIT_Command_EN)
SystemErrName ZFTPQUIT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //͸������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTRANSFER_Command_EN)
// ͸������ָ�� 
SystemErrName ZTRANSFER_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //ժ��������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZCALLTONE_Command_EN)
//��Ƶ���ָ�� 
SystemErrName ZCALLTONE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //ZDTMF ����������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZDTMFTONE_Command_EN)
SystemErrName ZDTMFTONE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�� DTMF ��⹦��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZKTDSWITCH_Command_EN)
SystemErrName ZKTDSWITCH_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //�����ϱ����� DTMF ����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZKTDIND_Command_EN)
SystemErrName ZKTDIND_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //����ͨ���л�ָ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != SPEAKER_Command_EN)
SystemErrName SPEAKER_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� MIC ��Ƶ����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZMICGB_Command_EN)
SystemErrName ZMICGB_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ȡС����Ϣ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CCED_Command_EN)
SystemErrName CCED_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� BCCH �ŵ����ݲ�֧�֣�
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZBCCH_Command_EN)
SystemErrName ZBCCH_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� GSM Ƶ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZBAND_Command_EN)
SystemErrName ZBAND_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //����������Ӫ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZOPT_Command_EN)
SystemErrName ZOPT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ȡ�ز��б�
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZCALIST_Command_EN)
SystemErrName ZCALIST_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //TTS(Text to Speech)��������ҵ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTTS_Command_EN)
SystemErrName ZTTS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� TTS(Text to Speech)������������(�ݲ�֧���л�������)
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTTSP_Command_EN)
SystemErrName ZTTSP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //¼������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZAUDREC_Command_EN)
SystemErrName ZAUDREC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ȡ�ļ�
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFILEREAD_Command_EN)
SystemErrName ZFILEREAD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //���� GPS ָ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSOPEN_Command_EN)
// GPS ���ָ�� 
SystemErrName ZGPSOPEN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //GPS ����ָ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSSLEEP_Command_EN)
SystemErrName ZGPSSLEEP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //NMEA �ϱ�����ָ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSNMEARPT_Command_EN)
SystemErrName ZGPSNMEARPT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;

}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ȡ GPS ��λ��γ��ָ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSLOC_Command_EN)
SystemErrName ZGPSLOC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ȡ GPS ʱ��ָ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSUTC_Command_EN)
SystemErrName ZGPSUTC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err; 
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //��ȡ�Ƕ��ٶ�ָ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSHS_Command_EN)
SystemErrName ZGPSHS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err; 
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //GPS ��λ״̬�ϱ�
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSIND_Command_EN)
SystemErrName ZGPSIND_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 ������������ʵ�ִ��� 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != SyncRate_Command_EN)
SystemErrName SyncRate_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR; 
 DataBuffSpec_Struct  AckData;
 
 AckData.length = CompString(data, "AT", &(AckData.pbuff));
 if(0 != AckData.length)
 {
   if(P_NULL !=MG2618Infr.CallBack)
   {MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);} 
 }
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIPMUX_Command_EN)
SystemErrName CIPMUX_ACK_Func(unsigned char *data, unsigned short length)
{
 DataBuffSpec_Struct  AckData;
 while(length)
 {
   if(IsLETTER(*(data))) {break;}
    length--;
   data++;
 }
 if(CompString(data, "OK", P_NULL)||(CompString(data, "ERROR", P_NULL)))
 {
   if(P_NULL !=MG2618Infr.CallBack)
   {
     AckData.length = length;
     AckData.pbuff = data;
     MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);
   } 
 }
 return NO_ERR;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName Default_ACK_Func(unsigned char *data, unsigned short length)
{
 DataBuffSpec_Struct  AckData;
 if(P_NULL !=MG2618Infr.CallBack)
 {
  while (1)
  {
   if(IsDigit(*data) || IsLETTER(*data) || Isletter(*data)){break;}
   if(0 == --length){return DataValid;}
   data ++;
  }
  AckData.pbuff = data;
  AckData.length = length;
  {MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);}
 }
 return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static unsigned char GetData_TCP_UDP(DataBuffSpec_Struct *Data, DataBuffSpec_Struct* Rec)
{
  unsigned char  CH = 255;
  
  while (Data->length)
  {
   if(IsDigit(*(Data->pbuff))){break;}
   Data->pbuff ++;
   Data->length --;
  }
  if(IsDigit(*(Data->pbuff)))
  {
    CH = *(Data->pbuff) - '0';
    if(CH > 6)
    {
      return 255;
    }
    Data->pbuff ++;
    Data->length --;
  }
  
  while (Data->length)
  {
   if(IsDigit(*(Data->pbuff))){break;}
   Data->pbuff ++;
   Data->length --;
  }
  
  Rec->length = 0; 
  while ((Data->length > 0)&&(IsDigit(*(Data->pbuff))))
  {
    Rec->length *= 10; 
    Rec->length += *(Data->pbuff) - '0';
    Data->pbuff ++;
    Data->length --;
  }
  while (Data->length)
  {
    if(('\r' == Data->pbuff[0])&&('\n' == Data->pbuff[1]))
    {
      Rec->pbuff = &Data->pbuff[2];
      Data->length -=2;
      if(Data->length < Rec->length)
      {
        Data->length = 0;
        CH = 255;
      }
      else
      {
       Data->length -= Rec->length;
       Data->pbuff = Rec->pbuff + Rec->length;
      }
      break;
    }
    Data->pbuff ++;
    Data->length --;
  }
  return CH;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
/*static void TcpClose_ACK(DataBuffSpec_Struct *Data)
{
  unsigned char  CH;
  while (1)
  {
   if(IsDigit(*(Data->pbuff))){break;}
   if(0 == --(Data->length)){return ;}
   Data->pbuff ++;
  }
  while (1)
  {
   if(!IsDigit(*(Data->pbuff))){break;}  
   if(0 == --(Data->length)){ return;}
   if(IsDigit(*(Data->pbuff)))
   {CH = *(Data->pbuff) - '0'; break; }
   Data->pbuff ++;
  }
 GprsClose_UEV(CH);
}*/
/***************************************END*****************************************/
