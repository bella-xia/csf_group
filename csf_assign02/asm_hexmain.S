/* Hexdump main function, assembly language version */
	.section .rodata
		ColonFmt: 
			.string ": "
		OneSpace:
			.string " "
		OneEnter:
			.string "\n"
		ThreeSpace: 
			.string "   "

	.section .text

	.globl main
main:
	/* TODO: add your code here */
	subq $72, %rsp			/* allocate space for %rsp */
	pushq %rbp				/* push the callee resgister %rbp to the stack*/
	pushq %r12 				/* push the callee resgister %r12 to the stack*/
	pushq %r13				/* push the callee resgister %r13 to the stack*/
	pushq %r14				/* push the callee resgister %r14 to the stack*/
	pushq %rbx				/* push the callee resgister %rbx to the stack*/
	movq %rsp, %rbp			/* store the original address of %rsp to %rbp */
	movq $16, %rdi			/* set the number of char to fill in function fillBlank as 16 */
	movb $32, %sil			/* set the char to fill in function fillBlank as 32(space) */
	leaq 0(%rbp), %r14 		/* set %r14 to store the address of the data[0] */
	leaq 0(%rbp), %rdx			/* set the char array pointer in function fillBlank to be data[0] */
	call fillBlank			/* call function fillBlank */
	movb $0, 16(%rbp, 1)		/* set data[16] to be the null terminator */

	movq $0, %r12  			/* set place count (%r12) as 0 */

	movq $8, %rdi			/* set the number of char to fill in function fillBlank as 8 */
	movb $48, %sil			/* set the char to fill in function fillBlank as 48('0') */
	leaq 17(%rbp), %rdx		/* set the char array pointer in function fillBlank to be place[0] */
	call fillBlank			/* call function fillBlank */
	movb $0, 25(%rbp, 1)	/* set place[8] to be the null terminator */
	movq %r12, %rdi			/* set placeCount as the unsigned value to be convert to string */
	leaq 17(%rbp), %rsi			/* set the char array pointer place[0] to store the converted string */
	call hex_format_offset	/* call function hex_format_offset */
	leaq 0(%rbp), %rdi			/* set the char array data[0] to store the data read from input */
	call hex_read			/* call function hex_read*/
	movq %rax, %r13 		/* store numDataRead in %r13 */
	jmp .Lloop				/* start the loop to print the hexdump */

.Lloop:
	cmpq $0, %r13			/* check whether numDataRead == 0 */
	jbe .Ldone				/* if numDataRead <= 0, loop break, done */
	leaq 17(%rbp), %rdi			/* move the address of place[0] to be written */
	call hex_write_string	/* call hex_write_string */

	movq $ColonFmt, %rdi	/* move the ": " to be print */
	call hex_write_string	/* call hex_write_string */	
	movb $0, 28(%rbp)		/* assign null terminator to tempStore[2] */
	movq $0, %rbx 			/* set for loop counter i (%rbx) into 0 */
	jmp .LloopWriteCol1		/* start for loop to printout individual hexidecimal character*/

.LloopWriteCol1:			/*enter the loop*/

	cmpq %r13, %rbx			/*compare the number of time of loop to the string len*/
	jae .LfillSpace			/*if at the length jump to the next step*/
	movb (%rbp, %rbx, 1), %dil	/*move one byte of the string into the rdi register*/
	leaq 26(%rbp), %rsi			/* move the empty tempStore array pointer to the rsi register*/
	call hex_format_byte_as_hex		/* call function hex_format_byte_as_hex */
	leaq 26(%rbp), %rdi		/* move the pointer to the tempStore array to rdi register*/
	call hex_write_string	/* call function hex_write_string to print the string */
	movq $OneSpace, %rdi	/* assign the first byte of rdi register the char ' ' (space)*/
	call hex_write_string	/* call function hex_write_string to print the string */

	incq %rbx				/*increment the rbx value by 1*/
	jmp .LloopWriteCol1		/*return to the start of the loop*/

.LfillSpace:
	cmpq $16, %rbx			/*compare the string length to the maximum length 16*/
	jae .Lintersession		/*if the length already 16 then move to the next step*/
	movq $ThreeSpace, %rdi	/*or else move three space to the rdi register */
	call hex_write_string	/* call function hex_write_string to print the string */
	incq %rbx 				/*increment the rbx value by 1*/
	jmp .LfillSpace			/*return to the start of the loop*/

.Lintersession:
	movq $OneSpace, %rdi	/* assign the first byte of rdi register the char ' ' (space)*/
	call hex_write_string	/* call function hex_write_string to print the string */	
	movq $16, %rdi			/* set the number of char to fill in function fillBlank as 16 */
	movb $32, %sil			/* set the char to fill in function fillBlank as 32(space) */
	leaq 29(%rbp), %rdx			/* set the char array pointer in function fillBlank to be dataPrint[0] */
	call fillBlank			/* call function fillBlank*/
	movq $0, 45(%rbp, 1)	/* assign null terminator to dataPrint[16] */
	movq $0, %rbx			/* give rbx register the initial value of 0 (function as iterator i)*/
	leaq 29(%rbp), %r14
	jmp .LloopWriteCol2

.LloopWriteCol2:			/* enter loop */
	cmpq %r13, %rbx			/* compare the current value in rbx (the number of iterations done) and the string length */
	jae .LalmostDone		/* if iterated the number of string length, move to next step*/
	movb (%rbp, %rbx, 1), %dil	/* move data[i] into the rdi register */
	call hex_to_printable	/* call function hex_to_printable*/
	movb %al, (%r14, %rbx, 1)	/* move the result char back to data[i] */
	incq %rbx				/* increment rbx register by 1	*/
	jmp .LloopWriteCol2		/*go back to the start of the loop*/

.LalmostDone:
	movb $0, 29(%rbp, %r13, 1)	/* add null terminator to the end of the string*/
	leaq 29(%rbp), %rdi		/* move the address of rbp (dataPrint start) to rdi register*/
	call hex_write_string	/* call function hex_write_string to print the string */ 	
	leaq (%rbp), %rdi		/* move the address for char array data into rdi register*/
	call hex_read 			/* call function hex_read*/
	movq %rax, %r13			/* move the string length read into variable numDataRead*/
	addq $16, %r12			/* increment placeCount by 16*/
	movq %r12, %rdi			/* move placeCount to the first parameter rdi register*/
	leaq 17(%rbp), %rsi			/* move the pointer to placecount in the second parameter rsi register*/
	call hex_format_offset	/* call function hex_format_offset*/
	movq $OneEnter, %rdi
	call hex_write_string	/* call function hex_write_string*/
	jmp .Lloop				/* jump back to the start of the while loop*/

.Ldone:
	movq $0, %rax			/* assign return value to 0*/
	popq %rbx				/* pop back register rbx*/
	popq %r14				/* pop back register r14*/
	popq %r13				/* pop back register r13*/
	popq %r12				/* pop back register r12*/
	popq %rbp				/* pop back register rbp*/
	addq $72, %rsp			/* add 72 back to the rsp register*/
	ret						/* return*/


/* vim:ft=gas:
 */
