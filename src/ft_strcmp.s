; ft_strcmp: Compares two strings lexicographically.
; Input: rdi - Pointer to the first string (s1).
;        rsi - Pointer to the second string (s2).
; Output: rax - Negative if s1 < s2, 0 if s1 == s2, positive if s1 > s2.

section .text
global ft_strcmp

ft_strcmp:
    xor rax, rax        ; Clear rax (will be used for return value)
    xor rcx, rcx        ; Clear rcx (index counter)

.compare_loop:
    mov al, [rdi + rcx]     ; Load character from s1
    mov dl, [rsi + rcx]     ; Load character from s2
    
    cmp al, dl              ; Compare characters
    jne .not_equal          ; If not equal, jump to calculate difference
    
    test al, al             ; Check if we reached end of string (null terminator)
    jz .equal               ; If null terminator, strings are equal
    
    inc rcx                 ; Move to next character
    jmp .compare_loop       ; Continue loop

.not_equal:
    sub al, dl              ; Calculate difference (s1[i] - s2[i])
    movsx rax, al           ; Sign-extend to 64-bit (preserve sign)
    ret

.equal:
    xor rax, rax            ; Return 0 (strings are equal)
    ret
