//------------------------------------------------------------------------------
// CONSTANT DEFINITION
//------------------------------------------------------------------------------

.equ	USR_MODE,		0x10
.equ 	FIQ_MODE,		0x11
.equ 	IRQ_MODE,		0x12
.equ 	SVC_MODE,		0x13
.equ 	ABT_MODE,		0x17
.equ 	UDF_MODE,		0x1b
.equ 	MODE_MASK,		0x1f
.equ 	NO_INT,			0xc0

.equ    PACONF1,        0x01C20804
.equ    PADATA,         0x01C20810
//------------------------------------------------------------------------------
// MACRO DEFINITION
//------------------------------------------------------------------------------

.macro led_on
    ldr r10, =PACONF1
    ldr r11, [r10]
    and r11, r11, #~(6<<28)
    str r11, [r10]

    ldr r10, =PADATA
    ldr r11, [r10]
    orr r11, r11, #(1<<10)
    str r11, [r10]
.endm

.macro led2_on
    ldr r0, =PACONF1
    ldr r1, [r0]
    and r1, r1, #~(6<<8)
    str r1, [r0]

    ldr r0, =PADATA
    ldr r1, [r0]
    orr r1, r1, #(1<<10)
    str r1, [r0]
.endm

.macro led_off
    ldr r10, =PACONF1
    ldr r11, [r10]
    and r11, r11, #~(6<<28)
    str r11, [r10]

    ldr r10, =PADATA
    ldr r11, [r10]
    and r11, r11, #~(1<<10)
    str r11, [r10]
.endm

.macro  def_irq_handler handler_name
    .align 4
    .weak    \handler_name
    .type    \handler_name, %function
\handler_name :
    led_on
    b    .
    .size    \handler_name, . - \handler_name
.endm

//------------------------------------------------------------------------------
// ISR VECTOR SECTION
//------------------------------------------------------------------------------
    .section .isr_vector
    .global	_start
_start:

	b	reset                             // Reset handler
	ldr	pc, Undefined_Instruction_Handler // Undefined instruction handler
	ldr	pc, Software_Interrupt_Handler    // Software interrupt handler
	ldr	pc, Prefetch_Abort_Handler        // Prefetch abort handler
	ldr	pc, Data_Abort_Handler            // Data abort handler
	nop                                   // Not use
	ldr	pc, IRQ_Handler                   // IRQ handler
	ldr	pc, FIQ_Handler                   // FIQ handler

Undefined_Instruction_Handler:
	.long udefined_instruction_handler
Software_Interrupt_Handler:
	.long swi_handler
Prefetch_Abort_Handler:
	.long prefetch_abort_handler
Data_Abort_Handler:
	.long data_abort_handler
IRQ_Handler:
	.long irq_handler
FIQ_Handler:
	.long fiq_handler

//------------------------------------------------------------------------------
// .TEXT SECTION
//------------------------------------------------------------------------------

    .text
	.arm

//------------------------------------------------------------------------------
// RESET HANDLER
//------------------------------------------------------------------------------

    .align 4
	.global	reset
reset:

	ldr r0,=0x31
	ldr r1,=0x01C28000


	str r0,[r1]
	ldr r0,=0x32
	str r0,[r1]
	
	/* Set the cpu to supervisor mode */
	mrs	r0, cpsr
	bic	r0, r0, #0x1f
	// allow irq, bit 7 cpsr
	orr	r0, r0, #0xd3
	// deny irq, clear bit 7, => init stack |NO_INT => also MASK IRQ FIQ
    orr	r0, r0, #0x53
	msr	cpsr, r0

	/* Enable VFP unit*/
	mrc p15, 0, r0, c1, c0, 2
	orr r0, r0, #(0xf << 20)
	mcr p15, 0, r0, c1, c0, 2
	mov r0, #0x40000000
	vmsr fpexc, r0
/*
	mov r1, #0
	mcr p15, 0, r1, c7, c5, 4
*/	

	/* Cache init */
	mrc	p15, 0, r0, c0, c0, 0		/* read main ID register */
	and	r1, r0, #0x00f00000			/* variant */
	and	r2, r0, #0x0000000f			/* revision */
	orr	r2, r2, r1, lsr #20-4		/* combine variant and revision */
	cmp	r2, #0x30
	mrceq	p15, 0, r0, c1, c0, 1	/* read ACTLR */
	orreq	r0, r0, #0x6			/* Enable DP1(2), DP2(1) */
	mcreq	p15, 0, r0, c1, c0, 1	/* write ACTLR */

	/* Invalidate L1 I/D */
	mov	r0, #0						/* set up for MCR */
	mcr	p15, 0, r0, c8, c7, 0		/* invalidate TLBs */
	mcr	p15, 0, r0, c7, c5, 0		/* invalidate icache */

	/* Disable mmu stuff and caches */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002000			/* clear bits 13 (--v-) */
	bic	r0, r0, #0x00000007			/* clear bits 2:0 (-cam) */
	orr	r0, r0, #0x00000002			/* set bit 1 (--a-) align */
	orr	r0, r0, #0x00000800			/* set bit 12 (z---) btb */
	mcr	p15, 0, r0, c1, c0, 0

	/* Set Vector Base Address Register */
	ldr r0, =_start
	mcr p15, 0, r0, c12, c0, 0
	// allow remap vector table
	mrc p15, 0, r0, c1, c0, 0
	bic r0, #(1<<13)
	mcr p15, 0, r0, c1, c0, 0


	 led_on
    /* initialize stacks */
	bl	init_stacks
	led_off
//------------------------------------------------------------------------------
// INITIALIZE MEMORY SECTIONS
//------------------------------------------------------------------------------

    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

.flash_to_ram_loop:
    cmp     r2, r3
    ittt    lt
    ldrlt   r0, [r1], #4
    strlt   r0, [r2], #4
    blt    .flash_to_ram_loop

    ldr  r2, =__bss_start__
    b  LoopFillZerobss
/* Zero fill the bss segment. */
FillZerobss:
    movs  r3, #0
    str  r3, [r2], #4

LoopFillZerobss:
    ldr  r3, = __bss_end__
    cmp  r2, r3
    bcc  FillZerobss

    /* Jump to main */
  bl main
//	bl	0x91 //test exception

//------------------------------------------------------------------------------
// INITIALIZE STACKS
//------------------------------------------------------------------------------

init_stacks:
	mrs	r0, cpsr
	bic	r0, r0, #MODE_MASK | NO_INT
	orr	r1, r0, #UDF_MODE
	msr	cpsr_cxsf, r1
	ldr	sp, _stack_und_end

	bic	r0, r0, #MODE_MASK | NO_INT
	orr	r1, r0, #ABT_MODE
	msr	cpsr_cxsf, r1
	ldr	sp, _stack_abt_end

	bic	r0, r0, #MODE_MASK | NO_INT
	orr	r1, r0, #IRQ_MODE
	msr	cpsr_cxsf, r1
	ldr	sp, _stack_irq_end

	bic	r0, r0, #MODE_MASK | NO_INT
	orr	r1, r0, #FIQ_MODE
	msr	cpsr_cxsf, r1
	ldr	sp, _stack_fiq_end

	bic	r0, r0, #MODE_MASK | NO_INT
	orr	r1, r0, #SVC_MODE
	msr	cpsr_cxsf, r1
	ldr	sp, _stack_srv_end
	mov	pc, lr

//------------------------------------------------------------------------------
// EXCEPTION HANDLER DEFINITON
//------------------------------------------------------------------------------

//    def_irq_handler udefined_instruction_handler
//    def_irq_handler swi_handler
//    def_irq_handler prefetch_abort_handler
//    def_irq_handler data_abort_handler
//    def_irq_handler fiq_handler

    .global	udefined_instruction_handler
udefined_instruction_handler:
    led_on
 //   bl test_un
    b .

    .global	swi_handler
swi_handler:
    led2_on
 //   bl test_sw
    b .
    .global	prefetch_abort_handler
prefetch_abort_handler:
    led_on
 //   bl test_pr
    b .

    .global	data_abort_handler
data_abort_handler:
    led_on
   // bl test_da
    b .

    .global	fiq_handler
fiq_handler:
    led_on
   // bl test_fi
    b .

    .global	irq_handler
irq_handler:
	subs lr, lr, #4

//	srsfd   sp!, #0x12

	/* switch back to 	system mode */
//	cps     #0x1f
#if 0
	/* save AAPCS reg */
	push 	{r0-r12}

	/* ensure stack is 8-byte aligned */
	and     r1, sp, #4
	sub     sp, sp, r1

	/* store adjustment and lr */
	push    {r1, lr}

    //
    ldr r12, =0x1c81184
    mov r11, #(1<<18)
    str r11, [r12]
    led_on

    bl test_ir
   	/* restore lr and unadjust stack */
	pop		{r1, lr}
    add		sp, sp, r1

    /* restore AAPCs reg */
    pop		{r0-r12}
# endif
push {r0-r12}
push {lr}
    //
#if 0
    ldr r12, =0x1c81184
    mov r11, #(1<<18)
    str r11, [r12]
#endif
    //    led_on
  //  bl test_ir
pop {lr}
pop {r0-r12}
    /* return */
//    rfefd	sp!
	movs pc, lr
//    led_off
//------------------------------------------------------------------------------
// STACK LOCATIONS
//------------------------------------------------------------------------------

 	.align 4
_stack_fiq_end:
	.long	__stack_fiq_end
_stack_irq_end:
	.long	__stack_irq_end
_stack_abt_end:
	.long	__stack_abt_end
_stack_und_end:
	.long	__stack_und_end
_stack_srv_end:
	.long	__stack_srv_end

.end
