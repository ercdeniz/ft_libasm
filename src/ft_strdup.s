section .text
global ft_strdup
extern malloc
extern ft_strlen
extern ft_strcpy
extern __errno_location

ft_strdup:
    test rdi, rdi
    jz .null_input
    push rdi
    call ft_strlen
    inc rax
    mov rdi, rax
    call malloc wrt ..plt
    test rax, rax
    jz .malloc_failed 
    mov rdi, rax
    pop rsi
    call ft_strcpy
    ret

.null_input:
    call __errno_location wrt ..plt
    mov byte [rax], 22
    xor rax, rax
    ret

.malloc_failed:
    pop rdi
    xor rax, rax
    ret
