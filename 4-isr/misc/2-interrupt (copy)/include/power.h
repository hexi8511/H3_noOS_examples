#ifndef __POWER_H__
#define __POWER_H__

int power_off(void);
void pwr_ldo_en_init();
void sys_3v3_off();
void sys_3v3_on();
void cpux_eth_off();
void cpux_eth_on();

#endif
