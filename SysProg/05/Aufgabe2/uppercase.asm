; Read one line, convert ASCII lowercase letters to uppercase, and print it.
; The System V AMD64 ABI passes the first three fgets arguments in rdi, rsi, rdx.

%define INPUT_CAPACITY 22          ; 20 input characters + optional newline + trailing NUL

global main
extern fgets
extern puts
extern stdin

section .bss
input_buffer: resb INPUT_CAPACITY  ; reserve writable storage; BSS starts zeroed

section .text
main:
    push rbp                       ; save caller's frame pointer
    mov rbp, rsp                   ; establish this function's stack-frame base
    sub rsp, 16                    ; keep rsp 16-byte aligned before each libc call

    ; fgets(input_buffer, INPUT_CAPACITY, stdin)
    lea rdi, [rel input_buffer]    ; rdi = address of destination buffer
    mov esi, INPUT_CAPACITY        ; rsi = maximum buffer size including final NUL
    mov rdx, [rel stdin]           ; rdx = FILE * for standard input
    call fgets wrt ..plt           ; read at most capacity-1 bytes and add NUL

    test rax, rax                  ; fgets returns NULL on EOF or read error
    jz .done                       ; no valid input string, so skip puts

    lea rsi, [rel input_buffer]    ; rsi walks across the bytes in the string
.convert:
    mov al, [rsi]                  ; read current character into low byte of rax
    test al, al                    ; NUL marks the end of the string
    jz .print                      ; keep the terminator and print the result
    cmp al, 10                     ; fgets keeps the newline if it fit in the buffer
    je .terminate                  ; replace newline with NUL so puts adds one newline
    cmp al, 'a'                    ; characters below 'a' are not lowercase letters
    jb .next
    cmp al, 'z'                    ; characters above 'z' are not lowercase letters
    ja .next
    sub byte [rsi], 32             ; ASCII lowercase and uppercase differ by 32
.next:
    inc rsi                        ; advance to the next byte in the input string
    jmp .convert                   ; inspect every byte through the terminator
.terminate:
    mov byte [rsi], 0              ; remove fgets' newline; puts emits its own newline
.print:
    lea rdi, [rel input_buffer]    ; puts expects the string address in rdi
    call puts wrt ..plt             ; write the converted NUL-terminated string
.done:
    xor eax, eax                   ; return status 0 from main
    leave                          ; restore rsp and the caller's saved rbp
    ret                            ; return to the C runtime

section .note.GNU-stack noalloc noexec nowrite progbits ; mark stack non-executable (Ohne diese zeile kommen bei der Assemblierung Warnungen)
