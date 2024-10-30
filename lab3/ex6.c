typedef struct Timestamp
{
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;
} Timestamp;

typedef struct Stop
{
    char *transport_id;
    Timestamp arrival_time;
    Timestamp departure_time;
    char *stop_type;
    struct Stop *next;
} Stop;

typedef struct Route
{
    Stop *head;
    struct Route *next;
} Route;

typedef struct Transport
{
    char *id;
    Route *routes;
    struct Transport *next;
} Transport;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структуры данных (Timestamp, Stop, Route, Transport) - как описано ранее

// Функции для работы со временем (Timestamp)

// Парсит строку с временем в формате "dd.MM.yyyy hh:mm:ss" и заполняет структуру Timestamp
int parse_timestamp(const char *str, Timestamp *timestamp)
{
    if (sscanf(str, "%d.%d.%d %d:%d:%d", 
               ×tamp->day, ×tamp->month, ×tamp->year,
               ×tamp->hour, ×tamp->minute, ×tamp->second) != 6)
    {
        return -1; // Ошибка парсинга
    }
    return 0;
}

// Сравнивает два объекта Timestamp
int compare_timestamps(const Timestamp *t1, const Timestamp *t2)
{
    if (t1->year != t2->year)
        return t1->year - t2->year;
    if (t1->month != t2->month)
        return t1->month - t2->month;
    if (t1->day != t2->day)
        return t1->day - t2->day;
    if (t1->hour != t2->hour)
        return t1->hour - t2->hour;
    if (t1->minute != t2->minute)
        return t1->minute - t2->minute;
    return t1->second - t2->second;
}

// Функции для работы с остановками (Stop)

// Создает новую остановку, выделяя память и заполняя ее данными
Stop *create_stop(const char *transport_id, const char *arrival_time_str,
                  const char *departure_time_str, const char *stop_type)
{
    Stop *stop = (Stop *)malloc(sizeof(Stop));
    if (!stop)
        return NULL; // Ошибка выделения памяти

    stop->transport_id = strdup(transport_id);
    if (!stop->transport_id)
    {
        free(stop);
        return NULL;
    }

    if (parse_timestamp(arrival_time_str, &stop->arrival_time) != 0 ||
        parse_timestamp(departure_time_str, &stop->departure_time) != 0)
    {
        free(stop->transport_id);
        free(stop);
        return NULL;
    }

    stop->stop_type = strdup(stop_type);
    if (!stop->stop_type)
    {
        free(stop->transport_id);
        free(stop);
        return NULL;
    }

    stop->next = NULL;
    return stop;
}

// Вставляет остановку в список остановок маршрута, упорядочивая по времени прибытия
void insert_stop(Route *route, Stop *stop)
{
    if (!route->head || compare_timestamps(&stop->arrival_time, &route->head->arrival_time) <= 0)
    {
        stop->next = route->head;
        route->head = stop;
        return;
    }

    Stop *current = route->head;
    while (current->next && compare_timestamps(&stop->arrival_time, ¤t->next->arrival_time) > 0)
    {
        current = current->next;
    }
    stop->next = current->next;
    current->next = stop;
}

// Функции для работы с маршрутами (Route)

// Создает новый маршрут
Route *create_route()
{
    Route *route = (Route *)malloc(sizeof(Route));
    if (!route)
        return NULL;
    route->head = NULL;
    route->next = NULL;
    return route;
}

// Добавляет маршрут в список маршрутов транспорта
void add_route(Transport *transport, Route *route)
{
    if (!transport->routes)
    {
        transport->routes = route;
        return;
    }
    Route *current = transport->routes;
    while (current->next)
    {
        current = current->next;
    }
    current->next = route;
}

// Функции для работы с транспортом (Transport)

// Создает новую запись о транспорте
Transport *create_transport(const char *id)
{
    Transport *transport = (Transport *)malloc(sizeof(Transport));
    if (!transport)
        return NULL;
    transport->id = strdup(id);
    if (!transport->id)
    {
        free(transport);
        return NULL;
    }
    transport->routes = NULL;
    transport->next = NULL;
    return transport;
}

// Ищет транспорт по идентификатору
Transport *find_transport(Transport *head, const char *id)
{
    Transport *current = head;
    while (current)
    {
        if (strcmp(current->id, id) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Добавляет транспорт в список транспортов
void add_transport(Transport **head, Transport *transport)
{
    if (!*head)
    {
        *head = transport;
        return;
    }
    Transport *current = *head;
    while (current->next)
    {
        current = current->next;
    }
    current->next = transport;
}

// Функции для обработки файлов

// Обрабатывает один файл с данными об остановках, добавляя информацию в хранилище
int process_file(Transport **transports, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        return -1;
    }

    char transport_id[100];
    char arrival_time_str[20];
    char departure_time_str[20];
    char stop_type[20];

    while (fscanf(file, "%s %s %s %s", transport_id, arrival_time_str,
                  departure_time_str, stop_type) == 4)
    {

        Transport *transport = find_transport(*transports, transport_id);
        if (!transport)
        {
            transport = create_transport(transport_id);
            if (!transport)
            {
                fclose(file);
                return -1; // Ошибка выделения памяти
            }
            add_transport(transports, transport);
        }

        Stop *stop = create_stop(transport_id, arrival_time_str, departure_time_str, stop_type);
        if (!stop)
        {
            fclose(file);
            return -1; // Ошибка выделения памяти
        }

        // Находим или создаем маршрут
        Route *route = NULL;
        if (strcmp(stop_type, "start") == 0)
        {
            route = create_route();
            if (!route)
            {
                fclose(file);
                free(stop);
                return -1;
            }
            add_route(transport, route);
        }
        else
        {
            Route *current_route = transport->routes;
            while (current_route && !route)
            {
                Stop *last_stop = current_route->head;
                while (last_stop && last_stop->next)
                {
                    last_stop = last_stop->next;
                }
                if (last_stop && strcmp(last_stop->stop_type, "end") != 0)
                {
                    route = current_route;
                }
                current_route = current_route->next;
            }
            if (!route)
            {
                // Ошибка: промежуточная или конечная остановка без начальной
                fclose(file);
                free(stop);
                return -1;
            }
        }

        insert_stop(route, stop);
    }

    fclose(file);
    return 0;
}

// Функции для поиска

// Находит транспорт с максимальным/минимальным количеством маршрутов
Transport *find_max_min_routes(Transport *head, int max)
{
    Transport *result = NULL;
    int max_min_count = max ? 0 : 1000000; // Инициализация для max/min

    Transport *current = head;
    while (current)
    {
        int route_count = 0;
        Route *route = current->routes;
        while (route)
        {
            route_count++;
            route = route->next;
        }

        if ((max && route_count > max_min_count) || (!max && route_count < max_min_count))
        {
            max_min_count = route_count;
            result = current;
        }
        current = current->next;
    }
    return result;
}
Transport *find_longest_shortest_route(Transport *head, int longest)
{
    Transport *result = NULL;
    int max_min_stops = longest ? 0 : 1000000;

    Transport *current_transport = head;
    while (current_transport)
    {
        Route *current_route = current_transport->routes;
        while (current_route)
        {
            int stop_count = 0;
            Stop *current_stop = current_route->head;
            while (current_stop)
            {
                stop_count++;
                current_stop = current_stop->next;
            }

            if ((longest && stop_count > max_min_stops) || (!longest && stop_count < max_min_stops))
            {
                max_min_stops = stop_count;
                result = current_transport;
            }

            current_route = current_route->next;
        }
        current_transport = current_transport->next;
    }
    return result;
}

void interactive_menu(Transport *transports)
{
    int choice;

    do
    {
        printf("\nInteractive Menu:\n");
        printf("1. Find transport with max routes\n");
        printf("2. Find transport with min routes\n");
        // ... (другие пункты меню)
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            Transport *transport = find_max_min_routes(transports, 1);
            if (transport)
            {
                printf("Transport with max routes: %s (%d routes)\n", transport->id, /* количество маршрутов */);
            }
            else
            {
                printf("No transports found.\n");
            }
            break;
        }
        case 2:
        {
            Transport *transport = find_max_min_routes(transports, 0);
            if (transport)
            {
                printf("Transport with min routes: %s (%d routes)\n", transport->id, /* количество маршрутов */);
            }
            else
            {
                printf("No transports found.\n");
            }
            break;
        }
        case 3:
        {
            Transport *transport = find_longest_shortest_route(transports, 1);
            if (transport)
            {
                printf("Transport with longest route: %s (%d stops)\n", transport->id, /* количество остановок */);
            }
            else
            {
                printf("No transports found.\n");
            }
            break;
        }
        // ... (обработка других пунктов меню)
        case 0:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice.\n");
        }
    } while (choice != 0);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <file1> <file2> ...\n", argv[0]);
        return 1;
    }

    Transport *transports = NULL;

    for (int i = 1; i < argc; ++i)
    {
        if (process_file(&transports, argv[i]) != 0)
        {
            // Обработка ошибок (освобождение памяти transports)
            return 1;
        }
    }

    interactive_menu(transports);

    // ... (освобождение памяти transports)

    return 0;
}
