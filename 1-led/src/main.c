
#include "basic_reg.h"
#include "gpio_reg.h"
#include "delay.h"
#include "led.h"
#if 0
void led_blue_init(void);
void led_blue_on(void);
void led_blue_off(void);
void led_blue_flash(void);
#endif

void config_pll1_para()
{

}

int main(void)
{
      led_init();

	while(1)
		{
		led_ss_flash();led_blue_flash();
		}

	return 0;
}

#if 0
/*unsigned int tmp_reg=0;*/
#define __set_bit(bit,bitw,val) do{reg=reg&(~(bitw<<bit));reg=reg|(val<<bit);}while(0)

void led_blue_init(void)
{
//gpio_A10 status
	volatile unsigned int reg;

	reg=__read_reg(PA_CFG1);
	__set_bit(8,BW3,IO_OUTPUT);
	__write_reg(PA_CFG1,reg);

	reg=__read_reg(PA_DRV0);
	__set_bit(20,BW2,DRV_LEV3);
	__write_reg(PA_DRV0,reg);


	reg=__read_reg(PA_PUL0);
	__set_bit(20,BW2,PUL_UP);
	__write_reg(PA_PUL0,reg);

}

void led_blue_on(void)
{
	volatile unsigned int reg=__read_reg(PA_DAT);
	__set_bit(10,BW1,1);
	__write_reg(PA_DAT,reg);
}

void led_blue_off(void)
{
	volatile unsigned int reg=__read_reg(PA_DAT);
	__set_bit(10,BW1,0);
	__write_reg(PA_DAT,reg);
}

void led_blue_flash()
{
	led_blue_on();
	delay();
	led_blue_off();
	delay();
}
#endif
