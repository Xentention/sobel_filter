cmake_minimum_required(VERSION 3.25)
project(sobel_filter C)

set(CMAKE_C_STANDARD 11)

add_executable(sobel_filter src/main.c src/sobel.c src/file_operations.c src/sobel.h src/macros.h src/file_operations.h)
