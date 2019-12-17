#include <windows.h>

// some code taken from:
// https://stackoverflow.com/questions/20725769/change-background-color-of-console-output-in-c-and-c

void SetPrintingColor(int new_background_color, int new_font_color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), new_font_color << 4 | new_background_color);
}