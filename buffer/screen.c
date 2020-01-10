#include <stdlib.h>

struct Pixel
{
    char text;
    int pos_x, pos_y;
    int font_color, background_color;
};

struct Screen
{
    int width;
    int height;
    struct Pixel ** pixels;
};

typedef struct Screen *SCREEN;

void ClearScreen(SCREEN scr)
{
    int width = scr->width;
    int height = scr->height;
    int i, j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            scr->pixels[i][j].text = ' ';
            scr->pixels[i][j].font_color = 15;
            scr->pixels[i][j].background_color = 0;
            scr->pixels[i][j].pos_x = j;
            scr->pixels[i][j].pos_y = i;
        }
    }
}

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
    screen->width = width;
    screen->height = height;
    screen->pixels = pixels;
    
    ClearScreen(screen);

    return screen;
}

void DeleteScreen(SCREEN scr)
{
    int i = 0;
    int screen_height = scr->height;
    for (i = 0; i < screen_height; ++i)
    {
        free(scr->pixels[i]);
    }
    free(scr->pixels);
    free(scr);
}

void UpdatePixelText(SCREEN scr, int x, int y, char new_text)
{
    scr->pixels[y][x].text = new_text;
}

void UpdatePixelColor(SCREEN scr, int x, int y, int new_font_color, int new_background_color)
{
    scr->pixels[y][x].font_color = new_font_color;
    scr->pixels[y][x].background_color = new_background_color;
}
