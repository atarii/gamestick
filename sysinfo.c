#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_system_file(const char* path, const char* title) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        printf("--- %s: Not Available ---\n", title);
        return;
    }
    printf("\n--- %s ---\n", title);
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), f)) {
        printf("%s", buffer);
    }
    fclose(f);
}

int main() {
    printf("==========================================\n");
    printf("   ROCKCHIP HANDHELD SYSTEM DIAGNOSTIC    \n");
    printf("==========================================\n");

    print_system_file("/proc/cpuinfo", "CPU SPECIFICATIONS");
    print_system_file("/proc/meminfo", "MEMORY USAGE");
    print_system_file("/proc/version", "KERNEL VERSION");
    print_system_file("/proc/device-tree/model", "HARDWARE MODEL");
    
    printf("\nListing /dev/input (Joystick Check):\n");
    system("ls -l /dev/input");

    printf("\nDiagnostic Complete. Restarting system in 10 seconds...\n");
    sleep(10);
    return 0;
}