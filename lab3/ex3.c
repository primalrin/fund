#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 256
#define MAX_LINE_LENGTH 1024

typedef struct
{
    int id;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    double salary;
} Employee;

// Вспомогательная функция для проверки, состоит ли строка только из букв
int is_alpha_string(const char *str)
{
    while (*str)
    {
        if (!isalpha(*str))
            return 0;
        str++;
    }
    return 1;
}

// Функция сравнения для qsort (по возрастанию)
int compare_asc(const void *a, const void *b)
{
    const Employee *emp1 = (const Employee *)a;
    const Employee *emp2 = (const Employee *)b;

    // Сравнение по зарплате
    if (emp1->salary < emp2->salary)
        return -1;
    if (emp1->salary > emp2->salary)
        return 1;

    // Если зарплаты равны, сравниваем по фамилии
    int last_name_cmp = strcmp(emp1->last_name, emp2->last_name);
    if (last_name_cmp != 0)
        return last_name_cmp;

    // Если фамилии равны, сравниваем по имени
    int first_name_cmp = strcmp(emp1->first_name, emp2->first_name);
    if (first_name_cmp != 0)
        return first_name_cmp;

    // Если всё равно, сравниваем по id
    return emp1->id - emp2->id;
}

// Функция сравнения для qsort (по убыванию)
int compare_desc(const void *a, const void *b)
{
    return -compare_asc(a, b);
}

// Функция для чтения данных сотрудника из строки
// Возвращает 0 в случае успеха, 1 в случае ошибки
int parse_employee_from_string(const char *line, Employee *emp)
{
    int id;
    double salary;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    int offset = 0;

    // Чтение id
    if (sscanf(line + offset, "%d%n", &id, &offset) != 1 || id < 0)
    {
        return 1;
    }

    // Чтение имени и фамилии
    int name_len = 0;
    while (line[offset + name_len] && isspace(line[offset + name_len]))
    {
        offset++;
    }

    // Чтение имени до первого пробела или конца строки
    while (line[offset + name_len] && !isspace(line[offset + name_len]) && name_len < MAX_NAME_LENGTH - 1)
    {
        first_name[name_len] = line[offset + name_len];
        name_len++;
    }
    first_name[name_len] = '\0';
    offset += name_len;

    // Пропуск пробелов между именем и фамилией
    while (line[offset] && isspace(line[offset]))
    {
        offset++;
    }

    // Чтение фамилии до конца строки
    name_len = 0;
    while (line[offset + name_len] && !isspace(line[offset + name_len]) && name_len < MAX_NAME_LENGTH - 1)
    {
        last_name[name_len] = line[offset + name_len];
        name_len++;
    }
    last_name[name_len] = '\0';
    offset += name_len;

    // Чтение зарплаты
    if (sscanf(line + offset, "%lf%n", &salary, &offset) != 1 || salary < 0)
    {
        return 1;
    }

    emp->id = id;
    strcpy(emp->first_name, first_name);
    strcpy(emp->last_name, last_name);
    emp->salary = salary;
    return 0;
}

int main(int argc, char *argv[])
{
    // Проверка количества аргументов
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s input_file -[a|d] output_file\n", argv[0]);
        return 1;
    }

    // Проверка флага
    if ((argv[2][0] != '-' && argv[2][0] != '/') ||
        (argv[2][1] != 'a' && argv[2][1] != 'd') ||
        argv[2][2] != '\0')
    {
        fprintf(stderr, "Invalid flag. Use -a or -d for ascending or descending sort\n");
        return 1;
    }

    // Открытие входного файла
    FILE *input = fopen(argv[1], "r");
    if (!input)
    {
        fprintf(stderr, "Error: Cannot open input file '%s'\n", argv[1]);
        return 1;
    }

    // Динамический массив сотрудников
    Employee *employees = NULL;
    size_t capacity = 0;
    size_t size = 0;

    // Чтение данных
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), input))
    {
        // Проверка на переполнение буфера
        if (strchr(line, '\n') == NULL && !feof(input))
        {
            fprintf(stderr, "Error: Line too long in input file\n");
            free(employees);
            fclose(input);
            return 1;
        }

        if (size >= capacity)
        {
            capacity = capacity == 0 ? 1 : capacity * 2;
            Employee *temp = realloc(employees, capacity * sizeof(Employee));
            if (!temp)
            {
                fprintf(stderr, "Error: Memory allocation error\n");
                free(employees);
                fclose(input);
                return 1;
            }
            employees = temp;
        }

        Employee emp;
        if (parse_employee_from_string(line, &emp) != 0)
        {
            fprintf(stderr, "Error: Invalid data format in input file\n");
            free(employees);
            fclose(input);
            return 1;
        }

        employees[size++] = emp;
    }

    fclose(input);

    // Сортировка
    qsort(employees, size, sizeof(Employee),
          argv[2][1] == 'a' ? compare_asc : compare_desc);

    // Запись результата
    FILE *output = fopen(argv[3], "w");
    if (!output)
    {
        fprintf(stderr, "Error: Cannot open output file '%s'\n", argv[3]);
        free(employees);
        return 1;
    }

    for (size_t i = 0; i < size; i++)
    {
        fprintf(output, "%d %s %s %.2f\n",
                employees[i].id,
                employees[i].first_name,
                employees[i].last_name,
                employees[i].salary);
    }

    fclose(output);
    free(employees);
    return 0;
}