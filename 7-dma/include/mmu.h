
#ifndef  __MMU_H_
#define  __MMU_H_


void   mmu_enable( void );
void   mmu_disable( void );
void   mmu_system_init(unsigned int dram_base,unsigned int dram_size, unsigned int mmu_base);



#endif
