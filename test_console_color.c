#include <stdio.h>
#include <windows.h>

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

    for (i = 0; i < 300; i++) 
    {
        printf("NR.%3d = %2c\n", i, i);
    }


    char x;
    scanf("%s", &x);
    return 0;
}