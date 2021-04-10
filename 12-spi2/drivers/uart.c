#include "gpio_reg.h"
#include "basic_reg.h"
#include "ccu_reg.h"
#include "uart_reg.h"
#include "ccu.h"
#include <linux/io.h>
#include "uart.h"

//#include "stdio.h"
#include "delay.h"


void uart0_clk_init()
{
	//set_ahb1_clksrc(1);//24MHz
	//set_apb2_clksrc(1);//24Mhz
	//ccm_module_enable(UART0_CKID);
	//ccm_clock_enable(UART0_CKID);
	writel(readl(BUS_CLK_GATING_REG3)|(1<<16),BUS_CLK_GATING_REG3);

	writel(readl(BUS_SOFT_RST_REG4)&(~(1<<16)),BUS_SOFT_RST_REG4);
	writel(readl(BUS_SOFT_RST_REG4)|(1<<16),BUS_SOFT_RST_REG4);
	#if 0
	int i=0;
	volatile unsigned int reg;
		//reset
		reg = readl(BUS_SOFT_RST_REG4);
		reg &= ~(1<<(16));
		writel(reg, BUS_CLK_GATING_REG3);
		for( i = 0; i < 100; i++ );
		reg |=  (1<<(16));
		writel(reg, BUS_SOFT_RST_REG4);
		//gate
		reg = readl(BUS_CLK_GATING_REG3);
		reg &= ~(1<<(16));
		writel(reg, BUS_CLK_GATING_REG3);
		for( i = 0; i < 100; i++ );
		reg |=  (1<<(16));
		writel(reg, BUS_CLK_GATING_REG3);
	#endif
}

void uart0_pin_init()
{
	//gpio_A4 uart0_tx
	volatile unsigned int tmp_reg;

	tmp_reg=__read_reg(PA_CFG0);
	__set_bit(16,BW3,IO_MUL);
	__write_reg(PA_CFG0,tmp_reg);
	//gpio_A5 uart0_rx
	tmp_reg=__read_reg(PA_CFG0);
	__set_bit(20,BW3,IO_MUL);
	__write_reg(PA_CFG0,tmp_reg);

	tmp_reg=__read_reg(PA_DRV0);
	__set_bit(8,BW2,DRV_LEV1);
	__write_reg(PA_DRV0,tmp_reg);


	tmp_reg=__read_reg(PA_PUL0);
	__set_bit(8,BW2,PUL_UP);
	__write_reg(PA_PUL0,tmp_reg);

	tmp_reg=__read_reg(PA_DRV0);
	__set_bit(10,BW2,DRV_LEV1);
	__write_reg(PA_DRV0,tmp_reg);


	tmp_reg=__read_reg(PA_PUL0);
	__set_bit(10,BW2,PUL_UP);
	__write_reg(PA_PUL0,tmp_reg);

}

void uart0_baud_init(unsigned int speed)
{
	volatile unsigned int tmp_reg;

	//set dll dlh register
	tmp_reg=readl(UART0_LCR);
	tmp_reg|=(1<<7);
	writel(tmp_reg,UART0_LCR);

	//unsigned int apb2_speed=24000000;
	//set baud speed
	//tmp_reg=apb2_speed/(speed*16);
	tmp_reg=13;

	while((readl(UART0_USR)&0x01)==1)
	{

	}
	writel(tmp_reg,UART0_DLL);
	writel(tmp_reg>>8,UART0_DLH);

	//
	tmp_reg=readl(UART0_LCR);
	tmp_reg&=(~(1<<7));
	writel(tmp_reg,UART0_LCR);
	delay();


	tmp_reg=readl(UART0_LCR);
	tmp_reg&=(~0x3f);
	tmp_reg=(0x03<<0)|(0<<2)|(0<<3)|(0<<4);//8 bit ,1 bit stop, no check
	writel(tmp_reg,UART0_LCR);

	//disable uart fifo
	tmp_reg=0x03;
	writel(tmp_reg,UART0_FCR); 
	tmp_reg=readl(UART0_FCR);
	printf("uart0_fcr %x\n",tmp_reg);

	//enable change for lcr
	tmp_reg=readl(UART0_HALT);
	tmp_reg&=~(3<<1);
	tmp_reg|=(3<<1);
	writel(tmp_reg,UART0_HALT);

}

void uart0_baud_init2(unsigned int speed)
{
	volatile unsigned int tmp_reg;

	//set dll dlh register
	tmp_reg=readl(UART0_LCR);
	tmp_reg|=(1<<7);
	writel(tmp_reg,UART0_LCR);

	unsigned int apb2_speed=600000000;
	//set baud speed
	//tmp_reg=apb2_speed/(speed*16);
	tmp_reg=325;

	while((readl(UART0_USR)&0x01)==1)
	{

	}
	writel(tmp_reg,UART0_DLL);
	writel(tmp_reg>>8,UART0_DLH);

	//
	tmp_reg=readl(UART0_LCR);
	tmp_reg&=(~(1<<7));
	writel(tmp_reg,UART0_LCR);
	delay();


	tmp_reg=readl(UART0_LCR);
	tmp_reg&=(~0x3f);
	tmp_reg=(0x03<<0)|(0<<2)|(0<<3)|(0<<4);//8 bit ,1 bit stop, no check
	writel(tmp_reg,UART0_LCR);

	tmp_reg=0x03;
	writel(tmp_reg,UART0_FCR); 
	tmp_reg=readl(UART0_FCR);
	printf("uart0_fcr %x\n",tmp_reg);

	//enable change for lcr
	tmp_reg=readl(UART0_HALT);
	tmp_reg&=~(3<<1);
	tmp_reg|=(3<<1);
	writel(tmp_reg,UART0_HALT);

}

void uart0_baud_init3(unsigned int speed)
{
	volatile unsigned int tmp_reg;

	//set dll dlh register
	tmp_reg=readl(UART0_LCR);
	tmp_reg|=(1<<7);
	writel(tmp_reg,UART0_LCR);

	unsigned int apb2_speed=12000000;
	//set baud speed
	//tmp_reg=apb2_speed/(speed*16);
	tmp_reg=13;

	while((readl(UART0_USR)&0x01)==1)
	{

	}
	writel(tmp_reg,UART0_DLL);
	writel(tmp_reg>>8,UART0_DLH);

	//
	tmp_reg=readl(UART0_LCR);
	tmp_reg&=(~(1<<7));
	writel(tmp_reg,UART0_LCR);
	delay();



	tmp_reg=readl(UART0_LCR);
	tmp_reg&=(~0x3f);
	tmp_reg=(0x03<<0)|(0<<2)|(0<<3)|(0<<4);//8 bit ,1 bit stop, no check
	writel(tmp_reg,UART0_LCR);
	

	tmp_reg=0x01;

	writel(tmp_reg,UART0_FCR); 
	tmp_reg=readl(UART0_FCR);
	printf("uart0_fcr %x\n",tmp_reg);
	//enable change for lcr
	tmp_reg=readl(UART0_HALT);
	tmp_reg&=~(3<<1);
	tmp_reg|=(3<<1);
	writel(tmp_reg,UART0_HALT);

}



void uart0_init(int baud)
{

	uart0_pin_init();
	uart0_clk_init();
	uart0_baud_init(baud);

}

void putc(unsigned char c)
{
	while(((1<<6)&readl(UART0_LSR))==0)
		{
		//led_blue_flash();
		}
	writel(c,UART0_THR);
}

unsigned char getc(void)
{
	while(((1<<0)&readl(UART0_LSR))==0)
	{
	
	}
	return readl(UART0_RBR);
}




void puts(char *str)
{
	while(*str!='\0')
	{
		putc(* str);
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
		putc( *str++ );
	}

	putc('\r');
	putc('\n');
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
			putc( '\r' );

		putc( *str++ );
	}
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
				case 'c':putc( va_arg( argp,  __s32 ) );
						  ++str;
						  break;
				case 's': uart_puts_no_newline( va_arg( argp, char * ) );
						  ++str;
						  break;
				default : putc( '%' );       // if current character is not Conversion Specifiers 'dxpXucs',
						  putc( *str );         // output directly '%' and current character, and then
						  ++str;                        // let 'str' point to next character.
			}
		}

		else
		{
			if( *str == '\n' )                      // if current character is '\n', insert and output '\r'
				putc( '\r' );
			putc( *str++ );
		}
	}

	va_end( argp );
#endif
}

