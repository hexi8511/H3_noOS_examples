#ifndef __TIMER_H__
#define __TIMER_H__

#define TIMER_BASE 0x01c20c00


#define TMR_IRQ_EN_REG 			(TIMER_BASE+0x00)
#define TMR_IRQ_STA_REG 		(TIMER_BASE+0x04)
#define TMR0_CTRL_REG 			(TIMER_BASE+0x10)
#define TMR0_INTV_VALUE_REG 	(TIMER_BASE+0x14)
#define TMR0_CUR_VALUE_REG 		(TIMER_BASE+0x18)
#define TMR1_CTRL_REG 			(TIMER_BASE+0x20)
#define TMR1_INTV_VALUE_REG 	(TIMER_BASE+0x24)
#define TMR1_CUR_VALUE_REG 		(TIMER_BASE+0x28)
#define AVS_CNT_CTL_REG 		(TIMER_BASE+0x80)
#define AVS_CNT0_REG 			(TIMER_BASE+0x84)
#define AVS_CNT1_REG 			(TIMER_BASE+0x88)
#define AVS_CNT_DIV_REG 		(TIMER_BASE+0x8c)
#define WDOG0_IRQ_EN_REG 		(TIMER_BASE+0xa0)
#define WDOG0_IRQ_STA_REG 		(TIMER_BASE+0xa4)
#define WDOG0_CTRL_REG 			(TIMER_BASE+0xb0)
#define WDOG0_CFG_REG 			(TIMER_BASE+0xb4)
#define WDOG0_MODE_REG 			(TIMER_BASE+0xb8)

#endif
