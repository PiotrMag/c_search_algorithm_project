#include <stdio.h>
#include <windows.h>
#include "characters.h"
#include "console_colors.h"
// #include "colors.h"

// some code taken from:
// https://stackoverflow.com/questions/20725769/change-background-color-of-console-output-in-c-and-c

int main()
{
    unsigned char b, f;

    for (b = 0; b < 16; b++)
    {
        for (f = 0; f < 16; f++)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), b << 4 | f);
            printf("%.2X", b << 4 | f);
        }
        printf("\n");
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10 << 4 | 11);
    printf("%.2X", b << 4 | f);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);

    printf("\n");

    int i = 0;
    for (i = 0; i < 16; i++) 
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0<<4 | i);
        printf("NR.%2d\n", i);
    }

    for (i = 0; i < 256; i++) 
    {
        printf("NR.%3d = %2c\n", i, i);
    }

    printf("%c\n", BLACK_BOX);
    printf("%c\n", DARK_GRAY_BOX);
    printf("%c\n", GRAY_BOX);
    printf("%c\n", LIGHT_GRAY_BOX);

    SetPrintingColor(LIGHT_YELLOW, RED);
    printf("sample texxt");

    printf("%c\n", BLACK_BOX);
    printf("%c\n", DARK_GRAY_BOX);
    printf("%c\n", GRAY_BOX);
    printf("%c\n", LIGHT_GRAY_BOX);


    char x;
    scanf("%s", &x);
    return 0;
}