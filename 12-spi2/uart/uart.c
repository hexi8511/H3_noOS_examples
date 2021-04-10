#include "uart.h"
#include "./include/interinc/egon_def.h"

//#include "string.h"
//#include "stdio.h"


#define UART0_LCR   (*(volatile unsigned int *)0x01C2800C) 
#define UART0_DLH   (*(volatile unsigned int *)0x01C28004) 
#define UART0_DLL   (*(volatile unsigned int *)0x01C28000) 
#define UART0_LSR   (*(volatile unsigned int *)0x01C28014) 
#define UART0_THR   (*(volatile unsigned int *)0x01C28000) 
#define UART0_RBR   (*(volatile unsigned int *)0x01C28000) 

void uart_init(void)
{
  /*select dll dlh*/
  /*bit7:Divisor Latch Access Bit
   *0:Select RX Buffer Register(RBR)/TX Holding Register(THR)and Interrupt Enable register(IER)
   *1:Select Divisor Latch LS Register(DLL) and Divisor Latch MS Register(DLM)
   */
  UART0_LCR = 0x80;
  /*set baudrate*/
  UART0_DLH = 0x0;
  /*bit[7:0]:Divisor Latch Low
   *baud rate=(serial clock freq)/(16*divisor)
   *divisor=24M/16/115200=13.02083
   */
  UART0_DLL = 0xd; 
  /*set line control*/
  /*bit3:Parity Enable 0:parity disable 1:parity enable
   *bit2:Number of stop bits 
   *0:1 stop bit;1:1.5 stop bits when DLS(LCR[1:0]is zero,else 2 stop bits)
   *bit[1:0] Data Length Select 00:5bits;01:6bits;10:7bits;11:8bits
   */
  UART0_LCR = ((0<<3)|(0<<2)|0x3);
}

char uart_getchar(void)
{
char c;
/* 查询状态寄存器，直到有有效数据 */
while (!(UART0_LSR & (1<<0)));
c = UART0_RBR; /* 读取接收寄存器的值 */
return c;
}

void uart_putchar(char c)
{
/* 查询状态寄存器，直到发送缓存为空 */
while (!(UART0_LSR & (1<<6)));
UART0_THR = c; /* 写入发送寄存器 */
return;
}
 
char getc(void)
{
return uart_getchar();
}
void putc(char c)
{
	uart_putchar( c);
}

void puts(char *str)
{
	while(*str!='\0')
	{
		uart_putchar(* str);
	}

}



//char UART_getchar( __u32 timeout )
//{
//	__u32  i;
//
//	if( timeout == 0 )           // infinite wait
//	{
//		while( !SERIAL_CHAR_READY() )
//			;
//		return SERIAL_READ_CHAR();
//	}
//	else
//	{
//		for( i = 0; i < timeout; i++ )
//		{
//			if(SERIAL_CHAR_READY())
//				break;
//		}
//		if( i == timeout )
//			return 0;
//		else
//			return SERIAL_READ_CHAR();
//	}
//}





//char UART_getchar_with_display( __u32 timeout )
//{
//	char c;
//
//	c = UART_getchar( timeout );
//
//	if( c != 0)
//		uart_putchar( c );
//
//	return c;
//}



void uart_puts( const char * str )
{
	while( *str != '\0' )
	{
		uart_putchar( *str++ );
	}

	uart_putchar('\r');
	uart_putchar('\n');
}

#if 0
int putchar(char c)
{
uart_putchar(c);
return (int)c;
}
#endif


void uart_puts_no_newline( const char * str )
{
	while( *str != '\0' )
	{
		if( *str == '\n' )                      // if current character is '\n', insert and output '\r'
			uart_putchar( '\r' );

		uart_putchar( *str++ );
	}
}
/*
******************************************************************************************************************
*
*Function Name : UART_printf
*
*Description : This function is to formatedly output through UART, similar to ANSI C function printf().
*                This function can support and only support the following Conversion Specifiers:
*              %d		Signed decimal integer.
*              %u		Unsigned decimal integer.
*              %x		Unsigned hexadecimal integer, using hex digits 0f.
*              %X		Unsigned hexadecimal integer, using hex digits 0F.
*              %c		Single character.
*              %s		Character string.
*              %p		A pointer.
*
*Input : refer to ANSI C function printf().
*
*Output : void, different form ANSI C function printf().
*
*call for : void int_to_string_dec( __s32 input, char * str ), defined in format_transformed.c.
*           void int_to_string_hex( __s32 input, char * str );  defined in format_transformed.c.
*           void Uint_to_string_dec( __u32 input, char * str );  defined in format_transformed.c.
*           void uart_putchar( __s32 ch); defined in boot loader.
*           void UART_puts( const char * string ); defined in boot loader.
*
*Others : None at present.
*
*******************************************************************************************************************
*/
void uart_printf2( const char * str, ...)
{
#if 1
	char string[13];
	char *p;
	__s32 hex_flag ;
	va_list argp;

	va_start( argp, str );

	while( *str )
	{
		if( *str == '%' )
		{
			++str;
			p = string;
			hex_flag = HEX_X;
			switch( *str )
			{
				case 'd': int_to_string_dec( va_arg( argp,  __s32 ), string );
                          uart_puts_no_newline( p );
						  ++str;
						  break;
				case 'x': hex_flag = HEX_x;	         // jump to " case 'X' "
				case 'p':
				case 'X': int_to_string_hex( va_arg( argp,  __s32 ), string, hex_flag );
						  uart_puts_no_newline( p );
                          ++str;
						  break;
				case 'u': Uint_to_string_dec( va_arg( argp,  __s32 ), string );
						  uart_puts_no_newline( p );
						  ++str;
						  break;
				case 'c':uart_putchar( va_arg( argp,  __s32 ) );
						  ++str;
						  break;
				case 's': uart_puts_no_newline( va_arg( argp, char * ) );
						  ++str;
						  break;
				default : uart_putchar( '%' );       // if current character is not Conversion Specifiers 'dxpXucs',
						  uart_putchar( *str );         // output directly '%' and current character, and then
						  ++str;                        // let 'str' point to next character.
			}
		}

		else
		{
			if( *str == '\n' )                      // if current character is '\n', insert and output '\r'
				uart_putchar( '\r' );

			uart_putchar( *str++ );
		}
	}

	va_end( argp );
#endif
}



void eFG_printf( const char * str, ...)
{
}











/*
******************************************************************************************************************
*
*Function Name : int_to_string_dec
*
*Description : This function is to convert an 'int' data 'input' to a string in decimalism, and the string
*              converted is in 'str'.
*
*Input : int input : 'int' data to be converted.
*        char * str : the start address of the string storing the converted result.
*
*Output : void
*
*call for :
*
*Others : None at present.
*
*******************************************************************************************************************
*/



//void _sys_exit( void )
//{
//	return;
//}



//#pragma arm section  code="int_to_string_dec"
void int_to_string_dec( int input , char * str)
{
#if 1
	char stack[12];
	char sign_flag = POSITIVE ;      // 'sign_flag indicates wheater 'input' is positive or negative, default
	int i ;                           // value is 'POSITIVE'.
	int j ;



	if( input == 0 )
	{
		str[0] = '0';
		str[1] = '\0';                   // 'str' must end with '\0'
		return ;
	}

	if( input < 0 )                      // If 'input' is negative, 'input' is assigned to its absolute value.
	{
		sign_flag = NEGATIVE ;
		input = -input ;
	}

	for( i = 0; input > 0; ++i )
	{
		stack[i] = input%10 + '0';      // characters in reverse order are put in 'stack' .
		input /= 10;
	}                                   // at the end of 'for' loop, 'i' is the number of characters.


    j = 0;
	if( sign_flag == NEGATIVE )
		str[j++] = '-';		            // If 'input' is negative, minus sign '-' is placed in the head.
	for( --i  ; i >= 0; --i, ++j )
		str[j] = stack[i];
	str[j] = '\0';				        // 'str' must end with '\0'
#endif
}







/*
******************************************************************************************************************
*
*Function Name : Uint_to_string_dec
*
*Description : This function is to convert an 'unsigned int' data 'input' to a string in decimalism, and the
*              string converted is in 'str'.
*
*Input : int input : 'unsigned int' data to be converted.
*        char * str : the start address of the string storing the converted result.
*
*Output : void
*
*call for :
*
*Others : None at present.
*
*******************************************************************************************************************
*/
//#pragma arm section  code="Uint_to_string_dec"
void Uint_to_string_dec( unsigned int input, char * str )
{
#if 1
	char stack[11] ;
	int i ;
	int j ;


	if( input == 0 )
	{
		str[0] = '0';
		str[1] = '\0';                   // 'str' must end with '\0'
		return ;
	}

	for( i = 0; input > 0; ++i )
	{
		stack[i] = input%10 + '0';       // characters in reverse order are put in 'stack' .
		input /= 10;
	}                                    // at the end of 'for' loop, 'i' is the number of characters.

	for( --i, j = 0; i >= 0; --i, ++j )
		str[j] = stack[i];
	str[j] = '\0';		                 // 'str' must end with '\0'
#endif
}




/*
******************************************************************************************************************
*
*Function Name : int_to_string_hex
*
*Description : This function is to convert an 'int' data 'input' to a string in hex, and the string
*              converted is in 'str'.
*
*Input : int input : 'int' data to be converted.
*        char * str : the start address of the string storing the converted result.
*        int hex_flag : hex_flag is just a option to distinguish whether hex format is '0f' or '0F'.
*
*Output : void
*
*call for :
*
*Others : None at present.
*
*******************************************************************************************************************
*/
//#pragma arm section  code="int_to_string_hex"
void int_to_string_hex( int input, char * str, int hex_flag )
{
	int i;
	static char base[] = "0123456789abcdef";


	for( i = 9; i > 1; --i )
	{
		str[i] = base[input&MASK_LOW4];
		input >>= 4;
	}

	str[0] = '0';
	str[1] = 'x';                         // Hex format shoud start with "0x" or "0X".
	str[10] = '\0';                        // 'str' must end with '\0'

	return;
}


