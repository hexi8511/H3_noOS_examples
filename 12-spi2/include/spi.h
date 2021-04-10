#ifndef __SPI_H__
#define __SPI_H__

#include "ccu_reg.h"
#include "stdio.h"
#include "string.h"
#include "intc.h"
#include <linux/types.h>
#include <linux/io.h>
#include "gpio.h"

#define SPI0_BASE	0x01c68000
#define SPI1_BASE	0x01c69000


#define SPI0_VER	(SPI0_BASE+0x00)
#define SPI0_GCR	(SPI0_BASE+0x04)
#define SPI0_TCR	(SPI0_BASE+0x08)
#define SPI0_IER		(SPI0_BASE+0x10)
#define SPI0_ISR		(SPI0_BASE+0x14)
#define SPI0_FCR	(SPI0_BASE+0x18)
#define SPI0_FSR	(SPI0_BASE+0x1c)
#define SPI0_WCR	(SPI0_BASE+0x20)
#define SPI0_CCR	(SPI0_BASE+0x24)
#define SPI0_MBC	(SPI0_BASE+0x30)
#define SPI0_MTC	(SPI0_BASE+0x34)
#define SPI0_BCC	(SPI0_BASE+0x38)
#define SPI0_NOR_DMA_CTRL	(SPI0_BASE+0x88)
#define SPI0_TXD	(SPI0_BASE+0x200)
#define SPI0_RXD	(SPI0_BASE+0x300)

#define SPI1_VER	(SPI1_BASE+0x00)
#define SPI1_GCR	(SPI1_BASE+0x04)
#define SPI1_TCR	(SPI1_BASE+0x08)
#define SPI1_IER		(SPI1_BASE+0x10)
#define SPI1_ISR		(SPI1_BASE+0x14)
#define SPI1_FCR	(SPI1_BASE+0x18)
#define SPI1_FSR	(SPI1_BASE+0x1c)
#define SPI1_WCR	(SPI1_BASE+0x20)
#define SPI1_CCR	(SPI1_BASE+0x24)
#define SPI1_MBC	(SPI1_BASE+0x30)
#define SPI1_MTC	(SPI1_BASE+0x34)
#define SPI1_BCC	(SPI1_BASE+0x38)
#define SPI1_NOR_DMA_CTRL	(SPI1_BASE+0x88)
#define SPI1_TXD	(SPI1_BASE+0x200)
#define SPI1_RXD	(SPI1_BASE+0x300)


#define SPI_WORK_MODE_MASTER 1
#define SPI_WORK_MODE_SLEVER		0

#define PHASE_0		0
#define PHASE_1		1

#define POL_HIGH 	0
#define POL_LOW		1

#define ASSERT_SS	0
#define NEGATE_SS	1

#define SPI_SS0		0
#define SPI_SS1		1
#define SPI_SS2		2
#define SPI_SS3		3

#define SPI_OWNER_HW 	0
#define SPI_OWNER_SW	1

#define LOW_LEVEL	0
#define HIGH_LEVEL	1

#define RECV_ALL_BURST 0
#define DISCARD_UNUSED_BURST	1

#define BRUST_BIT_ZERO	0
#define BRUST_BIT_ONE	1

#define NOR_WRITE_MODE		0
#define RAPIDS_WRITE_MODE 	1

#define NOR_OPT_MODE		0
#define DELAY_OPT_MODE		1

#define MSB_FIRST			0
#define LSB_FIRST			1

#define DELAY_SAMPLE_MODE	0
#define NORMAL_SAMPLE_MODE	1


//interrupt control bit
#define SS_INT_EN	(1<<13)
#define TC_INT_EN	(1<<12)
#define TF_UDR_INT_EN	(1<<11)
#define TF_OVF_INT_EN	(1<<10)
#define RF_UDR_INT_EN	(1<<9)
#define RF_OVF_INT_EN	(1<<8)

#define TF_FUL_INT_EN	(1<<6)
#define TX_EMP_INT_EN	(1<<5)
#define TX_ERQ_INT_EN	(1<<4)

#define RX_FUL_INT_EN	(1<<2)
#define RX_EMP_INT_EN	(1<<1)
#define RF_RDY_INT_EN	(1<<0)

#define SPI_ERR (TF_UDR_INT_EN|TF_OVF_INT_EN|RF_UDR_INT_EN|RF_OVF_INT_EN)

//interrupt statu bit
#define SS_INT_STA	(1<<13)
#define TC_INT_STA	(1<<12)
#define TF_UDR_INT_STA	(1<<11)
#define TF_OVF_INT_STA	(1<<10)
#define RF_UDR_INT_STA	(1<<9)
#define RF_OVF_INT_STA	(1<<8)

#define TF_FUL_INT_STA	(1<<6)
#define TX_EMP_INT_STA	(1<<5)
#define TX_ERQ_INT_STA	(1<<4)

#define RX_FUL_INT_STA	(1<<2)
#define RX_EMP_INT_STA	(1<<1)
#define RF_RDY_INT_STA	(1<<0)


#define TF_DRQ_INT_EN	(1<<24)
#define RF_DRQ_INT_EN	(1<<8)

//FIFO RST
#define TX_FIFO_RST	(1<<31)
#define RX_FIFO_RST	(1<<15)

#define SPI_CLK_DIV1	0
#define SPI_CLK_DIV2		1

#define SPI_MAX_FIFO_SIZE 64


struct spi_controller
{
	u8 transmit_pause_enable;			//0 irgnore rxfifo full, 1	stop transmit rxfifo full
	u8 work_mode;							//0 slave mode, 1 master mode
	u8 module_enable;						//0 disable,1 enable	
	u8 master_sample_data_mode;	//0 delay,1 normal
	u8 first_transmit_bit;					//0 msb, 1lsb
	u8 master_sample_data_opt;		//0 normal operate, 1delay
	u8 rapid_mode;							//0 normal write,1 repid writel	
	u8 dummy_burst_type;				//0 bit value is 0,1 bit value is 1
	u8 discard_hash_burst;				//0 receive all spi brust,1 discard unused burst
	u8 ss_level;									//0 set ss to low, 1 set ss to high
	u8 ss_owner;								//0 spi hardware, 1 softerware
	u8 ss_select;								//0 1 2 3=>ss0 ss1 ss2 ss3 
	u8 ss_ctrl;									//0 ss still asserted between brusts,1 ss negate between brusts
	u8 chip_select_pol;						//0 high polarity, 1 low polarity
	u8 spi_clk_pol;								//0 high ,1 low
	u8 spi_clk_phase;							//0 leading edge for sample data,1 leading edge for setup
	u32 int_ctrl_flag;
	
	//spi fifo dma
	u8 tx_fifo_dma_req;						//0 disable dma req, 1 enable dma req
	u8 tx_fifo_empty_trig_level;			//256-0
	u8 rx_fifo_dma_req;						//0 disable , 1 enable
	u8 rx_fifo_dma_mode;					//0 normal, 1 dedicate mode
	u8 rx_fifo_ready_req_trig_level;	//0-256

	//spi wait clock
	u8 dual_wait_clk_cnt;				//daul mode wait clk count between datas
	u16 signle_wait_clk_cnt;			//signle mode wait clk count between datas
	
	//below just for master 
	u8 clk_div_select;								//0 div rate 1,1 div rate 2
	u8 div1;									//spi clk=src/2^n     n=0~16
	u8 div2;									//spi clk=src/(2*(n+1))  n=0~255

	//
	u8 recv_s_or_d;						//0 signle mode, 1 daul modes
	u8 recv_dummy_brust_cnt_daul;//0-16
	u32 transmit_cnt_signle;//0~2^24
	
};
extern void spi_send_wait_clk(u8 spi_n,u8 dual_signle,u32 size);
extern void spi_wait_clk_count(u8 spi_n,u8 dual_signle,u16 count);
extern void spi_cs_soft_high(u8 spi_n );
extern void spi_cs_soft_low(u8 spi_n );
extern void spi_init(u8 spi_n,struct spi_controller *ctrl_cfg);
extern void spi_one_transmit(u8 spi_n, u8* tx_buf, u32 tx_size, u8* rx_buf, u32 rx_size);

#endif
