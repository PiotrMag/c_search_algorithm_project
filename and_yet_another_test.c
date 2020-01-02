#include <stdio.h>

#include "buffer.h"
#include "colors.h"
#include "characters.h"
#include "waiter.h"
#include "console.h"
#include "console_cursor.h"
#include "map.h"

#define WIDTH 10
#define HEIGHT 10

int main()
{
    HideConsoleCursor(GetConsole());
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

    int i, j;
    int was_inverted = 0;
    while(1)
    {
        for (i = 0; i < HEIGHT; i++)
        {
            for (j = 0; j < WIDTH; j++)
            {
                CellType cell_type = map->cells[i][j].cell_type;

                if (cell_type == Start)
                    if (was_inverted)
                        PrintToBuffer(buff, j, i, 'S', BLACK, RED);
                    else
                        PrintToBuffer(buff, j, i, 'S', RED, BLACK);
                else if (cell_type == End)
                    if (was_inverted)
                        PrintToBuffer(buff, j, i, 'X', BLACK, BLUE);
                    else
                        PrintToBuffer(buff, j, i, 'X', BLUE, BLACK);                    
                else if (cell_type == Road)
                    PrintToBuffer(buff, j, i, '.', WHITE, BLACK);
                else if (cell_type == Wall)
                    PrintToBuffer(buff, j, i, '#', BLACK, GRAY);
            }
        }
        DrawBuffer(buff);
        was_inverted = ++was_inverted % 2;
        wait(100);
    }

    DeleteMap(map);
    DeleteBuffer(buff);
    return 0;
}