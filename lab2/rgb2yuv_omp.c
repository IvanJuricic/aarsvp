#include <stdio.h>
#include <omp.h>

#define PIXEL_COUNT 3840 * 2160
#define MAX_FRAMES 60

typedef struct 
{
    char r, g, b;
} RGB;

typedef struct 
{
    char y, u, v;
} YUV;

YUV convert2yuv(RGB rgb);

int main(void) 
{
    double start, delta;

    RGB rgb;
    YUV yuv;

    printf("START!\n");

    FILE *rgbFile = fopen("rgb_video.yuv", "rb");
    FILE *yuvFile = fopen("yuv_video_sek.yuv", "ab");
    FILE *yuv420 = fopen("yuv420_video_sek.yuv", "ab");
    FILE *yuv444 = fopen("yuv444_video_sek.yuv", "ab");

    start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        {
            for(int frame = 0; frame < MAX_FRAMES; ++frame) {
                #pragma omp task
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

                        //Generic
                        fseek(yuvFile, pos1, SEEK_SET);
                        fputc(yuv.y, yuvFile);
                        fseek(yuvFile, pos2, SEEK_SET);
                        fputc(yuv.u, yuvFile);
                        fseek(yuvFile, pos3, SEEK_SET);
                        fputc(yuv.v, yuvFile);
                        
                        // 420
                        fseek(yuv420, pos1, SEEK_SET);
                        fputc(yuv.y, yuv420);

                        fseek(yuv444, pos1, SEEK_SET);
                        fputc(yuv.y, yuv444);

                        if (i % 2 == 0)
                        {
                            fseek(yuv420, pos2/2, SEEK_SET);
                            fputc(yuv.u, yuv420);

                            fseek(yuv444, pos2, SEEK_SET);
                            fputc(yuv.u, yuv444);
                            fseek(yuv444, pos2+1, SEEK_SET);
                            fputc(yuv.u, yuv444);

                            // 444
                            if (i % 4 == 0)
                            {
                                fseek(yuv420, pos3/4, SEEK_SET);
                                fputc(yuv.v, yuv420);

                                fseek(yuv444, pos3, SEEK_SET);
                                fputc(yuv.v, yuv444);
                                fseek(yuv444, pos3+1, SEEK_SET);
                                fputc(yuv.v, yuv444);
                                fseek(yuv444, pos3+2, SEEK_SET);
                                fputc(yuv.v, yuv444);
                                fseek(yuv444, pos3+3, SEEK_SET);
                                fputc(yuv.v, yuv444);
                            }
                        }
                    }   

                    delta = omp_get_wtime() - start;
                    printf("frame: %d time: %.5g seconds\n", frame+1, delta);
                    }
            }

        }
        

    }

    printf("DONE!\n");

    return 0;
}

YUV convert2yuv(RGB rgb) 
{

    YUV yuv;
    yuv.y = 0.257 * rgb.r + 0.504 * rgb.g + 0.098 * rgb.b + 16;
    yuv.u = -0.148 * rgb.r - 0.291 * rgb.g + 0.439 * rgb.b + 128;
    yuv.v = 0.439 * rgb.r - 0.368 * rgb.g - 0.071 * rgb.b + 128;

    return yuv;

}