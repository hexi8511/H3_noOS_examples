
#include "basic_reg.h"
#include "ccu_reg.h"
#include "stdio.h"
#include "ccu.h"
#include "delay.h"
#include "spi.h"
#include "gpio.h"
#include "led.h"
#include <linux/io.h>
#include "dma.h"


u8 spi_transmit_status=0;


void spi_gpio_init(u8 spi_n)
{
	u32 val=0;

	if(spi_n==0)//pc0 pc1 pc2 pc3--cs is gpio
	{	
		val=readl(PC_CFG0);
		val&=0xffff0000;
		val|=0x00001333;
		writel(val,PC_CFG0);
		printf("PC_CFG0=0x%x\n",readl(PC_CFG0));

		val=readl(PC_PUL0);
		val&=~(3<<6);
		val|=(1<<6);
		writel(val,PC_PUL0);
		printf("PC_PUL0=0x%x\n",readl(PC_PUL0));
	}
	else if(spi_n==1) //pa13 pa14 pa15 pa16
	{
		val=readl(PA_CFG1);
		val&=0x000fffff;
		val|=0x22100000;
		writel(val,PA_CFG1);	

		val=readl(PA_CFG2);
		val&=0xfffffff0;
		val|=0x00000002;
		writel(val,PA_CFG2);	
		
		val=readl(PA_PUL0);
		val&=~(3<<13*2);
		val|=(1<<13*2);
		writel(val,PA_PUL0);
		printf("PA_PUL0=0x%x\n",readl(PA_PUL0));
	}
}

void spi_cs_soft_high(u8 spi_n )
{

	u32 val;
	if(spi_n==0)
	{
	val=readl(PC_DAT);
		val|=(1<<3);
		writel(val,PC_DAT);
	}
	else
	{
		val=readl(PA_DAT);
		val|=(1<<13);
		writel(val,PA_DAT);
	}
}



void spi_cs_soft_low(u8 spi_n )
{
	u32 val;

	if(spi_n==0)
	{
		 val=readl(PC_DAT);
		val&=~(1<<3);
		writel(val,PC_DAT);
	}else
	{
		 val=readl(PA_DAT);
		val&=~(1<<13);
		writel(val,PA_DAT);
	}
}


void spi_clk_init(u8 spi_n)
{
	u32 val=0;
	if(spi_n==0)
	{	
		val=(0x80000000);//|(1<<24)|5);//100MHz periph0=600
		writel(val,SPI0_CLK_REG);//24MHz
		//open gate
		writel(readl(BUS_CLK_GATING_REG0)|(1<<20),BUS_CLK_GATING_REG0);
		//reset clk
		val=readl(BUS_SOFT_RST_REG0);
		val&=~(1<<20);
		writel(val,BUS_SOFT_RST_REG0);
		val|=(1<<20);
		writel(val,BUS_SOFT_RST_REG0);
		printf("BUS_SOFT_RST_REG0=0x%x\n",readl(BUS_SOFT_RST_REG0));
	}
	else if(spi_n==1) 
	{
		val=0x80000000;
		writel(val,SPI1_CLK_REG);//24MHz
		//open gate
		writel(readl(BUS_CLK_GATING_REG0)|(1<<21),BUS_CLK_GATING_REG0);
		//reset clk
		val=readl(BUS_SOFT_RST_REG0);
		val&=~(1<<21);
		writel(val,BUS_SOFT_RST_REG0);
		val|=(1<<21);
		writel(val,BUS_SOFT_RST_REG0);
	}

	delay_ms(10);
}

/* SPI Clock Control Register Bit Fields & Masks,default:0x0000_0002 */
#define SPI_CLK_CTL_CDR2		(0xFF <<  0)	/* Clock Divide Rate 2,master mode only : SPI_CLK = AHB_CLK/(2*(n+1)) */
#define SPI_CLK_CTL_CDR1		(0xF  <<  8)	/* Clock Divide Rate 1,master mode only : SPI_CLK = AHB_CLK/2^n */
#define SPI_CLK_CTL_DRS			(0x1  << 12)	/* Divide rate select,default,0:rate 1;1:rate 2 */
#define SPI_CLK_SCOPE			(SPI_CLK_CTL_CDR2+1)

static void spi_set_clk(u8 spi_n, u32 spi_clk, u32 ahb_clk)
{
	u32 reg=0;
    u32 reg_val = 0;
    u32 div_clk = ahb_clk / (spi_clk << 1);
	if(spi_n==0)
		reg=SPI0_CCR;
	else
		reg=SPI1_CCR;
    printf("set spi clock %d, mclk %d\n", spi_clk, ahb_clk);
    reg_val = readl(reg);

    /* CDR2 */
    if(div_clk <= SPI_CLK_SCOPE) {
        if (div_clk != 0) {
            div_clk--;
        }
        reg_val &= ~SPI_CLK_CTL_CDR2;
        reg_val |= (div_clk | SPI_CLK_CTL_DRS);
        printf("CDR2 - n = %d \n", div_clk);
    }/* CDR1 */
    else {
		div_clk = 0;
		while(ahb_clk > spi_clk){
			div_clk++;
			ahb_clk >>= 1;
		}
        reg_val &= ~(SPI_CLK_CTL_CDR1 | SPI_CLK_CTL_DRS);
        reg_val |= (div_clk << 8);
        printf("CDR1 - n = %d \n", div_clk);
    }
    writel(reg_val,reg);
}


void spi_module_reset(u8 spi_n)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_GCR;
	else
		reg=SPI1_GCR;
	val=readl(reg);
	val|=(1<<31);
	writel(val,reg);
//	delay_ms(10);
	
}

void spi_module_enable(u8 spi_n)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_GCR;
	else
		reg=SPI1_GCR;
	val=readl(reg);
	val|=1;
	writel(val,reg);

	printf("GCR:0x%x\n",readl(reg));
}

void spi_module_disable(u8 spi_n)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_GCR;
	else
		reg=SPI1_GCR;
	val=readl(reg);
	val&=~(1<<0);
	writel(val,reg);

}

void spi_transmit_pause_enable(u8 spi_n)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_GCR;
	else
		reg=SPI1_GCR;
	val=readl(reg);
	val|=(1<<7);
	writel(val,reg);

}

void spi_transmit_pause_ignore(u8 spi_n)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_GCR;
	else
		reg=SPI1_GCR;
	val=readl(reg);
	val&=~(1<<7);
	writel(val,reg);

}

/*
*spi_n: 0, 1
*mode=0 slave,1 master
*/
void spi_mode(u8 spi_n,u8 mode)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_GCR;
	else
		reg=SPI1_GCR;	

	val=readl(reg);
	val&=~(0x02);
	if(mode==1)
		val|=0x02;
	writel(val,reg);
}

/**
*start to transmit brust
*/
void spi_master_brust_start(u8 spi_n)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_TCR;
	else
		reg=SPI1_TCR;	

	val=readl(reg);
	val|=(0x1<<31);
	writel(val,reg);
}

/**
*0 delay sample mode
*1 normal sample mode
*/
void spi_sample_data_mode(u8 spi_n,u8 mode)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_TCR;
	else
		reg=SPI1_TCR;	

	val=readl(reg);
	val&=~(1<<13);
	if(mode==1)
		val|=(1<<13);
	writel(val,reg);
}
/**
*0 msb first
*1 lsb first
*/
void spi_first_tx_bit(u8 spi_n,u8 ml)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_TCR;
	else
		reg=SPI1_TCR;	

	val=readl(reg);
	val&=~(1<<12);
	if(ml==1)
		val|=(1<<12);
	writel(val,reg);
}
/**
*1 delay sample 
*0 normal sample operate
*/
void spi_sample_data_opt(u8 spi_n,u8 opt)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_TCR;
	else
		reg=SPI1_TCR;	

	val=readl(reg);
	val&=~(1<<11);
	if(opt==1)
		val|=(1<<11);
	writel(val,reg);
}

/**
*1 rapids write mode
*0 normal write mode
*/
void spi_write_mode(u8 spi_n,u8 mode)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_TCR;
	else
		reg=SPI1_TCR;	

	val=readl(reg);
	val&=~(1<<10);
	if(mode==1)
		val|=(1<<10);
	writel(val,reg);
}

/**
*0 all zero bit
*1 all one bit
*/
void spi_dummy_type(u8 spi_n,u8 type)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_TCR;
	else
		reg=SPI1_TCR;	

	val=readl(reg);
	val&=~(1<<9);
	if(type==1)
		val|=(1<<9);
	writel(val,reg);
}

/**
*0 receive all spi brust,
*1 discard unused burst
*/
void spi_discard_brust_type(u8 spi_n,u8 discard_type)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_TCR;
	else
		reg=SPI1_TCR;	

	val=readl(reg);
	val&=~(1<<8);
	if(discard_type==1)
		val|=(1<<8);
	writel(val,reg);
}
/**
*0 set ss to low,
*1 set ss to high
*
*/
void spi_set_ss_level(u8 spi_n,u8 ss_level)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_TCR;
	else
		reg=SPI1_TCR;	

	val=readl(reg);
	val&=~(1<7);
	if(ss_level==1)
		val|=(1<<7);
	writel(val,reg);
}
/*
*0 spi hardware, 1 softerware
*/

void spi_set_ss_owner(u8 spi_n,u8 ss_owner)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_TCR;
	else
		reg=SPI1_TCR;	

	val=readl(reg);
	val&=~(1<6);
	if(ss_owner==1)
		val|=(1<<6);
	writel(val,reg);
}

void spi_select_ss(u8 spi_n,u8 ss_index)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_TCR;
	else
		reg=SPI1_TCR;	

	val=readl(reg);
	val&=~(3<4);
	val|=(ss_index<<4);
	writel(val,reg);
}
/*
*0 ss still asserted between brusts,1 ss negate between brusts
*/
void spi_brusts_has_ss(u8 spi_n,u8 ss_assert)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_TCR;
	else
		reg=SPI1_TCR;	

	val=readl(reg);
	val&=~(1<<3);
	if(ss_assert==1)
		val|=(1<<3);
	writel(val,reg);
}
void spi_chip_select_pol(u8 spi_n,u8 pol)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_TCR;
	else
		reg=SPI1_TCR;	

	val=readl(reg);
	val&=~(1<<2);
	if(pol==1)
		val|=(1<<2);
	writel(val,reg);
}

void spi_clock_pol(u8 spi_n,u8 pol)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_TCR;
	else
		reg=SPI1_TCR;	

	val=readl(reg);
	val&=~(1<<1);
	if(pol==1)
		val|=(1<<1);
	writel(val,reg);
}

void spi_clock_data_phase(u8 spi_n,u8 phase)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_TCR;
	else
		reg=SPI1_TCR;	

	val=readl(reg);
	val&=~(1<<0);
	if(phase==1)
		val|=(1<<0);
	writel(val,reg);
}


void spi_tx_fifo_reset(u8 spi_n)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_FCR;
	else
		reg=SPI1_FCR;	

	val=readl(reg);
	val|=TX_FIFO_RST;
	writel(val,reg);
}


void spi_rx_fifo_reset(u8 spi_n)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_FCR;
	else
		reg=SPI1_FCR;	

	val=readl(reg);
	val|=RX_FIFO_RST;
	writel(val,reg);
}
/*0 disable dma req, 1 enable dma req
*
*/
void spi_tx_fifo_dma_req(u8 spi_n,u8 req)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_FCR;
	else
		reg=SPI1_FCR;	

	val=readl(reg);	
	val&=~(1<<24);
	if(req==1)
		val|=(1<<24);
	writel(val,reg);
	printf("wr_val%x\n",val);
}
/*
*0 disable dma req, 1 enable dma req
*/
void spi_rx_fifo_dma_req(u8 spi_n,u8 req)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_FCR;
	else
		reg=SPI1_FCR;	

	val=readl(reg);
	val&=~(1<<8);
	if(req==1)
		val|=(1<<8);
	writel(val,reg);
}
/*
*0~256 level
*/
void spi_tx_fifo_empty_trig_level(u8 spi_n,u8 level)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_FCR;
	else
		reg=SPI1_FCR;	

	level&=0xff;
	val=readl(reg);
	val&=~(0xff<<16);
	val|=(level<<16);
	writel(val,reg);
}
/*
*0~256 level
*/
void spi_rx_fifo_ready_trig_level(u8 spi_n,u8 level)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_FCR;
	else
		reg=SPI1_FCR;	

	level&=0xff;
	val=readl(reg);
	val&=~(0xff<<0);
	val|=(level<<0);
	writel(val,reg);
}

/*
*0 normal, 1 dedicate mode
*/
void spi_rx_fifo_dma_mode(u8 spi_n,u8 rx_dma_mode)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_FCR;
	else
		reg=SPI1_FCR;	

	val=readl(reg);
	val&=~(0x1<<9);
	val|=(rx_dma_mode<<9);
	writel(val,reg);
}

/*
*SPI module count wait CLK number,then begin to transmit next data.
*dual mode domain 0-16,signle 0-65535
*dual_signle 0=signle ,1=dual
*/
void spi_wait_clk_count(u8 spi_n,u8 dual_signle,u16 count)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_WCR;
	else
		reg=SPI1_WCR;	

	val=readl(reg);
	if(dual_signle==1)
		{
			val&=~(0xf<<16);
			val|=(count<<16);
		}
	else
		{
			val&=~(0xffff<<0);
			val|=(count<<0);
		}
	
	writel(val,reg);
}


void spi_master_brusts_count(u8 spi_n,u32 count)
{
	u32 val=0,reg;
	
	if(spi_n==0)
		reg=SPI0_MBC;
	else
		reg=SPI1_MBC;	

	//val=readl(reg);
	count&=0xffffff;
	//val&=~(0xffffff<<0);
	val=(count<<0);
	

	writel(val,reg);

}


void spi_brust2tx_fifo_count(u8 spi_n,u32 count)
{
	u32 val=0,reg;
	
	if(spi_n==0)
		reg=SPI0_MTC;
	else
		reg=SPI1_MTC;	

	//val=readl(reg);
	count&=0xffffff;
	//val&=~(0xf<<24);
	val=(count<<0);
	

	writel(val,reg);

}

/*
*Rx data bit mode.
*0=dual mode domain 1=signle mode
*/
void spi_rx_bit_mode(u8 spi_n,u8 dual_signle)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_BCC;
	else
		reg=SPI1_BCC;	

	val=readl(reg);

	val&=~(0x1<<28);
	val|=(dual_signle<<28);
	

	writel(val,reg);
}
/*
*before receive dual bit data, master should receive how many dummy brusts
*/
void spi_dual_dummy_brust_count(u8 spi_n,u32 count)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_BCC;
	else
		reg=SPI1_BCC;	

	val=readl(reg);
	count&=0xf;
	val&=~(0xf<<24);
	val|=(count<<24);
	

	writel(val,reg);

}
/**
*in signle bit mode, how many brusts should be sent before automatically send dummy brusts
*/


void spi_signle_brusts_count(u8 spi_n,u32 count)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_BCC;
	else
		reg=SPI1_BCC;	

	val=readl(reg);
	count&=0xffffff;
	val&=~(0xffffff<<0);
	val|=(count<<0);

	writel(val,reg);

}
/*
*0 div1; 1 div2
*/
void spi_clk_select(u8 spi_n,u8 div)
{
	u32 val,reg;
	

	if(spi_n==0)
		reg=SPI0_CCR;
	else
		reg=SPI1_CCR;	

	val=readl(reg);
	val&=~(0x1<<12);
	val|=(div<<12);

	writel(val,reg);
}

void spi_clk_div1(u8 spi_n, u8 nn)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_BCC;
	else
		reg=SPI1_BCC;	

	val=readl(reg);
	nn&=0xf;
	val&=~(0xf<<8);
	val|=(nn<<8);

	writel(val,reg);
}

void spi_clk_div2(u8 spi_n, u8 nn)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_BCC;
	else
		reg=SPI1_BCC;	

	val=readl(reg);
	nn&=0xff;
	val&=~(0xff<<0);
	val|=(nn<<0);

	writel(val,reg);
}


void spi_test_enable(u8 spi_n)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_FCR;
		reg=SPI1_FCR;	

	val=readl(reg);
	
	val&=~(0x1<<30);
	val|=(1<<30);

	val&=~(0x1<<14);
	val|=(1<<14);

	writel(val,reg);

	printf("wr_val%x\n",val);
}


void spi_test_disable(u8 spi_n)
{
	u32 val,reg;
	
	if(spi_n==0)
		reg=SPI0_FCR;
	else
		reg=SPI1_FCR;	

	val=readl(reg);
	val&=~(0x1<<30);
	val&=~(0x1<<14);
	writel(val,reg);
}

u8 spi_query_txfifo(u8 spi_n)
{
	u32 reg;
	u8 cnt;
	
	if(spi_n==0)
		reg=SPI0_FSR;
	else
		reg=SPI1_FSR;

	cnt=(readl(reg)>>16)&0xff;
	//printf("tx fifo %d\n",cnt);
	return cnt;
}

u8 spi_query_rxfifo(u8 spi_n)
{
	u8 cnt;
	u32 reg;
	
	if(spi_n==0)
		reg=SPI0_FSR;
	else
		reg=SPI1_FSR;

	cnt=(readl(reg)>>0)&0xff;
	//printf("rx fifo %d\n",cnt);
	return cnt;
}

u8 spi_query_txfifo_write_buf(u8 spi_n)
{
	u8 cnt;
	u32 reg;

	if(spi_n==0)
		reg=SPI0_FSR;
	else
		reg=SPI1_FSR;

	cnt=(readl(reg)>>28)&0x07;
	//	printf("tx write buf %d\n",cnt);
	return cnt;
}

u8 spi_query_rxfifo_read_buf(u8 spi_n)
{
	u8 cnt;
	u32 reg;

	if(spi_n==0)
		reg=SPI0_FSR;
	else
		reg=SPI1_FSR;

	 cnt=(readl(reg)>>12)&0x07;
	//printf("rx read buf %d\n",cnt);
	return cnt;
}



void spi_int_enable(u8 spi_n,u32 bitmap)
{
	u32 reg;
	u32 reg_val ;

	if(spi_n==0)
		reg=SPI0_IER;
	else
		reg=SPI1_IER;

 	reg_val = readl(reg);
  //  bitmap &= (0x3f77);
    reg_val |= bitmap;
    writel(reg_val, reg);
}

void spi_int_disable(u8 spi_n,u32 bitmap)
{

	u32 reg;
	u32 reg_val ;

	if(spi_n==0)
		reg=SPI0_IER;
	else
		reg=SPI1_IER;

   	reg_val = readl(reg);
  //  bitmap &= 0x3f77;
    reg_val &= (~bitmap);
    writel(reg_val, reg);
	
}


void spi_pend_clean(u8 spi_n,u32 bitmap)
{
 	u32 reg;
	u32 reg_val=0 ;

	if(spi_n==0)
		reg=SPI0_ISR;
	else
		reg=SPI1_ISR;

    reg_val |= bitmap;
    writel(reg_val, reg);
}

void spi_dma_enable(u8 spi_n,u32 bitmap)
{

 	u32 reg;
	u32 reg_val ;

	if(spi_n==0)
		reg=SPI0_FCR;
	else
		reg=SPI1_FCR;

   reg_val = readl(reg);
    bitmap &= (1<<8)|(1<<24);
    reg_val |= bitmap;
    writel(reg_val, reg);
}

void spi_dma_disable(u8 spi_n,u32 bitmap)
{
	u32 reg;
	u32 reg_val ;

	if(spi_n==0)
		reg=SPI0_FCR;
	else
		reg=SPI1_FCR;

   reg_val = readl(reg);
  //  bitmap &= (bitmap);
    reg_val &=( ~bitmap);
    writel(reg_val,reg);
}


void spi_config_brusts(u8 spi_n,u32 tx_cnt,u32 rx_cnt,u32 dummy_cnt)
{
	u32 all_bcnt=tx_cnt+rx_cnt+dummy_cnt;

	spi_dual_dummy_brust_count(spi_n,dummy_cnt);	
	
	spi_signle_brusts_count(spi_n,tx_cnt);
	spi_brust2tx_fifo_count(spi_n,tx_cnt);
	spi_master_brusts_count(spi_n,all_bcnt);
}
			


static u8 recv_flag=0;

s32 spi0_int_proc(void* argv)
{
	u32 spi_int_sta=0;
	spi_int_sta=readl(SPI0_ISR);
	//printf("[%s] ISR 0x%08x\n",__func__,spi_int_sta=readl(SPI0_ISR));
	//printf("[%s] IER 0x%08x\n",__func__,readl(SPI0_IER));
	//printf("[%s] FSR 0x%08x\n",__func__,readl(SPI0_FSR));

	
	writel(spi_int_sta,SPI0_ISR);
	if(spi_int_sta&(1<<12))
	{
		recv_flag=1;
	//printf("[%s] TC 0x%08x\n",__func__,readl(SPI0_ISR));	
		//spi_int_disable(0,spi_int_sta);
	//	spi_int_disable(0,TC_INT_EN|SS_INT_EN);
	}
	else if(SPI_ERR&spi_int_sta)
	{
		//printf("[%s] error 0x%08x\n",__func__,readl(SPI0_ISR));
		//spi_module_reset(0);
	}
	return 0;
}

s32 spi1_int_proc(void* argv)
{
	u32 spi_int_sta=0;
	//printf("[%s] ISR 0x%08x\n",__func__,spi_int_sta=readl(SPI1_ISR));
	//printf("[%s] IER 0x%08x\n",__func__,readl(SPI1_IER));
	//printf("[%s] FSR 0x%08x\n",__func__,readl(SPI1_FSR));

	
	writel(spi_int_sta,SPI1_ISR);
	if(spi_int_sta&(1<<12))
	{
		recv_flag=1;
	//	printf("[%s] TC 0x%08x\n",__func__,readl(SPI1_ISR));	
		//spi_int_disable(0,spi_int_sta);
		spi_int_disable(0,TC_INT_EN|SS_INT_EN);
	}
	else if(SS_INT_EN&spi_int_sta)
	{
	//	printf("[%s] CS invalid 0x%08x\n",__func__,readl(SPI1_ISR));
	}
	return 0;
}


void dump_spi_regs(u8 spi_n)
{
	if(spi_n==0)
	{

		printf("=============\n");
		printf("SPI0_VER:%x\n",readl(SPI0_VER));
		printf("SPI0_GCR:%x\n",readl(SPI0_GCR));
		printf("SPI0_TCR:%x\n",readl(SPI0_TCR));
		printf("SPI0_IER:%x\n",readl(SPI0_IER));
	
		printf("SPI0_ISR:%x\n",readl(SPI0_ISR));
		printf("SPI0_FCR:%x\n",readl(SPI0_FCR));
		printf("SPI0_FSR:%x\n",readl(SPI0_FSR));

		printf("SPI0_WCR:%x\n",readl(SPI0_WCR));
		printf("SPI0_CCR:%x\n",readl(SPI0_CCR));
		printf("SPI0_MBC:%x\n",readl(SPI0_MBC));

		printf("SPI0_MTC:%x\n",readl(SPI0_MTC));
		printf("SPI0_BCC:%x\n",readl(SPI0_BCC));
		printf("SPI0_TXD:%x\n",readl(SPI0_TXD));
		printf("SPI0_RXD:%x\n",readl(SPI0_RXD));
	}
else
	{
		printf("=============\n");
		printf("SPI1_VER:%x\n",readl(SPI1_VER));
		printf("SPI1_GCR:%x\n",readl(SPI1_GCR));
		printf("SPI1_TCR:%x\n",readl(SPI1_TCR));
		printf("SPI1_IER:%x\n",readl(SPI1_IER));
	
		printf("SPI1_ISR:%x\n",readl(SPI1_ISR));
		printf("SPI1_FCR:%x\n",readl(SPI1_FCR));
		printf("SPI1_FSR:%x\n",readl(SPI1_FSR));

		printf("SPI1_WCR:%x\n",readl(SPI1_WCR));
		printf("SPI1_CCR:%x\n",readl(SPI1_CCR));
		printf("SPI1_MBC:%x\n",readl(SPI1_MBC));

		printf("SPI1_MTC:%x\n",readl(SPI1_MTC));
		printf("SPI1_BCC:%x\n",readl(SPI1_BCC));
		printf("SPI1_TXD:%x\n",readl(SPI1_TXD));
		printf("SPI1_RXD:%x\n",readl(SPI1_RXD));
	}

}
void spi_init(u8 spi_n,struct spi_controller *ctrl_cfg)
{

	printf("%s %d init gpio\n",__func__,spi_n);
	spi_gpio_init(spi_n);
	printf("%s %d init clk\n",__func__,spi_n);
	spi_clk_init(spi_n);
	printf("%s %d reset spi module\n",__func__,spi_n);
	//spi_module_reset(spi_n);
	printf("%s %d enable spi module\n",__func__,spi_n);
	spi_module_enable(spi_n);

	dump_spi_regs(spi_n);

	if(ctrl_cfg->transmit_pause_enable)
		{
			spi_transmit_pause_enable(spi_n);
		}
	else
		{
			spi_transmit_pause_ignore(spi_n);
		}

	spi_mode(spi_n,ctrl_cfg->work_mode);

	spi_sample_data_mode(spi_n,ctrl_cfg->master_sample_data_mode);

	spi_first_tx_bit(spi_n,ctrl_cfg->first_transmit_bit);

	spi_sample_data_opt(spi_n,ctrl_cfg->master_sample_data_opt);

	spi_write_mode(spi_n,ctrl_cfg->rapid_mode);

	spi_dummy_type(spi_n,ctrl_cfg->dummy_burst_type);

	spi_discard_brust_type(spi_n,ctrl_cfg->discard_hash_burst);

	spi_set_ss_level(spi_n,ctrl_cfg->ss_level);

	spi_set_ss_owner(spi_n,ctrl_cfg->ss_owner);

	spi_select_ss(spi_n,ctrl_cfg->ss_select);

	spi_brusts_has_ss(spi_n,ctrl_cfg->ss_ctrl);

	spi_chip_select_pol(spi_n,ctrl_cfg->chip_select_pol);

	spi_clock_pol(spi_n,ctrl_cfg->spi_clk_pol);

	spi_clock_data_phase(spi_n,ctrl_cfg->spi_clk_phase);

	spi_rx_fifo_dma_req(spi_n,ctrl_cfg->rx_fifo_dma_req);
	spi_tx_fifo_dma_req(spi_n,ctrl_cfg->tx_fifo_dma_req);
	spi_tx_fifo_empty_trig_level(spi_n,ctrl_cfg->tx_fifo_empty_trig_level);
	spi_rx_fifo_ready_trig_level(spi_n,ctrl_cfg->rx_fifo_ready_req_trig_level);
	spi_rx_fifo_dma_mode(spi_n,ctrl_cfg->rx_fifo_dma_mode);

	spi_rx_bit_mode(spi_n,ctrl_cfg->recv_s_or_d);//1 daul, 0 signle
	if(ctrl_cfg->recv_s_or_d)
		{
		spi_wait_clk_count(spi_n,1,ctrl_cfg->dual_wait_clk_cnt);
		spi_dual_dummy_brust_count(spi_n,ctrl_cfg->recv_dummy_brust_cnt_daul);
		}
	else
		{
		spi_wait_clk_count(spi_n,0,ctrl_cfg->signle_wait_clk_cnt);
		spi_dual_dummy_brust_count(spi_n,0);		
		spi_signle_brusts_count(spi_n,ctrl_cfg->transmit_cnt_signle);
		}

	#if 0
	spi_clk_select(spi_n,ctrl_cfg->clk_div_select);
	if(ctrl_cfg->clk_div_select==ctrl_cfg->clk_div_select)
		spi_clk_div1(spi_n,ctrl_cfg->div1);
	else
		spi_clk_div2(spi_n,ctrl_cfg->div1);
	#endif
	//spi_set_clk(spi_n,20000000,100000000);
	spi_set_clk(spi_n,10000000,24000000);
	//register isr for spi master
	if(spi_n==0)
	{
		printf("Enable spi0 AW_IRQ_SPI0 interrupts!\n");
		eGon2_InsINT_Func(AW_IRQ_SPI0,(int*)spi0_int_proc,0);
		eGon2_EnableInt(AW_IRQ_SPI0);
	}
	else
	{
		printf("Enable spi1 AW_IRQ_SPI1 interrupts!\n");
		eGon2_InsINT_Func(AW_IRQ_SPI1,(int*)spi1_int_proc,0);
		eGon2_EnableInt(AW_IRQ_SPI1);
	}

	printf("Disable spi0 some interrupts!\n");
	spi_pend_clean(spi_n,0x3f77);
	spi_int_disable(spi_n,0x3f77);
	spi_dma_disable(spi_n,TF_DRQ_INT_EN|RF_DRQ_INT_EN);

	
	spi_tx_fifo_reset(spi_n);
	spi_rx_fifo_reset(spi_n);
	spi_tx_fifo_empty_trig_level(spi_n,32);
	spi_rx_fifo_ready_trig_level(spi_n,32);

	spi_test_disable(spi_n);

}

/*
*Note! spi controller does not supported wait status still send spi_clk
*Here we use gpio to simulate this function.
*/
void spi_send_wait_clk(u8 spi_n,u8 dual_signle,u32 size)
{	
	u32 val;
	u32 i;

	if(spi_n==0)
	{
		//config CLK as gpio
		val=readl(PC_CFG0);
		val&=0xffff0000;
		val|=0x00001133;
		writel(val,PC_CFG0);
		//printf("PC_CFG0=0x%x\n",readl(PC_CFG0));

		for(i=0;i<size;i++)
			{

				val=readl(PC_DAT);
				val|=(1<<2);
				writel(val,PC_DAT);
		
				val=readl(PC_DAT);
				val&=~(1<<2);
				writel(val,PC_DAT);
			}

		//recovery to hardware config spi_clk
		val=readl(PC_CFG0);
		val&=0xffff0000;
		val|=0x00001333;
		writel(val,PC_CFG0);
	}
	else
	{
		val=readl(PA_CFG1);
		val&=0x000fffff;
		val|=0x21100000;
		writel(val,PA_CFG1);	

		for(i=0;i<size;i++)
			{
			val=readl(PA_DAT);
			val|=(1<<14);
			writel(val,PA_DAT);

			val=readl(PA_DAT);
			val&=~(1<<14);
			writel(val,PA_DAT);
			}
		
		val=readl(PA_CFG1);
		val&=0x000fffff;
		val|=0x22100000;
		writel(val,PA_CFG1);	
	}
}

void spi_write2fifo(u8 spi_n,u8 *buf,u32 size)
{
	u32 j;
	u32 reg;
	u8 *pval=buf;
#if 1
	if(spi_n==0)
		{
		//printf("%s 0\n",__func__);
		reg=SPI0_TXD;
	}
	else
		reg=SPI1_TXD;

	//64byte packet
	if(size>SPI_MAX_FIFO_SIZE)
	{
	size=SPI_MAX_FIFO_SIZE;
	//printf("%s 1\n",__func__);
	}
#endif
	//write data into fifo
	for(j=0;j<(size);j++)
	{
		//printf("%s 2 %d 0x%x 0x%x\n",__func__,j,*pval,*buf);
		writeb(*pval,SPI0_TXD);
		pval++;
	}
}




void spi_read(u8 spi_n,u8 *buf,u32 size)
{
	u32 i;
	u32 reg;

	if(spi_n==0)
		reg=SPI0_RXD;
	else
		reg=SPI1_RXD;
spi_master_brust_start(spi_n);
	for(i=0;i<size;i++)
		{
		*buf=readl(reg);
		buf++;
		}
		
	
}

s32 spi_half_tx(u8 spi_n,u8* buf,u32 tx_size)
{
		s32 pool_time=0xfffffff;
		recv_flag=0;
	
		//if(tx_size==0)
		//	return -1;
		//spi_discard_brust_type(spi_n,1);
		spi_config_brusts(spi_n,tx_size,0,0);

	//	printf("TCR CUR:%x\n",readl(SPI0_TCR));
		
		//spi_wait_clk_count(0,0,0x8000);
		
		//fill fifo

		spi_write2fifo(spi_n,buf,tx_size);
		spi_master_brust_start(spi_n);
	//	printf("SPI0_FSR%d:%x %d\n",spi_n,readl(SPI0_FSR),tx_size);
		while(spi_query_txfifo(spi_n)&&((--pool_time)>0));

		if(pool_time<=0)
		{
			printf("spi tx data timeover!\n");
			return -1;
		}
		//else
		//{
			//printf("txFIFO ok!\n");
		//}

		udelay(1);
		//while(!recv_flag)
			{
			
			//printf("%d\n",recv_flag);
			}

		//printf("half tx ok!\n");
		return 0;
}
	
s32 spi_half_rx(u8 spi_n,u8* buf,u32 rx_size)
{
		s32 pool_time=0xfffff;
		u8 rx_len=rx_size;
		u8* rx_buf=buf;
		u32 reg;
		u32 last_tick2=0;
			
		//if(rx_size==0)
		//	return -1;		

		if(spi_n==0)
			reg=SPI0_RXD;
		else
			reg=SPI1_RXD;

		//recv burst config
		spi_discard_brust_type(spi_n,0);

		spi_config_brusts(spi_n,0,rx_len,0);
	//	printf("TCR CUR:%x\n",readl(SPI0_TCR));
		//last_tick2=sys_get_tick();
		//spi_wait_clk_count(0,0,0xffff);
		spi_master_brust_start(spi_n);
		

		while(rx_len && (--pool_time >0)) {
			/* rxFIFO counter */
			if(spi_query_rxfifo(0)){
				*rx_buf++ =  readb(reg);//fetch data
				--rx_len;
			}
		}
		//printf("cost tm:%d\n",(sys_get_tick()-last_tick2));
		if(pool_time <= 0) {
			//printf("spi receive data time out!\n");
			return -1;
		}	
	return 0;
}

//Full duplex work mode
//Received bytes=transmit bytes, must config controller to receive all brusts
s32 spi_full_tx_rx(u8 spi_n,u8* tx_buf,u8 *rx_buf,u32 tx_rx_size)
{
	u32 reg;
		s32 pool_time=0xfffff;
		u8* prx_buf=rx_buf;
		u32 rx_len=tx_rx_size;

		if(spi_n==0)
			reg=SPI0_RXD;
		else
			reg=SPI1_RXD;
	
		//send burst config 
		spi_discard_brust_type(spi_n,0);
		spi_config_brusts(spi_n,tx_rx_size,0,0);

		//fill fifo
		spi_write2fifo(spi_n,tx_buf,tx_rx_size);

		spi_master_brust_start(spi_n);
		while(spi_query_txfifo(spi_n)&&((--pool_time)>0));

		if(pool_time<=0)
		{
			printf("spi full tx data timeover!\n");
			return -1;
		}

		pool_time=0x7fffff;
		while(rx_len && (--pool_time >0)) {
				/* rxFIFO counter */
				if(spi_query_rxfifo(spi_n)){
					*prx_buf++ =  readb(reg);//fetch data
					--rx_len;
				}
			}
			if(pool_time <= 0) {
				printf("cpu receive data time out!\n");
				return -1;
			}
		return 0;

}

extern void spi_lcd_init_master(u8 spi_n);

void spi_tx_dma_config(u8* data_addr,u32 size);
u8 tx_dma_complete=0;
/*
*These some bugs, but can used
*/
void spi_one_transmit(u8 spi_n, u8* tx_buf, u32 tx_size, u8* rx_buf, u32 rx_size)
{
		u8 spi_transmit_mode=0;
		u32 i;


		//u8 one_tr_complete=1;

	//	printf("Enable spi0 some interrupts!\n");
//spi_lcd_init_master(0);
//		spi_pend_clean(spi_n,0x3f77);
//		spi_int_enable(spi_n,TC_INT_EN|SPI_ERR);
//	spi_tx_fifo_dma_req(0,1);
//	spi_tx_fifo_reset(0);
		//spi_cs_soft_low(spi_n);
	
		//check transmit mode is half tx/rx or full
		if((tx_size==0)&&(rx_size!=0))
		{
			spi_transmit_mode=1;
		}
		else if((tx_size!=0)&&(rx_size==0))
		{
			spi_transmit_mode=0;
		}
		else 
		{
			spi_transmit_mode=2;
		}



		switch(spi_transmit_mode)
			{
			case 0://tx cmd/data
				{

			if(/*tx_size>=64*/0)
				{
			//send burst config ,影响miso接收数据,
				spi_discard_brust_type(spi_n,1);
			//	spi_lcd_init_master(spi_n);
			//	spi_cs_soft_high(spi_n);
			//	spi_cs_soft_low(spi_n);
				spi_pend_clean(spi_n,0x3f77);
				spi_int_enable(spi_n,TC_INT_EN|TF_FUL_INT_EN|SPI_ERR);	
				//spi_int_enable(spi_n,TC_INT_EN|TF_FUL_INT_EN|SPI_ERR);
				//spi_int_enable(0,TF_FUL_INT_EN);
				spi_tx_fifo_dma_req(0,0);
				spi_rx_fifo_dma_req(0,0);
				spi_tx_fifo_reset(0);
				spi_rx_fifo_reset(0);

				spi_config_brusts(spi_n,tx_size,0,0);


				spi_master_brust_start(spi_n);

				spi_tx_fifo_dma_req(0,1);

				tx_dma_complete=1;
				spi_tx_dma_config(tx_buf,tx_size);
				while(tx_dma_complete);
				printf("[%s] SPI0_FSR 0x%08x\n",__func__,readl(SPI0_FSR));
				printf("DMA END\n");

				}
			else
				{
				u32 pack_n=tx_size/64;
				u32 left_byte=tx_size%64;
			
				//printf("half tx %d\n",tx_size);
						spi_discard_brust_type(spi_n,1);
					
					for(i=0;i<(pack_n);i++)
					{
						spi_half_tx( spi_n,tx_buf+64*i,64);
					}
						
					if(left_byte)
						spi_half_tx( spi_n,tx_buf+64*i,tx_size%64);
					//printf("[%s] IER 0x%08x\n",__func__,readl(SPI0_FSR));
				}
					break;
				}
			case 1://rx data
				{
			//		printf("half rx\n");
					for(i=0;i<(rx_size/64);i++)
					{
						spi_half_rx(spi_n,rx_buf+64*i,64);
					}

						spi_half_rx(spi_n,rx_buf+64*i,rx_size%64);

					break;
				}
			case 2://tx cmd,rxdata
				{
			//	printf("full tx rx\n");
					for(i=0;i<(tx_size/64);i++)
					{
					spi_full_tx_rx(spi_n,tx_buf+64*i,rx_buf+64*i,64);
					}
					spi_full_tx_rx(spi_n,tx_buf+64*i,rx_buf+64*i,tx_size%64);
	
				break;
				}
			default:break;
			}

		//isr result

		//spi_cs_soft_high(spi_n);

}


void spi_tx_dma_callback0(void* paras)
{
	tx_dma_complete=0;
	printf("%s\n",__func__);
	spi_tx_fifo_dma_req(0,0);
}

void spi_rx_dma_callback0(void* paras)
{
	printf("%s\n",__func__);
	spi_rx_fifo_dma_req(0,0);
}


/*
*DMA exmple function
*/

extern void  flush_cache(unsigned long start, unsigned long size);
void spi_tx_dma_config(u8* data_addr,u32 size)
{
	struct dma_channel_config_t chan_cfg;

	chan_cfg.src_dw=DMA_DATA_BIT_WIDTH_8;
	chan_cfg.src_bst_len=DMA_BRUST_LENGTH_1;
	chan_cfg.src_addr_mode=DMA_ADDR_MODE_LINEAR;
	chan_cfg.src_dqr_type=DMA_DRQ_TYPE_SRAM;

	chan_cfg.dest_dw=DMA_DATA_BIT_WIDTH_8;
	chan_cfg.dest_bst_len=DMA_BRUST_LENGTH_1;
	chan_cfg.dest_addr_mode=DMA_ADDR_MODE_IO;
	chan_cfg.dest_drq_type=DMA_DRQ_TYPE_SPI0_TX;

	



/*
*H3 does not include this register, but H5 have it
*/
//	printf("DMA CTRL %x\n",readl(SPI0_NOR_DMA_CTRL));
//	writel(0x5a,SPI0_NOR_DMA_CTRL);
//	printf("DMA CTRL %x\n",readl(SPI0_NOR_DMA_CTRL));

	struct dma_descriptor_info_t desc_info;
	desc_info.parameter=0;
	desc_info.pnext=0xfffff800;

	//spi_wait_clk_count(0,0,100);
	//writel(readl(SPI0_ISR) | 0x0200, SPI0_ISR);

	dma_cfg_src_mode(1,1);
	dma_cfg_dst_mode(1,1);


	dma_register(1,&chan_cfg,&desc_info);
	dma_set_callback(1,spi_tx_dma_callback0,0);
	//dma_transmit_start(1,data_addr,SPI0_TXD,size);
	//flush_cache((uint)data_addr, size);
	printf("DMA START %d\n",size);
	dma_transmit_start(1,data_addr,SPI0_TXD,size);
	delay_ms(10);
}

void spi_rx_dma_config(u8* data_addr,u32 size)
{
	struct dma_channel_config_t chan_cfg;


	chan_cfg.src_dw=DMA_DATA_BIT_WIDTH_8;
	chan_cfg.src_bst_len=DMA_BRUST_LENGTH_1;
	chan_cfg.src_addr_mode=DMA_ADDR_MODE_IO;
	chan_cfg.src_dqr_type=DMA_DRQ_TYPE_SPI0_RX;

	chan_cfg.dest_dw=DMA_DATA_BIT_WIDTH_8;
	chan_cfg.dest_bst_len=DMA_BRUST_LENGTH_1;
	chan_cfg.dest_addr_mode=DMA_ADDR_MODE_LINEAR;
	chan_cfg.dest_drq_type=DMA_DRQ_TYPE_SRAM;


	struct dma_descriptor_info_t desc_info;
	desc_info.parameter=0;
	desc_info.pnext=0xfffff800;

	dma_register(2,&chan_cfg,&desc_info);
	dma_set_callback(2,spi_rx_dma_callback0,0);
	dma_transmit_start(2,SPI0_RXD,data_addr,size);

}




