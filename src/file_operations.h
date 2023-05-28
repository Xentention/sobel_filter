//
// Created by xenia on 28.05.23.
//

#ifndef SOBEL_FILTER_FILE_OPERATIONS_H
#define SOBEL_FILTER_FILE_OPERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"

void readFile(char *file_name, byte **buffer, int buffer_size);
void writeFile(char *file_name, byte *buffer, int width, int height);

#endif //SOBEL_FILTER_FILE_OPERATIONS_H
