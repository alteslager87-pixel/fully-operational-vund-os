# boot.s - Універсальний запускач для VUND DOS
.code16                         # Починаємо в 16 бітах (BIOS)
.global _start

_start:
    cli                         # Вимикаємо переривання
    xor %ax, %ax                # Обнуляємо регістри
    mov %ax, %ds
    mov %ax, %es
    
    # Виводимо повідомлення, що VUND завантажується
    mov $0x0E, %ah
    mov $'V', %al
    int $0x10
    mov $'U', %al
    int $0x10

    # Перехід у 32-бітний захищений режим (Protected Mode)
    lgdt gdt_descriptor         # Завантажуємо таблицю дескрипторів
    mov %cr0, %eax
    or $1, %eax                 # Вмикаємо біт захищеного режиму
    mov %eax, %cr0

    ljmp $0x08, $start32        # Дальній стрибок у 32-бітний код

.code32                         # Тепер ми в 32 бітах!
start32:
    mov $0x10, %ax              # Налаштовуємо сегменти даних
    mov %ax, %ds
    mov %ax, %ss
    
    call kmain                  # Викликаємо твоє ядро на C!
    hlt                         # Зупинка, якщо ядро повернуло керування

# Таблиця GDT (Global Descriptor Table) - необхідна для 32/64 біт
gdt_start:
    .quad 0x0000000000000000    # Нульовий дескриптор
gdt_code:
    .quad 0x00cf9a000000ffff    # Код: Base 0, Limit 4GB
gdt_data:
    .quad 0x00cf92000000ffff    # Дані: Base 0, Limit 4GB
gdt_end:

gdt_descriptor:
    .word gdt_end - gdt_start - 1
    .long gdt_start