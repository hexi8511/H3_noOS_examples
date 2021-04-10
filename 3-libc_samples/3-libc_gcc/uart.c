#include "gpio_reg.h"
#include "basic_reg.h"
#include "ccu_reg.h"
#include "uart_reg.h"
#include "ccu.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE uart_fd;





void uart0_clk_init()
{
	//set_ahb1_clksrc(1);//24MHz
	//set_apb2_clksrc(1);//24Mhz
	//ccm_module_enable(UART0_CKID);
	//ccm_clock_enable(UART0_CKID);
	writel(readl(BUS_CLK_GATING_REG3)|(1<<16),BUS_CLK_GATING_REG3);

	writel(readl(BUS_SOFT_RST_REG4)&(~(1<<16)),BUS_SOFT_RST_REG4);
	writel(readl(BUS_SOFT_RST_REG4)|(1<<16),BUS_SOFT_RST_REG4);
	#if 0
	int i=0;
	volatile unsigned int reg;
		//reset
		reg = readl(BUS_SOFT_RST_REG4);
		reg &= ~(1<<(16));
		writel(reg, BUS_CLK_GATING_REG3);
		for( i = 0; i < 100; i++ );
		reg |=  (1<<(16));
		writel(reg, BUS_SOFT_RST_REG4);
		//gate
		reg = readl(BUS_CLK_GATING_REG3);
		reg &= ~(1<<(16));
		writel(reg, BUS_CLK_GATING_REG3);
		for( i = 0; i < 100; i++ );
		reg |=  (1<<(16));
		writel(reg, BUS_CLK_GATING_REG3);
	#endif
}

void uart0_pin_init()
{
	//gpio_A4 uart0_tx
	volatile unsigned int tmp_reg;

	tmp_reg=__read_reg(PA_CFG0);
	__set_bit(16,BW3,IO_MUL);
	__write_reg(PA_CFG0,tmp_reg);
	//gpio_A5 uart0_rx
	tmp_reg=__read_reg(PA_CFG0);
	__set_bit(20,BW3,IO_MUL);
	__write_reg(PA_CFG0,tmp_reg);

	tmp_reg=__read_reg(PA_DRV0);
	__set_bit(8,BW2,DRV_LEV1);
	__write_reg(PA_DRV0,tmp_reg);


	tmp_reg=__read_reg(PA_PUL0);
	__set_bit(8,BW2,PUL_UP);
	__write_reg(PA_PUL0,tmp_reg);

	tmp_reg=__read_reg(PA_DRV0);
	__set_bit(10,BW2,DRV_LEV1);
	__write_reg(PA_DRV0,tmp_reg);


	tmp_reg=__read_reg(PA_PUL0);
	__set_bit(10,BW2,PUL_UP);
	__write_reg(PA_PUL0,tmp_reg);

}

void uart0_baud_init(unsigned int speed)
{
	volatile unsigned int tmp_reg;

	//set dll dlh register
	tmp_reg=readl(UART0_LCR);
	tmp_reg|=(1<<7);
	writel(tmp_reg,UART0_LCR);

	unsigned int apb2_speed=24000000;
	//set baud speed
	//tmp_reg=apb2_speed/(speed*16);
	tmp_reg=13;

	while((readl(UART0_USR)&0x01)==1)
	{

	}
	writel(tmp_reg,UART0_DLL);
	writel(tmp_reg>>8,UART0_DLH);

	//
	tmp_reg=readl(UART0_LCR);
	tmp_reg&=(~(1<<7));
	writel(tmp_reg,UART0_LCR);
	delay();


	tmp_reg=readl(UART0_LCR);
	tmp_reg&=(~0x3f);
	tmp_reg=(0x03<<0)|(0<<2)|(0<<3)|(0<<4);//8 bit ,1 bit stop, no check
	writel(tmp_reg,UART0_LCR);

	//enable change for lcr
	tmp_reg=readl(UART0_HALT);
	tmp_reg&=~(3<<1);
	tmp_reg|=(3<<1);
	writel(tmp_reg,UART0_HALT);

}



int uart_putc(char c)
{
#if 1
	while(((1<<6)&readl(UART0_LSR))==0)
		{
		//led_blue_flash();
		}
	writel(c,UART0_THR);
#endif
return 0;
}


int SerialPortWrite(  const uint8_t* buf, int size)
{
	int i;
	const char* pbuf=buf;
	for(i=0;i<size;i++)
		{
		uart_putc(*pbuf);
		pbuf++;	
		}
	
	return size;
}

unsigned char uart_getc(void)
{
	while(((1<<0)&readl(UART0_LSR))==0)
	{
	
	}
	return readl(UART0_RBR);
}

int uart_write(struct _reent * fp, void* pp,
					  const char * buf, int size)
{
	int i;
	const char* pbuf=buf;
	for(i=0;i<size;i++)
		{
		uart_putc(*pbuf);
		pbuf++;	
		}
	
	return size;
}

int uart_read(struct _reent * fp, void* pp,
					   char * buf, int size)
{
	int i;
	char *pbuf=buf;
	for(i=0;i<size;i++)
	{
		*pbuf=uart_getc();
		pbuf++;	
	}
	return size;
}

void uart0_init(int baud)
{

	uart0_pin_init();
	uart0_clk_init();
	uart0_baud_init(baud);

	FILE *uart_fp=&uart_fd;
	uart_fp->_write=uart_write;
	uart_fp->_read=uart_read;

}


