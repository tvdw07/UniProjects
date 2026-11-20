%define INPUT_CAPACITY 22          ; 20 zeichen + zeilenumbruch + nullterminator

global main
extern fgets
extern puts
extern stdin

section .bss
input_buffer: resb INPUT_CAPACITY

section .text
main:
    push rbp
    mov rbp, rsp
    sub rsp, 16                    ; stackalignment für funktionsaufrufe

    ; Eingabe über fgets einlesen
    lea rdi, [rel input_buffer]
    mov esi, INPUT_CAPACITY
    mov rdx, [rel stdin]
    call fgets wrt ..plt

    test rax, rax                  ; prüfen ob einlesen erfolgreich war
    jz .done

    lea rsi, [rel input_buffer]

.convert:
    mov al, [rsi]
    test al, al                    ; ende des strings erreicht?
    jz .print
    cmp al, 10                     ; zeilenumbruch entfernen
    je .terminate

    ; prüfen ob zeichen zwischen 'a' und 'z' liegt
    cmp al, 'a'
    jb .next
    cmp al, 'z'
    ja .next
    sub byte [rsi], 32             ; kleinbuchstaben in großbuchstaben umwandeln

.next:
    inc rsi
    jmp .convert

.terminate:
    mov byte [rsi], 0              ; zeilenumbruch durch nulterminator ersetzen

.print:
    lea rdi, [rel input_buffer]
    call puts wrt ..plt

.done:
    xor eax, eax
    leave
    ret

; verhindert warnungen (macht den stack nicht ausführbar)
section .note.GNU-stack noalloc noexec nowrite progbits