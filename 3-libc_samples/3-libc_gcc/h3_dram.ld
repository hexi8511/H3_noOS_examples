/*
 * Copyright (c) 2017 Texas Instruments
 *
 * (C) Copyright 2017
 * Gary Jennejohn, DENX Software Engineering, <garyj@denx.de>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

STACK_FIQ_SIZE 	= 0x0400;
STACK_IRQ_SIZE 	= 0x0400;
STACK_ABT_SIZE 	= 0x0400;
STACK_UND_SIZE 	= 0x0400;
STACK_SRV_SIZE 	= 0x8000;

/* Memory Spaces Definitions */
MEMORY
{
	ROM (rx)	: ORIGIN = 0x4A000000, LENGTH = 0x02000000	/* 32 MB */
	RAM (rwx)	: ORIGIN = 0x4C000000, LENGTH = 0x0A000000	/* 160 MB */
}

SECTIONS
{
	.text :
	{
		KEEP(*(.isr_vector))
		*(.text*)
		*(.rodata*)
	} > ROM

    . = ALIGN(4);
    /*Why align here, well,
    in order to make __etext + 4*n = __data_end__
    if don't have this ALIGN, they will never equal
    and startup code loop forever*/

    __etext = .;

    .data : AT (__etext)
    {
        . = ALIGN(4);
		__data_start__ = .;
		*(.data)
		*(.data*)
		. = ALIGN(4);
		__data_end__ = .;
    } > RAM

    . = ALIGN(4);
	.bss (NOLOAD):
	{
		__bss_start__ = .;
		*(.bss*)
		*(COMMON)
		__bss_end__ = .;
	} > RAM

    .heap (NOLOAD):
	{
		. = ALIGN(8);
		end = .;
		. += 0xA0000; /* 64kB of heap memory */
		HeapLimit = .;
	} > RAM

    .stack (NOLOAD):
	{
		. = ALIGN(8);
		PROVIDE (__stack_start = .);
		PROVIDE (__stack_fiq_start = .);
		. += STACK_FIQ_SIZE;
		PROVIDE (__stack_fiq_end = .);
		. = ALIGN(8);
		PROVIDE (__stack_irq_start = .);
		. += STACK_IRQ_SIZE;
		PROVIDE (__stack_irq_end = .);
		. = ALIGN(8);
		PROVIDE (__stack_abt_start = .);
		. += STACK_ABT_SIZE;
		PROVIDE (__stack_abt_end = .);
		. = ALIGN(8);
		PROVIDE (__stack_und_start = .);
		. += STACK_UND_SIZE;
		PROVIDE (__stack_und_end = .);
		. = ALIGN(8);
		PROVIDE (__stack_srv_start = .);
		. += STACK_SRV_SIZE;
		PROVIDE (__stack_srv_end = .);
		. = ALIGN(8);
		PROVIDE (__stack_end = .);
	} > RAM
}
