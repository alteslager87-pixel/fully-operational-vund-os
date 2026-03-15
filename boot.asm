format binary as 'bin'
org 0x7C00

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    ; Завантажуємо ядро (CORE.BIN) з диска
    mov ah, 0x02            ; Функція читання
    mov al, 20              ; Читаємо 20 секторів (вистачить для великого ядра)
    mov ch, 0               ; Циліндр 0
    mov cl, 2               ; Починаємо з 2-го сектора (ядро лежить відразу за бутлоадером)
    mov dh, 0               ; Головка 0
    mov dl, 0x80            ; Перший жорсткий диск (або флешка)
    mov bx, 0x1000          ; Куди вантажимо (0x1000:0000)
    mov es, bx
    xor bx, bx
    int 0x13
    jc disk_error

    ; Стрибок у ядро
    jmp 0x1000:0000

disk_error:
    mov si, msg_err
    call print
    jmp $

print:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp print
.done: ret

msg_err db 'BOOT ERROR: Kernel not found!', 0

times 510-($-$$) db 0
dw 0xAA55                   ; Сигнатура завантажувача