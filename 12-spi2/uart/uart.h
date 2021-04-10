
#ifndef __UART_H__
#define __UART_H__

#include "./include/types.h"
#include "./include/arch/uart.h"
#define   UART_OFFSET   0x400
#define   TxFIFOSize    1024
//UART Baudrate Control
#define   UART_BAUD    115200      // Baud rate for UART
                                   // Compute the divisor factor
// UART Line Control Parameter
#define   PARITY       0           // Parity: 0,2 - no parity; 1 - odd parity; 3 - even parity
#define   STOP         0           // Number of Stop Bit: 0 - 1bit; 1 - 2(or 1.5)bits
#define   DLEN         3           // Data Length: 0 - 5bits; 1 - 6bits; 2 - 7bits; 3 - 8bits

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


void uart_init(void);
char uart_getchar(void);
void uart_putchar(char c);
void puts(char *str);


#define SERIAL_CHAR_READY()     ( UART_REG_LSR(port) & ( 1 << 0 ) )
#define SERIAL_READ_CHAR()      UART_REG_RBR(port)
#define SERIAL_READ_STATUS()    ( UART_REG_LSR(port) & 0xFF )
#define SERIAL_WRITE_STATUS()	UART_REG_LSR(port)
#define SERIAL_WRITE_READY()	( UART_REG_LSR(port) & ( 1 << 6 ) )
#define SERIAL_WRITE_CHAR(c)	( ( UART_REG_THR(port) ) = ( c ) )


#define msg(...)                   uart_printf2(__VA_ARGS__)

#define  HEX_x   'x'
#define  HEX_X   'X'
#define  MASK_LOW4      0xf
#define  NEGATIVE       1
#define  POSITIVE       0





//This function is to convert an 'int' data 'input' to a string in decimalism,
//and the string converted is in 'str'.
void int_to_string_dec( int input, char * str );


//This function is to convert an 'int' data 'input' to a string in hex,
//and the string converted is in 'str'.
void int_to_string_hex( int input, char * str, int hex_flag );


// This function is to convert an 'unsigned int' data 'input' to a string in decimalism, and
// the  string converted is in 'str'.
void Uint_to_string_dec( unsigned int input, char * str );



void   uart_puts( const char * string );
//char   UART_getchar_with_display( __u32 timeout );
void   uart_printf2( const char * str, ... );
void   eFG_printf( const char * str, ... );






#endif
