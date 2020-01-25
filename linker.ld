/* Designate a symbol as the entry point for execution. */
ENTRY(_start)
 
/* Specify section locations. */
SECTIONS
{
	/* Put the sections at 1MiB. */
	. = 1M;
 
	/* Multiboot header followed by the .text section */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}
 
	/* Read only data. */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}
 
	/* Initialized read/write data */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}
 
	/* Uninitialized read/write data */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON) /* TODO: Figure out what this is. */
		*(.bss)
	}
 
	/* Add more stuff here if necessary. */
}
