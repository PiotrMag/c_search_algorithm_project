#include <windows.h>

int MoveConsoleCursor(HANDLE console, int new_x, int new_y);
int HideConsoleCursor(HANDLE console);
int ShowConsoleCursor(HANDLE console);