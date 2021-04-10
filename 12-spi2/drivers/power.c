#include "gpio_reg.h"
#include "basic_reg.h"
#include "ccu_reg.h"
#include "uart.h"
#include "ccu.h"
#include "power.h"
#include "delay.h"
#include "asm/io.h"
#include "stdio.h"

int power_off(void)
{
	volatile unsigned int reg_val;
	//set PL05 musel output
	reg_val = readl(R_PIO_BASE + 0x00);
	reg_val &= ~(0xF << 20);
	reg_val |= (0x1 << 20);
	writel(reg_val, R_PIO_BASE + 0x00);

	//set PL05 low to set vcc-io power off
	reg_val = readl(R_PIO_BASE + 0x10);
	reg_val |= (0x1 << 5);
	writel(reg_val, R_PIO_BASE + 0x10);

	//set PL08,PL09 musel output
	reg_val = readl(R_PIO_BASE + 0x04);
	reg_val &= ~(0xFF);
	reg_val |= 0x11;
	writel(reg_val, R_PIO_BASE + 0x04);

	//set PL08,PL09 low to set  power off
	reg_val = readl(R_PIO_BASE + 0x10);
	reg_val &= ~(0x3 << 8);
	writel(reg_val, R_PIO_BASE + 0x10);
	return 0;
}

void pwr_ldo_en_init()
{
volatile unsigned int tmp_reg;
#if 0
//gpio_L9 pwr VCC-DRAM
tmp_reg=*((unsigned int *)PL_CFG1);
tmp_reg=tmp_reg&(~(0x7<<4));
tmp_reg=tmp_reg|(IO_OUTPUT<<4);
*((volatile unsigned int *)PL_CFG1)=tmp_reg;

tmp_reg=__read_reg(PL_DRV0);
//tmp_reg=tmp_reg&(~(0x3<<20));
//tmp_reg=tmp_reg|(DRV_LEV3<<20);
__set_bit(18,BW2,DRV_LEV3);
__write_reg(PL_DRV0,tmp_reg);


tmp_reg=__read_reg(PL_PUL0);
//tmp_reg=tmp_reg&(~(0x3<<20));
//tmp_reg=tmp_reg|(DRV_LEV3<<20);
__set_bit(18,BW2,PUL_UP);
__write_reg(PL_PUL0,tmp_reg);
#endif
printf("config pin L9\n");

printf("config pin L9 0\n");
tmp_reg=__read_reg(PL_DRV0);
//tmp_reg=tmp_reg&(~(0x3<<20));
//tmp_reg=tmp_reg|(DRV_LEV3<<20);
__set_bit(16,BW2,DRV_LEV3);
__write_reg(PL_DRV0,tmp_reg);
printf("config pin L9 1\n");

tmp_reg=__read_reg(PL_PUL0);
//tmp_reg=tmp_reg&(~(0x3<<20));
//tmp_reg=tmp_reg|(DRV_LEV3<<20);
__set_bit(16,BW2,PUL_UP);
__write_reg(PL_PUL0,tmp_reg);
printf("config pin L8\n");
#if 0

//gpio_L8 pwr VDD1V1-EPHY /VDD-CPUX
tmp_reg=*((volatile unsigned int *)PL_CFG1);
tmp_reg=tmp_reg&(~(0x7<<0));
tmp_reg=tmp_reg|(IO_OUTPUT<<0);
*(( volatile unsigned int *)PL_CFG1)=tmp_reg;
#endif

//gpio_L2 pwr VDD1V1-EPHY /VDD-CPUX
tmp_reg=*((volatile unsigned int *)PL_CFG0);
tmp_reg=tmp_reg&(~(0x7<<8));
tmp_reg=tmp_reg|(IO_OUTPUT<<8);
*((volatile unsigned int *)PL_CFG0)=tmp_reg;
printf("config pin L9.2\n");
tmp_reg=__read_reg(PL_DRV0);
//tmp_reg=tmp_reg&(~(0x3<<20));
//tmp_reg=tmp_reg|(DRV_LEV3<<20);
__set_bit(4,BW2,DRV_LEV3);
__write_reg(PL_DRV0,tmp_reg);
printf("config pin L9.3\n");

tmp_reg=__read_reg(PL_PUL0);
//tmp_reg=tmp_reg&(~(0x3<<20));
//tmp_reg=tmp_reg|(DRV_LEV3<<20);
__set_bit(4,BW2,PUL_UP);
__write_reg(PL_PUL0,tmp_reg);
printf("config pin L2\n");


//gpio_L5 VDD_SYS_3.3V
tmp_reg=*((volatile unsigned int *)PL_CFG0);
tmp_reg=tmp_reg&(~(0x7<<20));
tmp_reg=tmp_reg|(IO_OUTPUT<<20);
*((volatile unsigned int *)PL_CFG0)=tmp_reg;
printf("config pin L9.4\n");
tmp_reg=__read_reg(PL_DRV0);
//tmp_reg=tmp_reg&(~(0x3<<20));
//tmp_reg=tmp_reg|(DRV_LEV3<<20);
__set_bit(10,BW2,DRV_LEV3);
__write_reg(PL_DRV0,tmp_reg);

printf("config pin L9.5\n");
tmp_reg=__read_reg(PL_PUL0);
//tmp_reg=tmp_reg&(~(0x3<<20));
//tmp_reg=tmp_reg|(DRV_LEV3<<20);
__set_bit(10,BW2,PUL_UP);
__write_reg(PL_PUL0,tmp_reg);
printf("config pin L5\n");

//gpio_L6 pwr VDD1V1-EPHY /VDD-CPUX

tmp_reg=__read_reg(PL_DRV0);
//tmp_reg=tmp_reg&(~(0x3<<20));
//tmp_reg=tmp_reg|(DRV_LEV3<<20);
__set_bit(12,BW2,DRV_LEV3);
__write_reg(PL_DRV0,tmp_reg);
printf("config pin L9.7\n");

tmp_reg=__read_reg(PL_PUL0);
//tmp_reg=tmp_reg&(~(0x3<<20));
//tmp_reg=tmp_reg|(DRV_LEV3<<20);
__set_bit(12,BW2,PUL_UP);
__write_reg(PL_PUL0,tmp_reg);
printf("config pin L6\n");

//write PL6 bit as 1
tmp_reg=*((volatile unsigned int *)PL_DAT);
tmp_reg|=(1<<6);
*((volatile unsigned int *)PL_DAT)=tmp_reg;
printf("config pin L9.6\n");

//switch PL6 as output
tmp_reg=*((volatile unsigned int *)PL_CFG0);
printf("config pin %x\n",tmp_reg);
tmp_reg=tmp_reg&(~(0x7<<24));
printf("config pin %x\n",tmp_reg);
tmp_reg=tmp_reg|(IO_OUTPUT<<24);
printf("config pin %x\n",tmp_reg);
*((volatile unsigned int *)PL_CFG0)=tmp_reg;
printf("config pin L9.7\n");

}

void sys_3v3_off()
{
volatile unsigned int tmp_reg=__read_reg(PL_DAT);
__set_bit(5,BW1,1);
__write_reg(PL_DAT,tmp_reg);

}


void sys_3v3_on()
{
volatile unsigned int tmp_reg=__read_reg(PL_DAT);
__set_bit(5,BW1,0);
__write_reg(PL_DAT,tmp_reg);

}

void cpux_eth_off()
{
volatile unsigned int tmp_reg=__read_reg(PL_DAT);
__set_bit(8,BW1,0);
__write_reg(PL_DAT,tmp_reg);

}


void cpux_eth_on()
{
volatile unsigned int tmp_reg=__read_reg(PL_DAT);
__set_bit(8,BW1,1);
__write_reg(PL_DAT,tmp_reg);

}

