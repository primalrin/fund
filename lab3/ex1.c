#include <stdio.h>
#include <stdint.h>

// Функция для получения маски для извлечения r бит
uint32_t get_mask(uint32_t r)
{
    return ~(~0u << r);
}

// Битовая операция сложения
uint32_t bit_add(uint32_t a, uint32_t b)
{
    uint32_t carry;
    while (b)
    {
        carry = a & b;
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}

// Битовая операция вычитания
uint32_t bit_sub(uint32_t a, uint32_t b)
{
    return bit_add(a, bit_add(~b, 1));
}

// Функция получения символа для цифры в нужной системе счисления
char get_digit_char(uint32_t digit, uint32_t r)
{
    const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
    return digits[digit & get_mask(r)];
}

// Функция подсчёта необходимой длины для числа в системе с основанием 2^r
uint32_t get_length(uint32_t number, uint32_t r)
{
    uint32_t len = 1; // Минимальная длина 1 для нуля
    uint32_t mask = get_mask(r);
    while (number)
    {
        len = bit_add(len, 1);
        number = number >> r;
    }
    return len;
}

void convert_to_base_2r(uint32_t number, uint32_t r, char *result)
{
    if ((r == 0) || ((r & get_mask(3)) > 5))
    {
        result[0] = '\0';
        return;
    }

    // Если число 0, возвращаем "0"
    if (!number)
    {
        result[0] = '0';
        result[1] = '\0';
        return;
    }

    uint32_t mask = get_mask(r);
    uint32_t len = get_length(number, r);
    uint32_t pos = 0;

    // Добавляем ведущие нули в зависимости от основания
    uint32_t min_len;
    switch (r)
    {
    case 1:
        min_len = 8;
        break; // Для двоичной системы
    case 2:
        min_len = 4;
        break; // Для четверичной системы
    case 3:
        min_len = 3;
        break; // Для восьмеричной системы
    case 4:
        min_len = 2;
        break; // Для шестнадцатеричной системы
    default:
        min_len = 2;
        break; // Для системы с основанием 32
    }

    // Если длина меньше минимальной, добавляем нули
    while (len < min_len)
    {
        len = bit_add(len, 1);
    }

    // Заполняем ведущими нулями
    while (pos < bit_sub(len, get_length(number, r)))
    {
        result[pos] = '0';
        pos = bit_add(pos, 1);
    }

    // Заполняем значащими цифрами
    uint32_t temp = number;
    uint32_t digits[32]; // Временный массив для цифр
    uint32_t digit_count = 0;

    while (temp)
    {
        digits[digit_count] = temp & mask;
        temp = temp >> r;
        digit_count = bit_add(digit_count, 1);
    }

    // Записываем цифры в обратном порядке
    while (digit_count)
    {
        digit_count = bit_sub(digit_count, 1);
        result[pos] = get_digit_char(digits[digit_count], r);
        pos = bit_add(pos, 1);
    }

    result[pos] = '\0';
}

void demonstrate_conversion(uint32_t number)
{
    char result[33];
    printf("Number: %u\n", number);

    uint32_t r = 1;
    while (r < 6)
    {
        convert_to_base_2r(number, r, result);
        printf("Base 2^%u (=%u): %s\n", r, 1u << r, result);
        r = bit_add(r, 1);
    }
    printf("\n");
}

int main()
{
    demonstrate_conversion(255);
    demonstrate_conversion(1000);
    demonstrate_conversion(0);
    demonstrate_conversion(65535);

    return 0;
}