OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(start2)

SECTIONS
{
      
	. = 0x4a000000;
	
	.text ALIGN(4) : {
		*(.asm_text2);
		*(EXCLUDE_FILE(*arm_start.o) .text);
		
	}
	
	
	.arm_start ALIGN(4) : {
		__arm_start = .;
		arm_start.o(.text);
		__arm_end = .;	
	}
	
	
	.data ALIGN(4):{
		*.o(.data);
	}
	
	.bss ALIGN(4): {
		*(.bss)
	}

 }


