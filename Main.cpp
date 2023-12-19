#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <limits.h>
#include <float.h>

#define MAX_FILENAME_LEN 50 // ìàêñèìàëüíàÿ äëèíà èìåíè ôàéëà
#define MAX_USB_COUNT 1000 // ìàêñèìàëüíîå êîëè÷åñòâî USB-íàêîïèòåëåé â ìàññèâå
#define MAX_USB_PARAM_LEN 30 // ìàêñèìàëüíàÿ äëèíà ñòðîêè ðàçëè÷íûõ ïàðàìåòðîâ USB

#define ERR_FINE 0 // Êîððåêòíîå âûïîëíåíèå ïðîãðàììû
#define ERR_READ 1 // Îøèáêà ïðè ñ÷èòûâàíèè äàííûõ
#define ERR_OVERFLOW_ARRAY 2 // Îøèáêà ïðè ïåðåïîëíåíèè ìàññèâà
#define ERR_MEMORY 3 // Îøèáêà ïðè âûäåëåíèè ïàìÿòè
#define ERR_POINTER 4 // îøèáêà ïðè ïåðåäà÷å ïóñòîãî óêàçàòåëÿ
#define ERR_EMPTY_ARRAY 5 // îøèáêà ïðè ïóñòîì ìàññèâå USB
#define _CRT_SECURE_NO_WARNINGS

typedef struct
{
    int capacity; // åìêîñòü â ÃÁ
    char type[MAX_USB_PARAM_LEN]; // òèï USB (íàïðèìåð, USB 2.0, USB 3.0)
    char brand[MAX_USB_PARAM_LEN]; // áðåíä USB
} usb_t;

typedef struct
{
    usb_t arr[MAX_USB_COUNT]; // ìàññèâ USB-íàêîïèòåëåé
    int count; // êîëè÷åñòâî USB-íàêîïèòåëåé â ìàññèâå
} usbs_t;

int add_usbs(usbs_t* usbs);

int print_usbs_in_console(usbs_t* usbs);

int save_usbs_in_file(usbs_t* usbs);

int read_usbs_from_file(usbs_t* usbs, int flag_clear);

int search_usbs_by_ñapacity(usbs_t* usbs);

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
Äàííàÿ ôóíêöèÿ ñ÷èòûâàåò èç ñîîòâåòñòâóþùåãî ïîòîêà öåëîå ÷èñëî èç çàäàííîãî äèàïàçîíà
stream Óêàçàòåëü íà ïîòîê ââîäà
storage Óêàçàòåëü íà ïåðåìåííóþ, â êîòîðóþ áóäåò çàïèñàíà ñ÷èòàííàÿ åìêîñòü
message Ñîîáùåíèå, êîòîðîå áóäåò âûâåäåíî ïåðåä ñ÷èòûâàíèåì åìêîñòè
min_storage, max_storage Äèàïàçîí äîïóñòèìûõ çíà÷åíèé ñ÷èòûâàåìîé åìêîñòè
len Ìàêñèìàëüíàÿ äëèíà ñ÷èòûâàåìîé åìêîñòè
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
            printf("Â ââåäåííîé ñòðîêå ïðèñóòñòâóþò íåäîïóñòèìûå ñèìâîëû\n\n");
			system("pause");
        }
        else if (temp_str[strlen(temp_str) - 1] != '\n')
        {
            clear_input_buffer(temp_str);
            printf("Ââåäåíî ñëèøêîì ìíîãî ñèìâîëîâ\n\n");
			system("pause");
			
        }
        else if (strlen(temp_str) < 2)
        {
            clear_input_buffer(temp_str);
            printf("Ââåäåíà ïóñòàÿ ñòðîêà\n\n");
			system("pause");
			
        }
        else if (temp_str[0] == '0' && temp_str[1] != '\n')
        {
            clear_input_buffer(temp_str);
            printf("Â ñòðîêå ïðèñóòñòâóþò ëèäèðóþùèå íóëè\n\n");
			system("pause");
			
        }
        else
        {
            int temp_storage = atoi(temp_str);

            if ((temp_storage < min_storage) || (temp_storage > max_storage)){
                printf("Åìêîñòü íå âõîäèò â çàäàííûé äèàïàçîí\n\n");
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
Äàííàÿ ôóíêöèÿ ñ÷èòûâàåò èç ñîîòâåòñòâóþùåãî ïîòîêà ñòðîêó îïðåäåëåííîé äëèíû(òèï ïîäêëþ÷åíèÿ USB)
stream Óêàçàòåëü íà ïîòîê ââîäà
string Óêàçàòåëü íà ïåðåìåííóþ äëÿ õðàíåíèÿ ñòðîêè
message Ñîîáùåíèå ïðè çàïðîñå ââîäà
len Ìàêñèìàëüíàÿ äëèíà ñòðîêè
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
            printf("Ââåäåíî ñëèøêîì ìíîãî ñèìâîëîâ\n\n");
        }
        else if (strlen(temp_str) < 2)
        {
            clear_input_buffer(temp_str);
            printf("Ââåäåíà ïóñòàÿ ñòðîêà\n\n");
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
Äàííàÿ ôóíêöèÿ ñ÷èòûâàåò èç ñîîòâåòñòâóþùåãî ïîòîêà áðåíä USB-íàêîïèòåëÿ è çàïèñûâàåò åãî â ñîîòâåòñòâóþùóþ ïåðåìåííóþ
stream Óêàçàòåëü íà ïîòîê ââîäà
brand Óêàçàòåëü íà ïåðåìåííóþ äëÿ õðàíåíèÿ áðåíäà USB-íàêîïèòåëÿ
message Ñîîáùåíèå ïðè çàïðîñå ââîäà
len Ìàêñèìàëüíàÿ äëèíà áðåíäà
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
            printf("Ââåäåíî ñëèøêîì ìíîãî ñèìâîëîâ\n\n");
        }
        else if (strlen(temp_str) < 2)
        {
            clear_input_buffer(temp_str);
            printf("Ââåäåíà ïóñòàÿ ñòðîêà\n\n");
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
Äàííàÿ ôóíêöèÿ çàïðàøèâàåò êîëè÷åñòâî íîâûõ USB-íàêîïèòåëåé, ñ÷èòûâàåò è äîáàâëÿåò èõ â ñîîòâåòñòâóþùèé ìàññèâ
usbs Óêàçàòåëü íà ìàññèâ USB-íàêîïèòåëåé
*/

int add_usbs(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("Ïåðåäà÷à ïóñòîãî óêàçàòåëÿ\n");
        return ERR_POINTER;
    }

    if (usbs->count >= MAX_USB_COUNT)
    {
        printf("Ìàññèâ USB-íàêîïèòåëåé çàïîëíåí\n");
        return ERR_OVERFLOW_ARRAY;
    }

    int count;
    get_usb_storage(stdin, &count, "Ââåäèòå êîëè÷åñòâî äîáàâëÿåìûõ USB-íàêîïèòåëåé", 1, MAX_USB_COUNT - usbs->count, 4);

    for (int i = usbs->count; i < usbs->count + count; ++i)
    {
        usbs->arr[i] = read_usb_from_console();
        printf("\nUSB-íàêîïèòåëü óñïåøíî äîáàâëåí\n");
    }

    usbs->count += count;

    return ERR_FINE;
}

/*
Äàííàÿ ôóíêöèÿ ñ÷èòûâàåò USB-íàêîïèòåëü èç êîíñîëè è âîçâðàùàåò åãî â êà÷åñòâå ðåçóëüòàòà
return Ñ÷èòàííûé USB-íàêîïèòåëü
*/

usb_t read_usb_from_console()
{
    FILE* stream = stdin;

    usb_t usb;

    get_usb_storage(stream, &(usb.capacity), "Ââåäèòå åìêîñòü", 1, INT_MAX, MAX_USB_PARAM_LEN);
    get_usb_type(stream, usb.type, "Ââåäèòå òèï USB  â ôîðìàòå 'USB ×ÈÑËÎ.×ÈÑËÎ", MAX_USB_PARAM_LEN);
    get_usb_brand(stream, usb.brand, "Ââåäèòå áðåíä USB", MAX_USB_PARAM_LEN);

    return usb;
}
/*
Äàííàÿ ôóíêöèÿ âûâîäèò â êîíñîëü ñîîòâåòñòâóþùèé ìàññèâ USB-íàêîïèòåëåé
usbs Óêàçàòåëü íà ìàññèâ USB-íàêîïèòåëåé
*/

int display_usbs_in_console(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("Ïåðåäà÷à ïóñòîãî óêàçàòåëÿ\n");
        return ERR_POINTER;
    }

    if (usbs->count == 0)
    {
        printf("Äàííûå îòñóòñòâóþò\n");
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
Äàííàÿ ôóíêöèÿ âûâîäèò â êîíñîëü ñîîòâåòñòâóþùèé USB-íàêîïèòåëü
usb Óêàçàòåëü íà ïåðåìåííóþ äëÿ õðàíåíèÿ USB-íàêîïèòåëÿ
*/

int display_usb_in_console(usb_t* usb)
{
    if (!usb)
    {
        printf("Ïåðåäà÷à ïóñòîãî óêàçàòåëÿ\n");
        return ERR_POINTER;
    }

    printf("Åìêîñòü: %d\n", usb->capacity);
    printf("Òèï: %s\n", usb->type);
    printf("Áðåíä: %s\n", usb->brand);

    return ERR_FINE;
}
/*
Äàííàÿ ôóíêöèÿ çàïðàøèâàåò èìÿ âûõîäíîãî ôàéëà è çàïèñûâàåò â íåãî ñîîòâåòñòâóþùèé ìàññèâ USB-íàêîïèòåëåé
usbs Óêàçàòåëü íà ìàññèâ USB-íàêîïèòåëåé
*/

int store_usbs_in_file(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("Ïåðåäà÷à ïóñòîãî óêàçàòåëÿ\n");
        return ERR_POINTER;
    }

    if (usbs->count == 0)
    {
        printf("Äàííûå îòñóòñòâóþò\n");
        return ERR_EMPTY_ARRAY;
    }

    char filename[MAX_FILENAME_LEN + 1];

    FILE* file;

    int success = 0;

    while (!success)
    {
        get_usb_type(stdin, filename, "Ââåäèòå èìÿ âûõîäíîãî ôàéëà(â ôîðìàòå ÈÌß.ÔÎÐÌÀÒ)", MAX_FILENAME_LEN);

        file = fopen(filename, "w");

        if (!file)
            perror("Îøèáêà ïðè îòêðûòèè ôàéëà(âîçìîæíî, íåâåðíîå ìåñòîïîëîæåíèå ôàéëà èëè îïå÷àòêà â íàçâàíèè\ôîðìàòå)");
        else
            success = 1;
    }

    display_usbs_in_file(file, usbs);
    printf("Äàííûå óñïåøíî âûãðóæåíû â ôàéë\n");
    fclose(file);

    return ERR_FINE;
}

/*
Äàííàÿ ôóíêöèÿ âûâîäèò â ñîîòâåòñòâóþùèé ôàéë ìàññèâ USB-íàêîïèòåëåé
file Ôàéëîâûé óêàçàòåëü
usbs Óêàçàòåëü íà ìàññèâ USB-íàêîïèòåëåé
*/

int display_usbs_in_file(FILE* file, usbs_t* usbs)
{
    if (!file || !usbs)
    {
        printf("Ïåðåäà÷à ïóñòîãî óêàçàòåëÿ\n");
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
Äàííàÿ ôóíêöèÿ çàïðàøèâàåò èìÿ ôàéëà ñ äàííûìè, ñ÷èòûâàåò èç íåãî USB-íàêîïèòåëè è çàïîëíÿåò èìè ñîîòâåòñòâóþùèé ìàññèâ
usbs Óêàçàòåëü íà ìàññèâ USB-íàêîïèòåëåé
flag_clear Ôëàã äëÿ î÷èñòêè ïðåäûäóùèõ USB-íàêîïèòåëåé
*/

int read_usbs_from_file(usbs_t* usbs, int flag_clear)
{
    if (!usbs)
    {
        printf("Ïåðåäà÷à ïóñòîãî óêàçàòåëÿ\n");
        return ERR_POINTER;
    }

    if (flag_clear)
        usbs->count = 0;

    char filename[MAX_FILENAME_LEN + 1];

    FILE* file;

    int success = 0;

    while (!success)
    {
        get_usb_type(stdin, filename, "Ââåäèòå èìÿ ôàéëà ñ äàííûìè(â ôîðìàòå ÈÌß.ÔÎÐÌÀÒ)", MAX_FILENAME_LEN);

        file = fopen(filename, "r");

        if (!file){
			printf("Îøèáêà ïðè îòêðûòèè ôàéëà(âîçìîæíî, íåâåðíîå ìåñòîïîëîæåíèå ôàéëà èëè îïå÷àòêà â íàçâàíèè\ôîðìàòå)");
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
            printf("Ìàññèâ USB-íàêîïèòåëåé çàïîëíåí\n");
            fclose(file);
            return ERR_OVERFLOW_ARRAY;
        }
    }

    printf("Äàííûå óñïåøíî ñ÷èòàíû èç ôàéëà\n");
    fclose(file);

    return ERR_FINE;
}
/*
Äàííàÿ ôóíêöèÿ ñ÷èòûâàåò èç ôàéëà USB-íàêîïèòåëü è çàïèñûâàåò ïîëó÷åííûå äàííûå â ñîîòâåòñòâóþùóþ ïåðåìåííóþ
file Ôàéëîâûé óêàçàòåëü
usb Óêàçàòåëü íà ïåðåìåííóþ äëÿ õðàíåíèÿ èíôîðìàöèè î USB-íàêîïèòåëå
*/

int read_usb_from_file(FILE* file, usb_t* usb)
{
    if (!file || !usb)
    {
        printf("Ïåðåäà÷à ïóñòîãî óêàçàòåëÿ\n");
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
Äàííàÿ ôóíêöèÿ çàïðàøèâàåò òèï USB-íàêîïèòåëÿ, ïðîèçâîäèò ïîèñê äàííûõ USB-íàêîïèòåëåé â ñîîòâåòñòâóþùåì ìàññèâå è âûâîäèò èõ â êîíñîëü
usbs Óêàçàòåëü íà ìàññèâ USB-íàêîïèòåëåé
*/

int search_usbs_by_type(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("Ïåðåäà÷à ïóñòîãî óêàçàòåëÿ\n");
        return ERR_POINTER;
    }

    if (usbs->count == 0)
    {
        printf("Äàííûå îòñóòñòâóþò\n");
        return ERR_EMPTY_ARRAY;
    }

    char type[MAX_USB_PARAM_LEN];

    int count = 0;

    get_usb_type(stdin, type, "Ââåäèòå òèï USB â ôîðìàòå 'USB ×ÈÑËÎ.×ÈÑËÎ'", MAX_USB_PARAM_LEN);

    printf("\n");

    for (int i = 0; i < usbs->count; ++i)
        if (!strcmp(usbs->arr[i].type, type))
        {
            display_usb_in_console(usbs->arr + i);
            count++;
        }

    if (count == 0)
        printf("USB-íàêîïèòåëåé ñ òàêèì òèïîì íåò\n");

    return ERR_FINE;
}

/*
Äàííàÿ ôóíêöèÿ çàïðàøèâàåò áðåíä USB-íàêîïèòåëÿ, ïðîèçâîäèò ïîèñê äàííûõ USB-íàêîïèòåëåé â ñîîòâåòñòâóþùåì ìàññèâå è âûâîäèò èõ â êîíñîëü
usbs Óêàçàòåëü íà ìàññèâ USB-íàêîïèòåëåé
*/

int search_usbs_by_brand(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("Ïåðåäà÷à ïóñòîãî óêàçàòåëÿ\n");
        return ERR_POINTER;
    }

    if (usbs->count == 0)
    {
        printf("Äàííûå îòñóòñòâóþò\n");
        return ERR_EMPTY_ARRAY;
    }

    char brand[MAX_USB_PARAM_LEN];

    int count = 0;

    get_usb_brand(stdin, brand, "Ââåäèòå áðåíä USB", MAX_USB_PARAM_LEN);

    printf("\n");

    for (int i = 0; i < usbs->count; ++i)
        if (!strcmp(usbs->arr[i].brand, brand))
        {
            display_usb_in_console(usbs->arr + i);
            count++;
        }

    if (count == 0)
        printf("USB-íàêîïèòåëåé äàííîãî áðåíäà íåò\n");

    return ERR_FINE;
}
/*
Äàííàÿ ôóíêöèÿ çàïðàøèâàåò åìêîñòü USB-íàêîïèòåëÿ, ïðîèçâîäèò ïîèñê äàííûõ USB-íàêîïèòåëåé â ñîîòâåòñòâóþùåì ìàññèâå è âûâîäèò èõ â êîíñîëü
usbs Óêàçàòåëü íà ìàññèâ USB-íàêîïèòåëåé
*/

int search_usbs_by_ñapacity(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("Ïåðåäà÷à ïóñòîãî óêàçàòåëÿ\n");
        return ERR_POINTER;
    }

    if (usbs->count == 0)
    {
        printf("Äàííûå îòñóòñòâóþò\n");
        return ERR_EMPTY_ARRAY;
    }

    int storage;
    int count = 0;

    get_usb_storage(stdin, &storage, "Ââåäèòå åìêîñòü", 1, INT_MAX, MAX_USB_PARAM_LEN);

    printf("\n");

    for (int i = 0; i < usbs->count; ++i)
        if (usbs->arr[i].capacity == storage)
        {
            display_usb_in_console(usbs->arr + i);
            count++;
        }

    if (count == 0)
        printf("USB-íàêîïèòåëåé ñ òàêîé åìêîñòüþ íåò\n");

    return ERR_FINE;
}

/*
Äàííàÿ ôóíêöèÿ ñîðòèðóåò ñîîòâåòñòâóþùèé ìàññèâ USB-íàêîïèòåëåé ïî âîçðàñòàíèþ åìêîñòè ñ ïîìîùüþ ôóíêöèè qsort (áûñòðàÿ ñîðòèðîâêà) è âûâîäèò åãî â êîíñîëü
usbs Óêàçàòåëü íà ìàññèâ USB-íàêîïèòåëåé
*/

int sort_by_capacity_and_display_usbs(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("Ïåðåäà÷à ïóñòîãî óêàçàòåëÿ\n");
        return ERR_POINTER;
    }

    if (usbs->count == 0)
    {
        printf("Äàííûå îòñóòñòâóþò\n");
        return ERR_EMPTY_ARRAY;
    }

    qsort(usbs, usbs->count, sizeof(usb_t), usbs_cmp_capacity);
    printf("Äàííûå îòñîðòèðîâàíû!\n\n");
    display_usbs_in_console(usbs);

    return ERR_FINE;
}

/*
Äàííàÿ ôóíêöèÿ ñðàâíèâàåò åìêîñòè ñîîòâåòñòâóþùèõ USB-íàêîïèòåëåé è âîçâðàùàåò â êà÷åñòâå ðåçóëüòàòà èõ ðàçíîñòü
(> 0 - åìêîñòü ïåðâîãî USB-íàêîïèòåëÿ áîëüøå, == 0 - åìêîñòè ðàâíû, < 0 - åìêîñòü âòîðîãî USB-íàêîïèòåëÿ áîëüøå)
first Óêàçàòåëü íà ïåðâûé USB-íàêîïèòåëü
second Óêàçàòåëü íà âòîðîé USB-íàêîïèòåëü
return Ðàçíîñòü åìêîñòåé
*/

int usbs_cmp_capacity(const void* first, const void* second)
{
    usb_t first_usb = *(usb_t*)first;
    usb_t second_usb = *(usb_t*)second;
    return first_usb.capacity - second_usb.capacity;
}

/*
Äàííàÿ ôóíêöèÿ î÷èùàåò ìàññèâ USB-íàêîïèòåëåé
usbs Óêàçàòåëü íà ìàññèâ USB-íàêîïèòåëåé
*/

int clear_usbs(usbs_t* usbs)
{
    if (!usbs)
    {
        printf("Ïåðåäà÷à ïóñòîãî óêàçàòåëÿ\n");
        return ERR_POINTER;
    }

    usbs->count = 0;
    printf("Ìàññèâ USB-íàêîïèòåëåé î÷èùåí\n");

    return ERR_FINE;
}
/*
Äàííàÿ ôóíêöèÿ î÷èùàåò áóôåð
buffer Óêàçàòåëü íà áóôåð
*/

void clear_input_buffer(char* buffer)
{
    if (buffer[strlen(buffer) - 1] != '\n')
        while (getchar() != '\n');
}

/*
Äàííàÿ ôóíêöèÿ ïðîâåðÿåò, ÿâëÿåòñÿ ëè çàäàííûé ñèìâîë öèôðîé
symbol Ñèìâîë äëÿ ïðîâåðêè
*/

int is_numeric_char(char symbol)
{
    char digits[] = "0123456789";
    if (!strchr(digits, symbol))
        return 0;
    return 1;
}

/*
Äàííàÿ ôóíêöèÿ ïðîâåðÿåò, ÿâëÿåòñÿ ëè çàäàííàÿ ïîñëåäîâàòåëüíîñòü ÷èñëîì
sequence Óêàçàòåëü íà ñòðîêó
length Êîëè÷åñòâî ñèìâîëîâ â ñòðîêå
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
        printf("Ìåíþ:\n");
        printf("1. Äîáàâèòü USB-íàêîïèòåëü\n");
        printf("2. Ïîêàçàòü âñå USB-íàêîïèòåëè\n");
        printf("3. Ñîõðàíèòü èíôîðìàöèþ î USB-íàêîïèòåëÿõ â ôàéë\n");
        printf("4. Ïðî÷èòàòü èíôîðìàöèþ î USB-íàêîïèòåëÿõ èç ôàéëà (áåç î÷èñòêè òåêóùåãî ñïèñêà)\n");
        printf("5. Ïðî÷èòàòü èíôîðìàöèþ î USB-íàêîïèòåëÿõ èç ôàéëà (ñ î÷èñòêîé òåêóùåãî ñïèñêà)\n");
        printf("6. Íàéòè USB-íàêîïèòåëè ïî åìêîñòè\n");
        printf("7. Íàéòè USB-íàêîïèòåëè ïî òèïó\n");
		printf("8. Íàéòè USB-íàêîïèòåëè ïî áðåíäó\n");
        printf("9. Îòñîðòèðîâàòü è ïîêàçàòü USB-íàêîïèòåëè ïî åìêîñòè\n");
        printf("10. Î÷èñòèòü ñïèñîê USB-íàêîïèòåëåé\n");
        printf("0. Âûõîä\n");
        printf("====================================\n");

        get_usb_storage(stdin, &option, "Ââåäèòå íîìåð äåéñòâèÿ", 0, 9, 1);

        printf("\n");

        if (option == 0)
        {
            printf("Ïðîãðàììà çàâåðøåíà\n");
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
                search_usbs_by_ñapacity(&usbs);
            else if (option == 7)
                search_usbs_by_type(&usbs);
			else if (option == 8)
				search_usbs_by_brand(&usbs);
            else if (option == 9)
                sort_by_capacity_and_display_usbs(&usbs);
			else if (option == 10)
                clear_usbs(&usbs);
			
			else
				printf("Íåêîððåêòíûé ââîä");
            printf("\n");
        }
    }

    return ERR_FINE;
}

