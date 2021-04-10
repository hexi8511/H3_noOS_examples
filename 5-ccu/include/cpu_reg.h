#ifndef  __CPU_REG_H__
#define  __CPU_REG_H__

#define  CPUCFG 0x01F01C00

#define  CPUS_RST_CTRL_REG 	(CPUCFG+0x0000)//CPUS reset control register
#define  CPU0_RST_CTRL 		(CPUCFG+0x0040)// CPU0 reset control
#define  CPU0_CTRL_REG 		(CPUCFG+0x0044)// CPU0 control register
#define  CPU0_STATUS_REG 	(CPUCFG+0x0048)// CPU0 status register
#define  CPU1_RST_CTRL 		(CPUCFG+0x0080)// CPU1 reset control
#define  CPU1_CTRL_REG 		(CPUCFG+0x0084)// CPU1 control register
#define  CPU1_STATUS_REG 	(CPUCFG+0x0088)// CPU1 status register
#define  CPU2_RST_CTRL 		(CPUCFG+0x00C0)// CPU2 reset control
#define  CPU2_CTRL_REG 		(CPUCFG+0x00C4)// CPU2 control register
#define  CPU2_STATUS_REG 	(CPUCFG+0x00C8)// CPU2 status register
#define  CPU3_RST_CTRL 		(CPUCFG+0x0100)// CPU3 reset control
#define  CPU3_CTRL_REG 		(CPUCFG+0x0104)// CPU3 control register
#define  CPU3_STATUS_REG 	(CPUCFG+0x0108)// CPU3 status register
#define  CPU_SYS_RST_REG 	(CPUCFG+0x0140)// CPU System Reset Register
#define  CPU_CLK_GATING_REG 	(CPUCFG+0x0144)// CPU clock gating Register
#define  GENER_CTRL_REG 	(CPUCFG+0x0184)// General Control Register
#define  SUP_STAN_FLAG_REG 	(CPUCFG+0x01A0)// Super Standby Flag Register
#define  CNT64_CTRL_REG 	(CPUCFG+0x0280)// 64-bit Counter Control Register
#define  CNT64_LOW_REG 		(CPUCFG+0x0284)// 64-bit Counter Low Register
#define  CNT64_HIGH_REG 	(CPUCFG+0x0288)// 64-bit Counter High Register

#endif
