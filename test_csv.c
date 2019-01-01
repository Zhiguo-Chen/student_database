#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <curses.h> //Biblioteca para linux
#else
#include <conio.h> //Biblioteca para windows
#endif

void _readFile();
int group_by_class();
void type_line();
void instructions();
void handle_group_by_height();
int search_by_id();
int search_by_student_id();

struct
{
    FILE *pFile;
    char *path;
    int nr;
    int class2_number;
    int class3_number;
} global = {
    NULL,
    "./test.csv", //Replace user_name with your computer username For windows use the approriate file path
    0,
    0,
    0}; // 全局变量

typedef struct person
{
    char student_id[20];
    char class[20];
    char name[20];
    char gender[20];
    char id[30];
    int height;
} Person; // struct defination

void group_by_height(Person student_group[], int group_length);

Person database[200];
char student_database[100][6][20];
Person class_group_2[100];
Person class_group_3[100];

int main()
{
    _readFile(); // read data from csv file in this dictionary
    int code = -1;

    while (code != 0)
    {
        instructions();
        scanf("%d", &code); // input command to do something
        switch (code)
        {
        case 1:
            group_by_class();
            break;
        case 2:
            handle_group_by_height();
            break;
        case 3:
            search_by_student_id();
            break;
        case 4:
            search_by_id();
            break;
        case 0:
            code = 0;
            break;
        default:
            printf("\nEnd program!!\n");
            code = 0;
            break;
        }
    }
    return 0;
}

int group_by_class() // group by class
{
    int index_2 = 0, index_3 = 0;
    for (int i = 0; i < global.nr; i++)
    {
        if (strcmp("通信工程二班", database[i].class) == 0)
        {
            printf("%s\t%s\t%s\t%s\t%s\t%d\n", database[i].student_id, database[i].class,
                   database[i].name, database[i].gender, database[i].id, database[i].height);
            class_group_2[index_2] = database[i];
            index_2++;
        }
        if (strcmp("通信工程三班", database[i].class) == 0)
        {
            printf("%s\t%s\t%s\t%s\t%s\t%d\n", database[i].student_id, database[i].class,
                   database[i].name, database[i].gender, database[i].id, database[i].height);
            class_group_3[index_3] = database[i];
            index_3++;
        }
    }
    global.class2_number = index_2;
    global.class3_number = index_3;
    return -1;
}

void handle_group_by_height() // get command from std in
{
    printf("Please group by class fisrt!\n");
    printf("Type 2 to group class 2 by height.\n");
    printf("Type 3 to group class 3 by height.\n");
    printf("Your command is: ");
    int code = -1;
    while (code != 0)
    {
        scanf("%d", &code);
        switch (code)
        {
        case 2:
            group_by_height(class_group_2, global.class2_number);
            code = 0;
            break;
        case 3:
            group_by_height(class_group_3, global.class3_number);
            code = 0;
            break;
        case 0:
            code = 0;
            break;
        default:
            printf("\nKodi i gabuar!!\n");
            code = 0;
            break;
        }
    }
}

void group_by_height(Person student_group[], int group_length) // group by height in class
{
    int heights[6];
    int heights_index = 0;
    int heights_number[6] = {0, 0, 0, 0, 0, 0};
    Person student_groups[6][30];
    for (int index = 0; index < group_length; index++)
    {
        int height_group = student_group[index].height / 10;
        if (heights_index == 0)
        {
            heights[0] = height_group;
            // heights_number[0]++;
            heights_index++;
            student_groups[0][0] = student_group[index];
        }
        else
        {
            bool is_existing = false;
            for (int _index = 0; _index < heights_index; _index++)
            {
                if (heights[_index] == height_group)
                {
                    heights_number[_index]++;
                    student_groups[_index][heights_number[_index]] = student_group[index];
                    is_existing = true;
                    break;
                }
            }
            if (!is_existing)
            {
                heights[heights_index++] = height_group;
                student_groups[heights_index][heights_number[0]] = student_group[index];
            }
        }
    }
    printf("================== Group by height ======================\n");
    for (int _heights_index = 0; _heights_index < heights_index; _heights_index++)
    {
        for (int _heights_number = 0; _heights_number < heights_number[_heights_index]; _heights_number++)
        {
            printf("%s\t%s\t%s\t%s\t%s\t%d\n", student_groups[_heights_index][_heights_number].student_id, student_groups[_heights_index][_heights_number].class,
                   student_groups[_heights_index][_heights_number].name, student_groups[_heights_index][_heights_number].gender, student_groups[_heights_index][_heights_number].id, student_groups[_heights_index][_heights_number].height);
        }
    }
    printf("================== End ======================\n");
}

void _readFile() //read data from csv file
{
    char buffer[1024];
    char storage[1024];
    char *record, *line;
    int i = 0, j = 0;
    FILE *fstream = fopen(global.path, "rb");

    while ((line = fgets(buffer, sizeof(buffer), fstream)) != NULL)
    {
        printf("*******\n");
        Person _person = {
            "",
            "",
            "",
            "",
            "",
            0};
        printf("%s\n", database[i].name);
        j = 0;
        record = strtok(line, ",");
        while (record != NULL)
        {
            strcpy(student_database[i][j], record);
            switch (j)
            {
            case 0:
                strcpy(_person.student_id, record);
                break;
            case 1:
                strcpy(_person.class, record);
                break;
            case 2:
                strcpy(_person.name, record);
                break;
            case 3:
                strcpy(_person.gender, record);
                break;
            case 4:
                strcpy(_person.id, record);
                break;
            case 5:
                // strcpy(_person.height, record);
                _person.height = atoi(record);
                break;
            }
            record = strtok(NULL, ",");
            j++;
        }
        database[i] = _person;
        ++i;
    }
    global.nr = i;
    for (int a = 0; a < global.nr; a++)
    {
        printf("%d\n", database[a].height);
    }
}

void clearstdin()
{
    while (getchar() != '\n')
    {
        continue;
    }
}

int search_by_id()
{
    Person p;
    clearstdin();
    printf("\nGive number:");
    gets(p.id);

    for (int i = 0; i < global.nr; i++)
    {
        if (strcmp(p.id, database[i].id) == 0)
        {
            printf("\nPerson found!!\n");
            printf("%s\t%s\t%s\t%s\t%s\t%d\n", database[i].student_id, database[i].class,
                   database[i].name, database[i].gender, database[i].id, database[i].height);
            return i;
        }
    }
    return -1;
}

int search_by_student_id()
{
    Person p;
    clearstdin();
    printf("\nGive number:");
    gets(p.student_id);

    for (int i = 0; i < global.nr; i++)
    {
        if (strcmp(p.student_id, database[i].student_id) == 0)
        {
            printf("\nPerson found!!\n");
            printf("%s\t%s\t%s\t%s\t%s\t%d\n", database[i].student_id, database[i].class,
                   database[i].name, database[i].gender, database[i].id, database[i].height);
            return i;
        }
    }
    return -1;
}

void instructions()
{
    printf("===============================================================\n");
    printf("                       INSTRUCTIONS\n");
    printf("===============================================================\n");
    printf("Type 1 to group by class.\n");
    printf("Type 2 to group by height in class.\n");
    printf("Type 3 to search by student ID.\n");
    printf("Type 4 to search by ID.\n");
    printf("Your command: ");
}
