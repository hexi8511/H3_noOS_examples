#ifndef __RTC_H__
#define __RTC_H__


#define RTC_BASE 0x01f00000

#define LOSC_CTRL_REG 								(RTC_BASE+0x00)
#define LOSC_AUTO_SWT_STA_REG				(RTC_BASE+0x04)
#define INOSC_CLK_PRESCAL_REG 				(RTC_BASE+0x08)
#define RTC_YY_MM_DD_REG 						(RTC_BASE+0x10)
#define RTC_HH_MM_SS_REG 						(RTC_BASE+0x14)
#define ALARM0_COUNTER_REG  	(RTC_BASE+0x20)
#define ALARM0_CUR_VAL_REG 	(RTC_BASE+0x24)
#define ALARM0_ENABLE_REG 		(RTC_BASE+0x28)
#define ALARM0_IRQ_EN_REG 				(RTC_BASE+0x2c)
#define ALARM0_IRQ_STA_REG 		(RTC_BASE+0x30)
#define ALARM1_WK_HH_MM_SS 	(RTC_BASE+0x40)
#define ALARM1_ENABLE_REG 		(RTC_BASE+0x44)
#define ALARM1_IRQ_EN_REG				(RTC_BASE+0x48)
#define ALARM1_IRQ_STA_REG 		(RTC_BASE+0x4c)
#define ALARM_CONFIG_REG 		(RTC_BASE+0x50)
#define LOSC_OUT_GATING_REG 	(RTC_BASE+0x60)

#define GP_DATA_REG_ADDR(n)		(RTC_BASE+0x100+n*4)
#define RTC_DBG_REG					(RTC_BASE+0x170)
#define GPL_HOLD_OUTPUT_REG	(RTC_BASE+0x180)
#define VDD_RTC_REG					(RTC_BASE+0x190)
#define IC_CHARA_REG					(RTC_BASE+0x1f0)


#define LOSC_KEY 0x16aa

enum week_day
{
	MONDAY=0,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY,
	SUNDAY,

};

void rtc_test(void);
void rtc_show(void);

#endif
