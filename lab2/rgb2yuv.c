#include <stdio.h>

#define PIXEL_COUNT 3840 * 2160

typedef struct {
    char r, g, b;
} RGB;

typedef struct {
    char y, u, v;
} YUV;

YUV convert2yuv(RGB rgb);

int main(void) {

    printf("Hello there\n");

    char buff[255], c;

    RGB rgb;
    YUV yuv;

    FILE *rgbFile = fopen("rgb_video.yuv", "rb");
    FILE *yuvFile = fopen("yuv_video.yuv", "ab");

    if(rgbFile == NULL || yuvFile == NULL) {
        printf("Error opening file\n");
        return -1;
    }

    for(int i = 0; i < PIXEL_COUNT; i++) {
        fseek(rgbFile, i, SEEK_SET);
        rgb.r = fgetc(rgbFile);
        fseek(rgbFile, i + PIXEL_COUNT, SEEK_SET);
        rgb.g = fgetc(rgbFile);
        fseek(rgbFile, i + PIXEL_COUNT * 2, SEEK_SET);
        rgb.b = fgetc(rgbFile);

        yuv = convert2yuv(rgb);
    }

    printf("Tu smo\n");
    //yuv = convert2yuv(rgb);
    printf("A sad tu\n");

    fclose(rgbFile);
    fclose(yuvFile);

    return 0;
}

YUV convert2yuv(RGB rgb) {

    YUV yuv;
    yuv.y = 0.257 * rgb.r + 0.504 * rgb.g + 0.098 * rgb.b + 16;
    yuv.u = -0.148 * rgb.r - 0.291 * rgb.g + 0.439 * rgb.b + 128;
    yuv.v = 0.439 * rgb.r - 0.368 * rgb.g - 0.071 * rgb.b + 128;

    return yuv;

}