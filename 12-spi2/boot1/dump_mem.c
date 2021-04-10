#include "stdio.h"
#include "dump_mem.h"
void dump_mem(unsigned int buf_addr, unsigned int size)
{
		int n;
		unsigned char *psdram=(unsigned char *)buf_addr;
		for(n=0;n<size;n++)
			{
			if(((n)%16)==0)
				printf("[0x%08x] ",buf_addr+n);

			printf("0x%02x ",*psdram);
			psdram++;
			if(((n+1)%16)==0)
				printf("\n");
			}
}

extern void dump_cpu_regs(unsigned int sp_addr);


void dump_regs_test()
{
	
	unsigned int sp_addr=0x40000000+0x1000000;
	unsigned int *psp=(unsigned int*)sp_addr;
	int i;

	//clear buffer
	for(i=0;i<512;i++)
		{
		*psp=0;
		psp++;
		}
	psp=sp_addr;
	//printf("CPU REGS TRACK\n");	
	printf("=======================\n");
	dump_cpu_regs(sp_addr);

	for(i=0;i<16;i++)
		{
		printf("r%d=0x%08x\n",i,psp[i]);
		}
	printf("cpsr=0x%08x\n",i,psp[i]);
	
	for(i=0;i<128;i++)
		{
		printf("c%d=0x%08x\n",i,psp[17+i]);
		}
	printf("=======================\n");
}



