global aligned_read_32x8

section .text

; Windows ABI: (rcx, rdx, r8, r9, ?, stack ...)
aligned_read_32x8:
    xor rax, rax
    align 64
.loop:
    vmovups ymm0, [rdx]
    vmovups ymm0, [rdx + 0x20]
    vmovups ymm0, [rdx + 0x40]
    vmovups ymm0, [rdx + 0x60]
    vmovups ymm0, [rdx + 0x80]
    vmovups ymm0, [rdx + 0xa0]
    vmovups ymm0, [rdx + 0xc0]
    vmovups ymm0, [rdx + 0xe0]

    add rax, 256

    sub rcx, 256
    jnz .loop
    ret

