
#include "asm/types.h"
#include "uart.h"
#include "bsp_sdc_for_boot.h"
#include "load_boot1_from_sdmmc.h"


//extern const boot0_file_head_t  BT0_head;
/*******************************************************************************
*函数名称: load_boot1_from_sdmmc
*函数原型：int32 load_boot1_from_sdmmc( __u8 card_no )
*函数功能: 将一份好的Boot1从sdmmc flash中载入到SRAM中。
*入口参数: void
*返 回 值: OK                         载入并校验成功
*          ERROR                      载入并校验失败
*备    注:
*******************************************************************************/

//SD卡相关数据结构

//boot_sdcard_info_t boot_sdcard_info;


void init_sdcard_data()
{
	/*
boot_sdcard_info.card_ctrl_num=4;
boot_sdcard_info.boot_offset=0;
boot_sdcard_info.card_no[0]=0;
boot_sdcard_info.card_no[1]=1;
boot_sdcard_info.card_no[2]=2;
boot_sdcard_info.card_no[3]=3;
boot_sdcard_info.speed_mode[0]=0;
boot_sdcard_info.speed_mode[1]=0;
boot_sdcard_info.speed_mode[2]=0;
boot_sdcard_info.speed_mode[3]=0;
boot_sdcard_info.line_sel[0]=4;
boot_sdcard_info.line_sel[1]=4;
boot_sdcard_info.line_sel[2]=4;
boot_sdcard_info.line_sel[3]=4;
boot_sdcard_info.line_count[0]=4;
boot_sdcard_info.line_count[1]=4;
boot_sdcard_info.line_count[2]=4;
boot_sdcard_info.line_count[3]=4;*/
};


__s32 load_boot1_from_sdmmc( char *buf)
{
    __u32  length;
    __s32  card_no, i,j;
		init_sdcard_data();
	i =0;
	msg("card No = %d\n", i);

//	for(i=0;i<4;i++)
	{
		/* open sdmmc */
		card_no = i;
		msg("card no is %d\n", card_no);
		if(card_no < 0)
		{
			msg("bad card %d boot\n", card_no);

			goto __card_op_fail__;
		}
		msg("sdcard %d line count %d\n", card_no, 4);
		
		if( SDMMC_PhyInit( card_no,4 ) == -1)   //高速卡，4线配置
		{
			msg("Fail in Init sdmmc.\n");
			goto __card_op_fail__;
		}
		msg("sdcard %d init ok\n", card_no);

	    //read uboot from 64K offset and maxsize 512k into sdram 0x4a000000 (160M) offset
		
		//64K offset for uboot
	    if( SDMMC_PhyRead( 128, 1024*1024*5/512, (void *)0x4a000000, card_no )!= (1024*1024*5/512))
		{
			msg("Fail rd boot1 head.\n");
			goto __card_op_fail__;
		}
	#if 0
		unsigned char *ptmp=(unsigned char*)(0x4a000000);
		for( j=0;j<512;j++)
		{
			if((j+1)%8==0)
				msg("\n");
			msg("%x ",*ptmp);
			ptmp++;
		}
		
		msg("\n");
	#endif
		SDMMC_PhyExit( card_no );
		return 1;

__card_op_fail__:
		SDMMC_PhyExit(card_no );
	}

	return -1;
}


__s32 load_xboot_from_sdmmc( char *buf)
{
    __u32  length;
    __s32  card_no, i,j;
		init_sdcard_data();
	i =0;
	msg("card No = %d\n", i);

//	for(i=0;i<4;i++)
	{
		/* open sdmmc */
		card_no = i;
		msg("card no is %d\n", card_no);
		if(card_no < 0)
		{
			msg("bad card %d boot\n", card_no);

			goto __card_op_fail__;
		}
		msg("sdcard %d line count %d\n", card_no, 4);
		
		if( SDMMC_PhyInit( card_no,4 ) == -1)   //高速卡，4线配置
		{
			msg("Fail in Init sdmmc.\n");
			goto __card_op_fail__;
		}
		msg("sdcard %d init ok\n", card_no);

	    //read uboot from 64K offset and maxsize 512k into sdram 0x4a000000 (160M) offset
		
		//64K offset for uboot
	    if( SDMMC_PhyRead( 128, 1024*1024*5/512, (void *)0x4a000000, card_no )!= (1024*1024*5/512))
		{
			msg("Fail rd boot1 head.\n");
			goto __card_op_fail__;
		}
	#if 0
		unsigned char *ptmp=(unsigned char*)(0x4a000000);
		for( j=0;j<512;j++)
		{
			if((j+1)%8==0)
				msg("\n");
			msg("%x ",*ptmp);
			ptmp++;
		}
		
		msg("\n");
	#endif
		SDMMC_PhyExit( card_no );
		return 1;

__card_op_fail__:
		SDMMC_PhyExit(card_no );
	}

	return -1;
}



__s32 load_kernel_from_sdmmc( char *buf)
{
    __u32  length;
    __s32  card_no, i,j;

		/* open sdmmc */
		card_no = 0;
		msg("card no is %d\n", card_no);
		if(card_no < 0)
		{
			msg("bad card %d boot\n", card_no);

			goto __card_op_fail__;
		}
		msg("sdcard %d line count %d\n", card_no, 4);
		
		if( SDMMC_PhyInit( card_no,4 ) == -1)   //高速卡，4线配置
		{
			msg("Fail in Init sdmmc.\n");
			goto __card_op_fail__;
		}
		msg("sdcard %d init ok\n", card_no);

	    //read uboot from 64K offset and maxsize 512k into sdram 0x4a000000 (160M) offset
		
		//4096K offset for uboot
	    if( SDMMC_PhyRead( 4096*2, 1024*1024*6/512, (void *)0x42000000, card_no )!= (1024*1024*6/512))
		{
			msg("Fail rd boot1 head.\n");
			goto __card_op_fail__;
		}
	#if 0
		unsigned char *ptmp=(unsigned char*)(0x42000000);
		for( j=0;j<512;j++)
		{
			if((j+1)%8==0)
				msg("\n");
			msg("%x ",*ptmp);
			ptmp++;
		}
		msg("\n");
		#endif
		SDMMC_PhyExit( card_no );
		return 1;

__card_op_fail__:
		SDMMC_PhyExit(card_no );

	return -1;
}


__s32 load_kernel2_from_sdmmc( char *buf)
{
    __u32  length;
    __s32  card_no, i,j;
	//boot_file_head_t  *bfh;
	//boot_sdcard_info_t  *sdcard_info = &boot_sdcard_info;//(boot_sdcard_info_t *)buf;
	init_sdcard_data();
	i =0;// BT0_head.boot_head.platform[0];
	msg("card No = %d\n", i);

//	for(i=0;i<4;i++)
	{
		/* open sdmmc */
		card_no = i;
		msg("card no is %d\n", card_no);
		if(card_no < 0)
		{
			msg("bad card %d boot\n", card_no);

			goto __card_op_fail__;
		}
		msg("sdcard %d line count %d\n", card_no, 4);
		
		if( SDMMC_PhyInit( card_no,4 ) == -1)   //高速卡，4线配置
		{
			msg("Fail in Init sdmmc.\n");
			goto __card_op_fail__;
		}
		msg("sdcard %d init ok\n", card_no);

	    //read uboot from 64K offset and maxsize 512k into sdram 0x4a000000 (160M) offset
		
		//4096K offset for uboot
	    if( SDMMC_PhyRead( 4096*2, 1024*1024*16/512, (void *)0x40008000, card_no )!= (1024*1024*16/512))
		{
			msg("Fail rd boot1 head.\n");
			goto __card_op_fail__;
		}
	#if 0
		unsigned char *ptmp=(unsigned char*)(0x40008000);
		for( j=0;j<512;j++)
		{
			if((j+1)%8==0)
				msg("\n");
			msg("%x ",*ptmp);
			ptmp++;
		}
		msg("\n");
		#endif
		SDMMC_PhyExit( card_no );
		return 1;

__card_op_fail__:
		SDMMC_PhyExit(card_no );
	}

	return -1;
}

__s32 load_fex_from_sdmmc( char *buf)
{
    __u32  length;
    __s32  card_no, i,j;
	//boot_file_head_t  *bfh;
	//boot_sdcard_info_t  *sdcard_info = &boot_sdcard_info;//(boot_sdcard_info_t *)buf;
	init_sdcard_data();
	i =0;// BT0_head.boot_head.platform[0];
	msg("card No = %d\n", i);

//	for(i=0;i<4;i++)
	{
		/* open sdmmc */
		card_no = i;
		msg("card no is %d\n", card_no);
		if(card_no < 0)
		{
			msg("bad card %d boot\n", card_no);

			goto __card_op_fail__;
		}
		msg("sdcard %d line count %d\n", card_no, 4);
		
		if( SDMMC_PhyInit( card_no,4 ) == -1)   //高速卡，4线配置
		{
			msg("Fail in Init sdmmc.\n");
			goto __card_op_fail__;
		}
		msg("sdcard %d init ok\n", card_no);

	    //read uboot from 64K offset and maxsize 512k into sdram 0x4a000000 (160M) offset
		
		//4096K offset for uboot
	    if( SDMMC_PhyRead( 1024*2, 1024*40*1/512, (void *)0x43000000, card_no )!= (1024*40*1/512))
		{
			msg("Fail rd boot1 head.\n");
			goto __card_op_fail__;
		}
	#if 0
		unsigned char *ptmp=(unsigned char*)(0x43000000);
		for( j=0;j<512;j++)
		{
			if((j+1)%8==0)
				msg("\n");
			msg("%x ",*ptmp);
			ptmp++;
		}
		msg("\n");
	#endif
		SDMMC_PhyExit( card_no );
		return 1;

__card_op_fail__:
		SDMMC_PhyExit(card_no );
	}

	return -1;
}


