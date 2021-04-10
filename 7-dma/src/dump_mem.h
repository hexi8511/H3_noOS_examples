#ifndef __DUMP_MEM_H__
#define __DUMP_MEM_H__
struct cpu_arm_reg_t
{
	//ARM 						// 	APCS
	unsigned int r0;		//a1
	unsigned int r1;		//a2
	unsigned int r2;		//a3
	unsigned int r3;		//a4
	unsigned int r4;		//v1
	unsigned int r5;		//v2
	unsigned int r6;		//v3
	unsigned int r7;		//v4
	unsigned int r8;		//v5
	unsigned int r9;		//v6
	unsigned int r10;	//sl
	unsigned int r11;	//fp
	unsigned int r12;	//ip
	unsigned int r13;	//sp
	unsigned int r14;	//lr
	unsigned int r15;	//pc

	unsigned int cpsr;
	unsigned int c0;
	unsigned int c1;
	unsigned int c2;
	unsigned int c3;
	unsigned int c4;
	unsigned int c5;
	unsigned int c6;
	unsigned int c7;
	unsigned int c8;
	unsigned int c9;
	unsigned int c10;
	unsigned int c11;
	unsigned int c12;
	unsigned int c13;
	unsigned int c14;
	unsigned int c15;
};

void dump_mem(unsigned int buf_addr, unsigned int size);
void dump_regs_test(void);
#endif
