#include <stdio.h>

#include "buffer.h"
#include "colors.h"
#include "characters.h"
#include "waiter.h"
#include "console.h"
#include "console_cursor.h"

#define WIDTH 25
#define HEIGHT 15

int main()
{
    HideConsoleCursor(GetConsole());
    BUFFER buff = CreateBuffer(WIDTH, HEIGHT);
    int num = 97;
    int background_start = 8;
    int foreground_start = 0;
    int background = 8;
    int foreground = 0;
    int i, j;

    while(1)
    {
        background = background_start;
        foreground = foreground_start;
        for (i = 0; i < HEIGHT; i++)
        {
            for (j = 0; j < WIDTH; j++)
            {
                PrintToBuffer(buff, j, i, num, background, foreground);
                background = ++background % 16;
                foreground = ++foreground % 16;
            }
        }
        DrawBuffer(buff);
        wait(100);
        background_start = (background_start + 1) % 16;
        foreground_start = (foreground_start + 1) % 16;
    }

    DeleteBuffer(buff);
    return 0;
}