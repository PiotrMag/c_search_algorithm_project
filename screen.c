#include <stdlib.h>

extern struct Pixel
{
    int pos_x, pos_y;
    int font_color, background_color;
};

struct Screen
{
    struct Pixel ** pixels;
};

typedef struct Screen *SCREEN;

SCREEN CreateScreen(int width, int height)
{
    struct Pixel **pixels;
    pixels = (struct Pixel**)malloc(height * sizeof(struct Pixel*));
    int i = 0;
    for (i = 0; i < height; ++i)
    {
        pixels[i] = (struct Pixel*)malloc(width * sizeof(struct Pixel));
    }
    SCREEN screen = (SCREEN)malloc(sizeof(struct Screen));
    screen->pixels = pixels;
    return screen;
}