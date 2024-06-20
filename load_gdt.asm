section .data
    gdtr dq 0

section .text

global load_gdt

; void load_gdt(uint32_t gdt, uint16_t size)
load_gdt:
    mov ax, [esp + 8]
    dec eax
    mov [gdtr], ax
    
    mov eax, [esp + 4]
    mov [gdtr + 2], eax

    lgdt [gdtr]
    call reload_segments
    ret

reload_segments:
    jmp 0x08:.reload_CS
.reload_CS:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    mov ss, ax
    ret
