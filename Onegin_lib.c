#include "Onegin_hed.h"

int  full_file_reader  (char** copy, size_t* len, FILE* file)
{
    assert(file != NULL);
    assert(len != NULL);
    assert(copy != NULL);

    struct stat stats = {};
    if(fstat(fileno(file), &stats) != 0)
    {
        return INVALID_FIILE_STAT;
    }

    *len = (size_t)stats.st_size;

    free(*copy);

    *copy = (char*)calloc(*len, 8 * sizeof(char));

    if (copy == NULL)
    {
        return CANT_REALOK_MEMORY;
    }

    if(*len != fread(*copy, sizeof(char), *len, file))
    {
        return INVALID_FILE_LEN;
    }

    return OK;
}

int  text_normifikator (Text* text)
{
    
    assert(text != NULL);
    
    str_count(text);
    text->str_mass = (string*)calloc(text->str_count, sizeof(string));
    if(text->str_mass == NULL)
    {
        return CANT_REALOK_MEMORY;
    }
    size_t num_of_str = 0;
    size_t len_buf = 0;
    size_t last_real_len = 0;
    size_t iterator = 0;

    for(num_of_str = 0, iterator = 0; num_of_str < text->str_count; num_of_str++)
    {   
        
        for((text->str_mass + num_of_str)->str = (text->txt) + iterator; *((text->str_mass + num_of_str)->str) == ' '; )
        {
            iterator++;
            (text->str_mass + num_of_str)->str = (text->txt) + iterator;
        }

        
        for(len_buf = 0, last_real_len = -1;
            ((*((((text->str_mass) + num_of_str)->str) + len_buf) != '\n') && 
            (*((((text->str_mass) + num_of_str)->str) + len_buf) != '\0') &&
            (iterator < text->len)); len_buf++, iterator++)
        {   
            if (*((((text->str_mass) + num_of_str)->str) + len_buf) != ' ')
            {
                last_real_len = len_buf;
            }
        }

        iterator ++;
        ((text->str_mass) + num_of_str)->len = last_real_len + 1;
        *((((text->str_mass) + num_of_str)->str) + last_real_len + 1) = '\0';
    }

    return OK;
}

int  text_reader       (Text* text, FILE* file)
{
    assert(text != NULL);
    assert(file != NULL);

    int read_error = full_file_reader(&(text->txt), &(text->len), file);

    if(read_error != OK) 
    {
        return read_error;
    }

    int norm_error = text_normifikator(text);

    if(norm_error != OK)
    {
        return norm_error;
    }

    return OK;
}

void str_count         (Text* text)
{   
    assert(text != NULL);

    size_t iterator = 0;
    text->str_count = 1;

    for(iterator = 0; iterator < text->len; iterator += 1)
    {
        if((*(text->txt + iterator) == '\n') || (*(text->txt + iterator) == '\0'))
        {
            text->str_count += 1;
        }
    }
}


int  str_cmp_plus      (void* str_Av, void* str_Bv)
{
    assert(str_Av != NULL);
    assert(str_Bv != NULL);
    
    size_t itterator_A = 0;
    size_t itterator_B = 0;

    string str_A = *(string*)(str_Av);
    string str_B = *(string*)(str_Bv);

    for(itterator_A = 0, itterator_B = 0; (itterator_A < str_A.len) && (itterator_B < str_B.len); itterator_A ++, itterator_B ++)
    {   
        while((it_is_simvol(*(str_A.str + itterator_A)) == IT_IS_NO_SIMVOL) && (itterator_A <= str_A.len)) 
        {
            itterator_A++;
        }
        while((it_is_simvol(*(str_B.str + itterator_B)) == IT_IS_NO_SIMVOL) && (itterator_B <= str_B.len))
        {
            itterator_B++;
        }

        if((itterator_A > str_A.len) || (itterator_B > str_B.len))
        {
            break;
        }

        if(toupper(*(str_A.str + itterator_A)) > toupper(*(str_B.str + itterator_B)))
        {
            return A_LAGER_B;
        }
        else if(toupper(*(str_B.str + itterator_B)) > toupper(*(str_A.str + itterator_A)))
        {
            return B_LAGER_A;
        }
    }

    if((itterator_A == str_A.len) && (itterator_B != str_B.len))
    {
        return B_LAGER_A;
    }
    else if((itterator_A != str_A.len) && (itterator_B == str_B.len))
    {
        return A_LAGER_B;
    }
    
    return A_EQ_B;
}

int  str_rcmp_plus     (void* str_Av, void* str_Bv)
{
    size_t itterator_A = 0;
    size_t itterator_B = 0;

    string str_A = *(string*)(str_Av);
    string str_B = *(string*)(str_Bv);

    for(itterator_A = str_A.len - 1, itterator_B = str_B.len - 1; (itterator_A != -1) && (itterator_B != -1); itterator_A --, itterator_B --)
    {   
        while((it_is_simvol(*(str_A.str + itterator_A)) == IT_IS_NO_SIMVOL) && (itterator_A != -1)) 
        {
            itterator_A--;
        }
        while((it_is_simvol(*(str_B.str + itterator_B)) == IT_IS_NO_SIMVOL) && (itterator_B != -1))
        {
            itterator_B--;
        }

        if((itterator_A == -1) || (itterator_B == -1))
        {
            break;
        }

        if(toupper(*(str_A.str + itterator_A)) > toupper(*(str_B.str + itterator_B)))
        {
            return A_LAGER_B;
        }
        else if(toupper(*(str_B.str + itterator_B)) > toupper(*(str_A.str + itterator_A)))
        {
            return B_LAGER_A;
        }
    }

    if((itterator_A == -1) && (itterator_B != -1))
    {
        return B_LAGER_A;
    }
    else if((itterator_A != -1) && (itterator_B == -1))
    {
        return A_LAGER_B;
    }
    
    return A_EQ_B;
}

int  it_is_simvol      (char ch)
{
    if ((('0' <= ch) && (ch <= '9')) || (('A' <= ch) && (ch <= 'Z')) || (('a' <= ch) && (ch <= 'z'))/*как-нибудь позже) || (('А' < ch) && (ch < 'Я')) || (('а' < ch) && (ch < 'я'))*/)
    {
        return IT_IS_SIMVOL;
    }
    else
    {
        return IT_IS_NO_SIMVOL;
    }
}

int  qsort_bd          (void * first, size_t number, size_t size, int ( * comparator ) ( const void *, const void * ))
{
    assert(first != NULL);
    assert(comparator != NULL);
    
    assert(size != 0); // не оч уверен в необходимости
    assert(number != 0);

    void* pivot_mem = malloc(size);
    if(pivot_mem == NULL)
    {
        return CANT_REALOK_MEMORY;
    }
    
    qsort_c(first, number, size, comparator, pivot_mem);
    free(pivot_mem);

    return OK;    
}

void qsort_c           (void * first, size_t number, size_t size, int ( * comparator ) ( const void *, const void * ), void* pivot_mem)
{   
    assert(first != NULL);
    assert(comparator != NULL);
    assert(pivot_mem != NULL);

    memcpy(pivot_mem, (u_int8_t*)first + ((number - 1)/2) * size, size); 
    size_t m = 0;
    size_t i_iterator = 0;
    size_t j_iterator = number - 1;
    
    if(number < 2)
    {
        return;
    }
    

    while ((i_iterator <= j_iterator) && (j_iterator != -1))
    { 
        while((comparator(((void*)((u_int8_t*)first + i_iterator * size)), pivot_mem) == B_LAGER_A) && (i_iterator < number))
        {
            i_iterator++;
        }
        while((comparator(((void*)((u_int8_t*)first + j_iterator * size)), pivot_mem) == A_LAGER_B) && (j_iterator != -1))
        {
            j_iterator--;
        }
        if ((i_iterator >= j_iterator) || (j_iterator == -1))
        {
            break;
        }
        swap_c((void*)((u_int8_t*)first + (i_iterator * size)), (void*)((u_int8_t*)first + (j_iterator * size)), size);
        i_iterator++;
        j_iterator--;
    }

    if(number == 2)
    {
        return;
    }

    qsort_c(first, j_iterator + 1, size, comparator, pivot_mem);
    qsort_c((void*)((u_int8_t*)first + (j_iterator + 1) * size), number - (j_iterator + 1), size, comparator, pivot_mem);
}

void  swap_c            (void* a, void* b, size_t size)
{
    assert(a != NULL);
    assert(b != NULL);
    assert(size != 0);

    size_t itrrator = 0;

    while((size - itrrator) >= sizeof(u_int64_t))
    {
        u_int64_t x_bufer = 0;
        x_bufer = *(u_int64_t*)((u_int8_t*)(a) + itrrator);
        *(u_int64_t*)((u_int8_t*)(a) + itrrator) = *(u_int64_t*)((u_int8_t*)(b) + itrrator);
        *(u_int64_t*)((u_int8_t*)(b) + itrrator) = x_bufer;
        itrrator += (sizeof(u_int64_t));
    }

    while((size - itrrator) >= sizeof(u_int32_t))
    {
        u_int32_t t_bufer = 0;
        t_bufer = *(u_int32_t*)((u_int8_t*)(a) + itrrator);
        *(u_int32_t*)((u_int8_t*)(a) + itrrator) = *(u_int32_t*)((u_int8_t*)(b) + itrrator);
        *(u_int32_t*)((u_int8_t*)(b) + itrrator) = t_bufer;
        itrrator += sizeof(u_int32_t);
    }

    while((size - itrrator) >= sizeof(u_int16_t))
    {
        u_int16_t s_bufer = 0;
        s_bufer = *(u_int16_t*)((u_int8_t*)(a) + itrrator);
        *(u_int16_t*)((u_int8_t*)(a) + itrrator) = *(u_int16_t*)((u_int8_t*)(b) + itrrator);
        *(u_int16_t*)((u_int8_t*)(b) + itrrator) = s_bufer;
        itrrator += sizeof(u_int16_t);
    }

    while((size - itrrator) >= sizeof(u_int8_t))
    {
        u_int8_t e_bufer = 0;
        e_bufer = ((u_int8_t*)(a) + itrrator);
        *((u_int8_t*)(a) + itrrator) = *((u_int8_t*)(b) + itrrator);
        *((u_int8_t*)(b) + itrrator) = e_bufer;
        itrrator += sizeof(u_int8_t);
    }
}


void str_print_plus    (string str, FILE* stream)
{   
    assert(stream != NULL);
    
    size_t iterator = 0;
    int spasebuf = 0;
    
    for(spasebuf = 0, iterator = 0; iterator < str.len; iterator++)
    {   
        if(*(str.str + iterator) != ' ')
        {
            putc(*(str.str + iterator), stream);
            spasebuf = 0;
        }
        else if(spasebuf == 0)
        {
            putc(*(str.str + iterator), stream);
            spasebuf = 1;
        }
    }
    putc('\n', stream);
}

void text_print_plus   (Text text, FILE* stream)
{   
    assert(stream != NULL);

    size_t iterator = 0;
    for(iterator = 0; iterator < text.str_count; iterator++)
    {       
        if((text.str_mass + iterator)->len != 0)
        {
            str_print_plus(*(text.str_mass + iterator), stream);
        }
    }
}

void txt_buf_printer   (Text text, FILE* stream)
{   
    assert(stream != NULL);

    size_t text_iterator = 0;
    while(text_iterator < text.len - 1)
    {
        while(*(text.txt + text_iterator) == ' ')
        {
            text_iterator++;
        }
        while(*(text.txt + text_iterator) != '\0')
        {
            putc(*(text.txt + text_iterator), stream);
            text_iterator++;
        }
        if(text_iterator < text.len - 1)
        {
            putc('\n', stream);
            text_iterator++;
        }
        else
        {
            break;
        }
    }
}


void debag_char_print  (char* str, size_t start, size_t end, size_t line, char* func)
{   
    assert(str != NULL);
    assert(start < end);
    assert(func != NULL);

    int iterator = 0;
    printf("DEBAG_STR================\nFUNK: %s\nLINE: %lu\n", func, line);
    for(iterator = start; iterator < end; iterator++)
    {
        switch (*(str + iterator))
        {
        case '\0':
            printf("%d - '\\0' - %d\n\n", iterator, *(str + iterator));
            break;
        case '\n':
            printf("%d - '\\n' - %d\n\n", iterator, *(str + iterator));
            break;
        case '\r':
            printf("%d - '\\r' - %d\n\n", iterator, *(str + iterator));
            break;
        case EOF:
            printf("%d - 'EOF' - %d\n", iterator, *(str + iterator));
            break;
        default:
            printf("%d - '%c' - %d\n", iterator, *(str + iterator), *(str + iterator));
            break;
        }
    }
    printf("=========================\n");
}

int  debug_text_print  (Text* text)
{
    int stritter = 0;
    int charitter = 0;
    for(stritter = 0; stritter < text->str_count; stritter++)
    {
        printf("%3d|", stritter);
        for(charitter = 0; charitter < (text->str_mass + stritter)->len + 1; charitter ++)
        {
            switch (*(((text->str_mass) + stritter)->str + charitter))
            {
            case '\0':
                printf("'\\0'");
                break;
            case '\n':
                printf("'\\n'");
                break;
            case '\r':
                printf("'\\r'");
                break;
            case EOF:
                printf("'EOF'");
                break;
            default:
                printf("'%c'", *(((text->str_mass) + stritter)->str + charitter));
                break;
            }
        }
        printf("\n");
    }    
}