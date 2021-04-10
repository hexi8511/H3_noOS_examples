#ifndef __BASIC_REG_H__
#define __BASIC_REG_H__

#include <linux/types.h>

#define __write_reg(addr, val) (*((volatile unsigned int *)addr)=val)
#define __read_reg(addr)	  (*((volatile unsigned int *)addr))


//#define readl(addr) (*((volatile unsigned int *)addr))
//#define writel(val,addr)  	((*((volatile unsigned int *)addr)) = (val))

#define set_wbit(addr, v)   (*((volatile unsigned long  *)(addr)) |=  (unsigned long)(v))
#define clr_wbit(addr, v)   (*((volatile unsigned long  *)(addr)) &= ~(unsigned long)(v))



#define BW1 0x1
#define BW2 0x3
#define BW3 0x7
#define BW4 0xf
/*unsigned int tmp_reg=0;*/
#define __set_bit(bit,bitw,val) do{tmp_reg=tmp_reg&(~(bitw<<bit));tmp_reg=tmp_reg|(val<<bit);}while(0)


/*function section flag*/
#define __init __attribute__((section(".mod.init.text")))  
#define __exit __attribute__((section(".mod.exit.text")))  

#if 0

typedef unsigned int u32;
typedef signed int s32;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned char u8;
typedef signed char s8;

typedef unsigned int __u32;
typedef signed int __s32;
typedef unsigned short __u16;
typedef signed short __s16;
typedef unsigned char __u8;
typedef signed char __s8;
#endif
#endif
