OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(start)

SECTIONS
{
      
	. = 0x00002f8;
	
	.text ALIGN(4) : {
		start.o(.text);
		/*main.o(.text);*/
		*.o(text);
		
	}
			
	.data ALIGN(4):{
		start.o(.data);
		main.o(.data);
		*.o(.data);
		
	}

/*__bss_start__=.;*/
	.bss ALIGN(4): {
	*(.bss)
	}
/*__bss_end__=.;*/
	

}

