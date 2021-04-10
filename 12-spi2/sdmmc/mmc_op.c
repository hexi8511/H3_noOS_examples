/*
 * (C) Copyright 2007-2012
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 * Description: MMC driver for general mmc operations
 * Author: Aaron <leafy.myeh@allwinnertech.com>
 * Date: 2012-2-3 14:18:18
 */
#include "mmc_def.h"
#include "mmc.h"
//#include "stdio.h"
#include "asm/types.h"

static unsigned bootcard_offset;

extern int sunxi_mmc_init(int sdc_no, unsigned bus_width);
extern int sunxi_mmc_exit(int sdc_no);

s32 SDMMC_PhyInit(u32 card_no, u32 bus_width)
{
	int ret = 0;
	mmcmsg("card %d bus:%d\n", card_no, bus_width);
	ret = sunxi_mmc_init(card_no, bus_width);
	if ( ret <= 0) {
		mmcdbg("Init SD/MMC card failed !\n");
		return -1;
	}

	return ret;
}

s32 SDMMC_PhyExit(u32 card_no)
{
	sunxi_mmc_exit(card_no);
    return 0;
}

__s32 SDMMC_PhyRead(u32 start_sector, u32 nsector, void *buf, u32 card_no)
{
	return mmc_bread(card_no, start_sector, nsector, buf);
}

s32 SDMMC_PhyWrite(u32 start_sector, u32 nsector, void *buf, u32 card_no)
{
	return mmc_bwrite(card_no, start_sector, nsector, buf);
}

s32 SDMMC_PhyDiskSize(u32 card_no)
{
	struct mmc *mmc = find_mmc_device(card_no);
	return mmc->lba;
}

s32 SDMMC_PhyErase(u32 block, u32 nblock, u32 card_no)
{
	return mmc_berase(card_no, block, nblock);
}

s32 SDMMC_LogicalInit(u32 card_no, u32 card_offset, u32 bus_width)
{
	bootcard_offset = card_offset;
    return SDMMC_PhyInit(card_no, bus_width);
}

s32 SDMMC_LogicalExit(u32 card_no)
{
	bootcard_offset = 0;
    return SDMMC_PhyExit(card_no);
}

s32 SDMMC_LogicalRead(u32 start_sector, u32 nsector, void *buf, u32 card_no)
{
	return mmc_bread(card_no, start_sector + bootcard_offset, nsector, buf);
}

s32 SDMMC_LogicalWrite(u32 start_sector, u32 nsector, void *buf, u32 card_no)
{
	return mmc_bwrite(card_no, start_sector + bootcard_offset, nsector, buf);
}

s32 SDMMC_LogicalDiskSize(u32 card_no)
{
	return SDMMC_PhyDiskSize(card_no) - bootcard_offset;
}

s32 SDMMC_LogicaErase(u32 block, u32 nblock, u32 card_no)
{
	return mmc_berase(card_no, block + bootcard_offset, nblock);
}

void OSAL_CacheRangeFlush(void*Address, u32 Length, u32 Flags)
{
	;
}

