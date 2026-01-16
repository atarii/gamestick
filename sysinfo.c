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
    // Redirect stdout to the specified file on the mount point
    // "w" will overwrite the file if it already exists
    if (freopen("/mnt/sdcard/info.txt", "w", stdout) == NULL) {
        // If the mount point is read-only or doesn't exist, this will fail
        perror("Failed to open /mnt/sdcard/info.txt for writing");
        return 1;
    }

    printf("==========================================\n");
    printf("    ROCKCHIP HANDHELD SYSTEM DIAGNOSTIC    \n");
    printf("==========================================\n");

    print_system_file("/proc/cpuinfo", "CPU SPECIFICATIONS");
    print_system_file("/proc/meminfo", "MEMORY USAGE");
    print_system_file("/proc/version", "KERNEL VERSION");
    print_system_file("/proc/device-tree/model", "HARDWARE MODEL");
    
    printf("\nListing /dev/input (Joystick Check):\n");
    // Note: system() calls usually inherit the parent's file descriptors.
    // To ensure 'ls' output goes to the file, we explicitly redirect it.
    system("ls -l /dev/input >> /mnt/sdcard/info.txt 2>&1");

    printf("\nDiagnostic Complete. Results saved to /mnt/sdcard/info.txt.\n");
    printf("Restarting system in 10 seconds...\n");
    
    // Ensure all data is written to the SD card before sleeping/restarting
    fflush(stdout);
    sync(); 

    sleep(10);
    return 0;
}
