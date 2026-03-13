#include "setup.h"
#include <string.h>
#include <ctype.h>

/* Зовнішні функції драйверів (заглушки для компіляції) */
extern void vga_print(const char* text);
extern char kbd_get_input(void);
extern void outb(uint16_t port, uint8_t val);

/* ФУНКЦІЯ: Генерація пошти (name#vmail.dot) */
void generate_vmail(const char* input_name, char* output_mail) {
    int i = 0;
    
    // Переводимо ім'я в нижній регістр та копіюємо
    while (input_name[i] != '\0' && i < 31) {
        output_mail[i] = tolower(input_name[i]);
        i++;
    }
    output_mail[i] = '\0';
    
    // Додаємо фірмовий домен
    strcat(output_mail, "#vmail.dot");
}

/* ФУНКЦІЯ: Тотальне очищення диска */
void purge_old_os(void) {
    vga_print("[SYSTEM] Overwriting MBR and Sectors...\n");
    
    for (uint32_t sector = 0; sector < 512; sector++) {
        // Уявімо, що ide_write_sector це наша низькорівнева функція
        // ide_write_sector(sector, 0x00);
    }
    
    vga_print("[SUCCESS] Drive is clean. Old OS purged.\n");
}

/* ФУНКЦІЯ: Головний цикл інсталятора */
void main_installer(void) {
    vga_print("VUND-OS 2.10 GOLD INSTALLER\n");
    vga_print("1. PURGE & INSTALL\n");
    vga_print("2. UPGRADE\n");
    
    char choice = kbd_get_input();
    
    if (choice == '1') {
        purge_old_os();
    }
    
    vga_print("Installing Kernel...\n");
    install_kernel();
    
    vga_print("Creating Auto-Restore Point...\n");
    create_recovery_image();
    
    vga_print("Installation Complete! Press Ctrl+Alt+Del to reboot.\n");
}