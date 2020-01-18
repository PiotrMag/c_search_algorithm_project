#include <windows.h>
#include <stdio.h>
#include "../buffer/screen.h"
#include "../console/console_cursor.h"
#include "../console/console_colors.h"
#include "../console/console.h"
#include "../misc/colors.h"

struct Buffer
{
    int width, height;
    HANDLE console;
    SCREEN prev_screen;
    SCREEN pending_screen;
    int default_font_color, default_background_color;
};

typedef struct Buffer *BUFFER;

BUFFER CreateBuffer(int width, int height)
{
    BUFFER buff = (BUFFER)malloc(sizeof(struct Buffer));
    buff->width = width;
    buff->height = height;
    buff->default_font_color = WHITE;
    buff->default_background_color = BLACK;
    buff->console = (HANDLE) GetConsole();
    if (buff->console == NULL)
    {
        printf("\n\nError: Console handle NULL!");
        return NULL;
    }
    SCREEN prev_screen = CreateScreen(width, height);
    SCREEN pending_screen = CreateScreen(width, height);
    buff->prev_screen = prev_screen;
    buff->pending_screen = pending_screen;
    return buff;
}

void FlushBuffer(BUFFER buff)
{
    DeleteScreen(buff->prev_screen);
    buff->prev_screen = buff->pending_screen;
    SCREEN new_pending_screen = CreateScreen(buff->width, buff->height);
    buff->pending_screen = new_pending_screen;
}

void DeleteBuffer(BUFFER buff)
{
    DeleteScreen(buff->prev_screen);
    DeleteScreen(buff->pending_screen);
    free(buff);
}

void PrintToBuffer(BUFFER buff, int x, int y, char text, int font_color, int background_color)
{
    UpdatePixelText(buff->pending_screen, x, y, text);
    UpdatePixelColor(buff->pending_screen, x, y, font_color, background_color);
}

void DrawBuffer(BUFFER buff)
{
    int width = buff->width;
    int height = buff->height;
    int i, j;
    int prev_font_color, font_color, prev_background_color, background_color;
    struct Pixel prev_pixel, pixel;
    for(i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {
            prev_pixel = buff->prev_screen->pixels[i][j];
            pixel = buff->pending_screen->pixels[i][j];

            if (prev_pixel.font_color != pixel.font_color || prev_pixel.background_color != pixel.background_color || prev_pixel.text != pixel.text)
            {
                MoveConsoleCursor(buff->console, j, i);
                SetPrintingColor(pixel.font_color, pixel.background_color);
                printf("%c", pixel.text);
            }
        }
    }
    FlushBuffer(buff);
}

