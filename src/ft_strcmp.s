section .text
global ft_strcmp

ft_strcmp:
    xor rax, rax
    xor rcx, rcx

.compare_loop:
    mov al, [rdi + rcx]
    mov dl, [rsi + rcx]
    
    cmp al, dl
    jne .not_equal

    cmp al, 0
    jz .equal
    
    inc rcx
    jmp .compare_loop

.not_equal:
    sub al, dl
    movsx rax, al
    ret

.equal:
    xor rax, rax
    ret
