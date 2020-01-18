#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <math.h>

#include "src/console/console.h"
#include "src/console/console_colors.h"
#include "src/console/console_cursor.h"
#include "src/misc/colors.h"
#include "src/time/waiter.h"
#include "src/misc/characters.h"
#include "src/map/map.h"
#include "src/managers/file_manager.h"
#include "src/buffer/buffer.h"
#include "src/lists/sorted_list.h"

#define WALL_CHAR '#'
#define ROAD_CHAR ' '
#define START_CHAR 'S'
#define END_CHAR 'X'

#define CURSOR_DELAY_NORMAL 100
#define CURSOR_DELAY_INVERT 500

#define SIMULATION_STEP_DELAY 50

#define STEP_DIAGONAL_LENGTH 14
#define STEP_NORMAL_LENGTH 10

struct Tuple
{
    int a, b;
};
typedef struct Tuple Tuple;

struct Cursor
{
    int posX;
    int posY;
};
typedef struct Cursor Cursor;

void _PrintWelcomeScreen();
void _PrintMainMenu();
void _PrintNewMapWelcome();
void _PrintLoadMapWelcome();
void _PrintLoadMapLoadFile();
void _PrintLoadMapLoadError();
void _PrintSaveDialog();
void _PrintSaveMenu();
void _PrintSaveEnsureDialog();
void _PrintStartSimulationDialog();
void _PrintNoStartOrNoEndError();

void SaveMapToFile(MAP map, char *file);
int MakeStepFromCell(MAP map, int x, int y, SORTED_LIST *list_ptr, int end_pos_x, int end_pos_y);
int CalculateDistanceBetweenCells(int x1, int y1, int x2, int y2);
int CheckNeighbour(MAP map, int origin_x, int origin_y, SORTED_LIST *list_ptr, int dest_x, int dest_y, int step_length, int end_x, int end_y);

void PrintList(SORTED_LIST list)
{
    while(list != NULL)
    {
        printf("\n%d,%d", list->value, list->help_value);
        list = list->next_element;
    }
}

int main()
{
    Cursor cur;
    cur.posX = 0;
    cur.posY = 0;

    int cursor_mode = 0;
    char *mode_str = "WALL";

    HANDLE console = GetConsole();
    BUFFER buff = NULL;
    MAP map = NULL;

    HideConsoleCursor(console);
    _PrintWelcomeScreen();
    Wait(1000);
    
    int mode = 0;
    do {
        system("cls");
        _PrintMainMenu();
        ShowConsoleCursor(console);
        scanf("%d", &mode);
        HideConsoleCursor(console);
        fflush(stdin);
    } while (mode != 1 && mode != 2);

    int width = 0, height = 0;
    char file_name[151] = "";
    if (mode == 1)
    {
        do {
            system("cls");
            _PrintNewMapWelcome();
            ShowConsoleCursor(console);
            scanf("%d %d", &width, &height);
            HideConsoleCursor(console);
            fflush(stdin);
        } while (width <= 0 || height <= 0);

        map = CreateMap(width, height);
        buff = CreateBuffer(width, height);
    }
    else if (mode == 2)
    {
        int load_error = 0;
        do {
            system("cls");
            _PrintLoadMapWelcome();
            ShowConsoleCursor(console);
            scanf("%d %d", &width, &height);
            HideConsoleCursor(console);
            fflush(stdin);
        } while (width <= 0 || height <= 0);

        do {
            system("cls");
            if (!load_error) _PrintLoadMapLoadFile();
            else if (load_error == 1) _PrintLoadMapLoadError();
            ShowConsoleCursor(console);
            scanf("%s", file_name);
            FILE *file = fopen(file_name, "r");
            if (file == NULL) load_error = 1;
            else
            {
                load_error = -1;
                fclose(file);
            }
            HideConsoleCursor(console);
            fflush(stdin);
        } while (load_error != -1);

        map = CreateMap(width, height);
        buff = CreateBuffer(width, height);
        char *map_to_load = ReadFromFile(file_name);
        // printf("%s", map_to_load);
        // scanf("%c");
        char one_char;
        
        int i, j, index = 0;
        int flag_eof = 0;
        int flag_nl = 0;
        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                one_char = map_to_load[index];
                if (one_char == '\0')
                {
                    flag_eof = 1;
                    break;
                }
                if (one_char == '\n')
                {
                    flag_nl = 1;
                    index++;
                    break;
                }
                flag_nl = 0;
                if (one_char == WALL_CHAR)
                    PutWall(map, j, i);
                else if (one_char == ROAD_CHAR)
                    PutRoad(map, j, i);
                else if (one_char == START_CHAR)
                    PutStart(map, j, i);
                else if (one_char == END_CHAR)
                    PutEnd(map, j, i);
                index++;
            }
            if (flag_eof) break;
            if (!flag_nl)
            {
                for (; !(one_char != '\n' && flag_nl == 1); one_char = map_to_load[index])
                {
                    one_char = map_to_load[index];
                    if (one_char == '\0')
                    {
                        flag_eof = 1;
                        break;
                    }
                    else if (one_char == '\n')
                    {
                        flag_nl = 1;
                    }
                    else 
                    {
                        flag_nl = 0;
                    }
                    ++index;
                }
                if (flag_eof) break;
            }
        }

        system("cls");
        //printf("index: %d, plik: %s, szer: %d, wys: %d", index, file_name, width, height);
    }

    int inverted = 0;
    int old_millis = clock() * 1000 / CLOCKS_PER_SEC;
    int new_millis = old_millis;

    char response = ' ';
    // uruchomić edytor
    HideConsoleCursor(console);
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
        char file_to_save_name[151] = "";
        int can_save = 0;
        if (kbhit())
        {
            c = getch();
            if (c == 'w' || c == 'W')
                cur.posY = (cur.posY - 1 + height) % height;
            else if (c == 's' || c == 'S')
                cur.posY = (cur.posY + 1) % height;
            else if (c == 'a' || c == 'A')
                cur.posX = (cur.posX - 1 + width) % width;
            else if (c == 'd' || c == 'D')
                cur.posX = (cur.posX + 1) % width;
            else if (c == '0')
                cursor_mode = 0;
            else if (c == '1')
                cursor_mode = 1;
            else if (c == '2')
                cursor_mode = 2;
            else if (c == '3')
                cursor_mode = 3;
            else if (c == 'l' || c == 'L')
            {
                if (cursor_mode == 0)
                    PutRoad(map, cur.posX, cur.posY);
                else if (cursor_mode == 1)
                    PutStart(map, cur.posX, cur.posY);
                else if (cursor_mode == 2)
                    PutEnd(map, cur.posX, cur.posY);
                else if (cursor_mode == 3)
                    PutWall(map, cur.posX, cur.posY);
            }
            else if (c == 'y' || c == 'Y')
            {
                do {
                    system("cls");
                    _PrintSaveDialog();
                    ShowConsoleCursor(console);
                    scanf("%c", &response);
                    HideConsoleCursor(console);
                    fflush(stdin);
                } while (response != 'Y' && response != 'y' && response != 'N' && response != 'n');
                if (response == 'Y' || response == 'y')
                {
                    do {
                        system("cls");
                        _PrintSaveMenu();
                        ShowConsoleCursor(console);
                        scanf("%s", &file_to_save_name);
                        FILE *file = fopen(file_to_save_name, "r");
                        if (file != NULL)
                        {
                            do {
                                system("cls");
                                _PrintSaveEnsureDialog();
                                ShowConsoleCursor(console);
                                scanf("%c", &response);
                                HideConsoleCursor(console);
                                fflush(stdin);
                            } while (response != 'Y' && response != 'y' && response != 'N' && response != 'n');  
                            if (response == 'y' || response == 'Y')
                                can_save = 1;
                        }
                        else can_save = 1;
                        HideConsoleCursor(console);
                        fflush(stdin);
                    } while (can_save == 0);
                    //TODO: zapisac plik
                    SaveMapToFile(map, file_to_save_name);
                }
                break;
                //TODO: dodać kod do obsługi zapisywania mapy
            }
        }

        //TODO: add map drawing
        int i, j;
        char text = ' ';
        int fore_color = WHITE;
        int back_color = BLACK;
        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
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
                    text = DARK_GRAY_BOX;
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
        if (inverted)
            PrintToBuffer(buff, cur.posX, cur.posY, text, back_color, fore_color);
        else
            PrintToBuffer(buff, cur.posX, cur.posY, text, fore_color, back_color);
        //TODO: add moving cursor to the line after the map and write current cursor pos and block type
        MoveConsoleCursor(console, 0, height);
        SetPrintingColor(WHITE, BLACK);
        printf(" X: %d Y: %d TRYB: ", cur.posX, cur.posY);
        if (cursor_mode == 0)
        {
            mode_str = "DROGA";
            SetPrintingColor(WHITE, GRAY);
        }
        else if (cursor_mode == 1)
        {
            mode_str = "START";
            SetPrintingColor(BLACK, DARK_BLUE);
        }
        else if (cursor_mode == 2)
        {
            mode_str = "KONIEC";
            SetPrintingColor(BLACK, RED);
        }
        else if (cursor_mode == 3)
        {
            mode_str = "SCIANA";
            SetPrintingColor(BLACK, WHITE);
        }
        printf("%s", mode_str);
        SetPrintingColor(WHITE, BLACK);
        printf("           ");
        printf("\n ");
        SetPrintingColor(WHITE, GRAY);
        printf("0->DROGA ");
        SetPrintingColor(BLACK, DARK_BLUE);
        printf("1->START ");
        SetPrintingColor(BLACK, RED);
        printf("2->KONIEC ");
        SetPrintingColor(BLACK, WHITE);
        printf("3->SCIANA \n");
        SetPrintingColor(WHITE, BLACK);
        printf("\n Poruszanie: ");
        SetPrintingColor(BLACK, WHITE);
        printf("WASD\n");
        SetPrintingColor(WHITE, BLACK);
        printf(" Umiesc blok: ");
        SetPrintingColor(BLACK, WHITE);
        printf("L\n");
        SetPrintingColor(WHITE, BLACK);
        printf(" Zakoncz edytowanie: ");
        SetPrintingColor(BLACK, WHITE);
        printf("Y\n");
        SetPrintingColor(WHITE, BLACK);
        DrawBuffer(buff);
    }

    // uruchomić symulację
    system("cls");
    HideConsoleCursor(console);
    _PrintStartSimulationDialog();
    getch();

    SORTED_LIST next_cells = NULL;
    int start_x = -1, start_y = -1;
    int end_x = -1, end_y = -1;

    // find start cell and end cell
    int i, j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (map->cells[i][j].cell_type == Start)
            {
                start_x = j;
                start_y = i;
            }
            if (map->cells[i][j].cell_type == End)
            {
                end_x = j;
                end_y = i;
            }
        }
    }

    if (start_x < 0 || start_y < 0 || end_x < 0 || end_y < 0)
    {
        system("cls");
        _PrintNoStartOrNoEndError();
        getch();
        return 0;
    }

    system("cls");
    HideConsoleCursor(console);

    // char text = ' ';
    // int fore_color = WHITE;
    // int back_color = BLACK;
    // for (i = 0; i < height; i++)
    // {
    //     for (j = 0; j < width; j++)
    //     {
    //         if (map->cells[i][j].cell_type == Road)
    //         {
    //             text = ' ';
    //             fore_color = WHITE;
    //             back_color = BLACK;
    //         }
    //         else if (map->cells[i][j].cell_type == Start)
    //         {
    //             text = 'S';
    //             fore_color = BLACK;
    //             back_color = DARK_BLUE;
    //         }
    //         else if (map->cells[i][j].cell_type == End)
    //         {
    //             text = 'X';
    //             fore_color = BLACK;
    //             back_color = RED;
    //         }
    //         else if (map->cells[i][j].cell_type == Wall)
    //         {
    //             text = DARK_GRAY_BOX;
    //             fore_color = WHITE;
    //             back_color = BLACK;
    //         }
    //         PrintToBuffer(buff, j, i, text, fore_color, back_color);
    //     }
    // }
    // DrawBuffer(buff);

    FlushBuffer(buff);
    // printf("%d, %d", map->width, map->height);
    // getch();

    int step_result_code = -1;
    int flag_no_path = 0;
    step_result_code = MakeStepFromCell(map, start_x, start_y, &next_cells, end_x, end_y);

    //printf("list_len: %d", GetListLength(next_cells));

    int counter = 0;
    
    while(step_result_code != 1)
    {
        counter++;
        //printf("counter: %d, list_len: %d", counter, GetListLength(next_cells));
        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                struct Cell cell = map->cells[i][j];
                if (cell.cell_type == Road)
                {
                    if (cell.cell_state == Open)
                        PrintToBuffer(buff, j, i, ' ', BLACK, GREEN);
                    else if (cell.cell_state == Closed)
                        PrintToBuffer(buff, j, i, ' ', BLACK, DARK_GREEN);
                    else
                        PrintToBuffer(buff, j, i, ' ', WHITE, BLACK);
                }
                else if (cell.cell_type == Wall)
                    PrintToBuffer(buff, j, i, DARK_GRAY_BOX, WHITE, BLACK);
                else if (cell.cell_type == Start)
                    PrintToBuffer(buff, j, i, 'S', BLACK, BLUE);
                else if (cell.cell_type == End)
                    PrintToBuffer(buff, j, i, 'X', BLACK, RED);
            }
        }
        if (GetListLength(next_cells) == 0)
        {
            flag_no_path = 1;
            break;
        }
        Tuple *next_cell = PopFirstElement(&next_cells);
        step_result_code = MakeStepFromCell(map, next_cell->a, next_cell->b, &next_cells, end_x, end_y);
        DrawBuffer(buff);
        MoveConsoleCursor(console, 0, height);
        //printf("\n%d, %d", next_cell->a, next_cell->b);
        //PrintList(next_cells);
        Wait(SIMULATION_STEP_DELAY);
    }

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            struct Cell cell = map->cells[i][j];
            if (cell.cell_type == Road)
            {
                if (cell.cell_state == Open)
                    PrintToBuffer(buff, j, i, ' ', BLACK, GREEN);
                else if (cell.cell_state == Closed)
                    PrintToBuffer(buff, j, i, ' ', BLACK, DARK_GREEN);
                else
                    PrintToBuffer(buff, j, i, ' ', WHITE, BLACK);
            }
            else if (cell.cell_type == Wall)
                PrintToBuffer(buff, j, i, DARK_GRAY_BOX, WHITE, BLACK);
            else if (cell.cell_type == Start)
                PrintToBuffer(buff, j, i, 'S', BLACK, BLUE);
            else if (cell.cell_type == End)
                PrintToBuffer(buff, j, i, 'X', BLACK, RED);
        }
    }

    if (!flag_no_path)
    {
        CELL tmp_cell_for_path = &(map->cells[end_y][end_x]);
        while (tmp_cell_for_path->cell_type != Start)
        {
            char char_in_buffer = buff->pending_screen->pixels[tmp_cell_for_path->pos_y][tmp_cell_for_path->pos_x].text;
            PrintToBuffer(buff, tmp_cell_for_path->pos_x, tmp_cell_for_path->pos_y, char_in_buffer, BLACK, TEAL);
            tmp_cell_for_path = tmp_cell_for_path->origin;
        }
    }

    DrawBuffer(buff);


    // int i, j;
    // char text = ' ';
    // int fore_color = WHITE;
    // int back_color = BLACK;
    // for (i = 0; i < height; i++)
    // {
    //     for (j = 0; j < width; j++)
    //     {
    //         if (map->cells[i][j].cell_type == Road)
    //         {
    //             text = ' ';
    //             fore_color = WHITE;
    //             back_color = BLACK;
    //         }
    //         else if (map->cells[i][j].cell_type == Start)
    //         {
    //             text = 'S';
    //             fore_color = BLACK;
    //             back_color = DARK_BLUE;
    //         }
    //         else if (map->cells[i][j].cell_type == End)
    //         {
    //             text = 'X';
    //             fore_color = BLACK;
    //             back_color = RED;
    //         }
    //         else if (map->cells[i][j].cell_type == Wall)
    //         {
    //             text = DARK_GRAY_BOX;
    //             fore_color = WHITE;
    //             back_color = BLACK;
    //         }
    //         PrintToBuffer(buff, j, i, text, fore_color, back_color);
    //     }
    // }

    // DrawBuffer(buff);
    // ShowConsoleCursor(console);

    //printf("counter: %d", counter);

    MoveConsoleCursor(console, 0, height);
    printf("\nkoniec");

    getch();

    DeleteBuffer(buff);
    DeleteMap(map);
    DeleteList(&next_cells);

    return 0;
}

int CheckNeighbour(MAP map, int origin_x, int origin_y, SORTED_LIST *list_ptr, int dest_x, int dest_y, int step_length, int end_x, int end_y)
{
    if (dest_x < 0 || dest_x >= (map->width) || dest_y < 0 || dest_y >= (map->height))
        return -1;
    CELL neighbour = &(map->cells[dest_y][dest_x]);
    CELL caller = &(map->cells[origin_y][origin_x]);
    if (neighbour->cell_type == Road || neighbour->cell_type == End)
    {
        if (neighbour->cell_state == Undiscovered)
        {
            neighbour->cell_state = Open;
            neighbour->H_cost = CalculateDistanceBetweenCells(dest_x, dest_y, end_x, end_y);
            neighbour->G_cost = caller->G_cost + step_length;
            neighbour->origin = &(map->cells[origin_y][origin_x]);
            Tuple *coords = (Tuple*)malloc(sizeof(Tuple));
            coords->a = dest_x;
            coords->b = dest_y;
            int main_value = neighbour->G_cost + neighbour->H_cost;
            int help_value = neighbour->H_cost;
            AddElement(list_ptr, CreateListElement(main_value, help_value, coords));
            if (neighbour->cell_type == End)
                return 1;
            else
                return 0;
        }
        else if (neighbour->G_cost > (caller->G_cost + step_length))
        {
            neighbour->G_cost = caller->G_cost + step_length;
            neighbour->origin = &(map->cells[origin_y][origin_x]);
            return 0;
        }
    }
    return -1;
}

int CalculateDistanceBetweenCells(int x1, int y1, int x2, int y2)
{
    int diff_x = abs(x1 - x2) * 10;
    int diff_y = abs(y1 - y2) * 10;
    double d_diff_x = (double)diff_x;
    double d_diff_y = (double)diff_y;
    return (int)(sqrt(d_diff_x*d_diff_x + d_diff_y* d_diff_y));
}

int MakeStepFromCell(MAP map, int x, int y, SORTED_LIST *list_ptr, int end_pos_x, int end_pos_y)
{
    map->cells[y][x].cell_state = Closed;
    if (CheckNeighbour(map, x, y, list_ptr, x-1, y, STEP_NORMAL_LENGTH, end_pos_x, end_pos_y) == 1) return 1;
    if (CheckNeighbour(map, x, y, list_ptr, x-1, y-1, STEP_DIAGONAL_LENGTH, end_pos_x, end_pos_y) == 1) return 1;
    if (CheckNeighbour(map, x, y, list_ptr, x, y-1, STEP_NORMAL_LENGTH, end_pos_x, end_pos_y) == 1) return 1;
    if (CheckNeighbour(map, x, y, list_ptr, x+1, y-1, STEP_DIAGONAL_LENGTH, end_pos_x, end_pos_y) == 1) return 1;
    if (CheckNeighbour(map, x, y, list_ptr, x+1, y, STEP_NORMAL_LENGTH, end_pos_x, end_pos_y) == 1) return 1;
    if (CheckNeighbour(map, x, y, list_ptr, x+1, y+1, STEP_DIAGONAL_LENGTH, end_pos_x, end_pos_y) == 1) return 1;
    if (CheckNeighbour(map, x, y, list_ptr, x, y+1, STEP_NORMAL_LENGTH, end_pos_x, end_pos_y) == 1) return 1;
    if (CheckNeighbour(map, x, y, list_ptr, x-1, y+1, STEP_DIAGONAL_LENGTH, end_pos_x, end_pos_y) == 1) return 1;
    return 0;
}

void SaveMapToFile(MAP map, char *file_name)
{
    int width = map->width;
    int height = map->height;
    int i, j, index = 0;
    char *output = (char*)malloc((width + 1)*height*sizeof(char));
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (map->cells[i][j].cell_type == Wall)
                output[index] = WALL_CHAR;
            else if (map->cells[i][j].cell_type == Start)
                output[index] = START_CHAR;
            else if (map->cells[i][j].cell_type == End)
                output[index] = END_CHAR;
            else if (map->cells[i][j].cell_type == Road)
                output[index] = ROAD_CHAR;
            index++;
        }
        output[index]= '\n';
        index++;
    }
    output[index] = '\0';
    WriteToFile(file_name, output);
}

void _PrintNoStartOrNoEndError()
{
    SetPrintingColor(WHITE, BLACK);
    printf("\n ");
    SetPrintingColor(BLACK, WHITE);
    printf("                                      \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("          ");
    SetPrintingColor(BLACK, RED);
    printf(" NIEPELNA MAPA ");
    SetPrintingColor(WHITE, BLACK);
    printf("           ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf("                                      \n");
    SetPrintingColor(WHITE, BLACK);
    printf("\n  Na mapie brakuje punktu START(S) lub\n  punktu KONIEC(X)\n");
    printf("  Otworz program ponownie i zedytuj mape!");
}

void _PrintStartSimulationDialog()
{
    SetPrintingColor(WHITE, BLACK);
    printf("\n ");
    SetPrintingColor(BLACK, WHITE);
    printf("                                      \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("            ");
    SetPrintingColor(BLACK, GREEN);
    printf(" SYMULACJA ");
    SetPrintingColor(WHITE, BLACK);
    printf("             ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf("                                      \n");
    SetPrintingColor(WHITE, BLACK);
    printf("\n  Nacisnij dowlony klawisz\n  aby ROZPOCZAC symulacje ");
}

void _PrintSaveEnsureDialog()
{
    SetPrintingColor(WHITE, BLACK);
    printf("\n ");
    SetPrintingColor(BLACK, WHITE);
    printf("                                      \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("        ");
    SetPrintingColor(BLACK, SALMON);
    printf(" PLIK JUZ ISTNIEJE ");
    SetPrintingColor(WHITE, BLACK);
    printf("         ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf("                                      \n");
    SetPrintingColor(WHITE, BLACK);
    printf("\n  Czy nadpisac plik (Y/n): ");
}

void _PrintSaveMenu()
{
    SetPrintingColor(WHITE, BLACK);
    printf("\n ");
    SetPrintingColor(BLACK, WHITE);
    printf("                                      \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("        ");
    SetPrintingColor(BLACK, WHITE);
    printf(" PODAJ NAZWE PLIKU ");
    SetPrintingColor(WHITE, BLACK);
    printf("         ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf("                                      \n");
    SetPrintingColor(WHITE, BLACK);
    printf("\n  Podaj nzawe pliku, do ktorego\n  ma zostac zapisana mapa: ");
}

void _PrintSaveDialog()
{
    SetPrintingColor(WHITE, BLACK);
    printf("\n ");
    SetPrintingColor(BLACK, WHITE);
    printf("                                      \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("            ");
    SetPrintingColor(WHITE, DARK_BLUE);
    printf(" CZY ZAPISAC? ");
    SetPrintingColor(WHITE, BLACK);
    printf("          ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf("                                      \n");
    SetPrintingColor(WHITE, BLACK);
    printf("\n  Y/n: ");
}

void _PrintLoadMapLoadError()
{
    SetPrintingColor(WHITE, BLACK);
    printf("\n ");
    SetPrintingColor(BLACK, WHITE);
    printf("                                      \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("        ");
    SetPrintingColor(BLACK, SALMON);
    printf(" PLIK NIE ISTNIEJE! ");
    SetPrintingColor(WHITE, BLACK);
    printf("        ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf("                                      \n");
    SetPrintingColor(WHITE, BLACK);
    printf("\n  Podaj sciezke do pliku do wcztania\n");
    printf("\n  Sciezka (maks.150 znakow): ");
}

void _PrintLoadMapLoadFile()
{
    SetPrintingColor(WHITE, BLACK);
    printf("\n ");
    SetPrintingColor(BLACK, WHITE);
    printf("                                      \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("           ");
    SetPrintingColor(BLACK, WHITE);
    printf(" WCZYTAJ MAPE ");
    SetPrintingColor(WHITE, BLACK);
    printf("           ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf("                                      \n");
    SetPrintingColor(WHITE, BLACK);
    printf("\n  Podaj sciezke do pliku do wcztania\n");
    printf("\n  Sciezka (maks.150 znakow): ");
}

void _PrintLoadMapWelcome()
{
    SetPrintingColor(WHITE, BLACK);
    printf("\n ");
    SetPrintingColor(BLACK, WHITE);
    printf("                                      \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("           ");
    SetPrintingColor(BLACK, WHITE);
    printf(" WCZYTAJ MAPE ");
    SetPrintingColor(WHITE, BLACK);
    printf("           ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf("                                      \n");
    SetPrintingColor(WHITE, BLACK);
    printf("\n  Podaj rozmiar mapy do wczytania");
    printf("\n  Rozmiar (format: szer wys): ");
}

void _PrintNewMapWelcome()
{
    SetPrintingColor(WHITE, BLACK);
    printf("\n ");
    SetPrintingColor(BLACK, WHITE);
    printf("                                      \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("            ");
    SetPrintingColor(BLACK, WHITE);
    printf(" NOWA MAPA ");
    SetPrintingColor(WHITE, BLACK);
    printf("           ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf("                                      \n");
    SetPrintingColor(WHITE, BLACK);
    printf("\n  Podaj rozmiar mapy do utworzenia");
    printf("\n  Rozmiar (format: szer wys): ");
}

void _PrintMainMenu()
{
    SetPrintingColor(WHITE, BLACK);
    printf("\n ");
    SetPrintingColor(BLACK, WHITE);
    printf("                                      \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("               ");
    SetPrintingColor(BLACK, WHITE);
    printf(" MENU ");
    SetPrintingColor(WHITE, BLACK);
    printf("               ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("          1 <---- Nowa mapa         ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("          2 <- Wczytaj mape         ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf(" ");
    SetPrintingColor(WHITE, BLACK);
    printf("                                    ");
    SetPrintingColor(BLACK, WHITE);
    printf(" \n");
    SetPrintingColor(WHITE, BLACK);
    printf(" ");
    SetPrintingColor(BLACK, WHITE);
    printf("                                      \n");
    SetPrintingColor(WHITE, BLACK);
    printf("\n            Podaj numer: ");
}

void _PrintWelcomeScreen()
{
    printf("\n\n\n       ");
    SetPrintingColor(BLACK, WHITE);
    printf(" Projekt wyszukiwania A* ");
    SetPrintingColor(WHITE, BLACK);
    printf("\n       ");
    SetPrintingColor(BLACK, WHITE);
    printf("       Piotr Mag         ");
    SetPrintingColor(WHITE, BLACK);
    printf("");
}