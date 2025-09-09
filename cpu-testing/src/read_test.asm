global read_1x1
global read_1x2
global read_2x1
global read_2x2
global read_4x1
global read_4x2
global read_8x2
global read_16x2
global read_32x2

section .text

read_1x1:
    xor rax, rax
    align 64
.loop:
    mov r8b, [rdx]
    add rax, 1
    cmp rax, rcx
    jb .loop
    ret

read_1x2:
    xor rax, rax
    align 64
.loop:
    mov r8b, [rdx]
    mov r8b, [rdx + 1]
    add rax, 2
    cmp rax, rcx
    jb .loop
    ret

read_2x1:
    xor rax, rax
    align 64
.loop:
    mov r8w, [rdx]
    add rax, 2
    cmp rax, rcx
    jb .loop
    ret

read_2x2:
    xor rax, rax
    align 64
.loop:
    mov r8w, [rdx]
    mov r8w, [rdx + 2]
    add rax, 4
    cmp rax, rcx
    jb .loop
    ret

read_4x1:
    xor rax, rax
    align 64
.loop:
    mov r8d, [rdx]
    add rax, 4
    cmp rax, rcx
    jb .loop
    ret

read_4x2:
    xor rax, rax
    align 64
.loop:
    mov r8d, [rdx]
    mov r8d, [rdx + 4]
    add rax, 8
    cmp rax, rcx
    jb .loop
    ret

read_8x2:
    xor rax, rax
    align 64
.loop:
    mov r8, [rdx]
    mov r8, [rdx + 8]
    add rax, 16
    cmp rax, rcx
    jb .loop
    ret

; 128-bit wide register
read_16x2:
    xor rax, rax
    align 64
.loop:
    movups xmm0, [rdx]
    movups xmm0, [rdx + 16]
    add rax, 32
    cmp rax, rcx
    jb .loop
    ret

; 256-bit wide register (AVX2)
read_32x2:
    xor rax, rax
    align 64
.loop:
    vmovups ymm0, [rdx]
    vmovups ymm0, [rdx + 32]
    add rax, 64
    cmp rax, rcx
    jb .loop
    ret
