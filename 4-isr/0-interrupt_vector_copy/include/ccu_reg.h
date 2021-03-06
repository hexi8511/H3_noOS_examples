#ifndef  __CCU_REG_H__
#define  __CCU_REG_H__

#define CCU_REG_BASE_ADDR 0x01C20000


#define PLL_CPUX_CTRL_REG		(CCU_REG_BASE_ADDR+0x00)
#define PLL_AUDIO_CTRL_REG 		(CCU_REG_BASE_ADDR+0x08)
#define PLL_VIDEO_CTRL_REG		(CCU_REG_BASE_ADDR+0x10)
#define PLL_VE_CTRL_REG			(CCU_REG_BASE_ADDR+0x18)
#define PLL_DDR_CTRL_REG		(CCU_REG_BASE_ADDR+0x20)
#define PLL_PERIPH0_CTRL_REG	(CCU_REG_BASE_ADDR+0x28)
#define PLL_GPU_CTRL_REG		(CCU_REG_BASE_ADDR+0x38)
#define PLL_PERIPH1_CTRL_REG	(CCU_REG_BASE_ADDR+0x44)
#define PLL_DE_CTRL_REG			(CCU_REG_BASE_ADDR+0x48)

#define CPUX_AXI_CFG_REG		(CCU_REG_BASE_ADDR+0x50)
#define AHB1_APB1_CFG_REG		(CCU_REG_BASE_ADDR+0x54)
#define APB2_CFG_REG			(CCU_REG_BASE_ADDR+0x58)
#define AHB2_CFG_REG			(CCU_REG_BASE_ADDR+0x5c)

#define BUS_CLK_GATING_REG0 	(CCU_REG_BASE_ADDR+0x60)
#define BUS_CLK_GATING_REG1 	(CCU_REG_BASE_ADDR+0x64)
#define BUS_CLK_GATING_REG2 	(CCU_REG_BASE_ADDR+0x68)
#define BUS_CLK_GATING_REG3 	(CCU_REG_BASE_ADDR+0x6c)
#define BUS_CLK_GATING_REG4 	(CCU_REG_BASE_ADDR+0x70)

#define THS_CLK_REG				(CCU_REG_BASE_ADDR+0x74)
#define NAND_CLK_REG			(CCU_REG_BASE_ADDR+0x80)
#define SDMMC0_CLK_REG			(CCU_REG_BASE_ADDR+0x88)
#define SDMMC1_CLK_REG			(CCU_REG_BASE_ADDR+0x8c)
#define SDMMC2_CLK_REG			(CCU_REG_BASE_ADDR+0x90)
#define CE_CLK_REG				(CCU_REG_BASE_ADDR+0x9c)
#define SPI0_CLK_REG			(CCU_REG_BASE_ADDR+0xa0)
#define SPI1_CLK_REG			(CCU_REG_BASE_ADDR+0xa4)
#define I2S_PCM0_CLK_REG		(CCU_REG_BASE_ADDR+0xb0)
#define I2S_PCM1_CLK_REG		(CCU_REG_BASE_ADDR+0xb4)
#define I2S_PCM2_CLK_REG		(CCU_REG_BASE_ADDR+0xb8)
#define OWA_CLK_REG				(CCU_REG_BASE_ADDR+0xc0)
#define USBPHY_CFG_REG			(CCU_REG_BASE_ADDR+0xcc)
#define DRAM_CFG_REG			(CCU_REG_BASE_ADDR+0xf4)
#define MBUS_RST_REG			(CCU_REG_BASE_ADDR+0xfc)
#define DRAM_CLK_GATING_REG		(CCU_REG_BASE_ADDR+0x0100)
#define TCON0_CLK_REG			(CCU_REG_BASE_ADDR+0x0118)
#define TVE_CLK_REG				(CCU_REG_BASE_ADDR+0x0120)
#define DEINTERLACE_CLK_REG		(CCU_REG_BASE_ADDR+0x0124)
#define CSI_MISC_CLK_REG		(CCU_REG_BASE_ADDR+0x0130)
#define CSI_CLK_REG				(CCU_REG_BASE_ADDR+0x0134)
#define VE_CLK_REG				(CCU_REG_BASE_ADDR+0x013c)
#define AC_DIG_CLK_REG			(CCU_REG_BASE_ADDR+0x0140)
#define AVS_CLK_REG				(CCU_REG_BASE_ADDR+0x0144)
#define HDMI_CLK_REG			(CCU_REG_BASE_ADDR+0x0150)
#define HDMI_SLOW_CLK_REG		(CCU_REG_BASE_ADDR+0x0154)
#define MBUS_CLK_REG			(CCU_REG_BASE_ADDR+0x015C)
#define GPU_CLK_REG				(CCU_REG_BASE_ADDR+0x01A0)

#define PLL_STABLE_TIME_REG0	(CCU_REG_BASE_ADDR+0x0200)
#define PLL_STABLE_TIME_REG1	(CCU_REG_BASE_ADDR+0x0204)

#define PLL_CPUX_BIAS_REG		(CCU_REG_BASE_ADDR+0x0220)
#define PLL_AUDIO_BIAS_REG		(CCU_REG_BASE_ADDR+0x0224)
#define PLL_VIDEO_BIAS_REG		(CCU_REG_BASE_ADDR+0x0228)
#define PLL_VE_BIAS_REG			(CCU_REG_BASE_ADDR+0x022c)
#define PLL_DDR_BIAS_REG		(CCU_REG_BASE_ADDR+0x0230)
#define PLL_PERIPH0_BIAS_REG	(CCU_REG_BASE_ADDR+0x0234)
#define PLL_GPU_BIAS_REG		(CCU_REG_BASE_ADDR+0x023c)
#define PLL_PERIPH1_BIAS_REG	(CCU_REG_BASE_ADDR+0x0244)
#define PLL_DE_BIAS_REG			(CCU_REG_BASE_ADDR+0x0248)

#define PLL_CPUX_TUN_REG		(CCU_REG_BASE_ADDR+0x0250)
#define PLL_DDR_TUN_REG			(CCU_REG_BASE_ADDR+0x0260)

#define PLL_CPUX_PAT_CTRL_REG 	(CCU_REG_BASE_ADDR+0x0280)
#define PLL_AUDIO_PAT_CTRL_REG0	(CCU_REG_BASE_ADDR+0x0284)
#define PLL_VIDEO_PAT_CTRL_REG0	(CCU_REG_BASE_ADDR+0x0288)
#define PLL_VE_PAT_CTRL_REG		(CCU_REG_BASE_ADDR+0x028c)
#define PLL_DDR_PAT_CTRL_REG0 	(CCU_REG_BASE_ADDR+0x0290)// PLL_DDR Pattern Control Register
#define PLL_GPU_PAT_CTRL_REG 	(CCU_REG_BASE_ADDR+0x029C)//  PLL_GPU Pattern Control Register
#define PLL_PERIPH1_PAT_CTRL_REG1 	(CCU_REG_BASE_ADDR+0x02A4)//  PLL_PERIPH1 Pattern Control Register
#define PLL_DE_PAT_CTRL_REG 	(CCU_REG_BASE_ADDR+0x02A8)//  PLL_DE Pattern Control Register

#define BUS_SOFT_RST_REG0 		(CCU_REG_BASE_ADDR+0x02C0)//  Bus Software Reset Register 0
#define BUS_SOFT_RST_REG1 		(CCU_REG_BASE_ADDR+0x02C4)//  Bus Software Reset Register 1
#define BUS_SOFT_RST_REG2 		(CCU_REG_BASE_ADDR+0x02C8)//  Bus Software Reset Register 2
#define BUS_SOFT_RST_REG3 		(CCU_REG_BASE_ADDR+0x02D0)//  Bus Software Reset Register 3
#define BUS_SOFT_RST_REG4 		(CCU_REG_BASE_ADDR+0x02D8)//  Bus Software Reset Register 4

#define CCU_SEC_SWITCH_REG 		(CCU_REG_BASE_ADDR+0x02F0)//  CCU Security Switch Register
#define PS_CTRL_REG 			(CCU_REG_BASE_ADDR+0x0300)//  PS Control Register
#define PS_CNT_REG 				(CCU_REG_BASE_ADDR+0x0304)// 

#endif

