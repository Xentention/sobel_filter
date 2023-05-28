//
// Created by xenia on 28.05.23.
//

#include "file_operations.h"

/**
 * Читаем из .rgb
 */
void readFile(char *file_name, byte **buffer, int buffer_size) {
    FILE *file = fopen(file_name, "r");
    if(!file){
        printf("Error: Can't open the file.");
        exit(1);
    }

    *buffer = malloc(sizeof(byte) * buffer_size);
    for(int i=0; i<buffer_size; i++) {
        (*buffer)[i] = fgetc(file);
    }

    fclose(file);
}

/**
 * Записываем в .ppm
 */
void writeFile(char *file_name, byte *buffer,int width, int height) {
    FILE *file = fopen(file_name, "w");

    if(!file){
        printf("Error: Can't open the file.");
        exit(1);
    }

    fprintf(file, "P6\n");
    fprintf(file, "%d %d\n", width, height);
    fprintf(file, "%d\n", 255);

    for(int y = height - 1; y >=0; y--) {
        for (int x = 0; x < width; x++) {
            char p = (char) (buffer[y * width + x]);
            for(int i = 0; i < 3; i++) {
                fwrite(&p, sizeof(char), 1, file); //R
                fwrite(&p, sizeof(char), 1, file); //G
                fwrite(&p, sizeof(char), 1, file); //B
            }
        }
    }

    fclose(file);
}
