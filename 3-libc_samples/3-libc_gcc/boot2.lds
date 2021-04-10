OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(start2)

	
MEMORY
{
	ram  : org = 0x40000000, len = 768M
	heap : org = 0x60000000, len = 256M
}

SECTIONS
{

	. = 0x40000000;
	
	.text ALIGN(4) : {
		start2.o(.text);
		main2.o(.text);
		cpu/int.o(.text);
		*.o(text);
 		*(.rodata)
        *(.rodata.str1.1)
        *(.rodata.str1.4)
        *(.rodata.*)
	} >ram

	.text.startup ALIGN(4)  : {
	}>ram
	
	.arm_start ALIGN(4) : {
		__arm_start = .;
		cpu/arm_start.o(.text);
		__arm_end = .;	
	} >ram
	
	.data ALIGN(4):{
		start2.o(.data);
		main2.o(.data);
		*.o(.data);
	} >ram

	.ksymtab ALIGN(4):{
	
	}>ram
	
	.bss ALIGN(4): {	
		*(.bss)
	} >ram


	.heap ALIGN(8) : {
	PROVIDE(__heap_start = ORIGIN(heap));	
	PROVIDE(__heap_end = ORIGIN(heap) + LENGTH(heap));
	}>heap
 }


