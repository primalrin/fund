#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура String
typedef struct
{
    char *data;
    int len;
} String;

// Создание экземпляра String на основе char*
String *string_create(const char *str)
{
    if (str == NULL)
    {
        return NULL;
    }

    String *s = (String *)malloc(sizeof(String));
    if (s == NULL)
    {
        return NULL;
    }

    s->len = strlen(str);
    s->data = (char *)malloc(sizeof(char) * (s->len + 1));
    if (s->data == NULL)
    {
        free(s);
        return NULL;
    }

    strcpy(s->data, str);
    return s;
}

// Удаление содержимого String
void string_destroy(String *s)
{
    if (s == NULL)
    {
        return;
    }
    free(s->data);
    free(s);
}

// Отношение порядка между двумя String
int string_compare(const String *s1, const String *s2)
{
    if (s1 == NULL || s2 == NULL)
    {
        return 0; // Или другое значение, указывающее на ошибку
    }

    if (s1->len != s2->len)
    {
        return s1->len - s2->len;
    }
    return strcmp(s1->data, s2->data);
}

// Отношение эквивалентности между двумя String
int string_equal(const String *s1, const String *s2)
{
    if (s1 == NULL || s2 == NULL)
    {
        return 0; // Или другое значение, указывающее на ошибку
    }

    if (s1->len != s2->len)
    {
        return 0;
    }
    return strcmp(s1->data, s2->data) == 0;
}

// Копирование содержимого String в существующий String
int string_copy(String *dest, const String *src)
{
    if (dest == NULL || src == NULL)
    {
        return -1; // Ошибка
    }

    if (dest->data != NULL)
    {
        free(dest->data);
    }

    dest->len = src->len;
    dest->data = (char *)malloc(sizeof(char) * (dest->len + 1));
    if (dest->data == NULL)
    {
        dest->len = 0;
        return -1; // Ошибка выделения памяти
    }
    strcpy(dest->data, src->data);
    return 0; // Успех
}

// Копирование содержимого String в новый String
String *string_clone(const String *src)
{
    if (src == NULL)
    {
        return NULL;
    }

    String *new_str = string_create(src->data);
    return new_str;
}

// Конкатенация двух String
String *string_concat(const String *s1, const String *s2)
{
    if (s1 == NULL || s2 == NULL)
    {
        return NULL;
    }

    String *result = (String *)malloc(sizeof(String));
    if (result == NULL)
    {
        return NULL;
    }

    result->len = s1->len + s2->len;
    result->data = (char *)malloc(sizeof(char) * (result->len + 1));
    if (result->data == NULL)
    {
        free(result);
        return NULL;
    }

    strcpy(result->data, s1->data);
    strcat(result->data, s2->data);

    return result;
}

int main()
{
    String *s1 = string_create("Hello");
    String *s2 = string_create(" world");

    if (s1 != NULL && s2 != NULL)
    {
        String *s3 = string_concat(s1, s2);
        if (s3 != NULL)
        {
            printf("Concatenated string: %s\n", s3->data);
            string_destroy(s3);
        }

        String *s4 = string_clone(s1);
        if (s4 != NULL)
        {
            printf("Cloned string: %s\n", s4->data);
            string_destroy(s4);
        }

        string_destroy(s1);
        string_destroy(s2);
    }

    return 0;
}