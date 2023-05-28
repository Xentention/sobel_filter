#include "sobel.h"
#include "file_operations.h"
#include <time.h>

#define ARGS_NEEDED 5

int main(int argc, char *argv[]) {
    char *file_in,
         *file_out;

    byte *rgb,
         *gray,
         *sobel_h_res,
         *sobel_v_res,
         *contour_img;

    int  threads_num,
         rgb_size,
         width,
         height;

    if(argc != ARGS_NEEDED) {
        printf("ERROR: wrong amount of args. A command should"
               "look like sobel file_in.rpg file_out.ppm width_x_height num_of_threads.\n");
        return 1;
    }

    file_in = argv[1];
    file_out = argv[2];

    char *width_token = strtok(argv[3], "x");
    if(width_token) {
        width = atoi(width_token);

    } else {
        printf("ERROR: Bad image size argument\n");
        return 1;
    }

    char *height_token = strtok(NULL, "x");
    if(height_token) {
        height = atoi(height_token);
    } else {
        printf("ERROR: Bad image size argument\n");
        return 1;
    }

    threads_num = atoi(argv[4]);

    rgb_size = width*height*3;
    readFile(file_in, &rgb, rgb_size);

    clockid_t timer = CLOCK_REALTIME;
    struct timespec time;
    struct timespec st_time;
    clock_getres(timer, &time);
    printf("Запуск таймера, точность: %li с. %li нс.\n\n", time.tv_sec, time.tv_nsec);
    time.tv_nsec = 0;
    time.tv_sec = 0;
    clock_gettime(timer, &st_time);	//Запуск таймера

    sobelFilterMT(threads_num, rgb, &gray, &sobel_h_res, &sobel_v_res, &contour_img, width, height);
    writeFile(file_out, contour_img, width, height);

    clock_gettime(timer, &time);
    double nano = time.tv_sec * 1000000000 - st_time.tv_sec * 1000000000 + time.tv_nsec - st_time.tv_nsec;
    printf("Прошло %g нс.\n", nano);

    free(contour_img);
    return 0;
}

