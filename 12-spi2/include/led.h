#ifndef __LED_H__
#define __LED_H__

void led_status_init();
void led_red_init();
void led_ss_on();
void led_ss_off();
void led_blue_init();
void led_red_on();
void led_red_off();
void led_blue_on();
void led_blue_off();
void led_ss_flash();
void led_blue_flash();
void led_red_flash();

void p9_high();
void p9_low();
void p8_high();
void p8_low();
void p7_high();
void p7_low();

void led_init();
#endif
