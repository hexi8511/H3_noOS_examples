#include "gpio_reg.h"
#include "basic_reg.h"
#include "uart.h"


void config_pll1_para(void)
{

}


int main()
{

led_init();
uart0_init();

for (int i=0;i<20;i++)
{
delay();
}
send_str("UART TEST APP FOR EZPI-A1 !\r\n");



while(1)
	{
	led_blue_flash();
	led_ss_flash();
	led_red_flash();
	send_str("hello esdman !\r\n");
	send_str("hello huangtianbang !\r\n");
	}

return 0;
}
