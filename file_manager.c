#include <stdio.h>
#include <string.h>

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

    char *content = "";
    char char_to_append = fgetc(file);

    while(char_to_append != EOF)
    {
        strncat(content, &char_to_append, 1);
        char_to_append = fgetc(file);
    }

    fclose(file);
    return content;
}