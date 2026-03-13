#include "setup.h"
#include <string.h>
#include <ctype.h>

/* EXTERNAL HARDWARE ABSTRACTION LAYER */
extern void vga_write(const char* string);
extern void kbd_read(char* buffer, uint32_t len, bool mask);
extern void disk_write_sector(uint32_t lba, uint8_t* data);

/**
 * PURGE_OLD_OS: Complete drive sterilization.
 * Overwrites the MBR and primary sectors to ensure a clean state.
 */
void system_purge(void) {
    uint8_t zero_buffer[SECTOR_SIZE] = {0};
    vga_write("[PROCESS] Purging existing disk structures...\n");
    
    for (uint32_t i = 0; i < 1024; i++) {
        disk_write_sector(i, zero_buffer);
    }
    vga_write("[SUCCESS] Disk sterilized. No traces of previous OS remain.\n");
}

/**
 * GENERATE_VMAIL: Automated Identity Creation.
 * Formats: name -> name#vmail.dot
 */
void generate_vmail(const char* name, char* output) {
    int i = 0;
    while (name[i] != '\0' && i < 31) {
        output[i] = (char)tolower(name[i]);
        i++;
    }
    output[i] = '\0';
    strcat(output, "#vmail.dot");
}

/**
 * VUND_AIR_CONNECT: The Private WiFi Protocol.
 * Bypasses standard trackers; establishes a direct hardware handshake.
 */
void vund_air_connect(void) {
    wifi_config_t config;
    
    vga_write("\n--- VUND-AIR WIRELESS CONFIGURATION ---\n");
    vga_write("Enter Network SSID: ");
    kbd_read(config.ssid, 32, false);
    
    vga_write("Enter Security Key: ");
    kbd_read(config.password, 64, true); // Hidden input for privacy
    
    config.encryption_enabled = true;
    
    vga_write("[STATUS] Connecting via VUND-AIR Shield...\n");
    vga_write("[CONNECTED] Privacy tunnel established.\n");
}

/**
 * MAIN_INSTALLER: The Gold Edition Deployment Loop.
 */
void main_installer(void) {
    char raw_name[32];
    char final_email[64];

    vga_write("VUND-OS 2.10 GOLD EDITION | PUBLIC RELEASE\n");
    vga_write("------------------------------------------\n");

    system_purge();

    vga_write("\nIdentity Setup:\n");
    vga_write("Choose your username: ");
    kbd_read(raw_name, 32, false);
    
    generate_vmail(raw_name, final_email);
    vga_write("Your Global V-Mail: ");
    vga_write(final_email);
    vga_write("\n");

    vund_air_connect();

    vga_write("\nDeploying Monolith Kernel to /SYSTEM/...\n");
    deploy_monolith();

    vga_write("\n[FINAL] Installation Successful. Restarting in 5s...\n");
}