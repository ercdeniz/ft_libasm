; ft_strdup: Duplicate a string by allocating memory and copying.
; Input: rdi - Pointer to the string to duplicate
; Output: rax - Pointer to the duplicated string, or NULL on error

section .text
global ft_strdup
extern malloc
extern ft_strlen
extern ft_strcpy
extern __errno_location

ft_strdup:
    test rdi, rdi           ; Check if input string is NULL
    jz .null_input          ; If NULL, return NULL
    
    push rdi                ; Save original string pointer
    
    ; PIE-compatible call to ft_strlen
    call ft_strlen wrt ..plt
    
    inc rax                 ; Add 1 for null terminator
    
    mov rdi, rax            ; Set size for malloc
    
    ; PIE-compatible call to malloc
    call malloc wrt ..plt
    
    test rax, rax           ; Check if malloc failed
    jz .malloc_failed       ; If NULL, handle error
    
    mov rdi, rax            ; Destination (newly allocated memory)
    pop rsi                 ; Source (original string)
    push rax                ; Save allocated pointer
    
    ; PIE-compatible call to ft_strcpy
    call ft_strcpy wrt ..plt
    
    pop rax                 ; Restore allocated pointer
    ret                     ; Return pointer to duplicated string

.null_input:
    ; PIE-compatible call to __errno_location
    call __errno_location wrt ..plt
    
    mov dword [rax], 22     ; Set errno to EINVAL (22)
    xor rax, rax            ; Return NULL
    ret

.malloc_failed:
    pop rdi                 ; Clean up stack (remove saved string pointer)
    ; errno is already set by malloc
    xor rax, rax            ; Return NULL
    ret
