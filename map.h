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
    Origin = 2,
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

void InitializeMap(MAP map);
MAP CreateMap(int width, int height);
void DeleteMap(MAP map);