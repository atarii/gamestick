#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
uint8_t *fbp = NULL;

// This function creates a test image directly in memory
// Quadrants: Red (Top-Left), Green (Top-Right), Blue (Bottom-Left), White (Bottom-Right)
void generate_test_pattern() {
    for (uint32_t y = 0; y < vinfo.yres; y++) {
        // Calculate the start of the current row in the framebuffer
        // We use finfo.line_length to handle the "Stride/Offset" issue
        uint32_t *line_ptr = (uint32_t*)(fbp + (y * finfo.line_length));

        for (uint32_t x = 0; x < vinfo.xres; x++) {
            uint32_t color;

            if (y < vinfo.yres / 2) {
                // Top half
                color = (x < vinfo.xres / 2) ? 0xFFFF0000 : 0xFF00FF00; // Red : Green
            } else {
                // Bottom half
                color = (x < vinfo.xres / 2) ? 0xFF0000FF : 0xFFFFFFFF; // Blue : White
            }

            line_ptr[x] = color;
        }
    }
}

int main() {
    int fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        return 1;
    }

    // Get variable and fixed screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) || 
        ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
        perror("Error reading screen info");
        close(fbfd);
        return 1;
    }

    // Map the memory
    fbp = (uint8_t *)mmap(0, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (fbp == MAP_FAILED) {
        perror("Error: failed to mmap");
        close(fbfd);
        return 1;
    }

    printf("Displaying Test Pattern (%dx%d)...\n", vinfo.xres, vinfo.yres);
    
    generate_test_pattern();

    // Hold the image for 10 seconds so you can inspect it
    sleep(10);

    // Clean up
    munmap(fbp, finfo.smem_len);
    close(fbfd);
    return 0;
}
