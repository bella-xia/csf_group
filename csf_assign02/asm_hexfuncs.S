/* Assembly language implementation of hexdump functions */

	.section .rodata

	/* TODO: add read-only data such as strings here */

	.section .text

	/* TODO: add functions here */

	.globl hex_to_printable
hex_to_printable:

	subq $8, %rsp

	cmp $32, %rdi
	jl .LInprintable
	cmp $126, %rdi
	jg .LInprintable
	movq  %rdi, %rax
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
	movzbq %dil, %rdi
	movq %rdi, %rax
	movq $0, %rdx
	movq $16, %r10
	divq %r10

	cmp $10, %rax
	jl .LSmallerThanTenQuotient
	movq %rax, %r11
	addq $87, %r11
	movb %r11b, 0(%rsi)
	jmp .LNextDigit

.LSmallerThanTenQuotient:
	movq %rax, %r11
	addq $48, %r11
	movb %r11b, 0(%rsi)
	jmp .LNextDigit

.LNextDigit:
	cmp $10, %rdx
	jl .LSmallerThanTenRemainder
	movq %rdx, %r11
	addq $87, %r11
	movb %r11b, 1(%rsi)
	jmp .LDone

.LSmallerThanTenRemainder:
	movq %rdx, %r11
	addq $48, %r11
	movb %r11b, 1(%rsi)
	jmp .LDone

.LDone:
	movb $0, 2(%rsi)
	addq $8, %rsp
	ret

/* vim:ft=gas:
 */
