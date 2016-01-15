/* Define the read_ebp label so that it can be called from other files (.c or .S) */
.global read_ebp

read_ebp:
	pushl %ebp
	movl %esp, %ebp
	
		movl %ebp,%eax

	popl %ebp
	ret
