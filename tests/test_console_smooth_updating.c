#include <stdio.h>
#include <windows.h>

#define W 30
#define H 20

// some code taken from:
// https://stackoverflow.com/questions/10615042/how-to-update-refresh-console-screen
// https://stackoverflow.com/questions/18028808/remove-blinking-underscore-on-console-cmd-prompt

int main ()
{
    printf("abcdefghijk\n");
    printf("lmnopqrstuv\n");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!hConsole)
    {
        printf("Console handle is null");
        return 0;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi = {0};
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    COORD coordCur = csbi.dwCursorPosition;

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = 0; // set the cursor visibility
    cursorInfo.dwSize = 20;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    short newX = 0, newY = 0;
    int character = 0;

    while(1)
    {
        if (character > 250) character = 33;

        coordCur.X = newX;
        coordCur.Y = newY;
        SetConsoleCursorPosition(hConsole, coordCur);
        printf("%c", character);

        character++;
        newX++;
        if (newX > W)
        {
            newX = 0;
            newY++;
        }
        if (newY > H) newY = 0;
    }

    // for (newY = 0; newY < 2; newY++)
    // {
    //     for (newX = 0; newX < 11; newX++) 
    //     {
    //         if (character > 250) character = 33;

    //         coordCur.X = newX;
    //         coordCur.Y = newY;
    //         SetConsoleCursorPosition(hConsole, coordCur);
    //         printf("XXX");

    //         character++;
    //     }
    // }

    // coordCur.X = newX;
    // coordCur.Y = newY;
    // SetConsoleCursorPosition(hConsole, coordCur);
    // printf("XXX");

    // newX = 0;
    // newY = 1;

    // coordCur.X = newX;
    // coordCur.Y = newY;
    // SetConsoleCursorPosition(hConsole, coordCur);
    // printf("XXX");

    char x;
    scanf("%s", &x);
    return 0;
}