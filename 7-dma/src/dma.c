#include "dma.h"
#include <stdio.h>
#include <linux/types.h>
#include <linux/io.h>

/**
*DMA Channel information data array
*/
struct dma_channel_info_t  dma_channel_info[DMA_MAX_CHANNEL];


/**
*
*/
void dma_irq_enable(unsigned char channel,unsigned char type)
{
	unsigned int irq_reg0=readl(DMA_IRQ_EN_REG0);
	unsigned int irq_reg1=readl(DMA_IRQ_EN_REG1);

	if(channel<8)
	{
		irq_reg0&=(~(0xf<<(channel*4)));
		irq_reg0|=(type<<(channel*4));
	}
	else
	{
		irq_reg1&=(~(0xf<<((channel-8)*4)));
		irq_reg1|=(type<<((channel-8)*4));
	}

	writel(irq_reg0,DMA_IRQ_EN_REG0);
	writel(irq_reg1,DMA_IRQ_EN_REG1);
}


void dma_irq_disable(unsigned char channel,unsigned char type)
{
	unsigned int irq_reg0=readl(DMA_IRQ_EN_REG0);
	unsigned int irq_reg1=readl(DMA_IRQ_EN_REG1);

	if(channel<8)
	{
		irq_reg0&=(~(type<<(channel*4)));
	}
	else
	{
		irq_reg1&=(~(type<<((channel-8)*4)));
	}

	writel(irq_reg0,DMA_IRQ_EN_REG0);
	writel(irq_reg1,DMA_IRQ_EN_REG1);
}

void dma_irq_pend_clear(unsigned char channel,unsigned char type)
{
	unsigned int reg0=readl(DMA_IRQ_PEND_REG0);
	unsigned int reg1=readl(DMA_IRQ_PEND_REG1);

	//printf("reg0=0x%08x\n",reg0);
	//printf("reg1=0x%08x\n",reg1);
	if(channel<8)
	{
		reg0&=((type<<(channel*4)));
	}
	else
	{
		reg1&=((type<<((channel-8)*4)));
	}
	//printf("2reg0=0x%08x\n",reg0);
	//printf("2reg1=0x%08x\n",reg1);
	writel(reg0,DMA_IRQ_PEND_REG0);
	writel(reg1,DMA_IRQ_PEND_REG1);
}

void dma_irq_process(void)
{
	unsigned int reg0=readl(DMA_IRQ_PEND_REG0);
	unsigned int reg1=readl(DMA_IRQ_PEND_REG1);
	int n;
  unsigned int type=7;
//printf("dma_pend0:0x%08x, 0x%08x\n",reg0,reg1);

	for(n=0;n<8;n++)
		{
		if(	reg0&(0x7<<(n*4)))
			{
				
				//dma_irq_pend_clear(0,7);
				reg0&=((type<<(n*4)));
				writel(reg0,DMA_IRQ_PEND_REG0);
				
				//process channel n
				if(dma_channel_info[n].dma_process_handler)
					dma_channel_info[n].dma_process_handler(dma_channel_info[n].parameters);
				
			}
		}
	
	for(n=0;n<4;n++)
	{
		if(	reg1&(0x7<<(n*4)))
			{
			
				//dma_irq_pend_clear(0,7);
				reg1&=((type<<(n*4)));
				writel(reg1,DMA_IRQ_PEND_REG1);

				//process channel n+8
				if(dma_channel_info[n+8].dma_process_handler)
					dma_channel_info[n+8].dma_process_handler(dma_channel_info[n+8].parameters);
			}
		}
	
}



void dma_set_secure(unsigned char channel,unsigned char sec)
{
	unsigned int reg=readl(DMA_SEC_REG);

	if(sec==1)
	{
		reg|=(1<<channel);
	}
	else
	{
		reg&=(~(1<<channel));
	}
	
	writel(reg,DMA_SEC_REG);
}

int dma_is_busy(unsigned char channel)
{
	unsigned int reg=readl(DMA_STA_REG);

	return ((reg&(1<<channel))>>channel);
}

void dma_enable(unsigned char channel)
{
	writel(1,	DMA_EN_REG(channel));
}

void dma_disable(unsigned char channel)
{
	writel(0,	DMA_EN_REG(channel));
}

void dma_pause(unsigned char channel)
{
	writel(1,DMA_PAU_REG(channel));
}

void dma_resume(unsigned char channel)
{
	writel(0,DMA_PAU_REG(channel));
}


//struct dma_descriptor_info_t sram2sdram_di;

void dma_channel_init(unsigned char channel,struct dma_descriptor_info_t *di)
{	
	//set dma descriptor info struct address
	writel((unsigned int)di,DMA_DESC_ADDR_REG(channel));

	//enable dma channel 
	dma_enable(channel);
}	



void dma_register(unsigned char channel, struct dma_channel_config_t *cc, struct dma_descriptor_info_t* ci )
{

	dma_irq_pend_clear(channel,DMA_IRQ_TYPE_PKG|DMA_IRQ_TYPE_HALF|DMA_IRQ_TYPE_QUEUE);
	dma_irq_enable(channel,DMA_IRQ_TYPE_PKG|DMA_IRQ_TYPE_HALF|DMA_IRQ_TYPE_QUEUE);
	
	while(dma_is_busy(channel));

	printf("DMA channel %d is idle\n",channel);
	
	//full dma descriptor info struct 
	dma_channel_info[channel].desc_info.configuration=(cc->dest_dw<<25)|(cc->dest_bst_len<<22)|
		(cc->dest_addr_mode<<21)|(cc->dest_drq_type<<16)|(cc->src_dw<<9)|
		(cc->src_bst_len<<6)|(cc->src_addr_mode<<5)|(cc->src_dqr_type);

	dma_channel_info[channel].desc_info.parameter=ci->parameter;
	dma_channel_info[channel].desc_info.pnext=ci->pnext;

	//dma_channel_init(0,&sram2sdram_di);

	//set dma descriptor info struct address
	writel((unsigned int)&dma_channel_info[channel],DMA_DESC_ADDR_REG(channel));
}

void dma_set_callback(unsigned char channel,dma_callback func,void*paras)
{
		dma_channel_info[channel].dma_process_handler=func;
		dma_channel_info[channel].parameters=paras;
}



void dma_transmit_start(unsigned char channel, unsigned int src_addr, 
unsigned int dest_addr, unsigned int size)
{
	dma_channel_info[channel].desc_info.src_addr=src_addr;
	dma_channel_info[channel].desc_info.dest_addr=dest_addr;
	dma_channel_info[channel].desc_info.byte_cnt=size;
	
	//enable dma channel 
	dma_enable(channel);
}

void dma_transmit_stop()
{

}

void dma_callback0(void* paras)
{
	printf("%s\n",__func__);
}

/**
*DMA exmple function
*/
void dma_main(void)
{
	struct dma_channel_config_t chan_cfg;

	chan_cfg.dest_dw=DMA_DATA_BIT_WIDTH_8;
	chan_cfg.dest_bst_len=DMA_BRUST_LENGTH_1;
	chan_cfg.dest_addr_mode=DMA_ADDR_MODE_LINEAR;
	chan_cfg.dest_drq_type=0;
	chan_cfg.src_dw=DMA_DATA_BIT_WIDTH_8;
	chan_cfg.src_bst_len=DMA_BRUST_LENGTH_1;
	chan_cfg.src_addr_mode=DMA_ADDR_MODE_LINEAR;
	chan_cfg.src_dqr_type=0;

	struct dma_descriptor_info_t desc_info;
	desc_info.parameter=64;
	desc_info. pnext=0xfffff800;

	dma_register(1,&chan_cfg,&desc_info);
	dma_set_callback(1,dma_callback0,0);
	dma_transmit_start(1,0x40000000,0x60000000,0x10000000);

}

