#include "basic_reg.h"
#include "cp15_v.h"
//#include "stdio.h"
/**
*vector table address start from 0x00000000
*/
void  set_vect_low_addr( void )
{
	u32 c1format = 0;

	asm("mrc p15,0,%0,c1,c0,0":"=r"(c1format));
	//__asm{MRC p15,0,c1format,c1,c0,0}
	c1format &= ~ 0x00002000;
	c1format |= 0;
	asm("mcr p15,0,%0,c1,c0,0"::"r"(c1format));
	//__asm{MCR p15,0,c1format,c1,c0,0}
	printf("[esdman]%s\n",__func__);
}

/**
*vector table address start from 0xffff0000
*/
void  set_vect_high_addr( void )
{
	u32 c1format = 0;

	asm("mrc p15,0,%0,c1,c0,0":"=r"(c1format));
	//__asm{MRC p15,0,c1format,c1,c0,0}
	c1format &= ~ 0x00002000;
	c1format |= 0x00002000;
	asm("mcr p15,0,%0,c1,c0,0"::"r"(c1format));
	//__asm{MCR p15,0,c1format,c1,c0,0}
	printf("[esdman]%s\n",__func__);
}


void set_vect_addr(u32 addr)
{
	u32 r0 = addr;
	/* Set vector base address register */
	asm("mcr p15, 0, %0, c12, c0, 0":"=r"(r0));
		printf("[esdman]%s\n",__func__);
}

