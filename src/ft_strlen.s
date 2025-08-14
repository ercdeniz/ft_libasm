section .text
global ft_strlen

ft_strlen:
    mov rax, rdi
    mov rcx, -1
    xor al, al
    repne scasb
    not rcx
    dec rcx
    mov rax, rcx
    ret

; This is simple version that I don't use rep
;ft_strlen:
;    xor rax, rax
;
;.count_loop:
;    cmp byte [rdi + rax], 0
;    je .done
;    inc rax
;    jmp .count_loop
;
;.done:
;    ret