/*
 * (C) Copyright 2007-2012
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 * Description: MMC driver for general mmc operations
 * Author: Aaron <leafy.myeh@allwinnertech.com>
 * Date: 2012-2-3 14:18:18
 */

#ifndef _MMC_OP_H_
#define _MMC_OP_H_
#include "stdio.h"
#include "asm/types.h"

//物理操作
extern s32 SDMMC_PhyInit(u32 card_no, u32 bus_width);
extern s32 SDMMC_PhyExit(u32 card_no);

extern s32 SDMMC_PhyRead     (u32 start_sector, u32 nsector, void *buf, u32 card_no);
extern s32 SDMMC_PhyWrite    (u32 start_sector, u32 nsector, void *buf, u32 card_no);

//逻辑操作
extern s32 SDMMC_LogicalInit(u32 card_no, u32 card_offset, u32 bus_width)
extern s32 SDMMC_LogicalExit(u32 card_no);

extern s32 SDMMC_LogicalRead (u32 start_sector, u32 nsector, void *buf, u32 card_no);
extern s32 SDMMC_LogicalWrite(u32 start_sector, u32 nsector, void *buf, u32 card_no);

#endif /* _MMC_H_ */
