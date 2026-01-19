#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void log_title(FILE *out, const char *title) {
    fprintf(out, "\n========================================\n");
    fprintf(out, "  %s\n", title);
    fprintf(out, "========================================\n");
}

void log_file(FILE *out, const char *path) {
    log_title(out, path);
    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(out, "ERROR: Could not open %s\n", path);
        return;
    }
    char buf[1024];
    while (fgets(buf, sizeof(buf), f)) {
        fputs(buf, out);
    }
    fclose(f);
}

void log_command(FILE *out, const char *cmd) {
    log_title(out, cmd);
    FILE *pipe = popen(cmd, "r");
    if (!pipe) {
        fprintf(out, "ERROR: Could not execute %s\n", cmd);
        return;
    }
    char buf[1024];
    while (fgets(buf, sizeof(buf), pipe)) {
        fputs(buf, out);
    }
    pclose(pipe);
}

int main() {
    // Save to SD card - usually mounted at /mnt or /mnt/sdcard
    FILE *out = fopen("/mnt/sdcard/info.txt", "w");
    if (!out) return 1;

    fprintf(out, "RK-GAME MIPS DIAGNOSTIC REPORT\n");
    
    // --- Environment Info ---
    log_command(out, "whoami");
    log_command(out, "uname -a");
    log_command(out, "pwd");
    log_command(out, "df -h");
    log_command(out, "ls -l /");

    // --- System/Kernel Info ---
    log_file(out, "/proc/version");
    log_file(out, "/proc/cmdline");
    log_file(out, "/proc/cpuinfo");
    log_file(out, "/proc/meminfo");
    log_file(out, "/proc/partitions");
    log_file(out, "/proc/mounts");

    // --- Graphics/Framebuffer Info ---
    log_command(out, "ls -l /dev/fb*");
    log_file(out, "/sys/class/graphics/fb0/modes");
    log_command(out, "fbset -i");

    // --- Boot Logs ---
    log_command(out, "dmesg");

    fclose(out);
    sync(); // Force write to physical media
    return 0;
}
