#include <stdio.h>
#include <conio.h>
#include <time.h>

#include "buffer.h"
#include "colors.h"
#include "characters.h"
#include "console.h"
#include "console_cursor.h"
#include "map.h"

#define WIDTH 20
#define HEIGHT 20

#define CURSOR_DELAY_NORMAL 200
#define CURSOR_DELAY_INVERT 400

struct Cursor
{
    int posX;
    int posY;
};

typedef struct Cursor Cursor;

// some code inspired by:
// https://stackoverflow.com/questions/13547471/c-programming-check-if-key-pressed-without-stopping-program?lq=1

int main()
{
    Cursor cur;
    cur.posX = 0;
    cur.posY = 0;

    HANDLE console = GetConsole();
    HideConsoleCursor(console);
    BUFFER buff = CreateBuffer(WIDTH, HEIGHT);
    MAP map = CreateMap(WIDTH, HEIGHT);

// . . . . . . . . . X
// . . . . . . . . . .
// . . . . . . . O O .
// . . . . . . O O . .
// . . . . O O O . . .
// . . . . . . . . . .
// . . . . . . . . . .
// . . . . . . . . . .
// . . . . . . . . . .
// S . . . . . . . . .

    PutStart(map, 0, 9);
    PutWall(map, 4, 4);
    PutWall(map, 5, 4);
    PutWall(map, 6, 4);
    PutWall(map, 6, 3);
    PutWall(map, 7, 3);
    PutWall(map, 7, 2);
    PutWall(map, 8, 2);
    PutEnd(map, 9, 0);

    int inverted = 0;

    int old_millis = clock() * 1000 / CLOCKS_PER_SEC;
    int new_millis = old_millis;

    while(1)
    {
        new_millis = clock() * 1000 / CLOCKS_PER_SEC;
        if ((inverted && new_millis - old_millis >= CURSOR_DELAY_INVERT) ||
            (!inverted && new_millis - old_millis >= CURSOR_DELAY_NORMAL))
        {
            inverted = (inverted + 1) % 2;
            old_millis = new_millis;
        }

        char c;
        if (kbhit())
        {
            c = getch();
            if (c == 'w')
                cur.posY = (cur.posY - 1 + HEIGHT) % HEIGHT;
            else if (c == 's')
                cur.posY = (cur.posY + 1) % HEIGHT;
            else if (c == 'a')
                cur.posX = (cur.posX - 1 + WIDTH) % WIDTH;
            else if (c == 'd')
                cur.posX = (cur.posX + 1) % WIDTH;
            //TODO: add code for handling keys used to place things on map
        }

        //TODO: add map drawing
        int i, j;
        char text = ' ';
        int fore_color = WHITE;
        int back_color = BLACK;
        for (i = 0; i < HEIGHT; i++)
        {
            for (j = 0; j < WIDTH; j++)
            {
                if (map->cells[i][j].cell_type == Road)
                {
                    text = ' ';
                    fore_color = WHITE;
                    back_color = BLACK;
                }
                else if (map->cells[i][j].cell_type == Start)
                {
                    text = 'S';
                    fore_color = BLACK;
                    back_color = DARK_BLUE;
                }
                else if (map->cells[i][j].cell_type == End)
                {
                    text = 'X';
                    fore_color = BLACK;
                    back_color = RED;
                }
                else if (map->cells[i][j].cell_type == Wall)
                {
                    text = LIGHT_GRAY_BOX;
                    fore_color = WHITE;
                    back_color = BLACK;
                }
                PrintToBuffer(buff, j, i, text, fore_color, back_color);
            }
        }
        //TODO: add cursor drawing in overlap mode
        text = buff->pending_screen->pixels[cur.posY][cur.posX].text;
        fore_color = buff->pending_screen->pixels[cur.posY][cur.posX].font_color;
        back_color = buff->pending_screen->pixels[cur.posY][cur.posX].background_color;
        //TODO: add moving cursor to the line after the map and write current cursor pos and block type
        if (inverted)
            PrintToBuffer(buff, cur.posX, cur.posY, text, back_color, fore_color);
        else
            PrintToBuffer(buff, cur.posX, cur.posY, text, fore_color, back_color);
        MoveConsoleCursor(console, 0, HEIGHT);
        SetPrintingColor(WHITE, BLACK);
        printf("X: %d Y: %d       ", cur.posX, cur.posY);
        DrawBuffer(buff);
    }

    DeleteMap(map);
    DeleteBuffer(buff);
    return 0;
}