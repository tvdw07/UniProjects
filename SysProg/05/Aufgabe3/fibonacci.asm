; Calculate the recursive Fibonacci sequence with f(0)=0 and f(1)=1.
; SysV AMD64: input n is in rdi, and the 64-bit result is returned in rax.

global main
extern printf

section .data
number: dq 10                     ; example input: calculate f(10)
format: db "%ld", 10, 0           ; printf format: signed long followed by newline

section .text
main:
    push rbp                       ; preserve caller's frame pointer
    mov rbp, rsp                   ; establish main's frame; rsp is now 16-byte aligned

    mov rdi, [rel number]          ; first argument: n = number
    call fibonacci                 ; result f(n) is returned in rax

    ; printf(format, result): format is in rdi, variadic argument in rsi.
    lea rdi, [rel format]          ; rdi = address of the format string
    mov rsi, rax                   ; rsi = long value returned by fibonacci
    xor eax, eax                   ; SysV variadic-call rule: zero vector arguments
    call printf wrt ..plt           ; print result and the newline in format

    xor eax, eax                   ; main returns success (0)
    leave                          ; restore stack pointer and saved frame pointer
    ret                            ; return to the C runtime

; long fibonacci(unsigned long n)
fibonacci:
    push rbp                       ; save the caller's frame pointer
    mov rbp, rsp                   ; use rbp to address this recursive frame
    push rbx                       ; preserve caller's rbx; this frame uses it for n
    sub rsp, 8                     ; reserve local slot and align rsp before calls

    cmp rdi, 1                     ; unsigned n=0 or n=1 is a base case
    jbe .base_case                 ; f(0)=0 and f(1)=1, so return n itself

    mov rbx, rdi                   ; keep n in callee-saved rbx across recursion
    lea rdi, [rbx - 1]             ; prepare first recursive argument n-1
    call fibonacci                 ; compute f(n-1); child may overwrite rdi and rax
    mov [rbp - 16], rax            ; save f(n-1) in this frame's local stack slot

    lea rdi, [rbx - 2]             ; prepare second recursive argument n-2
    call fibonacci                 ; compute f(n-2), returned in rax
    add rax, [rbp - 16]            ; add saved f(n-1): result is f(n-1)+f(n-2)
    jmp .return                    ; both recursive results are now combined

.base_case:
    mov rax, rdi                   ; base-case result f(0)=0 or f(1)=1
.return:
    add rsp, 8                     ; discard the local slot/alignment padding
    pop rbx                        ; restore the caller's original rbx value
    leave                          ; restore this function's caller frame
    ret                            ; return result in rax

section .note.GNU-stack noalloc noexec nowrite progbits ; mark stack non-executable
