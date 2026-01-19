#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

int main() {
    // 1. Setup Environment (Common for Rockchip/Ingenic handhelds)
    setenv("SDL_NOMOUSE", "1", 1);
    setenv("HOME", "/mnt/sdcard", 1);

    // 2. Clear Framebuffer (Prevents "ghosting" from the previous UI)
    int fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd != -1) {
        struct fb_var_screeninfo vinfo;
        if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == 0) {
            size_t size = vinfo.xres * vinfo.yres * (vinfo.bits_per_pixel / 8);
            void *fbp = malloc(size);
            if (fbp) {
                memset(fbp, 0, size);
                write(fbfd, fbp, size);
                free(fbp);
            }
        }
        close(fbfd);
    }

    // 3. Define Game Paths
    char *doom_exe = "/mnt/sdcard/fbdoom_mips";
    char *wad_file = "/mnt/sdcard/doom1.wad";

    // 4. Arguments
    // Adjust -width and -height if your screen is different
    char *args[] = {
        "fbdoom_mips",
        "-iwad", wad_file,
        "-width", "640",
        "-height", "480",
        NULL
    };

    printf("Starting fbDOOM...\n");
    sync(); // Flush filesystem buffers
    sleep(1); 

    if (execv(doom_exe, args) == -1) {
        perror("Exec failed");
        return 1;
    }

    return 0;
}
