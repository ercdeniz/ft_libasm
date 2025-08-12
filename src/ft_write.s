; ft_write: Write data to a file descriptor.
; Input: rdi - File descriptor
;        rsi - Buffer to write from
;        rdx - Number of bytes to write
; Output: rax - Number of bytes written, or -1 on error

section .text
global ft_write
extern __errno_location

ft_write:
    mov rax, 1              ; sys_write syscall number
    syscall                 ; Make system call
    
    cmp rax, 0              ; Check if syscall failed
    jl .error               ; If negative, handle error
    ret                     ; Return number of bytes written

.error:
    neg rax                 ; Make error code positive
    push rax                ; Save error code
    
    ; PIE-compatible call to __errno_location
    call __errno_location wrt ..plt
    
    pop rdx                 ; Restore error code
    mov [rax], rdx          ; Set errno
    mov rax, -1             ; Return -1
    ret
