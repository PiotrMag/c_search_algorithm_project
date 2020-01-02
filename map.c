#include <stdlib.h>

enum CellState
{
    Undiscovered = 0,
    Open = 1,
    Closed = 2
};

enum CellType
{
    Road = 0,
    Wall = 1,
    Start = 2,
    End = 3
};

typedef enum CellState CellState;
typedef enum CellType CellType;
typedef struct Cell *CELL;

struct Cell
{
    CellType cell_type;
    int pos_x, pos_y;
    int weight;
    CellState cell_state;
    CELL origin;
};

struct Map
{
    int width, height;
    struct Cell **cells;
};

typedef struct Map *MAP;

void InitializeMap(MAP map)
{
    int width = map->width;
    int height = map->height;
    int i, j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            map->cells[i][j].cell_type = Road;
            map->cells[i][j].pos_x = j;
            map->cells[i][j].pos_y = i;
            map->cells[i][j].weight = 0;
            map->cells[i][j].cell_state = Undiscovered;
            map->cells[i][j].origin = NULL;
        }
    }
}

MAP CreateMap(int width, int height)
{
    struct Cell ** cells;
    cells = (struct Cell**)malloc(height * sizeof(struct Cell*));
    int i = 0;
    for (i = 0; i < height; ++i)
    {
        cells[i] = (struct Cell*)malloc(width * sizeof(struct Cell));
    }
    MAP map = (MAP)malloc(sizeof(struct Map));
    map->width = width;
    map->height = height;
    map->cells = cells;
    
    InitializeMap(map);

    return map;
}

void DeleteMap(MAP map)
{
    int i = 0;
    int map_height = map->height;
    for (i = 0; i < map_height; ++i)
    {
        free(map->cells[i]);
    }
    free(map->cells);
    free(map);
}

void PutWall(MAP map, int x, int y)
{
    map->cells[y][x].cell_type = Wall;
}

void PutStart(MAP map, int x, int y)
{
    map->cells[y][x].cell_type = Start;
}

void PutEnd(MAP map, int x, int y)
{
    map->cells[y][x].cell_type = End;
}