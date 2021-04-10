/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : arm_start
*
* By      : Jerry
*
* Version : V2.00
*
* Date	   :  2020-9-19
*
* Descript:
*
**********************************************************************************************************************
*/

	#include "arm_a8.h"
/*********************************BOOT ÏµÍ³³õÊŒ»¯ŽúÂë********************************/
/*
@start
@; ARM ÏòÁ¿±í ..........
@; 00 - Reset
@; 04 - Undefined instructions
@; 08 - SWI instructions
@; 0C - prefetch abort
@; 10 - Data abort
@; 14 - Reserved
@; 18 - IRQ interrupts
@; 1C - FIQ interrupts
@;Ö§³Ö³€Ìø×ª
*/
     ldr     pc,_undefined_instruction
     ldr     pc,_undefined_instruction
     ldr     pc,_software_interrupt
     ldr     pc,_undefined_instruction
     ldr     pc,_undefined_instruction
     ldr     pc,_undefined_instruction
     ldr     pc,_irq
     ldr     pc,_undefined_instruction

_undefined_instruction:	.word undefined_instruction
_software_interrupt:    .word software_interrupt
_irq:					.word irq_interrupt

#define     INTC_REG_VCTR    0x1c20400
//extern undefined_instruction_fun;
//extern software_interrupt_fun;
/*---------------------------------------------------------------------------------------*/

irq_interrupt:
	sub     lr, lr, #4                                      @; ±£Žæ·µ»ØµØÖ·
	stmfd   sp!, {r0-r12, lr}@; save context       	        @; ŒÄŽæÆ÷Ñ¹Õ»
	mrs     r3, spsr                                        @; ¶ÁÈ¡SPSR
	stmfd   sp!, {r3}	                                    @; Ñ¹Õ»

    msr     cpsr_c, #(ARMV7_FIQ_MASK | ARMV7_IRQ_MASK | ARMV7_SYSTEM_MODE)     @; ÇÐ»»µœSYSTEMÄ£Êœ
    stmfd   sp!, {r0-r12, lr}                                @; ±£Žælr_usrºÍÆäËüÓÃµœµÄŒÄŽæÆ÷

	bl      gic_irq_handler

    ldmfd   sp!, {r0-r12, lr}                           @; »ÖžŽSYSTEMÄ£ÊœŒÄŽæÆ÷
    msr     cpsr_c, #(ARMV7_FIQ_MASK | ARMV7_IRQ_MASK | ARMV7_IRQ_MODE)     @; ÇÐ»»µœIRQÄ£Êœ
    ldmfd   sp!, {r3} 	                                    @; ÊýŸÝ³öÕ»
    msr     spsr_cxsf, r3                                   @; »¹Ô­spsr

	ldmfd   sp!, {r0-r12, pc}^    @;ŽÓÒì³£Ä£Êœ·µ»Ø; unknown event ignore


undefined_instruction:
   bl  undefined_instruction_fun

software_interrupt:
   bl  software_interrupt_fun
/*------------------------------------------end------------------------------------------*/





