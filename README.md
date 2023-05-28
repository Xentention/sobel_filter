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

Изначальное фото; фото, обработанное в 1 поток и фото, обработанное в 512 потоков:
<p float="left">
  <img src="https://github.com/Xentention/sobel_filter/assets/96055384/91098ccc-0b2c-411c-9f1d-636105599a2d" width="250" />
  <img src="https://github.com/Xentention/sobel_filter/assets/96055384/c0fa24e0-54ef-4f80-b300-e42967b25ef9" width="250" /> 
  <img src="https://github.com/Xentention/sobel_filter/assets/96055384/388fde3b-ead2-40dc-b48b-23e9d2239b35" width="250" />
</p>

Или вариант, который мне нравится больше;)
<p float="left">
  <img src="https://github.com/Xentention/sobel_filter/assets/96055384/309662ec-e788-449d-84f0-5daad9700975" width="250" />
  <img src="https://github.com/Xentention/sobel_filter/assets/96055384/8f5c28b3-b3a2-4ac7-9ebb-f55eaa12c582" width="250" /> 
  <img src="https://github.com/Xentention/sobel_filter/assets/96055384/2823bf98-df9b-4046-9df5-3cd0254f1585" width="250" />
</p>


### Known issues🐞:
- При увеличении числа потоков увеличивается и качество итогового изображения.
- При некоторых вариантах количества потоков на изображении возникают полосы.
- Проблемы с не квадратными изображениями
- К сожалению, исходные файлы могут быть исключительно в формате .rgb, а выходные — .ppm
