section .text
	global	main
main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 32
	mov		rdx, 0
	mov		[rbp-8], rdx
	mov		rdx, 0
	mov		[rbp-16], rdx
.L5:
	mov		rdx, [rbp-8]
	mov		rcx, 5
	cmp		rdx, rcx
	je		.L105
.L7:
	mov		rdx, [rbp-8]
	mov		rcx, 2
	cmp		rdx, rcx
	jl		.L107

	push	rdi
	push	rax
	push	rsi

	mov		rdi, formInt
	mov		rax, [rbp-8]
	mov		rsi, rax
	xor		rax, rax
	call	printf
	pop		rsi
	pop		rax
	pop		rdi

.L107:
	mov		rdx, [rbp-8]
	mov		rcx, 1
	add		rdx, rcx
	mov		[rbp-8], rdx
	jmp		.L5
.L105:

	push	rdi
	push	rax
	push	rsi

	mov		rdi, formInt
	mov		rax, [rbp-8]
	mov		rsi, rax
	xor		rax, rax
	call	printf
	pop		rsi
	pop		rax
	pop		rdi

	add		rsp, 32
	pop		rbp
	mov		rax, 1
	xor		rbx, rbx
	int		0x80

	extern	printf
	extern	scanf

section .data
	formString db "%s", 10, 0
	formInt db "%d", 10, 0
	formInInt db "%d", 0
	bufInInt dq 0
