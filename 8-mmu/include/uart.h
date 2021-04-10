#ifndef __UART_H__
#define __UART_H__
#include "basic_reg.h"

void uart0_clk_init();
void uart0_pin_init();
void uart0_baud_init(unsigned int speed);
void uart0_init();
void send_byte(char c);
int send_bytes(char *buf,int size);
int send_str(char *str);

void uart0_baud_init2(unsigned int speed);

#endif
