OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(start)

SECTIONS
{
      
	. = 0x00002f8;
	
	.text ALIGN(4) : {
		start.o(.text);
		main.o(.text);
		cpu/int.o(.text);
		*.o(text);
		
	}
	
	
	.arm_start ALIGN(4) : {
		__arm_start = .;
		cpu/arm_start.o(.text);
		__arm_end = .;	
	}
	
	
	.data ALIGN(4):{
		start.o(.data);
		main.o(.data);
		*.o(.data);
		
	}
	
	.bss ALIGN(4): {
	*(.bss)
	}

 }


