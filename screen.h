struct Screen;
typedef struct Screen *SCREEN;

SCREEN CreateScreen(int width, int height);
void UpdatePixelText(SCREEN scr, int x, int y, char new_text);
void UpdatePixelColor(SCREEN scr, int x, int y, int new_font_color, int new_background_color);
void DeleteScreen(SCREEN scr);