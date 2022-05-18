#include <stdio.h>

int main(void) {

    printf("Hello there\n");
    FILE *rgbFile = fopen("rgb_video.yuv", "r");
    FILE *yuvFile = fopen("rgb_video.rgb", "a");

    if(rgbFile == NULL || yuvFile == NULL) {
        printf("Error opening file\n");
        return -1;
    }



    return 0;
}