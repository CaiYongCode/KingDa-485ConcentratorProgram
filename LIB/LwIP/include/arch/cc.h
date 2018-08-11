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
 *
 */
#ifndef __CC_H__
#define __CC_H__

#include  <cpu.h>

/* Types based on stdint.h */
typedef CPU_INT08U            u8_t;
typedef CPU_INT08S             s8_t;
typedef CPU_INT16U           u16_t;
typedef CPU_INT16S            s16_t;
typedef CPU_INT32U           u32_t;
typedef CPU_INT32S            s32_t;
typedef CPU_INT32U          mem_ptr_t;

/* Define (sn)printf formatters for these lwIP types */
#define U16_F "hu"
#define S16_F "hd"
#define X16_F "hx"
#define U32_F "lu"
#define S32_F "ld"
#define X32_F "lx"
#define SZT_F "uz"

/* ARM/LPC17xx is little endian only */
/* MCU�Ĵ�С������ */
#define BYTE_ORDER LITTLE_ENDIAN

/* Use LWIP error codes */
#define LWIP_PROVIDE_ERRNO

/* �ڴ�������� */
#if defined(__arm__) && defined(__ARMCC_VERSION)
	/* Keil uVision4 tools */
    #define PACK_STRUCT_BEGIN __packed
    #define PACK_STRUCT_STRUCT
    #define PACK_STRUCT_END
    #define PACK_STRUCT_FIELD(fld) fld
	#define ALIGNED(n)  __align(n)
#elif defined (__IAR_SYSTEMS_ICC__)
	/* IAR Embedded Workbench tools */
    #define PACK_STRUCT_BEGIN __packed
    #define PACK_STRUCT_STRUCT
    #define PACK_STRUCT_END
    #define PACK_STRUCT_FIELD(fld) fld
//    #define PACK_STRUCT_USE_INCLUDES
	#error NEEDS ALIGNED // FIXME TBD
#else
	/* GCC tools (CodeSourcery) */
    #define PACK_STRUCT_BEGIN
    #define PACK_STRUCT_STRUCT __attribute__ ((__packed__))
    #define PACK_STRUCT_END
    #define PACK_STRUCT_FIELD(fld) fld
	#define ALIGNED(n)  __attribute__((aligned (n)))
	//#define ALIGNED(n)  __align(n)
#endif

/* Used with IP headers only */
#define LWIP_CHKSUM_ALGORITHM 1

#ifdef LWIP_DEBUG
	void assert_printf(char *msg, int line, char *file);
	#define LWIP_PLATFORM_ASSERT(flag)  \
				do{ assert_printf((flag), __LINE__, __FILE__); }while(0)

	/* Plaform specific diagnostic output */
	#define LWIP_PLATFORM_DIAG(vars) printf vars

#else
	#define LWIP_PLATFORM_DIAG(msg) { ; }

	int raw_printf(const char *format, ...);
	#define LWIP_PLATFORM_ASSERT(flag)  \
			do{ \
				if (!(flag)){ \
					raw_printf("file is %s, line is %d\n",__FILE__, __LINE__);}\
			}while(0)
#endif

#endif /* __CC_H__ */
