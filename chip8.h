#include "screen.h"
#define FONTSET_SIZE	80
#define MEMORY_SIZE   4096
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

struct Chip8St{
  // 2 Bytes words opcodes
  unsigned short opcode;

  // Memory of 4 KiB with 1 Byte words.
  unsigned char memory[MEMORY_SIZE];

  // 16 registers of 1 Bytes.
  unsigned char V[16];

  // memory index and PC, 0x000 - 0xFFF (0 to 4095)
  unsigned short I;
  unsigned short pc;

  // screen
  unsigned char gfx[HEIGHT * WIDTH];

  // interrupt timers 
  unsigned char delay_timer;
  unsigned char sound_timer;

  // stack
  unsigned short stack[16];
  unsigned short sp;

  // keyboard
  unsigned char key[16];

  // draw againg? 
  bool draw;

  // ROM size
  long size_ROM;

};

typedef struct Chip8St * Chip8; 


Chip8 create_chip8();
Chip8 destroy_chip8(Chip8 emu);


void relase_keys(Chip8 emu);

void load_fontset(Chip8 emu);


void clear_stack(Chip8 emu);

void clear_regs(Chip8 emu);


void clear_memory(Chip8 emu);

void init_chip8(Chip8 emu);

void fetch_opcode(Chip8 emu);


void increment_pc(Chip8 emu);

void skip_instruction(Chip8 emu);


void decode_and_execute_opcode(Chip8 emu);

void update_timers(Chip8 emu);

void emulation_cycle(Chip8 emu); 


bool load_ROM(Chip8 emu, const char * filename);
