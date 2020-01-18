#include "screen.h"
#include "../console/console.h"

struct Buffer
{
    int width, height;
    HANDLE console;
    SCREEN prev_screen;
    SCREEN pending_screen;
};

typedef struct Buffer *BUFFER;

BUFFER CreateBuffer(int width, int height);
void FlushBuffer(BUFFER buff);
void DeleteBuffer(BUFFER buff);
void PrintToBuffer(BUFFER buff, int x, int y, char text, int font_color, int background_color);
void DrawBuffer(BUFFER buff);