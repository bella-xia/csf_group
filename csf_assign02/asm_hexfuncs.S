/* Assembly language implementation of hexdump functions */

	.section .rodata

	/* TODO: add read-only data such as strings here */

	.section .text

	/* TODO: add functions here */

	.globl hex_to_printable
hex_to_printable:

	subq $8, %rsp		/* adjust stack pointer */

	cmp $32, %rdi		/* compare with the smallest printable char (space) */
	jl .LInprintable	/* if lower jump to inprintable */
	cmp $126, %rdi		/* compare with the largest printable char '~' */
	jg .LInprintable   	/* of higher jump to inprintable */
	movq  %rdi, %rax	/* if passed both bounds, pass the current char to return*/
	jmp .Lhex_to_printable_Done	/* jump to final step */

.LInprintable:			/* enter inprintable */
	movq $46, %rax 		/* pass '.' into return value */
	jmp .Lhex_to_printable_Done /* jump to final step */

.Lhex_to_printable_Done:
	addq $8, %rsp		/* restore stack pointer */
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
	subq $8, %rsp			/* adjust stack pointer */
	movzbq %dil, %rdi		/* ensure all previous digits of rdi are 0*/
	movq %rdi, %rax			/* move rdi to rax for division operation*/
	movq $0, %rdx			/* set rdx to 0 for division operation */
	movq $16, %r10			/* move value 16 to r10 register for division operation */
	divq %r10				/* divide the current value in rax register by 16 */

	cmp $10, %rax			/* check the quotient stored in rax register */
	jl .LSmallerThanTenQuotient		/* if smaller than 10 jump */
	movq %rax, %r11			/* move the quotient value to r11 register */
	addq $87, %r11			/* add 87 ('a' - 10) to the r11 register value */
	movb %r11b, 0(%rsi)		/* move the current byte value in r11 into the first index of rsi */
	jmp .LNextDigit         /* jump to the next index */

.LSmallerThanTenQuotient:	/* quotient smaller than 10 */
	movq %rax, %r11			/* move the quotient value to r11 register */
	addq $48, %r11			/* add 48 ('0') to the r11 register value */
	movb %r11b, 0(%rsi)		/* move the current byte value in r11 into the first index of rsi */
	jmp .LNextDigit			/* jump to the next index */

.LNextDigit:
	cmp $10, %rdx			/* check the remainder stored in rdx register */
	jl .LSmallerThanTenRemainder	/* if smaller than 10 jump */
	movq %rdx, %r11			/* move the remainder value to r11 register */
	addq $87, %r11			/* add 87 ('a' - 10) to the r11 register value */
	movb %r11b, 1(%rsi)		/* move the current byte value in r11 into the second index of rsi */
	jmp .LDone         		/* jump to the next index */

.LSmallerThanTenRemainder:	/* remainder smaller than 10 */
	movq %rdx, %r11			/* move the remainder value to r11 register */
	addq $48, %r11			/* add 48 ('0') to the r11 register value */
	movb %r11b, 1(%rsi)		/* move the current byte value in r11 into the second index of rsi */
	jmp .LDone         		/* jump to the next index */

.LDone:
	movb $0, 2(%rsi)		/* set the third index of rsi to null pointer */
	addq $8, %rsp			/* restore stack pointer */
	ret

/* vim:ft=gas:
 */
