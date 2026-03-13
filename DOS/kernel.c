#include "config.h"

// ==========================================
// 1. БАЗОВІ ТИПИ ТА КОНСТАНТИ
// ==========================================
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

#define VIDEO_MEM      ((uint16_t*)0xB8000)
#define MAX_COLS       80
#define MAX_ROWS       25
#define CMD_SIZE       128

// ==========================================
// 2. ДОСТУП ДО ЗАЛІЗА (I/O PORTS)
// ==========================================
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

static inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ( "inw %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

// ==========================================
// 3. ДРАЙВЕР ЕКРАНУ (VGA TEXT MODE)
// ==========================================
int cursor_x = 0;
int cursor_y = 0;

void clear_screen() {
    for (int i = 0; i < MAX_COLS * MAX_ROWS; i++) VIDEO_MEM[i] = (uint16_t)0x0F00 | ' ';
    cursor_x = 0; cursor_y = 0;
}

void kprint_char(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\b') { // Backspace
        if (cursor_x > 0) {
            cursor_x--;
            VIDEO_MEM[cursor_y * MAX_COLS + cursor_x] = (uint16_t)0x0F00 | ' ';
        }
    } else {
        VIDEO_MEM[cursor_y * MAX_COLS + cursor_x] = (uint16_t)0x0F00 | c;
        cursor_x++;
    }
    
    // Авто-скрол екрану
    if (cursor_x >= MAX_COLS) { cursor_x = 0; cursor_y++; }
    if (cursor_y >= MAX_ROWS) {
        for (int i = 0; i < MAX_COLS * (MAX_ROWS - 1); i++) {
            VIDEO_MEM[i] = VIDEO_MEM[i + MAX_COLS];
        }
        for (int i = MAX_COLS * (MAX_ROWS - 1); i < MAX_COLS * MAX_ROWS; i++) {
            VIDEO_MEM[i] = (uint16_t)0x0F00 | ' ';
        }
        cursor_y = MAX_ROWS - 1;
    }
}

void kprint(char* str) {
    for (int i = 0; str[i] != '\0'; i++) kprint_char(str[i]);
}

// Вивід чисел для відсотків
void kprint_int(int num) {
    char str[10];
    int i = 0;
    if (num == 0) { kprint_char('0'); return; }
    while (num > 0) {
        str[i++] = (num % 10) + '0';
        num /= 10;
    }
    while (--i >= 0) kprint_char(str[i]);
}

// ==========================================
// 4. ДРАЙВЕР КЛАВІАТУРИ (З БУФЕРОМ)
// ==========================================
char scancode_to_char(uint8_t scancode) {
    static const char map[] = {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
        '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
    };
    if (scancode < sizeof(map)) return map[scancode];
    return 0;
}

void kinput(char* buffer) {
    int i = 0;
    while (1) {
        if (inb(0x64) & 1) { // Якщо є дані від клави
            uint8_t scancode = inb(0x60);
            if (scancode & 0x80) continue; // Ігноруємо відпускання клавіші
            
            char c = scancode_to_char(scancode);
            if (c == '\n') { // Enter
                buffer[i] = '\0';
                kprint_char('\n');
                break;
            } else if (c == '\b') { // Backspace
                if (i > 0) {
                    i--;
                    kprint_char('\b');
                }
            } else if (c && i < CMD_SIZE - 1) { // Звичайний символ
                buffer[i++] = c;
                kprint_char(c);
            }
        }
    }
}

// ==========================================
// 5. IDE DRIVER (ХАРДКОРНИЙ ДОСТУП ДО ДИСКУ)
// ==========================================
void ide_wait_ready() {
    while ((inb(0x1F7) & 0x80) == 0x80); // Чекаємо BSY (зайнято)
    while ((inb(0x1F7) & 0x21) != 0x08); // Чекаємо DRQ (готовність)
}

// ==========================================
// 6. УТИЛІТИ ДЛЯ РЯДКІВ (Заміна string.h)
// ==========================================
int str_eq(char* s1, char* s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return (*(uint8_t*)s1 - *(uint8_t*)s2) == 0;
}

void to_upper(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') str[i] -= 32;
    }
}

// ==========================================
// 7. KIM-1 МОНІТОР ПАМ'ЯТІ
// ==========================================
void kprint_hex(uint8_t n) {
    char* hex = "0123456789ABCDEF";
    kprint_char(hex[(n >> 4) & 0x0F]);
    kprint_char(hex[n & 0x0F]);
}

void cmd_monitor() {
    kprint("\n--- KIM-1 HEX DUMP (ADDR: 0x1000) ---\n");
    uint8_t* ptr = (uint8_t*)0x1000;
    for (int i = 0; i < 64; i++) {
        kprint_hex(ptr[i]); kprint_char(' ');
        if ((i + 1) % 8 == 0) kprint_char('\n');
    }
    kprint("--------------------------------------\n");
}

// ==========================================
// 8. ЛОГІКА VUND KERNEL 2.0 (PYTHON -> C)
// ==========================================
void print_logo() {
    kprint("\n");
    kprint("   __      __ _    _  _   _  _____  \n");
    kprint("   \\ \\    / /| |  | || \\ | ||  __ \\ \n");
    kprint("    \\ \\  / / | |  | ||  \\| || |  | |\n");
    kprint("     \\ \\/ /  | |  | || . ` || |  | |\n");
    kprint("      \\  /   | |__| || |\\  || |__| |\n");
    kprint("       \\/     \\____/ |_| \\_||_____/ \n");
    kprint("            SYSTEM v2.00 GOLD       \n\n");
}

void boot_animation() {
    for (int i = 0; i <= 20; i++) {
        clear_screen();
        kprint("\n\n           ----------------------------------------\n");
        print_logo();
        
        kprint("          [");
        for (int j = 0; j < i; j++) kprint_char(219); // Блок █ (кодування DOS)
        for (int j = 0; j < (20 - i); j++) kprint_char(176); // Тінь ░
        kprint("] ");
        kprint_int(i * 5); kprint("%\n");
        
        for (volatile int wait = 0; wait < 10000000; wait++); // Штучна затримка
    }
    clear_screen();
}

void cmd_system_info() {
    kprint("\n--- OS INFORMATION ---\n");
    kprint("OS Name: Vund-OS\nVersion: 2.00 GOLD\n");
    kprint("User: Coding God\nRoot Directory: /VUND_SYSTEM_ROOT\n");
    kprint("Status: STABLE / GOLD\n");
}

void cmd_tree() {
    kprint("\nROOT: VUND_SYSTEM_ROOT\n");
    kprint("    📂 apps/\n");
    kprint("        📄 system_monitor.app\n");
    kprint("    📂 system/\n");
    kprint("        📄 kernel.bin\n");
    kprint("        📄 config.h\n");
    kprint("    📄 art.img\n");
}

void cmd_createfolder() {
    char name[32];
    kprint("Folder name: ");
    kinput(name);
    kprint("[ OK ]: Folder '"); kprint(name); kprint("' created.\n");
}

void cmd_createimage() {
    char name[32];
    kprint("Image filename (e.g. art.img): ");
    kinput(name);
    kprint("Enter ASCII art (type 'SAVE' to finish):\n");
    
    char line[64];
    while(1) {
        kprint("> ");
        kinput(line);
        to_upper(line); // Переводимо в верхній регістр для перевірки
        if (str_eq(line, "SAVE")) break;
    }
    kprint("[ OK ]: Image '"); kprint(name); kprint("' saved.\n");
}

void cmd_install_app() {
    char app[32];
    kprint("Enter App Name to install: ");
    kinput(app);
    kprint("Connecting to Vund-Server...\n");
    for (volatile int wait = 0; wait < 30000000; wait++); // Емуляція завантаження
    kprint("[ OK ]: Application '"); kprint(app); kprint("' installed.\n");
}

// ==========================================
// 9. ОБРОБНИК КОМАНД (SHELL)
// ==========================================
void execute_command(char* cmd) {
    if (cmd[0] == '\0') return; // Пустий Enter

    if (str_eq(cmd, "ls")) kprint("apps/   system/   art.img\n");
    else if (str_eq(cmd, "help")) kprint("Commands: ls, createfolder, tree, system_info, logo, createimage, install_app, monitor, goto dsk, exit\n");
    else if (str_eq(cmd, "system_info")) cmd_system_info();
    else if (str_eq(cmd, "logo")) print_logo();
    else if (str_eq(cmd, "createfolder")) cmd_createfolder();
    else if (str_eq(cmd, "tree")) cmd_tree();
    else if (str_eq(cmd, "createimage")) cmd_createimage();
    else if (str_eq(cmd, "install_app") || str_eq(cmd, "install")) cmd_install_app();
    else if (str_eq(cmd, "monitor")) cmd_monitor();
    else if (str_eq(cmd, "goto dsk c") || str_eq(cmd, "goto dsk d")) {
        kprint("[IDE] Switching active drive. Reading sector 0...\n");
        // Тут міг би бути виклик ide_read_sector
        kprint("[ OK ] Drive mounted.\n");
    }
    else if (str_eq(cmd, "exit")) {
        kprint("Halting CPU. Goodbye, Coding God.\n");
        outb(0xF4, 0x00); // Команда вимкнення QEMU
        while(1) __asm__("hlt");
    }
    else {
        kprint("Unknown command: "); kprint(cmd); kprint("\n");
    }
}

// ==========================================
// 10. ГОЛОВНА ТОЧКА ВХОДУ ЯДРА
// ==========================================
void kmain() {
    boot_animation();
    
    kprint("Vund-OS 2.00 GOLD initialized.\n");
    kprint("Welcome, Coding God Yaroslav.\nType 'help' for commands.\n");

    char cmd_buffer[CMD_SIZE];

    while(1) {
        kprint("\nCodingGod@vund-gold > ");
        kinput(cmd_buffer);         // Чекаємо введення з механічної клави
        execute_command(cmd_buffer); // Виконуємо команду
    }
}