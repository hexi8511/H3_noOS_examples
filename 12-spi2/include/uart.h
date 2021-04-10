#ifndef __UART_H__
#define __UART_H__
#include "basic_reg.h"



#ifndef va_arg

#ifndef _VALIST
#define _VALIST
typedef char *va_list;
#endif /* _VALIST */


/*
 * Storage alignment properties
 */
#define	 NATIVE_INT		 int
#define  _AUPBND         (sizeof (NATIVE_INT) - 1)
#define  _ADNBND         (sizeof (NATIVE_INT) - 1)

/*
 * Variable argument list macro definitions
 */

#define _bnd(X, bnd)    (((sizeof (X)) + (bnd)) & (~(bnd)))
#define va_arg(ap, T)   (*(T *)(((ap) += (_bnd (T, _AUPBND))) - (_bnd (T,_ADNBND))))
#define va_end(ap)      (void) 0
#define va_start(ap, A) (void) ((ap) = (((char *) &(A)) + (_bnd (A,_AUPBND))))

#endif /* va_arg */


#define msg(...)                   uart_printf2(__VA_ARGS__)

#define  HEX_x   'x'
#define  HEX_X   'X'
#define  MASK_LOW4      0xf
#define  NEGATIVE       1
#define  POSITIVE       0

void uart0_clk_init();
void uart0_pin_init();
void uart0_baud_init(unsigned int speed);
void uart0_init();
void send_byte(char c);
int send_bytes(char *buf,int size);
int send_str(char *str);
void putc(unsigned char c);

void uart0_baud_init2(unsigned int speed);
void uart_printf2( const char * str, ...);
void uart0_baud_init3(unsigned int speed);

#endif
