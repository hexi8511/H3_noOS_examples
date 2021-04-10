#ifndef  __CCU_H__
#define  __CCU_H__

#include <linux/types.h>
enum PLL_TYPE
{
PLL_TYPE_CPUX,
PLL_TYPE_AUDIO,
PLL_TYPE_VIDEO,
PLL_TYPE_VE,
PLL_TYPE_DDR,
PLL_TYPE_PERIPH0,
PLL_TYPE_GPU,
PLL_TYPE_PERIPH1,
PLL_TYPE_DE,
};



#define CLR_BIT(val,bit) (val&(~(1<<bit)))
#define SET_BIT(val,bit) (val|(1<<bit))

#define PLL_EN_BIT_CLR (~(1<<31))
#define PLL_EN_BIT_SET ((1<<31))

#define PLL_EN_BIT		31
#define PLL_LOCKED_BIT 	28

/* clock ID */
#define AXI_BUS		(0)
#define AHB1_BUS0	(1)
#define AHB1_BUS1	(2)
#define AHB1_LVDS	(3)
#define APB1_BUS0	(4)
#define APB2_BUS0	(5)
#define AHB2_BUS	(6)
/* axi */
#define DRAMMEM_CKID	((AXI_BUS << 8) | 0)
/* ahb1 branc0 */
#define USBOHCI3_CKID	((AHB1_BUS0 << 8) | 31)
#define USBOHCI2_CKID	((AHB1_BUS0 << 8) | 30)
#define USBOHCI1_CKID	((AHB1_BUS0 << 8) | 29)
#define OTGOHCI0_CKID	((AHB1_BUS0 << 8) | 28)
#define USBEHCI3_CKID	((AHB1_BUS0 << 8) | 27)
#define USBEHCI2_CKID	((AHB1_BUS0 << 8) | 26)
#define USBEHCI1_CKID	((AHB1_BUS0 << 8) | 25)
#define OTGEHCI0_CKID	((AHB1_BUS0 << 8) | 24)
#define OTGDEV_CKID		((AHB1_BUS0 << 8) | 23)
#define SPI1_CKID		((AHB1_BUS0 << 8) | 21)
#define SPI0_CKID		((AHB1_BUS0 << 8) | 20)
#define HSTMR_CKID		((AHB1_BUS0 << 8) | 19)
#define TS_CKID			((AHB1_BUS0 << 8) | 18)
#define GMAC_CKID		((AHB1_BUS0 << 8) | 17)
#define DRAMREG_CKID	((AHB1_BUS0 << 8) | 14)
#define NAND0_CKID		((AHB1_BUS0 << 8) | 13)

#define SDC2_CKID		((AHB1_BUS0 << 8) | 10)
#define SDC1_CKID		((AHB1_BUS0 << 8) | 9 )
#define SDC0_CKID		((AHB1_BUS0 << 8) | 8 )

#define DMA_CKID		((AHB1_BUS0 << 8) | 6 )
#define SS_CKID			((AHB1_BUS0 << 8) | 5 ) //CE

/* ahb1 branc1 */

#define SPINLOCK_CKID	((AHB1_BUS1 << 8) | 22)
#define MSGBOX_CKID		((AHB1_BUS1 << 8) | 21)
#define GPU_CKID		((AHB1_BUS1 << 8) | 20)

#define BE_CKID		((AHB1_BUS1 << 8) | 12)
#define HDMI_CKID		((AHB1_BUS1 << 8) | 11)
#define TVE_CKID		((AHB1_BUS1 << 8) | 9)
#define CSI_CKID		((AHB1_BUS1 << 8) | 8)
#define DEINTERLACE_CKID		((AHB1_BUS1 << 8) | 5)
#define TCON1_CKID		((AHB1_BUS1 << 8) | 4)
#define TCON0_CKID		((AHB1_BUS1 << 8) | 3)
#define VE_CKID			((AHB1_BUS1 << 8) | 0)
/* ahb1 special for lvds */
#define LVDS_CKID		((AHB1_LVDS << 8) | 0)

/* apb1  */
#define IIS2_CKID		((APB1_BUS0 << 8) | 14)
#define IIS1_CKID		((APB1_BUS0 << 8) | 13)
#define IIS0_CKID		((APB1_BUS0 << 8) | 12)

#define THS_CKID		((APB1_BUS0 << 8) | 8)
#define PIO_CKID		((APB1_BUS0 << 8) | 5)
#define SPDIF_CKID		((APB1_BUS0 << 8) | 1)
#define CODEC_CKID		((APB1_BUS0 << 8) | 0)

/* apb2  */
#define SCR_CKID		((APB2_BUS0 << 8) | 20)
#define UART3_CKID		((APB2_BUS0 << 8) | 19)
#define UART2_CKID		((APB2_BUS0 << 8) | 18)
#define UART1_CKID		((APB2_BUS0 << 8) | 17)
#define UART0_CKID		((APB2_BUS0 << 8) | 16)

#define TWI2_CKID		((APB2_BUS0 << 8) | 2)
#define TWI1_CKID		((APB2_BUS0 << 8) | 1)
#define TWI0_CKID		((APB2_BUS0 << 8) | 0)

/*ahb2 branch*/
#define DBGSYS_CKID		((AHB2_BUS << 8) | 7)
#define EPHY_CKID		((AHB2_BUS << 8) | 0)


struct cpux_pll_table_t
{
	unsigned long long pll;
	unsigned int n;
	unsigned int k;
	unsigned int m;
	unsigned int p;
};

struct audio_pll_table_t
{
	unsigned long long pll;
	unsigned int n;
	unsigned int m;
	unsigned int p;
};

struct ddr_pll_table_t
{
	unsigned long long pll;
	unsigned int n;
	unsigned int k;
	unsigned int m;

};

struct periphx_pll_table_t
{
	unsigned long long pll;
	unsigned int n;
	unsigned int k;
	//unsigned int m;//just for debug
};

struct ve_pll_table_t
{
	unsigned long long pll;
	unsigned int n;
	unsigned int m;
};

int pll_enable(enum PLL_TYPE type);
int pll_disable(enum PLL_TYPE type);
int pll_set_out_freq(u8 type,unsigned long long int pll);

void set_cpux_clksrc(char src);
void set_cpux_apb_div(char div);
void set_cpux_axi_div(char div);
void set_ahb1_clksrc(char src);
void set_ahb1_prediv(char div);
void set_ahb1_div(char div);
void set_apb1_div(char div);
void set_apb2_clksrc(char src);
void set_apb2_prediv(char div);
void set_apb2_div(char div);
void set_ahb2_clksrc(char src);

void ccm_module_disable(u32 clk_id);
void ccm_module_enable(u32 clk_id);
void ccm_clock_disable(u32 clk_id);
void ccm_clock_enable(u32 clk_id);

void ccm_module_reset(u32 clk_id);
void ccm_clock_disable_all(void);
void ccm_reset_all_module(void);

void open_apb0_clk();
void init_ahb_apb_clk();
void sys_clk_init();

#endif
