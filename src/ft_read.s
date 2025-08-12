; ft_read: Read data from a file descriptor.
; Input: rdi - File descriptor
;        rsi - Buffer to read into
;        rdx - Number of bytes to read
; Output: rax - Number of bytes read, or -1 on error

section .text
global ft_read
extern __errno_location

ft_read:
    mov rax, 0              ; sys_read syscall number
    syscall                 ; Make system call
    
    cmp rax, 0              ; Check if syscall failed
    jl .error               ; If negative, handle error
    ret                     ; Return number of bytes read

.error:
    neg rax                 ; Make error code positive
    push rax                ; Save error code
    
    ; PIE-compatible call to __errno_location
    call __errno_location wrt ..plt
    
    pop rdx                 ; Restore error code
    mov [rax], rdx          ; Set errno
    mov rax, -1             ; Return -1
    ret
