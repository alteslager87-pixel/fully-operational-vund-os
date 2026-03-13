#include <stdint.h>
#include <stdbool.h>

/* Системні порти */
#define KBD_PORT_DATA 0x60
#define KBD_PORT_CMD  0x64

/* Стан клавіш модифікаторів */
static bool is_ctrl_pressed = false;
static bool is_alt_pressed = false;

/* Зовнішні виклики до ядра */
extern void trigger_bsod(const char* reason);
extern void reboot_system(void);
extern uint8_t inb(uint16_t port);
extern void outb(uint16_t port, uint8_t val);

/* ФУНКЦІЯ: Обробник переривання клавіатури */
void keyboard_interrupt_handler(void) {
    uint8_t scancode = inb(KBD_PORT_DATA);
    
    // Відстеження стану Ctrl та Alt
    if (scancode == 0x1D) is_ctrl_pressed = true;
    else if (scancode == 0x9D) is_ctrl_pressed = false;
    
    if (scancode == 0x38) is_alt_pressed = true;
    else if (scancode == 0xB8) is_alt_pressed = false;

    // Перевірка гарячих комбінацій
    if (is_ctrl_pressed) {
        switch (scancode) {
            case 0x1F: // Ctrl + S
                // save_document();
                break;
            case 0x30: // Ctrl + B
                // launch_browser();
                break;
            case 0x53: // Код клавіші Delete (разом з Ctrl+Alt)
                if (is_alt_pressed) {
                    outb(KBD_PORT_CMD, 0xFE); // Hard Reset
                }
                break;
            case 0x4F: // Ctrl + End
                // shutdown_system();
                break;
        }
    }
}