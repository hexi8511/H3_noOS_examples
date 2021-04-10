#include "arm_a8.h"
#define BOOT_SVC_BOTTOM					 ( 0x10000)
#define BOOT_SYSTEM_STACK_BOTTOM     ( BOOT_SVC_BOTTOM               - 0x001000 )
#define BOOT_IRQ_STACK_BOTTOM        ( BOOT_SYSTEM_STACK_BOTTOM - 0x0001000 )
#define BOOT_SVC_STACK_BOTTOM        ( BOOT_IRQ_STACK_BOTTOM    - 0x0001000 )
	
	.align 4
	.arm

start:
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
	ldr sp, =BOOT_SVC_BOTTOM
/*		ldr r0, =0x01c00044
    ldr r1,[r0]
    orr r1,#0x1800
    str r1,[r0]*/
	
	
@ set pll para
	bl  config_pll1_para


@;//disable all interrupts
	//bl close_sys_int
@;//set processor mode as SVC
	mrs r0, cpsr
	bic r0, r0, #ARMV7_MODE_MASK
	orr r0, r0, #ARMV7_SVC_MODE
	orr r0, r0, #( ARMV7_FIQ_MASK | ARMV7_IRQ_MASK )  @;//After reset, ARM automaticly disables IRQ and FIQ, and runs in SVC mode.
	bic r0, r0, #ARMV7_CC_E_BIT                       @;//set little-endian
	msr cpsr_c, r0


@;// configure memory system : disable MMU,cache and write buffer; set little_endian; set high interrupt vector table
	mrc p15, 0, r0, c1, c0
	bic r0, r0, #( ARMV7_C1_M_BIT | ARMV7_C1_C_BIT )  @;// disable MMU, data cache
	bic r0, r0, #( ARMV7_C1_I_BIT | ARMV7_C1_Z_BIT )  @;// disable instruction cache, disable flow prediction
	bic r0, r0, #( ARMV7_C1_A_BIT)                    @;// disable align
	mcr p15, 0, r0, c1, c0

@;// set SP for IRQ mode
	mrs r0, cpsr
    bic r0, r0, #ARMV7_MODE_MASK
    orr r0, r0, #ARMV7_IRQ_MODE
    msr cpsr_c, r0
	ldr	sp,	=BOOT_IRQ_STACK_BOTTOM

@;// set SP for SYSTEM mode
	mrs r0, cpsr
    bic r0, r0, #ARMV7_MODE_MASK
    orr r0, r0, #ARMV7_SYSTEM_MODE
    msr cpsr_c, r0
	ldr	sp,	=BOOT_SYSTEM_STACK_BOTTOM
#if 0
@;// set SP for SVC mode
	mrs r0, cpsr
    bic r0, r0, #ARMV7_MODE_MASK
    orr r0, r0, #ARMV7_SVC_MODE
    msr cpsr_c, r0
	ldr sp, =BOOT_SVC_STACK_BOTTOM

@;// set SP for USR mode
	mrs r0, cpsr
    bic r0, r0, #ARMV7_MODE_MASK
    orr r0, r0, #ARMV7_SYSTEM_MODE
    msr cpsr_c, r0
#endif

	bl main	
	b .

	.end
