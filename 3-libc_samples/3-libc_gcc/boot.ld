OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(start)

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
		start.o(.text);
		/*main.o(.text);*/
		*.o(.text);
		} > ROM
		
		. = ALIGN(4);
	.data :
		{
		start.o(.data);
		main.o(.data);
		*.o(.data);
		} > RAM


	.bss ALIGN(4): {
	*(.bss)
	} > RAM
	
	.heap (NOLOAD):
	{
		. = ALIGN(8);
		end = .;
		. += 0xA0000; /* 64kB of heap memory */
		HeapLimit = .;
	} > RAM


}

