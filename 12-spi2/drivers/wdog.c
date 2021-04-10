#include "ccu_reg.h"
#include "timer.h"
#include "stdio.h"
#include "string.h"
#include "intc.h"
#include <linux/types.h>
#include <linux/io.h>
/**
*watch dog0
*/
void watchdog0_int_en()
{
	writel(0x1,WDOG0_IRQ_EN_REG);
}

void watchdog0_clear_state_bit()
{

}

void watchdog0_init()
{
//enable inerrupt
writel(0x1,WDOG0_IRQ_EN_REG);	
writel(0x1,WDOG0_IRQ_STA_REG);

//reset all system
writel(0x01,WDOG0_CFG_REG);

//5s reset
writel(0x05<<4,WDOG0_MODE_REG);

//enable wdg
writel((0x05<<4)|1,WDOG0_MODE_REG);//the system will reset after 5 second

}

void watchdog0_feed()
{
writel(readl(WDOG0_CTRL_REG)|(0xa57<<1)|(1<<0),WDOG0_CTRL_REG);
}
