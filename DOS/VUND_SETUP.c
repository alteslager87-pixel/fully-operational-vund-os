#include <stdio.h>
#include <stdlib.h>

void print_header() {
    printf("========================================\n");
    printf("       VUND DOS GOLD EDITION SETUP      \n");
    printf("          Status: Coding God            \n");
    printf("========================================\n\n");
}

int main() {
    int mouse, speaker, net;

    print_header();

    printf("Configure optional drivers for your Monolith:\n");
    
    printf("1. Enable PS/2 Mouse Driver? (1-Yes, 0-No): ");
    scanf("%d", &mouse);

    printf("2. Enable PC Speaker ('Piu-piu' sounds)? (1-Yes, 0-No): ");
    scanf("%d", &speaker);

    printf("3. Enable Ethernet (Internet) Driver? (1-Yes, 0-No): ");
    scanf("%d", &net);

    printf("\n[SYSTEM] Generating config.h...\n");

    // Створюємо файл конфігурації, який прочитає GCC при збірці ОС
    FILE *f = fopen("src/config.h", "w");
    fprintf(f, "// VUND DOS Configuration File\n");
    fprintf(f, "#define OPT_MOUSE %d\n", mouse);
    fprintf(f, "#define OPT_SPEAKER %d\n", speaker);
    fprintf(f, "#define OPT_NET %d\n", net);
    fclose(f);

    printf("[SUCCESS] config.h created.\n");
    printf("[SYSTEM] Run 'make' or 'gcc' to build your custom VUND DOS!\n");

    return 0;
}