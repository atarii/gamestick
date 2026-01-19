#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    // 1. Define paths
    char *doom_path = "/mnt/sdcard/fbdoom_mips";
    char *wad_path  = "/mnt/sdcard/doom1.wad";

    // 2. Define arguments for fbDOOM
    // -width and -height should match your fbset -i output (likely 640x480 or 1280x720)
    // we use "fbdoom_mips" as the first arg (argv[0])
    char *args[] = {
        "fbdoom_mips",
        "-iwad", wad_path,
        "-width", "640",
        "-height", "480",
        NULL
    };

    printf("Wrapper: Launching fbDOOM...\n");

    // 3. Execute the binary
    // execv replaces this wrapper process with fbDOOM
    if (execv(doom_path, args) == -1) {
        perror("Wrapper Error: Failed to execute fbDOOM");
        return 1;
    }

    return 0; // Should never reach here
}
