#ifndef __UART_H__
#define __UART_H__
#include "basic_reg.h"

extern void uart0_clk_init();
extern void uart0_pin_init();
extern void uart0_baud_init(unsigned int speed);
extern void uart0_init();
extern void send_byte(char c);
extern int send_bytes(char *buf,int size);
extern int send_str(char *str);

extern void uart0_baud_init2(unsigned int speed);

#endif
