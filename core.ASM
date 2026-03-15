format binary as 'bin'
org 0x0000

start:
    mov ax, cs
    mov ds, ax
    mov es, ax
    
    call init_vga
    mov si, logo_data
    call print_string
    mov si, welcome_msg
    call print_string

; ====================================================================
; ?? ÃÎËÎÂÍÈÉ ÖÈÊË (SHELL)
; ====================================================================
shell_loop:
    mov si, prompt
    call print_string
    call get_input
    
    mov si, input_buffer
    cmp byte [si], 0        
    je shell_loop

    call execute_command
    jmp shell_loop

; ====================================================================
; ?? ²ÍÒÅÐÏÐÅÒÀÒÎÐ (ÁÅÇ ÇÀÉÂÈÕ ÑÈÌÂÎË²Â)
; ====================================================================
execute_command:
    mov si, input_buffer
    
    ; Ñèñòåìí³ êîìàíäè
    mov di, cmd_help
    call compare_cmd
    je do_help

    mov di, cmd_info
    call compare_cmd
    je do_info

    mov di, cmd_logo
    call compare_cmd
    je do_logo

    mov di, cmd_tasks
    call compare_cmd
    je do_tasks

    mov di, cmd_clear
    call compare_cmd
    je do_clear

    mov di, cmd_exit
    call compare_cmd
    je do_exit
    
    ; Ôàéëîâ³ êîìàíäè
    mov di, cmd_ls
    call compare_cmd
    je do_ls

    mov di, cmd_mkdir
    call compare_cmd
    je do_mkdir

    mov di, cmd_tree
    call compare_cmd
    je do_tree

    mov di, cmd_rm
    call compare_cmd
    je do_rm
    
    ; Ãðàô³êà òà Ïàêåòè
    mov di, cmd_image
    call compare_cmd
    je do_image

    mov di, cmd_install
    call compare_cmd
    je do_install
    
    ; Ðîçðîáêà òà Çàë³çî
    mov di, cmd_mon
    call compare_cmd
    je do_monitor

    mov di, cmd_dsk
    call compare_cmd
    je do_dsk
    
    ; Áåçïåêà
    mov di, cmd_pswd
    call compare_cmd
    je do_pswd

    mov di, cmd_bypass
    call compare_cmd
    je do_bypass
    
    ; Ìåðåæà
    mov di, cmd_connect
    call compare_cmd
    je do_connect

    mov di, cmd_vmail
    call compare_cmd
    je do_vmail

    mov di, cmd_netstat
    call compare_cmd
    je do_netstat

    mov si, err_unknown
    call print_string
    ret

; ====================================================================
; ??? ÎÁÐÎÁÍÈÊÈ (HANDLERS)
; ====================================================================
do_help:
    mov si, msg_help
    call print_string
    ret

do_info:
    mov si, msg_info
    call print_string
    ret

do_logo:
    mov si, logo_data
    call print_string
    ret

do_ls:
    mov si, msg_ls
    call print_string
    ret

do_mkdir:
    mov si, msg_mkdir
    call print_string
    ret

do_tree:
    mov si, msg_tree
    call print_string
    ret

do_rm:
    mov si, msg_rm
    call print_string
    ret

do_tasks:
    mov si, msg_tasks
    call print_string
    ret

do_install:
    mov si, msg_install
    call print_string
    ret

do_pswd:
    mov si, msg_pswd
    call print_string
    ret

do_dsk:
    mov si, msg_dsk
    call print_string
    ret

do_bypass:
    mov si, msg_bypass
    call print_string
    ret

do_connect:
    mov si, msg_connect
    call print_string
    ret

do_vmail:
    mov si, msg_vmail
    call print_string
    ret

do_netstat:
    mov si, msg_netstat
    call print_string
    ret

do_exit:
    mov si, msg_exit
    call print_string
    cli
    hlt

do_clear:
    call init_vga
    ret

do_image:
    mov ax, 0x0013
    int 0x10
    mov ax, 0xA000
    mov es, ax
    mov di, 320*100+140
    mov al, 15
    mov cx, 40
    rep stosb
    mov ah, 0x00
    int 0x16
    call init_vga
    ret

do_monitor:
    mov si, 0x0000
    mov cx, 8
.m_loop: 
    lodsb
    call print_hex_simple
    loop .m_loop
    ret

; ====================================================================
; ?? ÃÀÐß×² ÊËÀÂ²Ø² ÒÀ ÂÂ²Ä
; ====================================================================
get_input:
    mov di, input_buffer
    xor cx, cx
.key_loop:
    mov ah, 0x00
    int 0x16
    
    cmp al, 0x02
    je .hot_b
    cmp al, 0x13
    je .hot_s
    cmp al, 0x0C
    je .hot_l
    
    test al, al
    jz .check_f8

    cmp al, 0x0D
    je .done
    cmp al, 0x08
    je .backspace
    
    stosb
    mov ah, 0x0E
    int 0x10
    inc cx
    jmp .key_loop

.backspace:
    or cx, cx
    jz .key_loop
    dec di
    dec cx
    mov ah, 0x0E
    mov al, 0x08
    int 0x10
    mov al, ' '
    int 0x10
    mov al, 0x08
    int 0x10
    jmp .key_loop

.check_f8:
    cmp ah, 0x42
    je .hot_f8
    jmp .key_loop

.hot_b:
    mov si, hot_b_msg
    call print_string
    jmp .done
.hot_s:
    mov si, hot_s_msg
    call print_string
    jmp .done
.hot_l:
    mov si, hot_l_msg
    call print_string
    jmp .done
.hot_f8:
    mov si, hot_f8_msg
    call print_string
    jmp .done

.done:
    mov byte [di], 0
    mov si, newline
    call print_string
    ret

; ====================================================================
; ?? ÑÈÑÒÅÌÍ² Ï²ÄÏÐÎÃÐÀÌÈ
; ====================================================================
print_string:
    lodsb
    or al, al
    jz .e
    mov ah, 0x0E
    int 0x10
    jmp print_string
.e:
    ret

print_hex_simple:
    push ax
    mov al, '0'
    mov ah, 0x0E
    int 0x10
    mov al, 'x'
    int 0x10
    pop ax
    ret

compare_cmd:
    push si
    push di
.c:
    mov al, [si]
    mov bl, [di]
    cmp al, bl
    jne .n
    or al, al
    jz .y
    inc si
    inc di
    jmp .c
.n:
    pop di
    pop si
    clc
    ret
.y:
    pop di
    pop si
    add sp, 0
    ret

init_vga:
    mov ax, 0x0003
    int 0x10
    ret

; ====================================================================
; ?? ÑÅÊÖ²ß ÄÀÍÈÕ
; ====================================================================
logo_data   db '___ VUND-DOS GOLD MONOLITH ___', 13, 10, 0
welcome_msg db 'System active. Hello, Yaroslav!', 13, 10, 0
prompt      db 13, 10, 'VUND-DOS > ', 0
newline     db 13, 10, 0
err_unknown db 'Unknown command.', 0

cmd_help    db 'help', 0
cmd_info    db 'system_info', 0
cmd_logo    db 'logo', 0
cmd_ls      db 'ls', 0
cmd_mkdir   db 'createfolder', 0
cmd_tree    db 'tree', 0
cmd_image   db 'createimage', 0
cmd_install db 'install_app', 0
cmd_mon     db 'monitor', 0
cmd_pswd    db 'pswd', 0
cmd_dsk     db 'goto dsk', 0
cmd_bypass  db 'goto user-test', 0
cmd_clear   db 'clear', 0
cmd_exit    db 'exit', 0
cmd_connect db 'connect', 0
cmd_vmail   db 'vmail', 0
cmd_netstat db 'net_stat', 0
cmd_rm      db 'rm', 0
cmd_tasks   db 'tasks', 0

msg_help    db 'Commands: ls, help, system_info, logo, createfolder, tree, createimage, install_app, monitor, pswd, goto dsk, goto user-test, clear, exit, connect, vmail, net_stat, rm, tasks', 0
msg_info    db 'VUND-DOS Gold v2.00. Architect: Yaroslav (Coding God).', 0
msg_ls      db 'VUND_SYSTEM_ROOT: /SYSTEM, /USER, /DRIVERS, vmail.dot', 0
msg_mkdir   db 'Success: Directory created.', 0
msg_tree    db 'VUND_ROOT |--SYSTEM |--USER', 0
msg_rm      db 'Object removed (protected mode bypass).', 0
msg_tasks   db 'Tasks: [0] Kernel [1] VUND-AIR [2] Monitor [3] Shell', 0
msg_install db 'Installing: Extracting packages... Done.', 0
msg_pswd    db 'Security: Changing access key...', 0
msg_dsk     db 'Hardware: IDE Logical Drive switched.', 0
msg_bypass  db 'DEV_BYPASS: Welcome, Yaroslav.', 0
msg_exit    db 'System Shutdown...', 0
msg_connect db 'VUND-AIR: Handshake with Wi-Fi... OK.', 0
msg_vmail   db 'VMAIL: [1] New message from SYSTEM.', 0
msg_netstat db 'Signal: 100% | IP: 192.168.1.1', 0

hot_b_msg   db ' [HOTKEY: Browser Launcher...] ', 0
hot_s_msg   db ' [HOTKEY: Fast Disk Sync...] ', 0
hot_l_msg   db ' [HOTKEY: System Locked] ', 0
hot_f8_msg  db ' [RESTORING SHIELD...] ', 0

input_buffer rb 64