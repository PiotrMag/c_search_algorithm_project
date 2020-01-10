#include <windows.h>

// some code taken from:
// https://stackoverflow.com/questions/10615042/how-to-update-refresh-console-screen
// https://stackoverflow.com/questions/18028808/remove-blinking-underscore-on-console-cmd-prompt

void MoveConsoleCursor(HANDLE console, int new_x, int new_y)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi = {0};
    GetConsoleScreenBufferInfo(console, &csbi);
    COORD coordCur = csbi.dwCursorPosition;

    coordCur.X = new_x;
    coordCur.Y = new_y;
    SetConsoleCursorPosition(console, coordCur);
}

int HideConsoleCursor(HANDLE console)
{
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = 0; // set the cursor visibility
    cursorInfo.dwSize = 20;
    SetConsoleCursorInfo(console, &cursorInfo);
}

int ShowConsoleCursor(HANDLE console)
{
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = 1; // set the cursor visibility
    cursorInfo.dwSize = 20;
    SetConsoleCursorInfo(console, &cursorInfo);
}