#include "Onegin_hed.h"

int main(int argc, char* argv[])
{   
    if(argc != 3)
    {
        printf("Не хватет аргументов\n");
        return 1;
    }

    FILE* file_in = fopen(argv[1], "rb");

    if(file_in == NULL)
    {
        printf("Не удалось открыть входной файл\n");
    }

    Text text ={};
    int read_error = text_reader(&text, file_in);
    if(read_error != OK)
    {
        switch (read_error)
        {
        case CANT_REALOK_MEMORY:
            printf("Не могу ввыделть память для элементов структуры Text\n");
            break;
        case INVALID_FIILE_STAT:
            printf("Не получилось попределить длину файла\n");
            break;
        case INVALID_FILE_LEN:
            printf("Ошибка в определении длины считанной строки\n");
            break;
        default:
            break;
        }
    }
    fclose(file_in);

    FILE* file_out = fopen(argv[2], "w");

    if(file_out == NULL)
    {
        printf("Не удалось открыть\\создать выходной файл\n");
    }

    int q_error = 0;
    q_error = qsort_bd(text.str_mass, text.str_count, sizeof(string), str_cmp_plus);
    if(q_error == CANT_REALOK_MEMORY)
    {
        printf("Не вышло выделить память под pivot при прямой сортировке\n");
    }
    text_print_plus(text, file_out);
    fprintf(file_out, "######################################################\n");
    q_error = qsort_bd(text.str_mass, text.str_count, sizeof(string), str_rcmp_plus);
    if(q_error == CANT_REALOK_MEMORY)
    {
        printf("Не вышло выделить память под pivot при обратной сортировке\n");
    }
    text_print_plus(text, file_out);
    fprintf(file_out, "######################################################\n");
    txt_buf_printer(text, file_out);
    fclose(file_out);

    free(text.txt);
    free(text.str_mass);
}

