/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "NetWork.h"
#include "includes.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
#define IFNAME0 'e'
#define IFNAME1 'n'
#define LINK_SPEED_OF_YOUR_NETIF_IN_BPS  10000000
/*********************************************************************************
公共变量定义区
*********************************************************************************/
 struct netif   ENC28J60Netif;
/*********************************************************************************
外部变量声明区
*********************************************************************************/
/*********************************************************************************
私有数据类型定义区
*********************************************************************************/
struct ethernetif 
 {
  struct eth_addr *ethaddr;
 };

 unsigned long data_recv_cnt = 0 ;
/*********************************************************************************
私有变量定义区
*********************************************************************************/
static OS_ERR err;
static CPU_TS ts; 
static OS_MUTEX   Enc28j60_mutex;

OS_TCB  Enc28j60_input_thread_TCB;
static CPU_STK  ethernetif_inputStk[APP_TASK_START_STK_SIZE];

/*********************************************************************************
测试变量定义区
*********************************************************************************/
static unsigned char aa  = 0;
/*********************************************************************************
内部函数定义区
*********************************************************************************/
static void ethernetif_input( struct netif *netif);
static void Enc28j60_input_thread( void *p_arg );
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
static void low_level_init(struct netif *netif)
{
  
  //UBYTE	__ubOldPrio;
  //UBYTE	__ubErr;
  //struct ethernetif *ethernetif = netif->state;
  /* set MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  /* set MAC hardware address */
  netif->hwaddr[0] = 0x4a;
  netif->hwaddr[1] = 0x78;
  netif->hwaddr[2] = 0xad;
  netif->hwaddr[3] = 0xfe;
  netif->hwaddr[4] = 0xc2;
  netif->hwaddr[5] = 0x6c;

  /* maximum transfer unit */
  netif->mtu = 1500;
  
  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP ;//| NETIF_FLAG_LINK_UP;
 
  /* Do whatever else is needed to initialize interface. */ 
  
  
  //__ubOldPrio = OSTCBCur->OSTCBPrio;	
  //__ubErr = OSTaskChangePrio(OS_PRIO_SELF, OS_IDLE_PRIO - 1);
  
 OSMutexCreate (&Enc28j60_mutex ,"Enc28j60_mutex" ,&err);
 OSMutexPend (&Enc28j60_mutex , 0, OS_OPT_PEND_BLOCKING , &ts , &err); 	
 enc28j60Init(netif);
 OSMutexPost (&Enc28j60_mutex ,  OS_OPT_POST_NONE , &err);  
  //if(  ubErr == OS_NO_ERR)	
  //OSTaskChangePrio(OS_PRIO_SELF,   ubOldPrio);

 OSTaskCreate( &Enc28j60_input_thread_TCB, "Enc28j60_input_thread",Enc28j60_input_thread, (void *) netif, 1,
               &ethernetif_inputStk[0], APP_TASK_START_STK_SIZE / 10, APP_TASK_START_STK_SIZE,
               5u, 0u, 0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),&err);	
}
/*********************************************************************************
 Function:      //
 Description:   // This function should do the actual transmission of the packet
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
static err_t Enc28j60_output(struct netif *netif, struct pbuf *p)
{
 static unsigned long ouputcnt = 0;
 struct pbuf *q;
 //unsigned char* data ;
 
// data = p->payload; 
 ouputcnt ++ ;
#if ETH_PAD_SIZE
 pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif
 OSMutexPend (&Enc28j60_mutex , 0, OS_OPT_PEND_BLOCKING , &ts , &err); 
 enc28j60_ConfigSendBuf(p -> tot_len);
 for(q = p; q != NULL; q = q->next)
 { enc28j601_WBM_buff(q->payload ,q->len);} //enc28j60PacketSend(q->payload, q->len);
 enc28j60_SendActivate( );
 OSMutexPost ( &Enc28j60_mutex , OS_OPT_POST_NONE, &err); 
#if ETH_PAD_SIZE
 pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
 LINK_STATS_INC(link.xmit);
 return ERR_OK;
}
/*********************************************************************************
 Function:      //
 Description:   // 
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //Should allocate a pbuf and transfer the bytes of the incoming
                  packet from the interface into the pbuf.
*********************************************************************************/
static struct pbuf * Enc28j60_input(struct netif *netif)
{
 struct pbuf *p, *q;
 u16_t length = 1500;
 unsigned char  pktcnt ;
 static unsigned char state[6] ;
 OSMutexPend (&Enc28j60_mutex , 0, OS_OPT_PEND_BLOCKING , &ts , &err);
 pktcnt = enc28j60Read(0x19|0x20);
 if( pktcnt  ==0 ) 
 {
  OSMutexPost ( &Enc28j60_mutex , OS_OPT_POST_NONE, &err);
  p = NULL;
  return p;
 } 	
 get_enc28j60Packet_state( state );
  if(0 == (state[4] & (1ul << 7))) 
  {length  = 0;}
  else 
  {
   length = state[3];length  <<= 8;length |= state[2];
   if(length > 4) {length -= 4;}
   else{length = 0;}
  } 
  if(length == 0)
  {
   p = NULL;
   free_enc28j60Packet( state);
   OSMutexPost ( &Enc28j60_mutex , OS_OPT_POST_NONE, &err);
   return p;
  }
#if ETH_PAD_SIZE
  len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif
 p = pbuf_alloc(PBUF_RAW, length, PBUF_POOL); 
 if (p != NULL) 
 {
  #if ETH_PAD_SIZE
  pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
  #endif
  for(q = p; q != NULL; q = q->next)
  {enc28j601_RBM_buff (q->payload, q->len);}
  free_enc28j60Packet( state);	
  #if ETH_PAD_SIZE
  pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
  #endif
  LINK_STATS_INC(link.recv);
 } 
 else
 {
  free_enc28j60Packet( state);
  LINK_STATS_INC(link.memerr);
  LINK_STATS_INC(link.drop);
 }
 OSMutexPost ( &Enc28j60_mutex , OS_OPT_POST_NONE, &err); 
 return p;  
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
static void Enc28j60_input_thread( void *p_arg )
{
 struct netif *netif;
 netif = p_arg; 
 while (1)
 {
  OSTaskSemPend ( 2, OS_OPT_PEND_BLOCKING, &ts, &err);
  ethernetif_input( netif );
 }
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
static void ethernetif_input( struct netif *netif)
{
  struct eth_hdr *ethhdr;
  struct pbuf *p;
//  unsigned char *data_recv ;
  /* move received packet into a new pbuf */
  p = Enc28j60_input(netif);
  /* no packet could be read, silently ignore this */
  if (p == NULL) return;
  /* points to packet payload, which starts with an Ethernet header */
  ethhdr = p->payload;
//  data_recv = p->payload;
  data_recv_cnt += p->tot_len;

  switch (htons(ethhdr->type)) {
  /* IP or ARP packet? */
  case ETHTYPE_IP:
       {aa ++;}
  case ETHTYPE_ARP:
#if PPPOE_SUPPORT
  /* PPPoE packet? */
  case ETHTYPE_PPPOEDISC:
  case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
    /* full packet send to tcpip_thread to process */
    if (netif->input(p, netif)!=ERR_OK)
     { LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
       pbuf_free(p);
       p = NULL;
     }
    break;

  default:
    pbuf_free(p);
    p = NULL;
    break;
  }  
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
err_t netif_Enc28j60_init(struct netif *netif)
{
  struct ethernetif *ethernetif;

  LWIP_ASSERT("netif != NULL", (netif != NULL));
    
  ethernetif = mem_malloc(sizeof(struct ethernetif));
  if (ethernetif == NULL) {
    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
    return ERR_MEM;
  }

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

  netif->state = ethernetif;
  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
  netif->output = etharp_output;
  netif->linkoutput = Enc28j60_output;
  
  ethernetif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);
  
  /* initialize the hardware */
  low_level_init(netif);

  return ERR_OK;
}

/*********************************************************************************
 Function:      // ilvSetLwIP
 Description:   // 
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void  netif_Enc28j60_SetLwIP(void *arg)
{
// extern err_t ethernetif_init(struct netif *stNetif);
 struct ip_addr stIpAddr,   stNetMask,   stGateway;
 netif_init();
 stIpAddr.addr = 0;   stNetMask.addr = 0;	  stGateway.addr = 0;	 
 //IP4_ADDR(&stIpAddr,  IP_ADDR_0 , IP_ADDR_1, IP_ADDR_2,IP_ADDR_3);
 //IP4_ADDR(&stNetMask, NET_MASK_0, NET_MASK_1, NET_MASK_2, NET_MASK_3);
 //IP4_ADDR(&stGateway, GATEWAY_ADDR_0, GATEWAY_ADDR_1, GATEWAY_ADDR_2, GATEWAY_ADDR_3);
 netif_add(&ENC28J60Netif, &stIpAddr, &stNetMask, &stGateway, NULL, netif_Enc28j60_init, tcpip_input);
 netif_set_default(&ENC28J60Netif);
 netif_set_up(&ENC28J60Netif);
 dhcp_start(&ENC28J60Netif); 
}
