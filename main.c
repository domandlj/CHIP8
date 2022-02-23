#include "chip8.h"
#include <time.h>
#define DELAY 20
#define BACKGROUND {141,162,75, 0}
  


void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

void handle_input(Chip8 emu, SDL_Event event, SDL_bool *quit)
{
  assert (emu != NULL);

  switch (event.type) {          
    case SDL_KEYUP:
      relase_keys(emu);
      break;

    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {

        case SDLK_1:
          emu->key[0x1] = 1;
          break;
    
        case SDLK_2:
          emu->key[0x2] = 1;
          break;

        case SDLK_3:
          emu->key[0x3] = 1;
          break;
    
         case SDLK_4:
          emu->key[0xC] = 1;
          break;
    
         case SDLK_q:
          emu->key[0x4] = 1;
          break;

         case SDLK_w:
          emu->key[0x5] = 1;
          break;

         case SDLK_e:
          emu->key[0x6] = 1;
          break;

         case SDLK_r:
          emu->key[0xD] = 1;
          break;

         case SDLK_a:
          emu->key[0x7] = 1;
          break;

         case SDLK_s:
          emu->key[0x8] = 1;
           break;

         case SDLK_d:
          emu->key[0x9] = 1;
          break;
            
         case SDLK_f:
          emu->key[0xE] = 1; 
          break;

         case SDLK_z:
          emu->key[0xA] = 1;
          break;

         case SDLK_x:
          emu->key[0x0] = 1;
          break;

         case SDLK_c:
          emu->key[0xB] = 1;
          break;

         case SDLK_v:
           emu->key[0xF] = 1;
           break; 
    
       
        }
        break;

    case SDL_QUIT:
        *quit = SDL_TRUE;
        break;

    }
   
}

const char * art = "     \
                 JDOMANDL 2022 \n \
                  ------------- \n \
                /             /| \n \
               /             / | \n \
              /____________ /  | \n \
             | ___________ |   | \n \
             ||           ||   | \n \
             || CHIP-8    ||   | \n \
             ||           ||   | \n \
             ||___________||   | \n \
             | 1 2 3 C     |   | \n \
             | 4 5 6 D     |   | \n \
             | 7 8 9 E     |   |  \n \
             | A 0 B F     |  /   \n \
             |_____________|/    \n";

void print_state(Chip8 emu)
{
  assert(emu != NULL);
  printf("%s", art);
  printf("_______________________________________________\n");
  printf("opcode = 0x%x, pc = 0x%x, I = 0x%x, sp = 0x%x \n\n", 
    emu->opcode, emu->pc, emu->I, emu->sp);

  for(int i = 0; i < 16; ++i)
    printf("V[%d] = 0x%x     stack[%d]  = 0x%x \n", i, emu->V[i], i, emu->stack[i]);
  
  printf("\n");
  printf("_______________________________________________\n\n");
  printf("\e[1;1H\e[2J"); 
}


int main(int argc, char **argv) 
{
    Chip8 emu = create_chip8();
    init_chip8(emu);
    
    if (!load_ROM(emu, argv[1]))
      return EXIT_FAILURE;
    


    // + 1 so that the last grid lines fit in the screen.
    int window_width = (WIDTH * CELL_SIZE) + 1;
    int window_height = (HEIGHT * CELL_SIZE) + 1;


    SDL_Color background = BACKGROUND;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Initialize SDL: %s",
                     SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window;
    SDL_Renderer *renderer;
    if (SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window,
                                    &renderer) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Create window and renderer: %s", SDL_GetError());
        return EXIT_FAILURE;
    }
    
    SDL_SetWindowTitle(window, "CHIP 8");

    SDL_bool quit = SDL_FALSE;
    

    while (!quit) {
        emulation_cycle(emu);
        

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
          handle_input(emu, event, &quit); 
        }
        
    
        if (emu->draw){
          delay(DELAY);
          SDL_SetRenderDrawColor(renderer, background.r, background.g,
                               background.b, background.a);

          SDL_RenderClear(renderer);
          load_screen(renderer, emu->gfx);
          emu->draw = false;
          SDL_RenderPresent(renderer);
          print_state(emu);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    emu = destroy_chip8(emu);
    return EXIT_SUCCESS;
}





