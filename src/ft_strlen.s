; ft_strlen: Calculates the length of a null-terminated string.
; Input: rdi - Pointer to the string.
; Output: rax - Length of the string.

section .text
global ft_strlen

ft_strlen:
    xor rax, rax

.count_loop:
    cmp byte [rdi + rax], 0
    je .done
    inc rax
    jmp .count_loop

.done:
    ret