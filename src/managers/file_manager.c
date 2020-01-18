#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ReshapeString(char **src, int new_size)
{
    char *tmp_str = (char*)malloc(new_size * sizeof(char));
    int i;
    for (i = 0; i < new_size && (*src)[i] != '\0'; i++)
    {
        tmp_str[i] = (*src)[i];
    }
    tmp_str[i] = '\0';
    free(*src);
    *src = tmp_str;
}

// function returns -1 if error occured
// else it returns number of characters written
int WriteToFile(char *file_name, char *content)
{
    FILE *file = fopen(file_name, "w");

    if (file == NULL)
        return -1;

    int counter = 0;
    char one_char = content[counter];
    while (one_char != NULL && one_char != '\0')
    {
        fputc(one_char, file);
        counter++;
        one_char = content[counter];
    }
    fclose(file);
    return counter;
}

// some code inspired by:
// https://www.geeksforgeeks.org/how-to-append-a-character-to-a-string-in-c/
char* ReadFromFile(char *file_name)
{
    FILE *file = fopen(file_name, "r");

    if (file == NULL)
        return NULL;

    int str_size = 1;
    char *content = "";
    char char_to_append = fgetc(file);

    while(char_to_append != EOF)
    {
        str_size += 1;
        ReshapeString(&content, str_size);
        strncat(content, &char_to_append, 1);
        char_to_append = fgetc(file);
    }
    // char_to_append = '\0';
    // strncat(content, &char_to_append, 1);

    fclose(file);
    return content;
}