/* Hexdump main function, assembly language version */
	.section .rodata
		ColonFmt: 
			.string ": "
		ThreeSpace: 
			.string "   "
		IntFmt:
			.string "sdahsodah%d\n"

	.section .text

	.globl main
main:
	/* TODO: add your code here */
	pushq %rbp				/* push the callee resgister %rbp to the stack*/
	pushq %r12 				/* push the callee resgister %r12 to the stack*/
	pushq %r13				/* push the callee resgister %r13 to the stack*/
	pushq %r14				/* push the callee resgister %r14 to the stack*/
	pushq %r15				/* push the callee resgister %r15 to the stack*/
	pushq %rbx				/* push the callee resgister %rbx to the stack*/
	movq %rsp, %rbp			/* store the original address of %rsp to %rbp */
	subq $64, %rbp			/* allocate 64 bytes space */
	subq $72, %rsp			/* allocate space for %rsp */
	movq $16, %rdi			/* set the number of char to fill in function fillBlank as 16 */
	movq $32, %rsi			/* set the char to fill in function fillBlank as 32(space) */
	movq %rbp, %r14 		/* set %r14 to store the address of the data[0] */
	movq %rbp, %rdx			/* set the char array pointer in function fillBlank to be data[0] */
	call fillBlank			/* call function fillBlank */
	addq $16, %rbp			/* move %rbp to the data[16] */
	movb $0, (%rbp)			/* set data[16] to be the null terminator */
	addq $1, %rbp			/* move %rbp to the next empty space */

	movq $0, %r12  			/* set place count (%r12) as 0 */

	movq $8, %rdi			/* set the number of char to fill in function fillBlank as 8 */
	movq $48, %rsi			/* set the char to fill in function fillBlank as 48('0') */
	movq %rbp, %r15 		/* set %r15 to store the address of the place[0] */
	movq %rbp, %rdx			/* set the char array pointer in function fillBlank to be place[0] */
	call fillBlank			/* call function fillBlank */
	addq $8, %rbp			/* move %rbp to the place[8] */
	movb $0, (%rbp)			/* set place[8] to be the null terminator */
	addq $1, %rbp			/* move %rbp to the next empty space */

	movq %r12, %rdi			/* set placeCount as the unsigned value to be convert to string */
	movq %r15, %rsi			/* set the char array pointer place[0] to store the converted string */
	call hex_format_offset	/* call function hex_format_offset */
	movq %r14, %rdi			/* set the char array data[0] to store the data read from input */
	call hex_read			/* call function hex_read*/
	movq %rax, %r13 		/* store numDataRead in %r13 */
	jmp .Lloop				/* start the loop to print the hexdump */

.Lloop:
	cmpq $0, %r13			/* check whether numDataRead == 0 */
	jbe .Ldone				/* if numDataRead <= 0, loop break, done */
	movq %r15, %rdi			/* move the address of place[0] to be written */
	call hex_write_string	/* call hex_write_string */

	movq $ColonFmt, %rdi	/* move the ": " to be print */
	call hex_write_string	/* call hex_write_string */	
	movb $0, 2(%rbp)		/* assign null terminator to tempStore[2] */
	movq $0, %rbx 			/* set for loop counter i (%rbx) into 0 */
	jmp .LloopWriteCol1		/* start for loop to printout individual hexidecimal character*/

.LloopWriteCol1:

	cmpq %r13, %rbx			
	jae .LfillSpace
	movb (%r14, %rbx, 1), %dil
	movq %rbp, %rsi
	call hex_format_byte_as_hex
	movq %rbp, %rdi
	call hex_write_string
	movb $32, (%rdi)
	movb $0, 1(%rdi)
	call hex_write_string



	incq %rbx
	jmp .LloopWriteCol1

.LfillSpace:
	cmpq $16, %rbx
	jae .Lintersession
	movq $ThreeSpace, %rdi
	call hex_write_string
	incq %rbx 
	jmp .LfillSpace

.Lintersession:
	addq $3, %rbp
	movb $32, (%rdi)
	movb $0, 1(%rdi)
	call hex_write_string
	movq $16, %rdi
	movq $32, %rsi
	movq %rbp, %rdx		/* %rbp is dataPrint */
	call fillBlank
	movq $0, 16(%rbp)
	movq $0, %rbx
	jmp .LloopWriteCol2

.LloopWriteCol2:
	cmpq %r13, %rbx
	jae .LalmostDone
	movb (%r14, %rbx, 1), %dil
	call hex_to_printable
	movb %al, (%rbp, %rbx, 1)
	incq %rbx
	jmp .LloopWriteCol2

.LalmostDone:
	movq %rbp, %rdi
	call hex_write_string
	movq %r14, %rdi
	call hex_read
	movq %rax, %r13
	addq $16, %r12
	movq %r12, %rdi
	movq %r15, %rsi
	call hex_format_offset
	movb $10, (%rdi)
	movb $0, 1(%rdi)
	call hex_write_string
	jmp .Lloop

.Ldone:
	addq $72, %rsp
	popq %rbx
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbp

	ret


/* vim:ft=gas:
 */
