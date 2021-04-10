#ifndef __THS_H__
#define __THS_H__

#define THS_BASE 		0x01c25000
#define THS_CTRL0 		(THS_BASE+0x00)
#define THS_CTRL1 		(THS_BASE+0x04)
#define THS_ADC_CDAT 	(THS_BASE+0x14)
#define THS_CTRL2 		(THS_BASE+0x40)
#define THS_INT_CTRL 	(THS_BASE+0x44)
#define THS_STAT 		(THS_BASE+0x48)
#define THS_ALARM_CTRL 	(THS_BASE+0x50)
#define THS_SHUTDOWN_CTRL (THS_BASE+0x60)

#define THS_FILTER 		(THS_BASE+0x70)
#define THS_CDATA 		(THS_BASE+0x74)
#define THS_DATA 		(THS_BASE+0x80)

#endif
