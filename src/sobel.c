#include "sobel.h"

int rgbToGray(byte *rgb, byte **gray, int buffer_size) {
    int gray_size = buffer_size / 3;
    *gray = malloc(sizeof(byte) * gray_size);

    byte *p_rgb = rgb;
    byte *p_gray = *gray;

    for(int i=0; i<gray_size; i++) {
        *p_gray = 0.30*p_rgb[0] + 0.59*p_rgb[1] + 0.11*p_rgb[2];

        p_rgb += 3;
        p_gray++;
    }

    return gray_size;
}

void makeOpMem(byte *buffer, int buffer_size, int width, int cindex, byte *op_mem) {
    int bottom = cindex-width < 0;
    int top = cindex+width >= buffer_size;
    int left = cindex % width == 0;
    int right = (cindex+1) % width == 0;

    op_mem[0] = !bottom && !left  ? buffer[cindex-width-1] : 0;
    op_mem[1] = !bottom           ? buffer[cindex-width]   : 0;
    op_mem[2] = !bottom && !right ? buffer[cindex-width+1] : 0;

    op_mem[3] = !left             ? buffer[cindex-1]       : 0;
    op_mem[4] = buffer[cindex];
    op_mem[5] = !right            ? buffer[cindex+1]       : 0;

    op_mem[6] = !top && !left     ? buffer[cindex+width-1] : 0;
    op_mem[7] = !top              ? buffer[cindex+width]   : 0;
    op_mem[8] = !top && !right    ? buffer[cindex+width+1] : 0;
}

int convolution(byte *X, int *Y, int c_size) {
    int sum = 0;

    for(int i=0; i<c_size; i++) {
        sum += X[i] * Y[c_size-i-1];
    }

    return sum;
}

void itConv(byte *buffer, int buffer_size, int width, int *op, byte **res) {
    *res = malloc(sizeof(byte) * buffer_size);

    byte op_mem[SOBEL_OP_SIZE];
    memset(op_mem, 0, SOBEL_OP_SIZE);

    for(int i=0; i<buffer_size; i++) {
        makeOpMem(buffer, buffer_size, width, i, op_mem);
        (*res)[i] = (byte) abs(convolution(op_mem, op, SOBEL_OP_SIZE));
    }
}

void contour(byte *sobel_h, byte *sobel_v, int gray_size, byte **contour_img) {
    *contour_img = malloc(sizeof(byte) * gray_size);

    for(int i=0; i<gray_size; i++) {
        (*contour_img)[i] = (byte) (int) sqrt(pow(sobel_h[i], 2) + pow(sobel_v[i], 2));
    }
}

int sobelFilter(byte *rgb, byte **gray, byte **sobel_h_res, byte **sobel_v_res, byte **contour_img, int width, int height) {
    int sobel_h[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1},
            sobel_v[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

    int rgb_size = width*height*3;

    int gray_size = rgbToGray(rgb, gray, rgb_size);

    itConv(*gray, gray_size, width, sobel_h, sobel_h_res);
    itConv(*gray, gray_size, width, sobel_v, sobel_v_res);
    
    contour(*sobel_h_res, *sobel_v_res, gray_size, contour_img);

    return gray_size;
}

typedef struct {
    byte *rgb;
    byte *gray;
    byte *sobel_h_res;
    byte *sobel_v_res;
    byte *contour_img;
    int width;
    int height;
} image_part;

void* run(void *arg) {
    image_part *part = (image_part*)arg;
    sobelFilter(part->rgb, &part->gray, &part->sobel_h_res, &part->sobel_v_res, &part->contour_img, part->width, part->height);
    pthread_exit(NULL);
}

void sobelFilterMT(int threads_num, byte *rgb, byte **gray, byte **sobel_h_res, byte **sobel_v_res, byte **contour_img, int width, int height) {
    threads_num = (threads_num > height) ? height : threads_num;
    printf("The picture's been processed in %d threads.\n", threads_num);
    int height_per_thread = height / threads_num;
    pthread_t threads[threads_num];
    image_part parts[threads_num];

    // Разбиваем картинку на части и создаем потоки
    for (int t = 0; t < threads_num; t++) {
        byte *part_rgb = (byte*) malloc(width * height_per_thread * 3 * sizeof(byte));
        byte *part_gray = (byte*) malloc(width * height_per_thread * sizeof(byte));
        byte *part_sobel_h_res = (byte*) malloc(width * height_per_thread * sizeof(byte));
        byte *part_sobel_v_res = (byte*) malloc(width * height_per_thread * sizeof(byte));
        byte *part_contour_img = (byte*) malloc(width * height_per_thread * sizeof(byte));
        memcpy(part_rgb, &rgb[t * height_per_thread * width * 3], width * height_per_thread * 3 * sizeof(byte));
        parts[t] = (image_part) {part_rgb, part_gray, part_sobel_h_res, part_sobel_v_res, part_contour_img, width, height_per_thread};

        pthread_create(&threads[t], NULL, run, &parts[t]);
    }

    for (int t = 0; t < threads_num; t++) {
        pthread_join(threads[t], NULL);
    }

    // Соединяем разбитую картинку воедино
    *gray = (byte*) malloc(width * height * sizeof(byte));
    *sobel_h_res = (byte*) malloc(width * height * sizeof(byte));
    *sobel_v_res = (byte*) malloc(width * height * sizeof(byte));
    *contour_img = (byte*) malloc(width * height * sizeof(byte));

    int offset = 0;
    for (int t = 0; t < threads_num; t++) {
        int part_size = width * height_per_thread;
        memcpy(&(*gray)[offset], parts[t].gray, part_size * sizeof(byte));
        memcpy(&(*sobel_h_res)[offset], parts[t].sobel_h_res, part_size * sizeof(byte));
        memcpy(&(*sobel_v_res)[offset], parts[t].sobel_v_res, part_size * sizeof(byte));
        memcpy(&(*contour_img)[offset], parts[t].contour_img, part_size * sizeof(byte));
        offset += part_size;
    }
}