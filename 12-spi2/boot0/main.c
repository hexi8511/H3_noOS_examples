#include "gpio_reg.h"
#include "basic_reg.h"
#include "uart.h"
//#include "stdio.h"
//#include "string.h"
#include "led.h"
//#include "cp15_v.h"
//#include "ths.h"
#include "ccu_reg.h"
#include <linux/types.h>
#include <linux/io.h>
#include "dram_sun8i_h3.h"
#include "ccu.h"
#include "asm/io.h"

void config_pll1_para(void)
{

}




/**
*This applicant function as follow:
*1.Initization DDR;
*2.Initization Serial port;
*3.Initization LED;
*4.Initization SD/MMC;
*5.Initization System CLK;
*5.Copy u-boot into DDR special address;
*6.Jump to special address and run u-boot.
*
*/

extern unsigned long sunxi_dram_init(void);
extern __s32 load_boot1_from_sdmmc( char *buf);
extern __s32 load_xboot_from_sdmmc( char *buf);
typedef void(*pfunc)(void);

int main(void)
{
	int count=0,i;
	pfunc uboot=(pfunc)0x4a000000;

	//pfunc uboot=(pfunc)0x40000000;

	for(int i=0;i<20;i++)
	{
		delay();
	}
	
	//init uart 
	uart0_init(115200);

	//init blue led color
	led_init();
	msg("#######    Allwinner H3 boot0 test!     #######\n");

	msg("SYS CLK Init\n");
	sys_clk_init();

	msg("DDR Init\n");
	sunxi_dram_init();
	
	msg("Load boot1 !\n");
	
	//cpus apb0 device operate test
	led_red_init();
	for(i=0;i<5;i++)
	led_red_flash();

	//load_xboot_from_sdmmc(0);
	load_boot1_from_sdmmc(0);
	msg("spi ver %x\n",readl(0x01c68000));
	msg("boot0 %x\n",readl(0x1c68000+4));
	msg("mem:%x\n",readl(0x4a000000));
	msg("mem:%x\n",readl(0x4a000000+4));
	msg("mem:%x\n",readl(0x4a000000+8));
	uboot();
	while(1)
	{
	led_blue_flash();

	//add your codes for other function
	//msg("#######    Allwinner H3 SDIO test!     %d#######\n",count);
 
	count++;
	}

	return 0;
}
