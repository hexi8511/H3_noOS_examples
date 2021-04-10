#include "arm_a8.h"
#define BOOT_SVC_BOTTOM					 ( 0x4c000)
#define BOOT_SYSTEM_STACK_BOTTOM     ( BOOT_SVC_BOTTOM               -0x001000 )
#define BOOT_IRQ_STACK_BOTTOM        ( BOOT_SYSTEM_STACK_BOTTOM - 0x0001000 )
#define BOOT_SVC_STACK_BOTTOM        ( BOOT_IRQ_STACK_BOTTOM    - 0x0001000 )


#define  SRAM_A1_ADDR 	0x00000000 
#define  SRAM_A2_ADDR 	0x00044000 	
#define  SRAM_C_ADDR 	0x00010000 


#define SRAM_A1_SIZE  	0x10000
#define SRAM_A2_SIZE  	0x8000
#define SRAM_C_SIZE  	0xb000
	.globl start
	.align 4
	.arm
	

start:
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

		
@ set pll para
	bl  config_pll1_para

	bl main	
	b .

	.end
