#include <SDL2/SDL.h>
#define CELL_SIZE   15
#define HEIGHT      32
#define WIDTH       64
#define CELL_SIZE   15



void draw_pixel(unsigned char on);

int read_screen(int i, int j, unsigned char screen[HEIGHT * WIDTH]);

void write_screen(int i, int j, unsigned char screen[HEIGHT * WIDTH], unsigned char val);

unsigned char write_xor_screen(int i, int j, unsigned char screen[HEIGHT * WIDTH], unsigned char val);

void init_clear_screen(unsigned char screen[HEIGHT * WIDTH]);

void flick_screen(unsigned char screen[HEIGHT * WIDTH]);

void draw_point(SDL_Renderer *renderer, int i, int j);

void load_screen(SDL_Renderer *renderer, unsigned char screen[HEIGHT * WIDTH]);
