#include <stdio.h>

#define u32 unsigned int
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


