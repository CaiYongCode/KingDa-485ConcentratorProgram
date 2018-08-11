/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *         Simon Goldschmidt
 *
 */

#include <lwip/opt.h>
#include <lwip/arch.h>
#include <lwip/stats.h>
#include <lwip/debug.h>
#include <lwip/sys.h>
#include <os_cfg_app.h>

#include "UserCode/inc/dfg.h"


#define LWIP_ARCH_TICK_PER_MS       1000/OS_CFG_TICK_RATE_HZ

//#define sys_arch_mbox_tryfetch(mbox, msg)	sys_arch_mbox_fetch(mbox ,msg, 1)


const void * const pvNullPointer = (mem_ptr_t*)0xffffffff;

/******************************************************************************/
/*
 * port_malloc port_free是为了自己管理内存所用函数，非LwIP移植必须
 */
/******************************************************************************/
#if (LwIP_ARCH_MemoryMode == 1)
#define LWIP_POOL_SIZE    (4096)
static MEM_POOL lwip_pool;
static u8_t __attribute__((aligned(4))) _lwip_pool[LWIP_POOL_SIZE];
#elif (LwIP_ARCH_MemoryMode == 2)
#define LWIP_SEM_NBR      (50)
#define LWIP_MBOX_NBR     (50)
static OS_MEM    LWIP_SEM_pool ;
static OS_MEM    LWIP_MBOX_pool;
static u8_t __attribute__((aligned(4))) _LWIP_SEM_pool  [LWIP_SEM_NBR] [sizeof(OS_SEM)]; 
static u8_t __attribute__((aligned(4))) _LWIP_MBOX_pool [LWIP_MBOX_NBR][sizeof(OS_Q)  ];
#endif

static LIB_ERR liberr;
static OS_ERR  oserr;

static OS_TCB  lwip_tcp;
static CPU_STK  Stk[1024];




extern Client_NetTypeDef    Client_Net_Thread[MAX_THREAD_NUM]; 

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        // 
 Return:		//
 Others:        //
**********************************************************************************/
void sys_init()
{
#if (LwIP_ARCH_MemoryMode == 1)
 unsigned char size ;
 size =  sizeof(OS_SEM);
 if(size < sizeof(OS_Q))
 {size =  sizeof(OS_Q);}
 Mem_PoolCreate (&lwip_pool,
                  _lwip_pool,             
                  LWIP_POOL_SIZE,         
                  LWIP_POOL_SIZE/size,
                  size,
                  4,
                  (void *) 0,
                  &liberr);
#elif (LwIP_ARCH_MemoryMode == 2) 
 OS_ERR err;
 OSMemCreate( &LWIP_SEM_pool,
              "LWIP_SEM_pool",
			  _LWIP_SEM_pool,
			  LWIP_SEM_NBR ,
			  sizeof(OS_SEM),
			  &err );

 OSMemCreate( &LWIP_MBOX_pool,
              "LWIP_MBOX_pool",
			  _LWIP_MBOX_pool,
			  LWIP_MBOX_NBR ,
			  sizeof(OS_Q),
			  &err );
#endif
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        // 
 Return:		//
 Others:        //
**********************************************************************************/
#if (LwIP_ARCH_MemoryMode == 1)
void *port_malloc(u32_t size , LIB_ERR *err)
{
 void *ret;
 ret = Mem_PoolBlkGet ( &lwip_pool,size, err);
 return ret;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        // 
 Return:		//
 Others:        //
**********************************************************************************/
void port_free(void *free_ptr)
{
 Mem_PoolBlkFree( &lwip_pool,free_ptr, &liberr);
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        // 
 Return:		//
 Others:        //
**********************************************************************************/

u32_t sys_now()
{
 #define  DWT_CYCCNT  *(CPU_REG32 *)0xE0001004
 return ((CPU_TS_TMR)DWT_CYCCNT); //CPU_TS_TmrRd (void)
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        // 
 Return:		//
 Others:        //
**********************************************************************************/
sys_prot_t sys_arch_protect(void)
{
  return CPU_SR_Save();
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        // 
 Return:		//
 Others:        //
**********************************************************************************/
void sys_arch_unprotect(sys_prot_t pval)
{
 CPU_SR_Restore( pval);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        // 
 Return:		//
 Others:        //
**********************************************************************************/
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
 OS_SEM * lwipsem;
#if (LwIP_ARCH_MemoryMode == 1)

 lwipsem = port_malloc(sizeof(OS_SEM), &liberr);
 if(LIB_MEM_ERR_NONE != liberr)
 {
  sem->sem = NULL;
  return ERR_MEM ;
  }
#elif (LwIP_ARCH_MemoryMode == 2) 
 lwipsem = OSMemGet(&LWIP_SEM_pool , &oserr);
 if(oserr != OS_ERR_NONE)
 {
  sem->sem = NULL;
  return ERR_MEM ;
  }
#endif
 OSSemCreate(lwipsem, "LWIP Sem", count,&oserr);
 if(OS_ERR_NONE == oserr )
 {
  sem->sem = lwipsem;
 return ERR_OK;
 }
 else
 {
  sem->sem = NULL;
  return ERR_MEM ;
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
**********************************************************************************/
void sys_sem_free(sys_sem_t *sem)
{
 OSSemDel(sem->sem, OS_OPT_DEL_ALWAYS ,&oserr);
#if (LwIP_ARCH_MemoryMode == 1)
 port_free(sem->sem);
#elif (LwIP_ARCH_MemoryMode == 2)
 OSMemPut(&LWIP_SEM_pool , sem->sem ,&oserr);
#endif
 sem->sem = NULL;
}
/*********************************************************************************
 Function:      //
 Description:   //等待指定的信号并阻塞线程
 Input:         //
                //
 Output:        // 
 Return:		//
 Others:        //timeout 参数为 0，线程会一直被阻塞至收到指定的信号非 0，则线程仅
                //被阻塞至指定的 timeout 时间（单位为毫秒）。
				//在timeout参数值非0的情况下,返回值为等待指定的信号所消耗的毫秒数
				//如果在指定的时间内并没有收 到信号,返回 值 为SYS_ARCH_TIMEOUT
**********************************************************************************/
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
 CPU_TS  ts;
 CPU_TS  in_timeout = timeout/LWIP_ARCH_TICK_PER_MS;
 u32_t   starttime, endtime;

 if((timeout != 0) && (in_timeout == 0))
 in_timeout = 1; 
 starttime = sys_now();
OSSemPend ( sem->sem, in_timeout, OS_OPT_PEND_BLOCKING, &ts, &oserr);
 if(OS_ERR_NONE != oserr )
 {return SYS_ARCH_TIMEOUT;}
 endtime = sys_now();
 if((endtime > starttime))
 {return   ( (endtime - starttime)  / 72000 );}
 else
 {
  return   (0xffffffff-(starttime - endtime ) + 1) / 72000;
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
**********************************************************************************/
void sys_sem_signal(sys_sem_t *sem)
{ 
 OSSemPost(sem->sem,OS_OPT_POST_ALL,&oserr);
}
/*********************************************************************************
 Function:      //	sys_mbox_new
 Description:   //
 Input:         //
                //
 Output:        // 
 Return:		//
 Others:        //
**********************************************************************************/
err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
 OS_Q * q = NULL;
#if (LwIP_ARCH_MemoryMode == 1)
 q=port_malloc(sizeof(OS_Q), &liberr);
 if(LIB_MEM_ERR_NONE != liberr)
 {
  mbox -> queue =  NULL;
  return ERR_MEM;
 }
#elif (LwIP_ARCH_MemoryMode == 2)

 q = OSMemGet(&LWIP_MBOX_pool ,&oserr);
 if(OS_ERR_NONE != oserr)
 {
  mbox -> queue =  NULL;
  return ERR_MEM;
 }
#endif

 OSQCreate ( q,"queue_mbox",1,&oserr);
 if(OS_ERR_NONE != oserr )
 {mbox -> queue =  NULL;return ERR_MEM;}
 mbox -> queue =  q;
 return ERR_OK;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        // 
 Return:		//
 Others:        //
**********************************************************************************/
void sys_mbox_free(sys_mbox_t *mbox)
{
 if ((mbox == NULL) || (mbox->queue == NULL)){return;}
 OSQFlush(mbox ->queue,& oserr);
 OSQDel(mbox ->queue, OS_OPT_DEL_ALWAYS, &oserr);

#if (LwIP_ARCH_MemoryMode == 1)
 port_free(mbox ->queue);
#elif (LwIP_ARCH_MemoryMode == 2)
 OSMemPut(&LWIP_MBOX_pool , mbox ->queue ,&oserr);
#endif
}
/*********************************************************************************
 Function:      // sys_mbox_post
 Description:   // 投递消息"msg"到邮箱,必须阻塞直到"msg"真正被投递。
 Input:         //
                //
 Output:        // 
 Return:		//
 Others:        //
**********************************************************************************/
void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
 CPU_INT08U  i=0; 
 if ((mbox == NULL) || (mbox->queue == NULL))
 {return;}

 //if( msg == NULL ) msg = (void*)&pvNullPointer;
  /* try 10 times */
  while(i<10)
  {
    OSQPost(mbox ->queue, msg,4,OS_OPT_POST_ALL,&oserr);
    if(oserr == OS_ERR_NONE)
    break;
    i++;
    OSTimeDly(5,OS_OPT_TIME_DLY,&oserr);
  }
  LWIP_ASSERT( "sys_mbox_post error!\n", i !=10 );  
}
/*********************************************************************************
 Function:      // sys_mbox_trypost 
 Description:   // 投递消息"msg"到邮箱
 Input:         //
                //
 Output:        // 如果邮箱已满则返回ERR_MEM，否则返回ERR_OK
 Return:		//
 Others:        //
**********************************************************************************/
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
 //if(msg == NULL ) msg = (void*)&pvNullPointer;  
 OSQPost(mbox ->queue, msg,4,OS_OPT_POST_ALL,&oserr);    
 if(oserr != OS_ERR_NONE)
 {return ERR_MEM;}
 return ERR_OK;	
}
/*********************************************************************************
 Function:      // sys_arch_mbox_fetch
 Description:   // 阻塞线程直至邮箱收到至少一条消息 
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
**********************************************************************************/
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox ,void **msg, u32_t timeout)
{
 OS_MSG_SIZE   msg_size;
 CPU_TS        ucos_timeout;  
 CPU_TS        in_timeout = timeout/LWIP_ARCH_TICK_PER_MS;
 u32_t  starttime, endtime;

 if(timeout && in_timeout == 0)
   in_timeout = 1;
 starttime = sys_now();
 *msg  = OSQPend (mbox->queue,in_timeout,OS_OPT_PEND_BLOCKING,&msg_size, 
                          &ucos_timeout,&oserr);
 endtime = sys_now();
 ucos_timeout = (endtime - starttime)  * LWIP_ARCH_TICK_PER_MS	;

 if ( oserr == OS_ERR_TIMEOUT ) 
 ucos_timeout = SYS_ARCH_TIMEOUT;  
 return ucos_timeout; 
}
/*********************************************************************************
 Function:      // sys_arch_mbox_fetch
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
**********************************************************************************/
u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
 return sys_arch_mbox_fetch(mbox, msg, 1);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        // 
 Return:		//
 Others:        //
**********************************************************************************/
sys_thread_t sys_thread_new( char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
 OSTaskCreate( &lwip_tcp,         
               name,
               thread,
               arg,
               prio,
               &(Stk[0]),
               1024 / 10,//stacksize / 10,
               1024,
                5u,
                0u,
               (void       *) 0,
               (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               &oserr);
 return 1;
}