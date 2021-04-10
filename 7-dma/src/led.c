#include "gpio_reg.h"
#include "basic_reg.h"
#include "ccu_reg.h"
#include "uart.h"
#include "ccu.h"
#include "delay.h"


void led_red_init()
{
	//gpio_L10 pwr
	volatile unsigned int tmp_reg;

	tmp_reg=__read_reg(PL_CFG1);
	__set_bit(8,BW3,IO_OUTPUT);
	__write_reg(PL_CFG1,tmp_reg);

	tmp_reg=__read_reg(PL_DRV0);

	__set_bit(20,BW2,DRV_LEV1);
	__write_reg(PL_DRV0,tmp_reg);


	tmp_reg=__read_reg(PL_PUL0);

	__set_bit(20,BW2,PUL_UP);
	__write_reg(PL_PUL0,tmp_reg);

}


void led_status_init()
{

	//gpio_A15 status
	volatile unsigned int tmp_reg;

	tmp_reg=__read_reg(PA_CFG1);
	__set_bit(28,BW3,IO_OUTPUT);
	__write_reg(PA_CFG1,tmp_reg);

	tmp_reg=__read_reg(PA_DRV0);
	__set_bit(30,BW2,DRV_LEV1);
	__write_reg(PA_DRV0,tmp_reg);


	tmp_reg=__read_reg(PA_PUL0);

	__set_bit(30,BW2,PUL_UP);
	__write_reg(PA_PUL0,tmp_reg);


}



void led_ss_on()
{
	volatile unsigned int tmp_reg=__read_reg(PA_DAT);
	__set_bit(15,BW1,0);
	__write_reg(PA_DAT,tmp_reg);
}

void led_ss_off()
{
	volatile unsigned int  tmp_reg=__read_reg(PA_DAT);
	__set_bit(15,BW1,1);
	__write_reg(PA_DAT,tmp_reg);

}

void led_blue_init()
{

	//gpio_A10 status
	volatile unsigned int tmp_reg;


	tmp_reg=__read_reg(PA_CFG1);
	__set_bit(8,BW3,IO_OUTPUT);
	__write_reg(PA_CFG1,tmp_reg);

	tmp_reg=__read_reg(PA_DRV0);
	__set_bit(20,BW2,DRV_LEV3);
	__write_reg(PA_DRV0,tmp_reg);


	tmp_reg=__read_reg(PA_PUL0);
	__set_bit(20,BW2,PUL_UP);
	__write_reg(PA_PUL0,tmp_reg);


}



void led_red_on()
{
	volatile unsigned int tmp_reg=__read_reg(PL_DAT);

	__set_bit(10,BW1,0);
	__write_reg(PL_DAT,tmp_reg);

}

void led_red_off()
{
	volatile unsigned int  tmp_reg=__read_reg(PL_DAT);
	__set_bit(10,BW1,1);
	__write_reg(PL_DAT,tmp_reg);

}


void led_blue_on()
{
	volatile unsigned int tmp_reg=__read_reg(PA_DAT);
	__set_bit(10,BW1,1);
	__write_reg(PA_DAT,tmp_reg);


}

void led_blue_off()
{
	volatile unsigned int tmp_reg=__read_reg(PA_DAT);
	__set_bit(10,BW1,0);
	__write_reg(PA_DAT,tmp_reg);
}

void led_red_flash()
{
	led_red_on();
	delay();
	led_red_off();
	delay();
}

void led_blue_flash()
{

	led_blue_on();
	delay();
	led_blue_off();
	delay();

}

void led_ss_flash()
{
	led_ss_on();
	delay();
	led_ss_off();
	delay();
}


void led_init()
{
	led_red_init();
	led_blue_init();
	led_status_init();
}
