#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <limits.h>
#include <float.h>

#define MAX_FILENAME_LEN 50 // ������������ ����� ����� �����
#define MAX_USB_COUNT 1000 // ������������ ���������� USB-����������� � �������
#define MAX_USB_PARAM_LEN 30 // ������������ ����� ������ ��������� ���������� USB

#define ERR_FINE 0 // ���������� ���������� ���������
#define ERR_READ 1 // ������ ��� ���������� ������
#define ERR_OVERFLOW_ARRAY 2 // ������ ��� ������������ �������
#define ERR_MEMORY 3 // ������ ��� ��������� ������
#define ERR_POINTER 4 // ������ ��� �������� ������� ���������
#define ERR_EMPTY_ARRAY 5 // ������ ��� ������ ������� USB
#define _CRT_SECURE_NO_WARNINGS

typedef struct
{
    int capacity; // ������� � ��
    char type[MAX_USB_PARAM_LEN]; // ��� USB (��������, USB 2.0, USB 3.0)
    char brand[MAX_USB_PARAM_LEN]; // ����� USB
} usb_t;

typedef struct
{
    usb_t arr[MAX_USB_COUNT]; // ������ USB-�����������
    int count; // ���������� USB-����������� � �������
} usbs_t;

int add_usbs(usbs_t* usbs);

int print_usbs_in_console(usbs_t* usbs);

int save_usbs_in_file(usbs_t* usbs);

int read_usbs_from_file(usbs_t* usbs, int flag_clear);

int search_usbs_by_�apacity(usbs_t* usbs);

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
������ ������� ��������� �� ���������������� ������ ����� ����� �� ��������� ���������
stream ��������� �� ����� �����
storage ��������� �� ����������, � ������� ����� �������� ��������� �������
message ���������, ������� ����� �������� ����� ����������� �������
min_storage, max_storage �������� ���������� �������� ����������� �������
len ������������ ����� ����������� �������
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
            printf("� ��������� ������ ������������ ������������ �������\n\n");
			system("pause");
        }
        else if (temp_str[strlen(temp_str) - 1] != '\n')
        {
            clear_input_buffer(temp_str);
            printf("������� ������� ����� ��������\n\n");
			system("pause");
			
        }
        else if (strlen(temp_str) < 2)
        {
            clear_input_buffer(temp_str);
            printf("������� ������ ������\n\n");
			system("pause");
			
        }
        else if (temp_str[0] == '0' && temp_str[1] != '\n')
        {
            clear_input_buffer(temp_str);
            printf("� ������ ������������ ���������� ����\n\n");
			system("pause");
			
        }
        else
        {
            int temp_storage = atoi(temp_str);

            if ((temp_storage < min_storage) || (temp_storage > max_storage)){
                printf("������� �� ������ � �������� ��������\n\n");
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
������ ������� ��������� �� ���������������� ������ ������ ������������ �����(��� ����������� USB)
stream ��������� �� ����� �����
string ��������� �� ���������� ��� �������� ������
message ��������� ��� ������� �����
len ������������ ����� ������
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
            printf("������� ������� ����� ��������\n\n");
        }
        else if (strlen(temp_str) < 2)
        {
            clear_input_buffer(temp_str);
            printf("������� ������ ������\n\n");
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
������ ������� ��������� �� ���������������� ������ ����� USB-���������� � ���������� ��� � ��������������� ����������
stream ��������� �� ����� �����
brand ��������� �� ���������� ��� �������� ������ USB-����������
message ��������� ��� ������� �����
len ������������ ����� ������
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
            printf("������� ������� ����� ��������\n\n");
        }
        else if (strlen(temp_str) < 2)
        {
            clear_input_buffer(temp_str);
            printf("������� ������ ������\n\n");
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
������ ������� ����������� ���������� ����� USB-�����������, ��������� � ��������� �� � ��������������� ������
usbs ��������� �� ������ USB-�����������
*/

int add_usbs(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("�������� ������� ���������\n");
        return ERR_POINTER;
    }

    if (usbs->count >= MAX_USB_COUNT)
    {
        printf("������ USB-����������� ��������\n");
        return ERR_OVERFLOW_ARRAY;
    }

    int count;
    get_usb_storage(stdin, &count, "������� ���������� ����������� USB-�����������", 1, MAX_USB_COUNT - usbs->count, 4);

    for (int i = usbs->count; i < usbs->count + count; ++i)
    {
        usbs->arr[i] = read_usb_from_console();
        printf("\nUSB-���������� ������� ��������\n");
    }

    usbs->count += count;

    return ERR_FINE;
}

/*
������ ������� ��������� USB-���������� �� ������� � ���������� ��� � �������� ����������
return ��������� USB-����������
*/

usb_t read_usb_from_console()
{
    FILE* stream = stdin;

    usb_t usb;

    get_usb_storage(stream, &(usb.capacity), "������� �������", 1, INT_MAX, MAX_USB_PARAM_LEN);
    get_usb_type(stream, usb.type, "������� ��� USB  � ������� 'USB �����.�����", MAX_USB_PARAM_LEN);
    get_usb_brand(stream, usb.brand, "������� ����� USB", MAX_USB_PARAM_LEN);

    return usb;
}
/*
������ ������� ������� � ������� ��������������� ������ USB-�����������
usbs ��������� �� ������ USB-�����������
*/

int display_usbs_in_console(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("�������� ������� ���������\n");
        return ERR_POINTER;
    }

    if (usbs->count == 0)
    {
        printf("������ �����������\n");
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
������ ������� ������� � ������� ��������������� USB-����������
usb ��������� �� ���������� ��� �������� USB-����������
*/

int display_usb_in_console(usb_t* usb)
{
    if (!usb)
    {
        printf("�������� ������� ���������\n");
        return ERR_POINTER;
    }

    printf("�������: %d\n", usb->capacity);
    printf("���: %s\n", usb->type);
    printf("�����: %s\n", usb->brand);

    return ERR_FINE;
}
/*
������ ������� ����������� ��� ��������� ����� � ���������� � ���� ��������������� ������ USB-�����������
usbs ��������� �� ������ USB-�����������
*/

int store_usbs_in_file(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("�������� ������� ���������\n");
        return ERR_POINTER;
    }

    if (usbs->count == 0)
    {
        printf("������ �����������\n");
        return ERR_EMPTY_ARRAY;
    }

    char filename[MAX_FILENAME_LEN + 1];

    FILE* file;

    int success = 0;

    while (!success)
    {
        get_usb_type(stdin, filename, "������� ��� ��������� �����(� ������� ���.������)", MAX_FILENAME_LEN);

        file = fopen(filename, "w");

        if (!file)
            perror("������ ��� �������� �����(��������, �������� �������������� ����� ��� �������� � ��������\�������)");
        else
            success = 1;
    }

    display_usbs_in_file(file, usbs);
    printf("������ ������� ��������� � ����\n");
    fclose(file);

    return ERR_FINE;
}

/*
������ ������� ������� � ��������������� ���� ������ USB-�����������
file �������� ���������
usbs ��������� �� ������ USB-�����������
*/

int display_usbs_in_file(FILE* file, usbs_t* usbs)
{
    if (!file || !usbs)
    {
        printf("�������� ������� ���������\n");
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
������ ������� ����������� ��� ����� � �������, ��������� �� ���� USB-���������� � ��������� ��� ��������������� ������
usbs ��������� �� ������ USB-�����������
flag_clear ���� ��� ������� ���������� USB-�����������
*/

int read_usbs_from_file(usbs_t* usbs, int flag_clear)
{
    if (!usbs)
    {
        printf("�������� ������� ���������\n");
        return ERR_POINTER;
    }

    if (flag_clear)
        usbs->count = 0;

    char filename[MAX_FILENAME_LEN + 1];

    FILE* file;

    int success = 0;

    while (!success)
    {
        get_usb_type(stdin, filename, "������� ��� ����� � �������(� ������� ���.������)", MAX_FILENAME_LEN);

        file = fopen(filename, "r");

        if (!file){
			printf("������ ��� �������� �����(��������, �������� �������������� ����� ��� �������� � ��������\�������)");
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
            printf("������ USB-����������� ��������\n");
            fclose(file);
            return ERR_OVERFLOW_ARRAY;
        }
    }

    printf("������ ������� ������� �� �����\n");
    fclose(file);

    return ERR_FINE;
}
/*
������ ������� ��������� �� ����� USB-���������� � ���������� ���������� ������ � ��������������� ����������
file �������� ���������
usb ��������� �� ���������� ��� �������� ���������� � USB-����������
*/

int read_usb_from_file(FILE* file, usb_t* usb)
{
    if (!file || !usb)
    {
        printf("�������� ������� ���������\n");
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
������ ������� ����������� ��� USB-����������, ���������� ����� ������ USB-����������� � ��������������� ������� � ������� �� � �������
usbs ��������� �� ������ USB-�����������
*/

int search_usbs_by_type(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("�������� ������� ���������\n");
        return ERR_POINTER;
    }

    if (usbs->count == 0)
    {
        printf("������ �����������\n");
        return ERR_EMPTY_ARRAY;
    }

    char type[MAX_USB_PARAM_LEN];

    int count = 0;

    get_usb_type(stdin, type, "������� ��� USB � ������� 'USB �����.�����'", MAX_USB_PARAM_LEN);

    printf("\n");

    for (int i = 0; i < usbs->count; ++i)
        if (!strcmp(usbs->arr[i].type, type))
        {
            display_usb_in_console(usbs->arr + i);
            count++;
        }

    if (count == 0)
        printf("USB-����������� � ����� ����� ���\n");

    return ERR_FINE;
}

/*
������ ������� ����������� ����� USB-����������, ���������� ����� ������ USB-����������� � ��������������� ������� � ������� �� � �������
usbs ��������� �� ������ USB-�����������
*/

int search_usbs_by_brand(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("�������� ������� ���������\n");
        return ERR_POINTER;
    }

    if (usbs->count == 0)
    {
        printf("������ �����������\n");
        return ERR_EMPTY_ARRAY;
    }

    char brand[MAX_USB_PARAM_LEN];

    int count = 0;

    get_usb_brand(stdin, brand, "������� ����� USB", MAX_USB_PARAM_LEN);

    printf("\n");

    for (int i = 0; i < usbs->count; ++i)
        if (!strcmp(usbs->arr[i].brand, brand))
        {
            display_usb_in_console(usbs->arr + i);
            count++;
        }

    if (count == 0)
        printf("USB-����������� ������� ������ ���\n");

    return ERR_FINE;
}
/*
������ ������� ����������� ������� USB-����������, ���������� ����� ������ USB-����������� � ��������������� ������� � ������� �� � �������
usbs ��������� �� ������ USB-�����������
*/

int search_usbs_by_�apacity(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("�������� ������� ���������\n");
        return ERR_POINTER;
    }

    if (usbs->count == 0)
    {
        printf("������ �����������\n");
        return ERR_EMPTY_ARRAY;
    }

    int storage;
    int count = 0;

    get_usb_storage(stdin, &storage, "������� �������", 1, INT_MAX, MAX_USB_PARAM_LEN);

    printf("\n");

    for (int i = 0; i < usbs->count; ++i)
        if (usbs->arr[i].capacity == storage)
        {
            display_usb_in_console(usbs->arr + i);
            count++;
        }

    if (count == 0)
        printf("USB-����������� � ����� �������� ���\n");

    return ERR_FINE;
}

/*
������ ������� ��������� ��������������� ������ USB-����������� �� ����������� ������� � ������� ������� qsort (������� ����������) � ������� ��� � �������
usbs ��������� �� ������ USB-�����������
*/

int sort_by_capacity_and_display_usbs(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("�������� ������� ���������\n");
        return ERR_POINTER;
    }

    if (usbs->count == 0)
    {
        printf("������ �����������\n");
        return ERR_EMPTY_ARRAY;
    }

    qsort(usbs, usbs->count, sizeof(usb_t), usbs_cmp_capacity);
    printf("������ �������������!\n\n");
    display_usbs_in_console(usbs);

    return ERR_FINE;
}

/*
������ ������� ���������� ������� ��������������� USB-����������� � ���������� � �������� ���������� �� ��������
(> 0 - ������� ������� USB-���������� ������, == 0 - ������� �����, < 0 - ������� ������� USB-���������� ������)
first ��������� �� ������ USB-����������
second ��������� �� ������ USB-����������
return �������� ��������
*/

int usbs_cmp_capacity(const void* first, const void* second)
{
    usb_t first_usb = *(usb_t*)first;
    usb_t second_usb = *(usb_t*)second;
    return first_usb.capacity - second_usb.capacity;
}

/*
������ ������� ������� ������ USB-�����������
usbs ��������� �� ������ USB-�����������
*/

int clear_usbs(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("�������� ������� ���������\n");
        return ERR_POINTER;
    }

    usbs->count = 0;
    printf("������ USB-����������� ������\n");

    return ERR_FINE;
}
/*
������ ������� ������� �����
buffer ��������� �� �����
*/

void clear_input_buffer(char* buffer)
{
    if (buffer[strlen(buffer) - 1] != '\n')
        while (getchar() != '\n');
}

/*
������ ������� ���������, �������� �� �������� ������ ������
symbol ������ ��� ��������
*/

int is_numeric_char(char symbol)
{
    char digits[] = "0123456789";
    if (!strchr(digits, symbol))
        return 0;
    return 1;
}

/*
������ ������� ���������, �������� �� �������� ������������������ ������
sequence ��������� �� ������
length ���������� �������� � ������
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
        printf("����:\n");
        printf("1. �������� USB-����������\n");
        printf("2. �������� ��� USB-����������\n");
        printf("3. ��������� ���������� � USB-����������� � ����\n");
        printf("4. ��������� ���������� � USB-����������� �� ����� (��� ������� �������� ������)\n");
        printf("5. ��������� ���������� � USB-����������� �� ����� (� �������� �������� ������)\n");
        printf("6. ����� USB-���������� �� �������\n");
        printf("7. ����� USB-���������� �� ����\n");
		printf("8. ����� USB-���������� �� ������\n");
        printf("9. ������������� � �������� USB-���������� �� �������\n");
        printf("10. �������� ������ USB-�����������\n");
        printf("0. �����\n");
        printf("====================================\n");

        get_usb_storage(stdin, &option, "������� ����� ��������", 0, 9, 1);

        printf("\n");

        if (option == 0)
        {
            printf("��������� ���������\n");
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
                search_usbs_by_�apacity(&usbs);
            else if (option == 7)
                search_usbs_by_type(&usbs);
			else if (option == 8)
				search_usbs_by_brand(&usbs);
            else if (option == 9)
                sort_by_capacity_and_display_usbs(&usbs);
			else if (option == 10)
                clear_usbs(&usbs);
			
			else
				printf("������������ ����");
            printf("\n");
        }
    }

    return ERR_FINE;
}

