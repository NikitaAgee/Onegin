# Onegin
## Description
Данная программа призвана отсортировать енциклопедию русской жизьни "Онегина", но пока она способна работать только с Гамлетом. Также она призвана сгенерировать стихотворение на основе "Онегина" и возврата исходного текста, чтобы показать, что он не был уничтожен нашими действиями.
## How to start
 1. Скачайте gcc(Ubuntu 9.3.0-17ubuntu1~20.04)   
 2. Скачайте все файлы(разрешения .c и .h и Makefile) и поместите в одну папку  
    Желательно, чтобы в ней не было лишних файлов    
 3. Открой этот файл в терминале  
 4. Запустите сборку при помощи команды make
## How it work
Для работы достаточно вызвать програму по следующему патерну
./Onegin [Имя файла с текстом] [Имя файла для записи результата]
После вызова в файл для выхода будет записан текст отсортированный в прямом, обратном порядке и исходный текст.
