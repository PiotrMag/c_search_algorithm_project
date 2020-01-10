#include <stdio.h>

#include "buffer.h"
#include "colors.h"
#include "characters.h"

#define WIDTH 5
#define HEIGHT 5

int main()
{
    BUFFER buff = CreateBuffer(WIDTH, HEIGHT);
    int num = 97;
    int background = 8;
    int foreground = 0;
    int i, j;
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            PrintToBuffer(buff, j, i, num, background, foreground);
            background = ++background % 16;
            foreground = ++foreground % 16;
            num++;
        }
    }
    DrawBuffer(buff);

    char x[10];
    scanf("%s", &x);

    num = 97;
    background = 0;
    foreground = 8;
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            PrintToBuffer(buff, j, i, num, background, foreground);
            background = ++background % 16;
            foreground = ++foreground % 16;
            num++;
        }
    }
    DrawBuffer(buff);

    //char x;
    scanf("%s", &x);

    DeleteBuffer(buff);
    return 0;
}