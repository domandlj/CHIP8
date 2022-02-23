#include "screen.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int read_screen(int i, int j, unsigned char screen[HEIGHT * WIDTH])
{
  return screen[i * WIDTH+j];
}


void write_screen(int i, int j, unsigned char screen[HEIGHT * WIDTH], unsigned char val)
{
  screen[i * WIDTH+j] = val;
}

unsigned char write_xor_screen(int i, int j, unsigned char screen[HEIGHT * WIDTH], unsigned char val)
{
  unsigned char prev = screen[i * WIDTH+j];  
  screen[i * WIDTH+j] ^= val;
  unsigned char actual = screen[i * WIDTH+j];

  if (prev == 1 && actual == 0){
    // there was a collition!
    return 1;
  }
  // no collition
  return 0;
}



void init_clear_screen(unsigned char screen[HEIGHT * WIDTH])
{
  for (int i = 0; i < HEIGHT; ++i){
    for (int j = 0; j < WIDTH; ++j){
      write_screen(i, j, screen, 0);
    }
  }
}

void flick_screen(unsigned char screen[HEIGHT * WIDTH])
{
  for (int i = 0; i < HEIGHT; ++i){
    for (int j = 0; j < WIDTH; ++j){
      unsigned char val = ! read_screen(i, j, screen);
      write_screen(i, j, screen, val);
    }
  }
}





// SDL!

void draw_point(SDL_Renderer *renderer, int i, int j){
  SDL_Rect point = {
        .x = 0,
        .y = 0,
        .w = CELL_SIZE,
        .h = CELL_SIZE,
    };

  point.x += CELL_SIZE * j;
  point.y += CELL_SIZE * i;

  SDL_SetRenderDrawColor(renderer, 48, 54, 25, 0);


  SDL_RenderFillRect(renderer, &point);
}

void load_screen(SDL_Renderer *renderer, unsigned char screen[HEIGHT * WIDTH]){
  for (int i = 0; i < HEIGHT; ++i){
    for (int j = 0; j < WIDTH; ++j){
      if (read_screen(i, j, screen)  == 1){
        draw_point(renderer, i, j);
      }
    }
  }
} 
