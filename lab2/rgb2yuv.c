#include <stdio.h>
#include <stdint.h>
//#include <omp.h>
#include <time.h>

#define PIXEL_COUNT 3840 * 2160
#define MAX_FRAMES 60

typedef struct {
    char r, g, b;
} RGB;

typedef struct {
    char y, u, v;
} YUV;

YUV convert2yuv(RGB rgb);

int main(void) {
    double start, delta;

    RGB rgb;
    YUV yuv;

    printf("TRACE\n");

    // #pragma omp parallel
    // {
        FILE *rgbFile = fopen("rgb_video.yuv", "rb");
        FILE *yuvFile = fopen("yuv_video.yuv_omp", "ab");
        FILE *yuv420File = fopen("yuv420_video.yuv_omp", "ab");
        FILE *yuv444File = fopen("yuv444_video.yuv_omp", "ab");

        // if(rgbFile == NULL || yuvFile == NULL || yuv420File == NULL || yuv444File == NULL)
        // {
        //     printf("Error opening file\n");
        //     return -1;
        // }

        clock_t begin = clock();

        for(int frame = 0; frame < MAX_FRAMES; ++frame)
        {
            for(int i = 0; i < PIXEL_COUNT; i++) 
            {
                int pos1 = frame*PIXEL_COUNT*3+i;
                int pos2 = frame*PIXEL_COUNT*3+i+PIXEL_COUNT;
                int pos3 = frame*PIXEL_COUNT*3+i+PIXEL_COUNT*2;

                fseek(rgbFile, pos1, SEEK_SET);
                rgb.r = fgetc(rgbFile);
                fseek(rgbFile, pos2, SEEK_SET);
                rgb.g = fgetc(rgbFile);
                fseek(rgbFile, pos3, SEEK_SET);
                rgb.b = fgetc(rgbFile);

                yuv = convert2yuv(rgb);

                // ========== yuv ==========
                fseek(yuvFile, pos1, SEEK_SET);
                fputc(yuv.y, yuvFile);
                fseek(yuvFile, pos2, SEEK_SET);
                fputc(yuv.u, yuvFile);
                fseek(yuvFile, pos3, SEEK_SET);
                fputc(yuv.v, yuvFile);
/*
                // ========== yuv420 && yuv444 ==========
                fseek(yuv420File, pos1, SEEK_SET);
                fputc(yuv.y, yuv420File);

                fseek(yuv444File, pos1, SEEK_SET);
                fputc(yuv.y, yuv444File);

                if (i % 2 == 0)
                {
                    fseek(yuv420File, pos2/2, SEEK_SET);
                    fputc(yuv.u, yuv420File);

                    fseek(yuv444File, pos2, SEEK_SET);
                    fputc(yuv.u, yuv444File);
                    fseek(yuv444File, pos2+1, SEEK_SET);
                    fputc(yuv.u, yuv444File);

                    if (i % 4 == 0)
                    {
                        fseek(yuv420File, pos3/4, SEEK_SET);
                        fputc(yuv.v, yuv420File);

                        fseek(yuv444File, pos3, SEEK_SET);
                        fputc(yuv.v, yuv444File);
                        fseek(yuv444File, pos3+1, SEEK_SET);
                        fputc(yuv.v, yuv444File);
                        fseek(yuv444File, pos3+2, SEEK_SET);
                        fputc(yuv.v, yuv444File);
                        fseek(yuv444File, pos3+3, SEEK_SET);
                        fputc(yuv.v, yuv444File);
                    }
                }*/
            }   

            clock_t end = clock();
            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            printf("frame: %d time: %ld seconds\n", frame+1, time_spent);
        }

    return 0;
}

YUV convert2yuv(RGB rgb) {

    YUV yuv;
    yuv.y = 0.257 * rgb.r + 0.504 * rgb.g + 0.098 * rgb.b + 16;
    yuv.u = -0.148 * rgb.r - 0.291 * rgb.g + 0.439 * rgb.b + 128;
    yuv.v = 0.439 * rgb.r - 0.368 * rgb.g - 0.071 * rgb.b + 128;

    return yuv;

}