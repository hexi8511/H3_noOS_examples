
#include "mmu.h"
#include "basic_reg.h"

extern void    mmu_set_smp( void);
extern void    mmu_flush_TLB( void);
extern void    flush_icache( void );
extern void    flush_dcache( void );
extern void    mmu_set_domain_access( void );


void mmu_system_init(u32 dram_base, u32 dram_size, u32 mmu_base)
{
	s32   *mmu_tlb_address = (s32 *)mmu_base;
	u32    i;

	//建立16k的段表，表项大小为1M
	for(i = 0; i < 4 * 1024; i++)
	{
		mmu_tlb_address[i] =  (i << 20)    |
							  (0 << 19)    |
							  (0 << 18)    |
							  (0 << 17)    |
							  (0 << 16)    |
							  (0 << 15)    |
							  (0 << 12)    |
							  (3 << 10)    |
							  (0 <<  9)    |
							  (15 << 5)    |
							  (0  << 4)    |
							  (0  << 3)    |
							  (0  << 2)    |
							  (2  << 0);
	}
	//cache sram
	mmu_tlb_address[0] = (0 << 20)    |         //地址
						 (0 << 19)    |         //安全区域
						 (0 << 18)    |         //1M段表
						 (0 << 17)    |         //not global
						 (0 << 16)    |         //not shared
						 (0 << 15)    |         //
						 (0 << 12)    |         //
						 (3 << 10)    |         //访问权限 特权
						 (0 <<  9)    |         //
						 (15 << 5)    |         //域控制
						 (0  << 4)    |         //
						 (1  << 3)    |         //有cache
						 (0  << 2)    |         //无buffer
						 (2  << 0);             //段表

	//cache dram
	for(i = 0; i < dram_size; i++)
	{
		mmu_tlb_address[i + (dram_base>>20)] = (dram_base + (i << 20))  |
									     (0 << 19)    |
										 (0 << 18)    |
										 (0 << 17)    |
										 (0 << 16)    |
										 (0 << 15)    |
										 (0 << 12)    |
										 (3 << 10)    |
									     (0 <<  9)    |
										 (15 << 5)    |
										 (0  << 4)    |
										 (1  << 3)    |
										 (0  << 2)    |
										 (2  << 0);
	}

	//map 0x01c00000~0x01cfffff to 0x01c00000~0x01cfffff
	mmu_tlb_address[0x01c] = (0x01c00000)  |
									     (0 << 19)    |
										 (0 << 18)    |
										 (0 << 17)    |
										 (0 << 16)    |
										 (0 << 15)    |
										 (0 << 12)    |
										 (3 << 10)    |
									     (0 <<  9)    |
										 (0 << 5)   |
										 (1  << 4)    |
										 (0  << 3)    |
										 (0  << 2)    |
										 (2  << 0);

	mmu_flush_TLB();

	//set ttb
	asm("mcr p15,0,%0,c2,c0,0"::"r"(mmu_base));
	asm("mcr p15,0,%0,c2,c0,1"::"r"(mmu_base));

	//clean i/d cache
	flush_icache();
	flush_dcache();

	//set domain controller
	mmu_set_domain_access();
	//
	//mmu_set_smp();

	return ;
}

void   mmu_enable( void )
{
	u32 c1format = 0;

	asm("mrc p15,0,%0,c1,c0,0":"=r"(c1format));
	c1format &= ~ 0x1007;
	c1format |= 0x1001;				//打开ICACHE，MMU，DISABLE ALIGIN CHECK
	asm("mcr p15,0,%0,c1,c0,0"::"r"(c1format));
	
	
}



void   mmu_disable( void )
{
	u32 c1format = 0;

	asm("mrc p15,0,%0,c1,c0,0":"=r"(c1format));
	c1format &= ~ 0x1007;
	c1format |= 0;
	asm("mcr p15,0,%0,c1,c0,0"::"r"(c1format));
}
