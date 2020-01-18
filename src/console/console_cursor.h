#include <windows.h>

void MoveConsoleCursor(HANDLE console, int new_x, int new_y);
void HideConsoleCursor(HANDLE console);
void ShowConsoleCursor(HANDLE console);