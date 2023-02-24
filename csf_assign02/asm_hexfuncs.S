/* Assembly language implementation of hexdump functions */

	.section .rodata

	/* TODO: add read-only data such as strings here */

	.section .text

	/* TODO: add functions here */

	.globl hex_to_printable
hex_to_printable:

	subq $8, %rsp		/* adjust stack pointer */

	cmp $32, %rdi		/* compare with the smallest printable char (space) */
	jb .LInprintable	/* if lower jump to inprintable */
	cmp $126, %rdi		/* compare with the largest printable char '~' */
	ja .LInprintable   	/* of higher jump to inprintable */
	movq  %rdi, %rax	/* if passed both bounds, pass the current char to return*/
	jmp .Lhex_to_printable_Done	/* jump to final step */

.LInprintable:			/* enter inprintable */
	movq $46, %rax 		/* pass '.' into return value */
	jmp .Lhex_to_printable_Done /* jump to final step */

.Lhex_to_printable_Done:
	addq $8, %rsp		/* restore stack pointer */
	ret					/* return */

	.globl hex_read
hex_read:
	subq $8, %rsp		/* adjust stack pointer */

	movq %rdi, %rsi     /* set the second argument of read() to be data_buf */
	movq $0, %rdi		/* set the first argument of read() to be the default input file descriptor */
	movq $16, %rdx		/* set the third argument of read() to be 16 */
	call read			/* call read function to read from file */

	addq $8, %rsp		/* restore stack pointer */
	ret					/* return */

	.globl hex_write_string
hex_write_string:
	subq $8, %rsp		/* adjust stack pointer */
	
	call strLen			/* get the length of the string storing in %rax*/
	movq %rax, %rdx		/* set the third argument of write() to be the length */
	movq %rdi, %rsi		/* set the second argument of write() to be the char array */
	movq $1, %rdi 		/* set the first argument of write() to be the default output file descriptor */			
	call write			/* call write function to output */
	
	addq $8, %rsp		/* restore stack pointer */
	ret					/* return */

	.globl strLen
strLen:
	pushq %r12			/* push %r12 to the stack frame */
	pushq %r13	 		/* push %r13 to the stack frame */
	movq $0, %r13		/* initialize count as 0 */
	jmp .Lwhiletop		/* loop through all characters in the string until reaches '\0' */

.Lwhiletop:
	movb (%rdi, %r11, 1), %r12b /* move %r10 to be the point at the array[count] */

	cmpb $0, %r12b		/* compare the null terminator with the content in array[count] */
	je .LwhileDone		/* loop is done if %10 reaches the null terminator */
	
	incq %r13			/* increment count if %10 has not reaches the null terminator */
	jmp .Lwhiletop		/* repeat the loop */
	
.LwhileDone:
	movq %r13, %rax 	/* move count into the output repository %rax with zero extension */
	popq %r13			/* pop %r13 from stack frame */
	popq %r12			/* pop %r12 from stack frame */
	
	ret					/* return */

	.globl hex_format_offset
hex_format_offset:
	pushq %r12			/* push %r12 onto the stack frame */
	pushq %r13			/* push %r13 onto the stack frame */
	mov $3, %r12 		/* set index to 3 */
	movq %rdi, %r13		/* store the offset into %r14 */
	jmp .LoffsetLoop	/* loop when the index >= 0 */
.LoffsetLoop: 
	cmp $0, %r12		/* compare index with 0 */
	jl .LoffsetDone		/* if index < 0, done*/
	
	movq %r13, %r10		/* store the offset in %r10 for local calculation */
	shrq $24, %r10		/* get the lowest index-th 8 digits */
	movq %r10, %rdi		/* pass the 8 digits to %rdi */
	call hex_format_byte_as_hex	/* convert the 8 digits to 2 chars storing in byteval */
	addq $2, %rsi		/* move byteval pointer 2 bytes forward */
	shlq $8, %r13		/* left shift 8 bits to remove the lowest index-th 8 digits */
	decq %r12			/* decrement index */
	jmp .LoffsetLoop	/* start the loop again */

.LoffsetDone:
	subq $8, %rsi		/* move the byteval pointer back to head */
	popq %r13			/* pop %r13 from stack frame */
	popq %r12			/* pop %r12 from stack frame */
	ret					/* return */

	.globl hex_format_byte_as_hex
hex_format_byte_as_hex:
	subq $8, %rsp			/* adjust stack pointer */
	movzbq %dil, %rdi		/* ensure all previous digits of rdi are 0*/
	movq %rdi, %rax			/* move rdi to rax for division operation*/
	movq $0, %rdx			/* set rdx to 0 for division operation */
	movq $16, %r10			/* move value 16 to r10 register for division operation */
	divq %r10				/* divide the current value in rax register by 16 */

	cmp $10, %rax			/* check the quotient stored in rax register */
	jb .LSmallerThanTenQuotient		/* if smaller than 10 jump */
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
	jb .LSmallerThanTenRemainder	/* if smaller than 10 jump */
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
.globl fillBlank
fillBlank:
	subq $8, %rsp			/* adjust stack pointer */
	movq $0, %r10
	jmp .LfillLoop

.LfillLoop:
	cmpq %rdi, %r10
	jae .Ldone
	movb %sil, (%rdx, %r10, 1)
	addq $1, %r10
	jmp .LfillLoop

.Ldone:
	addq $8, %rsp
	ret
/* vim:ft=gas:
 */
