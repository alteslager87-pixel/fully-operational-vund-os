#ifndef VUND_SETUP_H
#define VUND_SETUP_H

#include <stdint.h>
#include <stdbool.h>

/* MEMORY ARCHITECTURE */
#define KERNEL_ENTRY_POINT 0x00100000  // 1MB Mark for Protected Mode
#define VGA_TEXT_BUFFER    0x000B8000  // Standard CGA/VGA Text Buffer
#define SECTOR_SIZE        512

/* SYSTEM PATHS */
#define ROOT_SYS           "/SYSTEM/"
#define ROOT_USER          "/USER/"
#define CONFIG_WIFI        "/SYSTEM/VUND_AIR.CFG"

/* DATA STRUCTURES */
typedef struct {
    char ssid[32];
    char password[64];
    bool encryption_enabled;
} wifi_config_t;

typedef struct {
    char username[32];
    char email[64];
    uint8_t security_level; // Level 0: Coding God
} user_profile_t;

/* FUNCTION PROTOTYPES */
void system_purge(void);
void generate_vmail(const char* name, char* output);
void vund_air_connect(void);
void deploy_monolith(void);

#endifd);         // Копіювання ядра
void create_recovery_image(void);  // Створення бекапу
void setup_drivers(int config);    // Конфігурація драйверів

#endif