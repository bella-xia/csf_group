/* Assembly language implementation of hexdump functions */

	.section .rodata

	/* TODO: add read-only data such as strings here */

	.section .text

	/* TODO: add functions here */

	.globl hex_to_printable
hex_to_printable:

	subq $8, %rsp

	cmpb $32, (%rdi)
	jl .LInprintable
	cmpb $126, (%rdi)
	jg .LInprintable
	movq  (%rdi), %rax
	jmp .Lhex_to_printable_Done

.LInprintable:
	movq $46, %rax 
	jmp .Lhex_to_printable_Done

.Lhex_to_printable_Done:
	addq $8, %rsp
	ret

	.globl hex_read
hex_read:
	subq $8, %rsp
	addq $8, %rsp
	ret

	.globl hex_write_string
hex_write_string:
	subq $8, %rsp
	addq $8, %rsp
	ret

	.globl hex_format_offset
hex_format_offset:
	subq $8, %rsp
	addq $8, %rsp
	ret

	.globl hex_format_byte_as_hex
hex_format_byte_as_hex:
	subq $8, %rsp
	addq $8, %rsp
	ret

/* vim:ft=gas:
 */
