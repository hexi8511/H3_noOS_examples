	.global dump_cpu_regs
	.align 4
	.arm

dump_cpu_regs:
	stmfd sp!,{r0-r12,r14}
	@mov r0,#0
	@mov r1,#1
	@mov r2,#2
	@mov r3,#3
	@mov r4,#4
	@mov r5,#5
	@mov r6,#6
	@mov r7,#7
	@mov r8,#8
	@mov r9,#9
	@mov r10,#10
	@mov r11,#11
	@mov r12,#12
	@mov r13,#13
	@mov r14,#14

	stmea r0!,{r0-r15}
	mrs r1, cpsr
	str   r1,[r0],#4
	
	@main flag symbol register readonly
	mrc p15,0,r1,c0,c0,0
	str r1,[r0],#4

	@cache type and symbol register
	mrc p15,0,r1,c0,c0,1
	str r1,[r0],#4

	@TLB Type Register
	mrc p15,0,r1,c0,c0,3
	str r1,[r0],#4

	@Multiprocessor Affinity Register
	mrc p15,0,r1,c0,c0,5
	str r1,[r0],#4
	
	@Revision ID Register
	mrc p15,0,r1,c0,c0,6
	str r1,[r0],#4

	@Processor Feature Register 0
	mrc p15,0,r1,c0,c1,0
	str r1,[r0],#4

	@Processor Feature Register 1
	mrc p15,0,r1,c0,c1,1
	str r1,[r0],#4

	@Debug Feature Register 0
	mrc p15,0,r1,c0,c1,2
	str r1,[r0],#4

	@Memory Model Feature Register 0
	mrc p15,0,r1,c0,c1,4
	str r1,[r0],#4

	@Memory Model Feature Register 1
	mrc p15,0,r1,c0,c1,5
	str r1,[r0],#4

	@Memory Model Feature Register 2
	mrc p15,0,r1,c0,c1,6
	str r1,[r0],#4

	@Memory Model Feature Register3
	mrc p15,0,r1,c0,c1,7
	str r1,[r0],#4
	
	@Instruction Set Attribute Register 0
	mrc p15,0,r1,c0,c2,0
	str r1,[r0],#4

	@Instruction Set Attribute Register 1
	mrc p15,0,r1,c0,c2,1
	str r1,[r0],#4

	@Instruction Set Attribute Register2
	mrc p15,0,r1,c0,c2,2
	str r1,[r0],#4
	
	@Instruction Set Attribute Register3
	mrc p15,0,r1,c0,c2,3
	str r1,[r0],#4

	@Instruction Set Attribute Register4
	mrc p15,0,r1,c0,c2,4
	str r1,[r0],#4

	@Cache Size ID Register
	mrc p15,1,r1,c0,c0,0
	str r1,[r0],#4

	@Read Cache Level ID Register
	mrc p15,1,r1,c0,c0,1
	str r1,[r0],#4
	
	
@system control register
	mrc p15,0,r1,c1,c0,0
	str r1,[r0],#4
	orr r1,r1,#(1<<12)
	mcr p15,0,r1,c1,c0,0
	mrc p15,0,r1,c1,c0,0
	str r1,[r0],#4


	#if 0
	mrc p15,0,r5,c1,c0,0
	str r5,[r0],#4

	mrc p15,0,r5,c2,c0,0
	str r1,[r0],#4
	
	mrc p15,0,r5,c3,c0,0
	str r5,[r0],#4
	
	@not used in coterx A7s
	@mrc p15,0,r1,c4,c0,0
	@str r1,[r0],#4

	mrc p15,0,r5,c5,c0,0
    str r5,[r0],#4

	mrc p15,0,r5,c6,c0,0
	str r5,[r0],#4
	
	@write only register
	@mrc p15,0,r5,c7,c0,0
	@str r5,[r0],#4
	
	@write only register
	@mrc p15,0,r5,c8,c0,0
	@str r5,[r0],#4

	@mrc p15,0,r5,c9,c0,0
	@str r5,[r0],#4


	@mrc p15,0,r5,c10,c0,0
	@str r5,[r0],#4

	@resever
	@mrc p15,0,r5,c11,c0,0
	@str r5,[r0],#4

	@resever
	@mrc p15,0,r5,c12,c0,0
	@str r5,[r0],#4

	mrc p15,0,r5,c13,c0,0
	str r5,[r0],#4

	@resever
	@mrc p15,0,r5,c14,c0,0
	@str r5,[r0],#4

	@mrc p15,0,r5,c15,c0,0
	@str r5,[r0],#4
#endif


	ldmfd sp!,{r0-r12,pc}
