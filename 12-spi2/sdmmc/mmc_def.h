/*
 * (C) Copyright 2007-2012
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 * Description: MMC driver for general mmc operations
 * Author: Aaron <leafy.myeh@allwinnertech.com>
 * Date: 2012-2-3 14:18:18
 */

#ifndef _MMC_DEF_H_
#define _MMC_DEF_H_

#include "nano_lib.h"
//#include "types.h"
//#include <string.h>
//#include "../uart.h"

#define MMC_SUN8I
#define MAX_MMC_NUM			3
//#define MMC_TRANS_BY_DMA
//#define MMC_DEBUG

#define MMC_REG_BASE		0x01c0f000
#define MMC_REG_FIFO_OS		(0x200)
#define CCMU_HCLKGATE0_BASE 	0x01c20060
#define CCMU_HCLKRST0_BASE 	0x01c202c0
#define CCMU_MMC0_CLK_BASE 	0x01c20088
#define CCMU_MMC1_CLK_BASE 	0x01c2008c
#define CCMU_MMC2_CLK_BASE 	0x01c20090
#define CCMU_PLL5_CLK_BASE 	0x01c20020


#ifndef NULL
#define NULL (void*)0
#endif

extern void printf( char* buf);
//#define MMC_DEBUG
#ifdef MMC_DEBUG
#define mmcinfo(fmt...)	printf("[mmc]: "fmt)
#define mmcdbg(fmt...)	printf("[mmc]: "fmt)
#define mmcmsg(fmt...)	printf(fmt)
#else
#define mmcinfo(fmt...)	
//printf("[mmc]: "fmt)
#define mmcdbg(fmt...)
#define mmcmsg(fmt...)
#endif



//extern void OSAL_CacheRangeFlush(void*Address, __u32 Length, __u32 Flags);

#define DMAC_DES_BASE_IN_SRAM		(0x20000 + 0xC000)
#define DMAC_DES_BASE_IN_SDRAM		(0x42000000)
#define DRAM_START_ADDR					(0x40000000)
#define SRAM_END_ADDR						(32*1024)


#define DRIVER_VER  "2014-01-23 20:15"



#endif /* _MMC_H_ */
