#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <limits.h>
#include <float.h>

#define MAX_FILENAME_LEN 50 // максимальная длина имени файла
#define MAX_USB_COUNT 1000 // максимальное количество USB-накопителей в массиве
#define MAX_USB_PARAM_LEN 30 // максимальная длина строки различных параметров USB

#define ERR_FINE 0 // Корректное выполнение программы
#define ERR_READ 1 // Ошибка при считывании данных
#define ERR_OVERFLOW_ARRAY 2 // Ошибка при переполнении массива
#define ERR_MEMORY 3 // Ошибка при выделении памяти
#define ERR_POINTER 4 // ошибка при передаче пустого указателя
#define ERR_EMPTY_ARRAY 5 // ошибка при пустом массиве USB
#define _CRT_SECURE_NO_WARNINGS

typedef struct
{
    int capacity; // емкость в ГБ
    char type[MAX_USB_PARAM_LEN]; // тип USB (например, USB 2.0, USB 3.0)
    char brand[MAX_USB_PARAM_LEN]; // бренд USB
} usb_t;

typedef struct
{
    usb_t arr[MAX_USB_COUNT]; // массив USB-накопителей
    int count; // количество USB-накопителей в массиве
} usbs_t;

int add_usbs(usbs_t* usbs);

int print_usbs_in_console(usbs_t* usbs);

int save_usbs_in_file(usbs_t* usbs);

int read_usbs_from_file(usbs_t* usbs, int flag_clear);

int search_usbs_by_сapacity(usbs_t* usbs);

int search_usbs_by_type(usbs_t* usbs);

int sort_by_capacity_and_print_usbs(usbs_t* usbs);

int clear_usbs(usbs_t* usbs);

int get_usb_storage(FILE* stream, int* storage, char* message, int min_storage, int max_storage, size_t len);

int get_usb_type(FILE* stream, char* type, char* message, size_t len);

int get_usb_brand(FILE* stream, char* brand, char* message, size_t len);

int read_usb_from_file(FILE* file, usb_t* usb);

int display_usb_in_console(usb_t* usb);

int usbs_cmp_capacity(const void* first, const void* second);

usb_t read_usb_from_console();

int display_usbs_in_file(FILE* file, usbs_t* usbs);

int is_numeric_sequence(char* sequence, int length);

void clear_input_buffer(char* buffer);


/*
Данная функция считывает из соответствующего потока целое число из заданного диапазона
stream Указатель на поток ввода
storage Указатель на переменную, в которую будет записана считанная емкость
message Сообщение, которое будет выведено перед считыванием емкости
min_storage, max_storage Диапазон допустимых значений считываемой емкости
len Максимальная длина считываемой емкости
*/

int get_usb_storage(FILE* stream, int* storage, char* message, int min_storage, int max_storage, size_t len)
{
    int success = 0;

    char* temp_str = (char*)malloc(sizeof(char) * (len + 2));

    if (!temp_str)
        return ERR_MEMORY;

    while (!success)
    {
        printf("%s: ", message);

        fgets(temp_str, len + 2, stream);

        if (!is_numeric_sequence(temp_str, strlen(temp_str) - 1))
        {
            clear_input_buffer(temp_str);
            printf("В введенной строке присутствуют недопустимые символы\n\n");
			system("pause");
        }
        else if (temp_str[strlen(temp_str) - 1] != '\n')
        {
            clear_input_buffer(temp_str);
            printf("Введено слишком много символов\n\n");
			system("pause");
			
        }
        else if (strlen(temp_str) < 2)
        {
            clear_input_buffer(temp_str);
            printf("Введена пустая строка\n\n");
			system("pause");
			
        }
        else if (temp_str[0] == '0' && temp_str[1] != '\n')
        {
            clear_input_buffer(temp_str);
            printf("В строке присутствуют лидирующие нули\n\n");
			system("pause");
			
        }
        else
        {
            int temp_storage = atoi(temp_str);

            if ((temp_storage < min_storage) || (temp_storage > max_storage)){
                printf("Емкость не входит в заданный диапазон\n\n");
				system("pause");
				
			}
            else
            {
                *storage = temp_storage;
                success = 1;
            }
        }
    }

    free(temp_str);

    return ERR_FINE;
}
/*
Данная функция считывает из соответствующего потока строку определенной длины(тип подключения USB)
stream Указатель на поток ввода
string Указатель на переменную для хранения строки
message Сообщение при запросе ввода
len Максимальная длина строки
*/


int get_usb_type(FILE* stream, char* type, char* message, size_t len)
{
    int success = 0;

    char* temp_str = (char*)malloc(sizeof(char) * (len + 2));

    if (!temp_str)
        return ERR_MEMORY;

    while (!success)
    {
        printf("%s: ", message);

        fgets(temp_str, len + 2, stream);

        if (temp_str[strlen(temp_str) - 1] != '\n')
        {
            clear_input_buffer(temp_str);
            printf("Введено слишком много символов\n\n");
        }
        else if (strlen(temp_str) < 2)
        {
            clear_input_buffer(temp_str);
            printf("Введена пустая строка\n\n");
        }
        else
        {
            temp_str[strlen(temp_str) - 1] = '\0';
            strcpy(type, temp_str);
            success = 1;
        }
    }

    free(temp_str);

    return ERR_FINE;
}

/*
Данная функция считывает из соответствующего потока бренд USB-накопителя и записывает его в соответствующую переменную
stream Указатель на поток ввода
brand Указатель на переменную для хранения бренда USB-накопителя
message Сообщение при запросе ввода
len Максимальная длина бренда
*/

int get_usb_brand(FILE* stream, char* brand, char* message, size_t len)
{
    int success = 0;

    char* temp_str = (char*)malloc(sizeof(char) * (len + 2));

    if (!temp_str)
        return ERR_MEMORY;

    while (!success)
    {
        printf("%s: ", message);

        fgets(temp_str, len + 2, stream);

        if (temp_str[strlen(temp_str) - 1] != '\n')
        {
            clear_input_buffer(temp_str);
            printf("Введено слишком много символов\n\n");
        }
        else if (strlen(temp_str) < 2)
        {
            clear_input_buffer(temp_str);
            printf("Введена пустая строка\n\n");
        }
        else
        {
            temp_str[strlen(temp_str) - 1] = '\0';
            strcpy(brand, temp_str);
            success = 1;
        }
    }

    free(temp_str);

    return ERR_FINE;
}

/*
Данная функция запрашивает количество новых USB-накопителей, считывает и добавляет их в соответствующий массив
usbs Указатель на массив USB-накопителей
*/

int add_usbs(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("Передача пустого указателя\n");
        return ERR_POINTER;
    }

    if (usbs->count >= MAX_USB_COUNT)
    {
        printf("Массив USB-накопителей заполнен\n");
        return ERR_OVERFLOW_ARRAY;
    }

    int count;
    get_usb_storage(stdin, &count, "Введите количество добавляемых USB-накопителей", 1, MAX_USB_COUNT - usbs->count, 4);

    for (int i = usbs->count; i < usbs->count + count; ++i)
    {
        usbs->arr[i] = read_usb_from_console();
		printf("\n\nUSB-накопитель:\n");
		printf("---------------------------------\n");
		display_usb_in_console(usbs->arr+i);
		printf("---------------------------------\n");
		printf("Успешно добавлен в базу данных\n\n");
    }

    usbs->count += count;


    return ERR_FINE;
}

/*
Данная функция считывает USB-накопитель из консоли и возвращает его в качестве результата
return Считанный USB-накопитель
*/

usb_t read_usb_from_console()
{
    FILE* stream = stdin;

    usb_t usb;

    get_usb_storage(stream, &(usb.capacity), "Введите емкость", 1, INT_MAX, MAX_USB_PARAM_LEN);
    get_usb_type(stream, usb.type, "Введите тип USB  в формате 'USB ЧИСЛО.ЧИСЛО", MAX_USB_PARAM_LEN);
    get_usb_brand(stream, usb.brand, "Введите бренд USB", MAX_USB_PARAM_LEN);

    return usb;
}
/*
Данная функция выводит в консоль соответствующий массив USB-накопителей
usbs Указатель на массив USB-накопителей
*/

int display_usbs_in_console(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("Передача пустого указателя\n");
        return ERR_POINTER;
    }

    if (usbs->count == 0)
    {
        printf("Данные отсутствуют\n");
        return ERR_EMPTY_ARRAY;
    }

    printf("---------------------------------\n");
    for (int i = 0; i < usbs->count; ++i)
    {
        display_usb_in_console(usbs->arr + i);
        printf("---------------------------------\n");
    }

    return ERR_FINE;
}

/*
Данная функция выводит в консоль соответствующий USB-накопитель
usb Указатель на переменную для хранения USB-накопителя
*/

int display_usb_in_console(usb_t* usb)
{
    if (!usb)
    {
        printf("Передача пустого указателя\n");
        return ERR_POINTER;
    }

    printf("Емкость: %d\n", usb->capacity);
    printf("Тип: %s\n", usb->type);
    printf("Бренд: %s\n", usb->brand);

    return ERR_FINE;
}
/*
Данная функция запрашивает имя выходного файла и записывает в него соответствующий массив USB-накопителей
usbs Указатель на массив USB-накопителей
*/

int store_usbs_in_file(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("Передача пустого указателя\n");
        return ERR_POINTER;
    }

    if (usbs->count == 0)
    {
        printf("Данные отсутствуют\n");
        return ERR_EMPTY_ARRAY;
    }

    char filename[MAX_FILENAME_LEN + 1];

    FILE* file;

    int success = 0;

    while (!success)
    {
        get_usb_type(stdin, filename, "Введите имя выходного файла(в формате ИМЯ.ФОРМАТ)", MAX_FILENAME_LEN);

        file = fopen(filename, "w");

        if (!file)
            perror("Ошибка при открытии файла(возможно, неверное местоположение файла или опечатка в названии\формате)");
        else
            success = 1;
    }

    display_usbs_in_file(file, usbs);
    printf("Данные успешно выгружены в файл\n");
    fclose(file);

    return ERR_FINE;
}

/*
Данная функция выводит в соответствующий файл массив USB-накопителей
file Файловый указатель
usbs Указатель на массив USB-накопителей
*/

int display_usbs_in_file(FILE* file, usbs_t* usbs)
{
    if (!file || !usbs)
    {
        printf("Передача пустого указателя\n");
        return ERR_POINTER;
    }

    for (int i = 0; i < usbs->count; ++i)
    {
        fprintf(file, "%d\n", usbs->arr[i].capacity);
        fprintf(file, "%s\n", usbs->arr[i].type);
        fprintf(file, "%s\n", usbs->arr[i].brand);
    }

    return ERR_FINE;
}

/*
Данная функция запрашивает имя файла с данными, считывает из него USB-накопители и заполняет ими соответствующий массив
usbs Указатель на массив USB-накопителей
flag_clear Флаг для очистки предыдущих USB-накопителей
*/

int read_usbs_from_file(usbs_t* usbs, int flag_clear)
{
    if (!usbs)
    {
        printf("Передача пустого указателя\n");
        return ERR_POINTER;
    }

    if (flag_clear)
        usbs->count = 0;

    char filename[MAX_FILENAME_LEN + 1];

    FILE* file;

    int success = 0;

    while (!success)
    {
        get_usb_type(stdin, filename, "Введите имя файла с данными(в формате ИМЯ.ФОРМАТ)", MAX_FILENAME_LEN);

        file = fopen(filename, "r");

        if (!file){
			printf("Ошибка при открытии файла(возможно, неверное местоположение файла или опечатка в названии\формате)");
		}
        else
            success = 1;
    }

    printf("\n");

    usb_t usb;

    while (!feof(file))
    {
        read_usb_from_file(file, &usb);

        if (usbs->count < MAX_USB_COUNT)
        {
            usbs->arr[usbs->count] = usb;
            usbs->count++;
        }
        else
        {
            printf("Массив USB-накопителей заполнен\n");
            fclose(file);
            return ERR_OVERFLOW_ARRAY;
        }
    }

    printf("Данные успешно считаны из файла\n");
    fclose(file);

    return ERR_FINE;
}
/*
Данная функция считывает из файла USB-накопитель и записывает полученные данные в соответствующую переменную
file Файловый указатель
usb Указатель на переменную для хранения информации о USB-накопителе
*/

int read_usb_from_file(FILE* file, usb_t* usb)
{
    if (!file || !usb)
    {
        printf("Передача пустого указателя\n");
        return ERR_POINTER;
    }

    fscanf(file, "%d\n", &(usb->capacity));

    char temp_str[MAX_USB_PARAM_LEN + 2];
    fgets(temp_str, sizeof(temp_str), file);
    temp_str[strlen(temp_str) - 1] = '\0';
    strcpy(usb->type, temp_str);

    fgets(temp_str, sizeof(temp_str), file);
    temp_str[strlen(temp_str) - 1] = '\0';
    strcpy(usb->brand, temp_str);

    fscanf(file, "\n");

    return ERR_FINE;
}

/*
Данная функция запрашивает тип USB-накопителя, производит поиск данных USB-накопителей в соответствующем массиве и выводит их в консоль
usbs Указатель на массив USB-накопителей
*/

int search_usbs_by_type(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("Передача пустого указателя\n");
        return ERR_POINTER;
    }

    if (usbs->count == 0)
    {
        printf("Данные отсутствуют\n");
        return ERR_EMPTY_ARRAY;
    }

    char type[MAX_USB_PARAM_LEN];

    int count = 0;

    get_usb_type(stdin, type, "Введите тип USB в формате 'USB ЧИСЛО.ЧИСЛО'", MAX_USB_PARAM_LEN);

    printf("---------------------------------\n");

    for (int i = 0; i < usbs->count; ++i)
        if (!strcmp(usbs->arr[i].type, type))
        {
            display_usb_in_console(usbs->arr + i);
			printf("---------------------------------\n");
            count++;
        }

    if (count == 0)
        printf("USB-накопителей с таким типом нет\n---------------------------------\n");
	else if (count > 9 && count < 21) 
		 printf("Найдено %d накопителей, удовлетворяющих типу %s \n",count,type);
	else if (count%10 == 1)
		printf("Найден %d накопитель, удовлетворяющий типу %s \n",count,type);
	else if (count%10 > 1 && count%10 < 5)
		printf("Найдено %d накопителя, удовлетворяющих типу %s \n",count,type);
	else printf("Найдено %d накопителей, удовлетворяющих типу %s \n",count,type);
    return ERR_FINE;
}

/*
Данная функция запрашивает бренд USB-накопителя, производит поиск данных USB-накопителей в соответствующем массиве и выводит их в консоль
usbs Указатель на массив USB-накопителей
*/

int search_usbs_by_brand(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("Передача пустого указателя\n");
        return ERR_POINTER;
    }

    if (usbs->count == 0)
    {
        printf("Данные отсутствуют\n");
        return ERR_EMPTY_ARRAY;
    }

    char brand[MAX_USB_PARAM_LEN];

    int count = 0;

    get_usb_brand(stdin, brand, "Введите бренд USB", MAX_USB_PARAM_LEN);

    printf("---------------------------------\n");

    for (int i = 0; i < usbs->count; ++i)
        if (!strcmp(usbs->arr[i].brand, brand))
        {
            display_usb_in_console(usbs->arr + i);
			printf("---------------------------------\n");
            count++;
        }

    if (count == 0)
        printf("USB-накопителей данного бренда нет\n---------------------------------\n");
	else if (count > 9 && count < 21) 
		 printf("Найдено %d накопителей бренда %s \n",count,brand);
	else if (count%10 == 1)
		printf("Найден %d накопитель бренда %s \n",count,brand);
	else if (count%10 > 1 && count%10 < 5)
		printf("Найдено %d накопителя бренда %s \n",count,brand);
	else printf("Найдено %d накопителей бренда %s \n",count,brand);
    return ERR_FINE;
}
/*
Данная функция запрашивает емкость USB-накопителя, производит поиск данных USB-накопителей в соответствующем массиве и выводит их в консоль
usbs Указатель на массив USB-накопителей
*/

int search_usbs_by_сapacity(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("Передача пустого указателя\n");
        return ERR_POINTER;
    }

    if (usbs->count == 0)
    {
        printf("Данные отсутствуют\n");
        return ERR_EMPTY_ARRAY;
    }

    int storage;
    int count = 0;

    get_usb_storage(stdin, &storage, "Введите емкость", 1, INT_MAX, MAX_USB_PARAM_LEN);

    printf("---------------------------------\n");

    for (int i = 0; i < usbs->count; ++i)
        if (usbs->arr[i].capacity == storage)
        {
            display_usb_in_console(usbs->arr + i);
			printf("---------------------------------\n");
            count++;
        }

    if (count == 0)
        printf("USB-накопителей с такой емкостью нет\n---------------------------------\n");
	else if (count > 9 && count < 21) 
		 printf("Найдено %d накопителей с данной емокстью \n",count);
	else if (count%10 == 1)
		printf("Найден %d накопитель с данной емкостью \n",count);
	else if (count%10 > 1 && count%10 < 5)
		printf("Найдено %d накопителя с данной емкостью\n",count);
    return ERR_FINE;
}

/*
Данная функция сортирует соответствующий массив USB-накопителей по возрастанию емкости с помощью функции qsort (быстрая сортировка) и выводит его в консоль
usbs Указатель на массив USB-накопителей
*/

int sort_by_capacity_and_display_usbs(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("Передача пустого указателя\n");
        return ERR_POINTER;
    }

    if (usbs->count == 0)
    {
        printf("Данные отсутствуют\n");
        return ERR_EMPTY_ARRAY;
    }

    qsort(usbs, usbs->count, sizeof(usb_t), usbs_cmp_capacity);
    printf("Данные отсортированы!\n\n");
    display_usbs_in_console(usbs);

    return ERR_FINE;
}

/*
Данная функция сравнивает емкости соответствующих USB-накопителей и возвращает в качестве результата их разность
(> 0 - емкость первого USB-накопителя больше, == 0 - емкости равны, < 0 - емкость второго USB-накопителя больше)
first Указатель на первый USB-накопитель
second Указатель на второй USB-накопитель
return Разность емкостей
*/

int usbs_cmp_capacity(const void* first, const void* second)
{
    usb_t first_usb = *(usb_t*)first;
    usb_t second_usb = *(usb_t*)second;
    return first_usb.capacity - second_usb.capacity;
}

/*
Данная функция очищает массив USB-накопителей
usbs Указатель на массив USB-накопителей
*/

int clear_usbs(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("Передача пустого указателя\n");
        return ERR_POINTER;
    }

    usbs->count = 0;
    printf("Массив USB-накопителей очищен\n");

    return ERR_FINE;
}
/*
Данная функция очищает буфер
buffer Указатель на буфер
*/

void clear_input_buffer(char* buffer)
{
    if (buffer[strlen(buffer) - 1] != '\n')
        while (getchar() != '\n');
}

/*
Данная функция проверяет, является ли заданный символ цифрой
symbol Символ для проверки
*/

int is_numeric_char(char symbol)
{
    char digits[] = "0123456789";
    if (!strchr(digits, symbol))
        return 0;
    return 1;
}

/*
Данная функция проверяет, является ли заданная последовательность числом
sequence Указатель на строку
length Количество символов в строке
*/

int is_numeric_sequence(char* sequence, int length)
{
    int is_numeric = 1;

    for (int i = 0; i < length && is_numeric; ++i)
    {
        if (*sequence && is_numeric_char(*sequence))
            sequence++;
        else
            is_numeric = 0;
    }

    return is_numeric;
}
int main()
{
    printf("\n");
    setlocale(LC_ALL, "");
    usbs_t usbs = {{0}, 0};

    int option;
    int flag_menu = 1;

    while (flag_menu)
    {
        printf("====================================\n");
        printf("Меню:\n");
        printf("1. Добавить USB-накопитель\n");
        printf("2. Показать все USB-накопители\n");
        printf("3. Сохранить информацию о USB-накопителях в файл\n");
        printf("4. Прочитать информацию о USB-накопителях из файла (без очистки текущего списка)\n");
        printf("5. Прочитать информацию о USB-накопителях из файла (с очисткой текущего списка)\n");
        printf("6. Найти USB-накопители по емкости\n");
        printf("7. Найти USB-накопители по типу\n");
		printf("8. Найти USB-накопители по бренду\n");
        printf("9. Отсортировать и показать USB-накопители по емкости\n");
        printf("10. Очистить список USB-накопителей\n");
        printf("0. Выход\n");
        printf("====================================\n");

        get_usb_storage(stdin, &option, "Введите номер действия", 0, 9, 1);

        printf("\n");

        if (option == 0)
        {
            printf("Программа завершена\n");
            flag_menu = 0;
        }
        else
        {
            if (option == 1)
                add_usbs(&usbs);
            else if (option == 2)
                display_usbs_in_console(&usbs);
            else if (option == 3)
                store_usbs_in_file(&usbs);
            else if (option == 4)
                read_usbs_from_file(&usbs, 0);
            else if (option == 5)
                read_usbs_from_file(&usbs, 1);
            else if (option == 6)
                search_usbs_by_сapacity(&usbs);
            else if (option == 7)
                search_usbs_by_type(&usbs);
			else if (option == 8)
				search_usbs_by_brand(&usbs);
            else if (option == 9)
                sort_by_capacity_and_display_usbs(&usbs);
			else if (option == 10)
                clear_usbs(&usbs);
			
			else
				printf("Некорректный ввод");
            printf("\n");
        }
    }

    return ERR_FINE;
}
