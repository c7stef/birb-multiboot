section .data
    idtr dq 0

section .text

global load_idt

; void load_idt(uint32_t gdt, uint16_t size)
load_idt:
    mov ax, [esp + 8]
    dec eax
    mov [idtr], ax
    
    mov eax, [esp + 4]
    mov [idtr + 2], eax

    lidt [idtr]
    ret
