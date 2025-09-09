global cache_index_read

section .text

; Windows ABI: (rcx, rdx, r8, r9, stack ...)
cache_index_read:
    align 64
.outer_loop:
    mov r10, r8
    mov rax, rdx

.inner_loop:
    ; read a whole cache line (64 bytes)
    vmovups ymm0, [rax]
    vmovups ymm0, [rax + 0x20]
    add rax, r9 ; advance by stride
    dec r10 ; cache lines per read counter
    jnz .inner_loop

    dec rcx ; read counter
    jnz .outer_loop
    ret
