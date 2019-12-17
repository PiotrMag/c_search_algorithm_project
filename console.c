#include <windows.h>

// some code taken from:
// https://stackoverflow.com/questions/10615042/how-to-update-refresh-console-screen

HANDLE GetConsole()
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!console)
        return NULL;
    return console;
}