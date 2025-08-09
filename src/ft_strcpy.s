; ft_strcpy: Copies a string from src to dst
; Input: rdi = dst, rsi = src  
; Output: rax = dst

section .text
global ft_strcpy

ft_strcpy:
    push rdi
    
.copy_loop:
    mov al, [rsi]
    mov [rdi], al
    inc rsi
    inc rdi
    test al, al
    jnz .copy_loop

    pop rax
    ret
