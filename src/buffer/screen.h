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

SCREEN CreateScreen(int width, int height);
void UpdatePixelText(SCREEN scr, int x, int y, char new_text);
void UpdatePixelColor(SCREEN scr, int x, int y, int new_font_color, int new_background_color);
void DeleteScreen(SCREEN scr);