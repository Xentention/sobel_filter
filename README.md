# sobel_filter
Наложение фильтра Собеля на фотографию в режиме многопоточности.

### How to:

Скомпилировать код:
```
make -f MakeFile
```

Запустить программу
```
sobel file_in.rpg file_out.ppm width_x_height num_of_threads
```
Так как у файлов формата .rgb отсутствуют дополнительные данные о изображении, необходимо ввести размер изображения вручную.

### Результаты:

Изначальное фото; фото, обработанное в 1 поток; фото, обработанное в 512 потоков:
<p float="left">
  <img src="https://github.com/Xentention/sobel_filter/assets/96055384/4f7257f5-8f0d-4eae-8cec-a3db95a39de7" width="200" />
  <img src="https://github.com/Xentention/sobel_filter/assets/96055384/1a7286d2-a545-42e0-9ac3-75117f2da43e" width="200" /> 
  <img src="https://github.com/Xentention/sobel_filter/assets/96055384/ac12b37b-1f51-4044-a602-bb2b1619b2a3" width="200" />
</p>


### Known issues🐞:
- При увеличении числа потоков увеличивается и качество итогового изображения.
- При некоторых вариантах количества потоков на изображении возникают полосы.
- К сожалению, исходные файлы могут быть исключительно в формате .rgb, а выходные — .ppm
