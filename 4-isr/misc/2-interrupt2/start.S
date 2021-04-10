#define BOOT1_BASE					 ( 0x42400000)
#define BOOT_SYSTEM_STACK_BOTTOM     ( BOOT1_BASE               + 0x00300000 )
#define BOOT_IRQ_STACK_BOTTOM        ( BOOT_SYSTEM_STACK_BOTTOM - 0x00080000 )
#define BOOT_SVC_STACK_BOTTOM        ( BOOT_IRQ_STACK_BOTTOM    - 0x00080000 )
	
	.align 4
	.arm

start2:
	mrs	r0, CPSR
   	bic	r0, r0, #31
   	orr	r0, r0, #19
   	orr	r0, r0, #192	@ 0xc0
   	bic	r0, r0, #512	@ 0x200
  	msr	cpsr_c, r0

  	mrc p15, 0, r0, c1, c0
    bic	r0, r0, #5
    bic	r0, r0, #6144	@ 0x1800
    bic	r0, r0, #2
   	mcr p15, 0, r0, c1, c0
	
@set SP for C language
	ldr sp, =0x0F7DC
	ldr r0, =0x01c00044
    ldr r1,[r0]
    orr r1,#0x1800
    str r1,[r0]
	
	
@ set pll para
	bl  config_pll1_para

	bl main	
	b .

	.end
