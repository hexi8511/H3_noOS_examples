/*
 * sun8i H3 platform dram controller register and constant defines
 *
 * (C) Copyright 2007-2015 Allwinner Technology Co.
 *                         Jerry Wang <wangflord@allwinnertech.com>
 * (C) Copyright 2015      Vishnu Patekar <vishnupatekar0510@gmail.com>
 * (C) Copyright 2014-2015 Hans de Goede <hdegoede@redhat.com>
 * (C) Copyright 2015      Jens Kuske <jenskuske@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _SUNXI_DRAM_SUN8I_H3_H
#define _SUNXI_DRAM_SUN8I_H3_H

struct sunxi_ccm_reg {
	u32 pll1_cfg;		/* 0x00 pll1 control */
	u32 reserved0;
	u32 pll2_cfg;		/* 0x08 pll2 control */
	u32 reserved1;
	u32 pll3_cfg;		/* 0x10 pll3 control */
	u32 reserved2;
	u32 pll4_cfg;		/* 0x18 pll4 control */
	u32 reserved3;
	u32 pll5_cfg;		/* 0x20 pll5 control */
	u32 reserved4;
	u32 pll6_cfg;		/* 0x28 pll6 control */
	u32 reserved5;
	u32 pll7_cfg;		/* 0x30 pll7 control */
	u32 reserved6;
	u32 pll8_cfg;		/* 0x38 pll8 control */
	u32 reserved7;
	u32 mipi_pll_cfg;	/* 0x40 MIPI pll control */
	u32 pll9_cfg;		/* 0x44 pll9 control */
	u32 pll10_cfg;		/* 0x48 pll10 control */
	u32 pll11_cfg;		/* 0x4c pll11 (ddr1) control (A33 only) */
	u32 cpu_axi_cfg;	/* 0x50 CPU/AXI divide ratio */
	u32 ahb1_apb1_div;	/* 0x54 AHB1/APB1 divide ratio */
	u32 apb2_div;		/* 0x58 APB2 divide ratio */
	u32 axi_gate;		/* 0x5c axi module clock gating */
	u32 ahb_gate0;		/* 0x60 ahb module clock gating 0 */
	u32 ahb_gate1;		/* 0x64 ahb module clock gating 1 */
	u32 apb1_gate;		/* 0x68 apb1 module clock gating */
	u32 apb2_gate;		/* 0x6c apb2 module clock gating */
	u32 bus_gate4;          /* 0x70 gate 4 module clock gating */
	u8 res3[0xc];
	u32 nand0_clk_cfg;	/* 0x80 nand0 clock control */
	u32 nand1_clk_cfg;	/* 0x84 nand1 clock control */
	u32 sd0_clk_cfg;	/* 0x88 sd0 clock control */
	u32 sd1_clk_cfg;	/* 0x8c sd1 clock control */
	u32 sd2_clk_cfg;	/* 0x90 sd2 clock control */
	u32 sd3_clk_cfg;	/* 0x94 sd3 clock control */
	u32 ts_clk_cfg;		/* 0x98 transport stream clock control */
	u32 ss_clk_cfg;		/* 0x9c security system clock control */
	u32 spi0_clk_cfg;	/* 0xa0 spi0 clock control */
	u32 spi1_clk_cfg;	/* 0xa4 spi1 clock control */
	u32 spi2_clk_cfg;	/* 0xa8 spi2 clock control */
	u32 spi3_clk_cfg;	/* 0xac spi3 clock control */
	u32 i2s0_clk_cfg;	/* 0xb0 I2S0 clock control*/
	u32 i2s1_clk_cfg;	/* 0xb4 I2S1 clock control */
	u32 reserved10[2];
	u32 spdif_clk_cfg;	/* 0xc0 SPDIF clock control */
	u32 reserved11[2];
	u32 usb_clk_cfg;	/* 0xcc USB clock control */
	u32 gmac_clk_cfg;	/* 0xd0 GMAC clock control */
	u32 reserved12[7];
	u32 mdfs_clk_cfg;	/* 0xf0 MDFS clock control */
	u32 dram_clk_cfg;	/* 0xf4 DRAM configuration clock control */
	u32 dram_pll_cfg;	/* 0xf8 PLL_DDR cfg register, A33 only */
	u32 mbus_reset;		/* 0xfc MBUS reset control, A33 only */
	u32 dram_clk_gate;	/* 0x100 DRAM module gating */
#ifdef CONFIG_SUNXI_DE2
	u32 de_clk_cfg;		/* 0x104 DE module clock */
#else
	u32 be0_clk_cfg;	/* 0x104 BE0 module clock */
#endif
	u32 be1_clk_cfg;	/* 0x108 BE1 module clock */
	u32 fe0_clk_cfg;	/* 0x10c FE0 module clock */
	u32 fe1_clk_cfg;	/* 0x110 FE1 module clock */
	u32 mp_clk_cfg;		/* 0x114 MP module clock */
#ifdef CONFIG_SUNXI_DE2
	u32 tcon0_clk_cfg;	/* 0x118 TCON0 module clock */
#else
	u32 lcd0_ch0_clk_cfg;	/* 0x118 LCD0 CH0 module clock */
#endif
	u32 lcd1_ch0_clk_cfg;	/* 0x11c LCD1 CH0 module clock */
	u32 reserved14[3];
	u32 lcd0_ch1_clk_cfg;	/* 0x12c LCD0 CH1 module clock */
	u32 lcd1_ch1_clk_cfg;	/* 0x130 LCD1 CH1 module clock */
	u32 csi0_clk_cfg;	/* 0x134 CSI0 module clock */
	u32 csi1_clk_cfg;	/* 0x138 CSI1 module clock */
	u32 ve_clk_cfg;		/* 0x13c VE module clock */
	u32 adda_clk_cfg;	/* 0x140 ADDA module clock */
	u32 avs_clk_cfg;	/* 0x144 AVS module clock */
	u32 dmic_clk_cfg;	/* 0x148 Digital Mic module clock*/
	u32 reserved15;
	u32 hdmi_clk_cfg;	/* 0x150 HDMI module clock */
#ifdef CONFIG_SUNXI_DE2
	u32 hdmi_slow_clk_cfg;	/* 0x154 HDMI slow module clock */
#else
	u32 ps_clk_cfg;		/* 0x154 PS module clock */
#endif
	u32 mtc_clk_cfg;	/* 0x158 MTC module clock */
	u32 mbus0_clk_cfg;	/* 0x15c MBUS0 module clock */
	u32 mbus1_clk_cfg;	/* 0x160 MBUS1 module clock */
	u32 reserved16;
	u32 mipi_dsi_clk_cfg;	/* 0x168 MIPI DSI clock control */
	u32 mipi_csi_clk_cfg;	/* 0x16c MIPI CSI clock control */
	u32 reserved17[4];
	u32 iep_drc0_clk_cfg;	/* 0x180 IEP DRC0 module clock */
	u32 iep_drc1_clk_cfg;	/* 0x184 IEP DRC1 module clock */
	u32 iep_deu0_clk_cfg;	/* 0x188 IEP DEU0 module clock */
	u32 iep_deu1_clk_cfg;	/* 0x18c IEP DEU1 module clock */
	u32 reserved18[4];
	u32 gpu_core_clk_cfg;	/* 0x1a0 GPU core clock config */
	u32 gpu_mem_clk_cfg;	/* 0x1a4 GPU memory clock config */
	u32 gpu_hyd_clk_cfg;	/* 0x1a0 GPU HYD clock config */
	u32 reserved19[21];
	u32 pll_lock;		/* 0x200 PLL Lock Time */
	u32 pll1_lock;		/* 0x204 PLL1 Lock Time */
	u32 reserved20[6];
	u32 pll1_bias_cfg;	/* 0x220 PLL1 Bias config */
	u32 pll2_bias_cfg;	/* 0x224 PLL2 Bias config */
	u32 pll3_bias_cfg;	/* 0x228 PLL3 Bias config */
	u32 pll4_bias_cfg;	/* 0x22c PLL4 Bias config */
	u32 pll5_bias_cfg;	/* 0x230 PLL5 Bias config */
	u32 pll6_bias_cfg;	/* 0x234 PLL6 Bias config */
	u32 pll7_bias_cfg;	/* 0x238 PLL7 Bias config */
	u32 pll8_bias_cfg;	/* 0x23c PLL8 Bias config */
	u32 mipi_bias_cfg;	/* 0x240 MIPI Bias config */
	u32 pll9_bias_cfg;	/* 0x244 PLL9 Bias config */
	u32 pll10_bias_cfg;	/* 0x248 PLL10 Bias config */
	u32 reserved21[5];
	u32 pll5_tuning_cfg;	/* 0x260 PLL5 Tuning config */
	u32 reserved21_5[7];
	u32 pll1_pattern_cfg;	/* 0x280 PLL1 Pattern config */
	u32 pll2_pattern_cfg;	/* 0x284 PLL2 Pattern config */
	u32 pll3_pattern_cfg;	/* 0x288 PLL3 Pattern config */
	u32 pll4_pattern_cfg;	/* 0x28c PLL4 Pattern config */
	u32 pll5_pattern_cfg;	/* 0x290 PLL5 Pattern config */
	u32 pll6_pattern_cfg;	/* 0x294 PLL6 Pattern config */
	u32 pll7_pattern_cfg;	/* 0x298 PLL7 Pattern config */
	u32 pll8_pattern_cfg;	/* 0x29c PLL8 Pattern config */
	u32 mipi_pattern_cfg;	/* 0x2a0 MIPI Pattern config */
	u32 pll9_pattern_cfg;	/* 0x2a4 PLL9 Pattern config */
	u32 pll10_pattern_cfg;	/* 0x2a8 PLL10 Pattern config */
	u32 pll11_pattern_cfg0; /* 0x2ac PLL11 Pattern config0, A33 only */
	u32 pll11_pattern_cfg1; /* 0x2b0 PLL11 Pattern config0, A33 only */
	u32 reserved22[3];
	u32 ahb_reset0_cfg;	/* 0x2c0 AHB1 Reset 0 config */
	u32 ahb_reset1_cfg;	/* 0x2c4 AHB1 Reset 1 config */
	u32 ahb_reset2_cfg;	/* 0x2c8 AHB1 Reset 2 config */
	u32 reserved23;
	u32 apb1_reset_cfg;	/* 0x2d0 APB1 Reset config */
	u32 reserved24;
	u32 apb2_reset_cfg;	/* 0x2d8 APB2 Reset config */
	u32 reserved25[5];
	u32 ccu_sec_switch;	/* 0x2f0 CCU Security Switch, H3 only */
};

struct sunxi_mctl_com_reg {
	u32 cr;			/* 0x00 control register */
	u8 res0[0xc];		/* 0x04 */
	u32 mcr[16][2];		/* 0x10 */
	u32 bwcr;		/* 0x90 bandwidth control register */
	u32 maer;		/* 0x94 master enable register */
	u32 mapr;		/* 0x98 master priority register */
	u32 mcgcr;		/* 0x9c */
	u32 cpu_bwcr;		/* 0xa0 */
	u32 gpu_bwcr;		/* 0xa4 */
	u32 ve_bwcr;		/* 0xa8 */
	u32 disp_bwcr;		/* 0xac */
	u32 other_bwcr;		/* 0xb0 */
	u32 total_bwcr;		/* 0xb4 */
	u8 res1[0x8];		/* 0xb8 */
	u32 swonr;		/* 0xc0 */
	u32 swoffr;		/* 0xc4 */
	u8 res2[0x8];		/* 0xc8 */
	u32 cccr;		/* 0xd0 */
	u8 res3[0x72c];		/* 0xd4 */
	u32 protect;		/* 0x800 */
};
#define SUNXI_DRAM_COM_BASE		0x01c62000
#define SUNXI_DRAM_CTL0_BASE		0x01c63000
#define SUNXI_DRAM_CTL1_BASE		0x01c64000
#define SUNXI_DRAM_PHY0_BASE		0x01c65000
#define SUNXI_DRAM_PHY1_BASE		0x01c66000


#define MCTL_CR_BL8		(0x4 << 20)

#define MCTL_CR_1T		(0x1 << 19)
#define MCTL_CR_2T		(0x0 << 19)

#define MCTL_CR_LPDDR3		(0x7 << 16)
#define MCTL_CR_LPDDR2		(0x6 << 16)
#define MCTL_CR_DDR3		(0x3 << 16)
#define MCTL_CR_DDR2		(0x2 << 16)

#define MCTL_CR_SEQUENTIAL	(0x1 << 15)
#define MCTL_CR_INTERLEAVED	(0x0 << 15)

#define MCTL_CR_32BIT		(0x1 << 12)
#define MCTL_CR_16BIT		(0x0 << 12)
#define MCTL_CR_BUS_WIDTH(x)	((x) == 32 ? MCTL_CR_32BIT : MCTL_CR_16BIT)

#define MCTL_CR_PAGE_SIZE(x)	((fls(x) - 4) << 8)
#define MCTL_CR_ROW_BITS(x)	(((x) - 1) << 4)
#define MCTL_CR_EIGHT_BANKS	(0x1 << 2)
#define MCTL_CR_FOUR_BANKS	(0x0 << 2)
#define MCTL_CR_DUAL_RANK	(0x1 << 0)
#define MCTL_CR_SINGLE_RANK	(0x0 << 0)

#define PROTECT_MAGIC		(0x94be6fa3)

struct sunxi_mctl_ctl_reg {
	u32 pir;		/* 0x00 PHY initialization register */
	u32 pwrctl;		/* 0x04 */
	u32 mrctrl;		/* 0x08 */
	u32 clken;		/* 0x0c */
	u32 pgsr[2];		/* 0x10 PHY general status registers */
	u32 statr;		/* 0x18 */
	u8 res1[0x14];		/* 0x1c */
	u32 mr[4];		/* 0x30 mode registers */
	u32 pllgcr;		/* 0x40 */
	u32 ptr[5];		/* 0x44 PHY timing registers */
	u32 dramtmg[9];		/* 0x58 DRAM timing registers */
	u32 odtcfg;		/* 0x7c */
	u32 pitmg[2];		/* 0x80 PHY interface timing registers */
	u8 res2[0x4];		/* 0x88 */
	u32 rfshctl0;		/* 0x8c */
	u32 rfshtmg;		/* 0x90 refresh timing */
	u32 rfshctl1;		/* 0x94 */
	u32 pwrtmg;		/* 0x98 */
	u8  res3[0x20];		/* 0x9c */
	u32 dqsgmr;		/* 0xbc */
	u32 dtcr;		/* 0xc0 */
	u32 dtar[4];		/* 0xc4 */
	u32 dtdr[2];		/* 0xd4 */
	u32 dtmr[2];		/* 0xdc */
	u32 dtbmr;		/* 0xe4 */
	u32 catr[2];		/* 0xe8 */
	u32 dtedr[2];		/* 0xf0 */
	u8 res4[0x8];		/* 0xf8 */
	u32 pgcr[4];		/* 0x100 PHY general configuration registers */
	u32 iovcr[2];		/* 0x110 */
	u32 dqsdr;		/* 0x118 */
	u32 dxccr;		/* 0x11c */
	u32 odtmap;		/* 0x120 */
	u32 zqctl[2];		/* 0x124 */
	u8 res6[0x14];		/* 0x12c */
	u32 zqcr;		/* 0x140 ZQ control register */
	u32 zqsr;		/* 0x144 ZQ status register */
	u32 zqdr[3];		/* 0x148 ZQ data registers */
	u8 res7[0x6c];		/* 0x154 */
	u32 sched;		/* 0x1c0 */
	u32 perfhpr[2];		/* 0x1c4 */
	u32 perflpr[2];		/* 0x1cc */
	u32 perfwr[2];		/* 0x1d4 */
	u8 res8[0x2c];		/* 0x1dc */
	u32 aciocr;		/* 0x208 */
	u8 res9[0xf4];		/* 0x20c */
	struct {		/* 0x300 DATX8 modules*/
		u32 mdlr;		/* 0x00 */
		u32 lcdlr[3];		/* 0x04 */
		u32 iocr[11];		/* 0x10 IO configuration register */
		u32 bdlr6;		/* 0x3c */
		u32 gtr;		/* 0x40 */
		u32 gcr;		/* 0x44 */
		u32 gsr[3];		/* 0x48 */
		u8 res0[0x2c];		/* 0x54 */
	} datx[4];
	u8 res10[0x388];	/* 0x500 */
	u32 upd2;		/* 0x888 */
};

#define PTR3_TDINIT1(x)		((x) << 20)
#define PTR3_TDINIT0(x)		((x) <<  0)

#define PTR4_TDINIT3(x)		((x) << 20)
#define PTR4_TDINIT2(x)		((x) <<  0)

#define DRAMTMG0_TWTP(x)	((x) << 24)
#define DRAMTMG0_TFAW(x)	((x) << 16)
#define DRAMTMG0_TRAS_MAX(x)	((x) <<  8)
#define DRAMTMG0_TRAS(x)	((x) <<  0)

#define DRAMTMG1_TXP(x)		((x) << 16)
#define DRAMTMG1_TRTP(x)	((x) <<  8)
#define DRAMTMG1_TRC(x)		((x) <<  0)

#define DRAMTMG2_TCWL(x)	((x) << 24)
#define DRAMTMG2_TCL(x)		((x) << 16)
#define DRAMTMG2_TRD2WR(x)	((x) <<  8)
#define DRAMTMG2_TWR2RD(x)	((x) <<  0)

#define DRAMTMG3_TMRW(x)	((x) << 16)
#define DRAMTMG3_TMRD(x)	((x) << 12)
#define DRAMTMG3_TMOD(x)	((x) <<  0)

#define DRAMTMG4_TRCD(x)	((x) << 24)
#define DRAMTMG4_TCCD(x)	((x) << 16)
#define DRAMTMG4_TRRD(x)	((x) <<  8)
#define DRAMTMG4_TRP(x)		((x) <<  0)

#define DRAMTMG5_TCKSRX(x)	((x) << 24)
#define DRAMTMG5_TCKSRE(x)	((x) << 16)
#define DRAMTMG5_TCKESR(x)	((x) <<  8)
#define DRAMTMG5_TCKE(x)	((x) <<  0)

#define RFSHTMG_TREFI(x)	((x) << 16)
#define RFSHTMG_TRFC(x)		((x) <<  0)

#define PIR_CLRSR	(0x1 << 27)	/* clear status registers */
#define PIR_QSGATE	(0x1 << 10)	/* Read DQS gate training */
#define PIR_DRAMINIT	(0x1 << 8)	/* DRAM initialization */
#define PIR_DRAMRST	(0x1 << 7)	/* DRAM reset */
#define PIR_PHYRST	(0x1 << 6)	/* PHY reset */
#define PIR_DCAL	(0x1 << 5)	/* DDL calibration */
#define PIR_PLLINIT	(0x1 << 4)	/* PLL initialization */
#define PIR_ZCAL	(0x1 << 1)	/* ZQ calibration */
#define PIR_INIT	(0x1 << 0)	/* PHY initialization trigger */

#define PGSR_INIT_DONE	(0x1 << 0)	/* PHY init done */

#define ZQCR_PWRDOWN	(0x1 << 31)	/* ZQ power down */

#define DATX_IOCR_DQ(x)	(x)		/* DQ0-7 IOCR index */
#define DATX_IOCR_DM	(8)		/* DM IOCR index */
#define DATX_IOCR_DQS	(9)		/* DQS IOCR index */
#define DATX_IOCR_DQSN	(10)		/* DQSN IOCR index */

#define DATX_IOCR_WRITE_DELAY(x)	((x) << 8)
#define DATX_IOCR_READ_DELAY(x)		((x) << 0)

#endif /* _SUNXI_DRAM_SUN8I_H3_H */
