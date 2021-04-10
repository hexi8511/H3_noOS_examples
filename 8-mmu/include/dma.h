#ifndef __DMA_H__
#define __DMA_H__

#define DMA_BASE_ADDR 0x01c02000


#define DMA_IRQ_EN_REG0				(DMA_BASE_ADDR+0x00)
#define DMA_IRQ_EN_REG1				(DMA_BASE_ADDR+0x04)
#define DMA_IRQ_PEND_REG0			(DMA_BASE_ADDR+0x10)
#define DMA_IRQ_PEND_REG1			(DMA_BASE_ADDR+0x14)
#define DMA_SEC_REG					(DMA_BASE_ADDR+0x20)
#define DMA_AUTO_GATE_REG			(DMA_BASE_ADDR+0x28)
#define DMA_STA_REG					(DMA_BASE_ADDR+0x30)

#define DMA_EN_REG(n)				(DMA_BASE_ADDR+0x100+n*0x40+0x00)
#define DMA_PAU_REG(n)				(DMA_BASE_ADDR+0x100+n*0x40+0x04)
#define DMA_DESC_ADDR_REG(n)		(DMA_BASE_ADDR+0x100+n*0x40+0x08)
#define DMA_CFG_REG(n)				(DMA_BASE_ADDR+0x100+n*0x40+0x0c)
#define DMA_CUR_SRC_REG(n)			(DMA_BASE_ADDR+0x100+n*0x40+0x10)
#define DMA_CUR_DEST_REG(n)			(DMA_BASE_ADDR+0x100+n*0x40+0x14)
#define DMA_BCNT_LEFT_REG(n)		(DMA_BASE_ADDR+0x100+n*0x40+0x18)
#define DMA_PARA_REG(n)				(DMA_BASE_ADDR+0x100+n*0x40+0x1c)
#define DMA_FDESC_ADDR_REG(n)		(DMA_BASE_ADDR+0x100+n*0x40+0x2c)
#define DMA_PKG_NUM_REG(n)			(DMA_BASE_ADDR+0x100+n*0x40+0x30)

#if 0
#define DMA_CHAN0_EN_REG			(DMA_BASE_ADDR+0x100+0*0x40+0x00)
#define DMA_CHAN0_PAU_REG			(DMA_BASE_ADDR+0x100+0*0x40+0x04)
#define DMA_CHAN0_DESC_ADDR_REG		(DMA_BASE_ADDR+0x100+0*0x40+0x08)
#define DMA_CHAN0_CFG_REG			(DMA_BASE_ADDR+0x100+0*0x40+0x0c)
#define DMA_CHAN0_CUR_SRC_REG		(DMA_BASE_ADDR+0x100+0*0x40+0x10)
#define DMA_CHAN0_CUR_DEST_REG		(DMA_BASE_ADDR+0x100+0*0x40+0x14)
#define DMA_CHAN0_BCNT_LEFT_REG		(DMA_BASE_ADDR+0x100+0*0x40+0x18)
#define DMA_CHAN0_PARA_REG			(DMA_BASE_ADDR+0x100+0*0x40+0x1c)
#define DMA_CHAN0_FDESC_ADDR_REG	(DMA_BASE_ADDR+0x100+0*0x40+0x2c)
#define DMA_CHAN0_PKG_NUM_REG		(DMA_BASE_ADDR+0x100+0*0x40+0x30)
#endif

#define DMA_PACKAGE_END 0xfffff800
#define DMA_MAX_CHANNEL 12
/**
* DMA data width
*/
enum dma_data_bit_width
{
	DMA_DATA_BIT_WIDTH_8=0,
	DMA_DATA_BIT_WIDTH_16=1,
	DMA_DATA_BIT_WIDTH_32=2,
	DMA_DATA_BIT_WIDTH_64=3,
};

/**
* DMA Burst length
*/
enum  dma_brust_length
{
	DMA_BRUST_LENGTH_1=0,
	DMA_BRUST_LENGTH_4=1,
	DMA_BRUST_LENGTH_8=2,
	DMA_BRUST_LENGTH_16=3,
};

/**
* DMA Address mode
*/
enum dma_addr_mode
{
	DMA_ADDR_MODE_LINEAR=0,
	DMA_ADDR_MODE_IO=1,
};

/**
* DMA DRQ type
*/
enum dma_drq_typ
{
MAX_TYPE=0
};

struct dma_channel_config_t
{
	/*dest config parameter*/
	unsigned char dest_dw;
	unsigned char dest_bst_len;
	unsigned char dest_addr_mode;
	unsigned char dest_drq_type;
	
	/*source config parameter*/
	unsigned char src_dw;
	unsigned char src_bst_len;
	unsigned char src_addr_mode;
	unsigned char src_dqr_type;
};

struct dma_descriptor_info_t
{
	unsigned int configuration;
	unsigned int src_addr;
	unsigned int dest_addr;
	unsigned int byte_cnt;
	unsigned int parameter;
	struct dma_descriptor_info_t* pnext;
} ;


typedef 	void (* dma_callback)(void* data);

struct dma_channel_info_t
{
	struct dma_descriptor_info_t desc_info;	//DMA description information
	unsigned char channel;								//Channel
	void *parameters;										//DMA process parameters
	dma_callback dma_process_handler;					//DMA process handler
};


enum DMA_IRQ_TYPE
{
	DMA_IRQ_TYPE_HALF = 0x01,
	DMA_IRQ_TYPE_PKG  = 0x02,
	DMA_IRQ_TYPE_QUEUE= 0x04,
};

enum 
{
	SECURE=1,
	NOSECURE=0
};


void dma_irq_pend_clear(unsigned char channel,unsigned char type);
void dma_main(void);
#endif
