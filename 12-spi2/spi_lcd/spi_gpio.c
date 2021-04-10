#include "gpio_reg.h"
#include "basic_reg.h"
#include "ccu_reg.h"
#include "uart.h"
#include "ccu.h"
#include "delay.h"
#include "timer.h"

void lcd_ctrl_gpio_init()
{
volatile unsigned int tmp_reg;
//A9

	tmp_reg=__read_reg(PA_CFG1);
	__set_bit(4,BW3,IO_OUTPUT);
	__write_reg(PA_CFG1,tmp_reg);

	tmp_reg=__read_reg(PA_DRV0);
	__set_bit(18,BW2,DRV_LEV3);
	__write_reg(PA_DRV0,tmp_reg);


	tmp_reg=__read_reg(PA_PUL0);
	__set_bit(18,BW2,PUL_UP);
	__write_reg(PA_PUL0,tmp_reg);


//A8

	tmp_reg=__read_reg(PA_CFG1);
	__set_bit(0,BW3,IO_OUTPUT);
	__write_reg(PA_CFG1,tmp_reg);

	tmp_reg=__read_reg(PA_DRV0);
	__set_bit(16,BW2,DRV_LEV3);
	__write_reg(PA_DRV0,tmp_reg);


	tmp_reg=__read_reg(PA_PUL0);
	__set_bit(16,BW2,PUL_UP);
	__write_reg(PA_PUL0,tmp_reg);



//A7

	tmp_reg=__read_reg(PA_CFG0);
	__set_bit(29,BW3,IO_OUTPUT);
	__write_reg(PA_CFG0,tmp_reg);

	tmp_reg=__read_reg(PA_DRV0);
	__set_bit(14,BW2,DRV_LEV3);
	__write_reg(PA_DRV0,tmp_reg);


	tmp_reg=__read_reg(PA_PUL0);
	__set_bit(14,BW2,PUL_UP);
	__write_reg(PA_PUL0,tmp_reg);

//A9 RST

	tmp_reg=__read_reg(PA_CFG1);
	__set_bit(4,BW3,IO_OUTPUT);
	__write_reg(PA_CFG1,tmp_reg);

	tmp_reg=__read_reg(PA_DRV0);
	__set_bit(18,BW2,DRV_LEV3);
	__write_reg(PA_DRV0,tmp_reg);


	tmp_reg=__read_reg(PA_PUL0);
	__set_bit(18,BW2,PUL_UP);
	__write_reg(PA_PUL0,tmp_reg);


//A8 CD/RS

	tmp_reg=__read_reg(PA_CFG1);
	__set_bit(0,BW3,IO_OUTPUT);
	__write_reg(PA_CFG1,tmp_reg);

	tmp_reg=__read_reg(PA_DRV0);
	__set_bit(16,BW2,DRV_LEV3);
	__write_reg(PA_DRV0,tmp_reg);


	tmp_reg=__read_reg(PA_PUL0);
	__set_bit(16,BW2,PUL_UP);
	__write_reg(PA_PUL0,tmp_reg);

//a7 block
	tmp_reg=__read_reg(PA_CFG0);
	__set_bit(29,BW3,IO_OUTPUT);
	__write_reg(PA_CFG0,tmp_reg);

	tmp_reg=__read_reg(PA_DRV0);
	__set_bit(14,BW2,DRV_LEV3);
	__write_reg(PA_DRV0,tmp_reg);


	tmp_reg=__read_reg(PA_PUL0);
	__set_bit(14,BW2,PUL_UP);
	__write_reg(PA_PUL0,tmp_reg);

//cs

}
void p9_high()
{
	volatile unsigned int tmp_reg=__read_reg(PA_DAT);

	__set_bit(9,BW1,1);
	__write_reg(PA_DAT,tmp_reg);

}

void p9_low()
{
	volatile unsigned int  tmp_reg=__read_reg(PA_DAT);
	__set_bit(9,BW1,0);
	__write_reg(PA_DAT,tmp_reg);

}

void p8_high()
{
	volatile unsigned int tmp_reg=__read_reg(PA_DAT);

	__set_bit(8,BW1,1);
	__write_reg(PA_DAT,tmp_reg);

}

void p8_low()
{
	volatile unsigned int  tmp_reg=__read_reg(PA_DAT);
	__set_bit(8,BW1,0);
	__write_reg(PA_DAT,tmp_reg);

}

void p7_high()
{
	volatile unsigned int tmp_reg=__read_reg(PA_DAT);

	__set_bit(7,BW1,1);
	__write_reg(PA_DAT,tmp_reg);

}

void p7_low()
{
	volatile unsigned int  tmp_reg=__read_reg(PA_DAT);
	__set_bit(7,BW1,0);
	__write_reg(PA_DAT,tmp_reg);

}

