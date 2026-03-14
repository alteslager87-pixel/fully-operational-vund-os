;---------------------------------------------------------
; VUND-OS 2.10 GOLD - SP1 BOOTLOADER
; Developer: Yaroslav (Coding God)
; Goal: Initialize System, Check Tracker, Load VUND-CORE
;---------------------------------------------------------

use16
org 0x7C00

start:
    jmp 0:init          ; Обнуляємо сегмент коду

init:
    cli                 ; Вимикаємо переривання на час налаштування
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00      ; Стек росте вниз від початку завантажувача
    sti                 ; Вмикаємо назад

    ; --- ВІЗУАЛІЗАЦІЯ (Команда LOGO) ---
    call clear_screen
    mov si, msg_welcome
    call print_string

    ; --- СЕКРЕТНИЙ ВІДСТЕЖУВАЧ (VUND-TRACKER) ---
    ; Перевіряємо, чи була натиснута таємна клавіша під час завантаження
    mov ah, 01h         ; Перевірка буфера клавіатури
    int 16h
    jz .no_secret
    cmp al, 0x09        ; Якщо натиснуто TAB (твій фірмовий знак)
    jne .no_secret
    mov si, msg_artifact ; Активуємо "Артефакт 2.20"
    call print_string

.no_secret:
    ; --- ПЕРЕХІД ДО ЗАВАНТАЖЕННЯ ЯДРА (VUND-CORE) ---
    ; Тут ми будемо викликати переривання диска, щоб завантажити решту команд
    mov si, msg_loading
    call print_string

    ; [ТУТ БУДЕ КОД ЗАВАНТАЖЕННЯ ЯДРА З ФЛЕШКИ]
    
    hlt                 ; Тимчасова зупинка, поки ми не написали ядро

; --- СИСТЕМНІ ФУНКЦІЇ ---

print_string:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp print_string
.done:
    ret

clear_screen:
    mov ax, 0x0003      ; Текстовий режим 80x25
    int 0x10
    ret

; --- ДАНІ (МАТРИЦЯ) ---
msg_welcome  db 'VUND-OS 2.10 GOLD [SP1] - Initializing...', 13, 10, 0
msg_loading  db 'Loading VUND-CORE and Wi-Fi Stack...', 13, 10, 0
msg_artifact db '[!!!] ARTIFACT 2.20 DETECTED: DOCS UNLOCKED', 13, 10, 0

; СЕКРЕТНИЙ ВІДСТЕЖУВАЧ - ПРИХОВАНИЙ РЯДОК ДЛЯ HEX-РЕДАКТОРІВ
db 'VUND_TRACKER_ACTIVE_X_666' 

times 510 - ($ - $$) db 0
dw 0xAA55               ; Магічна сигнатура