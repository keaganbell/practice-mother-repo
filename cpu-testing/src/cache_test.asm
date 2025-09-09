global cache_read

section .text

; Windows ABI: (rcx, rdx, r8, r9, ?, stack ...)
cache_read:
    xor r9, r9
    mov rax, rdx
    align 64
.loop:
    vmovups ymm0, [rax]
    vmovups ymm0, [rax + 0x20]
    vmovups ymm0, [rax + 0x40]
    vmovups ymm0, [rax + 0x60]
    vmovups ymm0, [rax + 0x80]
    vmovups ymm0, [rax + 0xa0]
    vmovups ymm0, [rax + 0xc0]
    vmovups ymm0, [rax + 0xe0]

    add r9, 256
    and r9, r8

    mov rax, rdx
    add rax, r9

    sub rcx, 256
    jnz .loop
    ret

