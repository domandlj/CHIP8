#include "chip8.h"

const unsigned char chip8_fontset[FONTSET_SIZE] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0,		// 0
	0x20, 0x60, 0x20, 0x20, 0x70,		// 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0,		// 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0,		// 3
	0x90, 0x90, 0xF0, 0x10, 0x10,		// 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0,		// 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0,		// 6
	0xF0, 0x10, 0x20, 0x40, 0x40,		// 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0,		// 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0,		// 9
	0xF0, 0x90, 0xF0, 0x90, 0x90,		// A
	0xE0, 0x90, 0xE0, 0x90, 0xE0,		// B
	0xF0, 0x80, 0x80, 0x80, 0xF0,		// C
	0xE0, 0x90, 0x90, 0x90, 0xE0,		// D
	0xF0, 0x80, 0xF0, 0x80, 0xF0,		// E
	0xF0, 0x80, 0xF0, 0x80, 0x80		// F
};

Chip8 create_chip8()
{
  Chip8 emu = malloc(sizeof(struct Chip8St));

  if (emu == NULL){
    exit(EXIT_FAILURE);
  }

  return emu;
}

Chip8 destroy_chip8(Chip8 emu)
{
  assert(emu != NULL);
  free(emu);
  return NULL;
}

void relase_keys(Chip8 emu)
{
  assert(emu != NULL);
  for (int i= 0; i< 16; ++i)
    emu->key[i] = 0; 
}

void load_fontset(Chip8 emu)
{
  assert(emu != NULL);
  for(int i = 0; i < FONTSET_SIZE; ++i)
    emu->memory[i] = chip8_fontset[i];
}

void clear_stack(Chip8 emu)
{
  assert(emu != NULL);
  for (int i = 0; i < 16; ++i)
    emu->stack[i] = 0;
}

void clear_regs(Chip8 emu)
{
  assert(emu != NULL);
  for (int i = 0; i < 16; ++i)
    emu->V[i] = 0; 
}

void clear_memory(Chip8 emu)
{
  assert(emu != NULL);
  for (int i = 0; i < MEMORY_SIZE; ++i)
    emu->memory[i] = 0;
}

void init_chip8(Chip8 emu)
{
  assert(emu != NULL);
  emu->pc = 0x200;
  emu->opcode = 0;
  emu->I = 0;
  emu->sp = 0;
  emu->draw = false;

  init_clear_screen(emu->gfx);
  clear_stack(emu);
  clear_regs(emu);
  clear_memory(emu);

  load_fontset(emu);

  // reset timers
  emu->delay_timer = 0;
  emu->sound_timer = 0;
}

void fetch_opcode(Chip8 emu)
{
  assert(emu != NULL);
  // opcode of 2 Bytes, memory of 1 Byte
  // we merge two  contigous memory locations! 
  emu->opcode = emu->memory[emu->pc] << 8 | emu->memory[emu->pc+1];
}

void increment_pc(Chip8 emu)
{
  assert(emu != NULL);
  emu->pc += 2;
}

void skip_instruction(Chip8 emu)
{
  assert(emu != NULL);
  emu->pc += 4;
}

void execute_clear_screen(Chip8 emu)
{
  assert(emu != NULL);
  init_clear_screen(emu->gfx);
  increment_pc(emu);
}

void execute_return(Chip8 emu)
{
  assert(emu != NULL);
  emu->sp--;
  emu->pc = emu->stack[emu->sp]; // saved return address.
  increment_pc(emu);  
}

unsigned short getNNN(Chip8 emu)
{
  assert(emu != NULL);
  return emu->opcode & 0x0FFF;
}

unsigned short getNN(Chip8 emu)
{
  assert(emu != NULL);
  return emu->opcode & 0x00FF;
}

unsigned short getN(Chip8 emu)
{
  assert(emu != NULL);
  return emu->opcode & 0x000F;
}

unsigned short getX(Chip8 emu)
{  
  assert(emu != NULL);
  unsigned short X = ((emu->opcode & 0x0F00) >> 8);
  return X;
}

void execute_goto(Chip8 emu)
{
  assert(emu != NULL);
  unsigned short addr = getNNN(emu); 
  emu->pc = addr;
}

void execute_subroutine(Chip8 emu)
{
   assert(emu != NULL);
   unsigned short addr = getNNN(emu);
   emu->stack[emu->sp] = emu->pc;
   emu->sp++;
   emu->pc = addr;  
}

unsigned short getVx(Chip8 emu)
{
  assert(emu != NULL);
  return emu->V[(emu->opcode & 0x0F00) >> 8];
}

unsigned short getVy(Chip8 emu)
{
  assert(emu != NULL);
  return emu->V[(emu->opcode & 0x00F0) >> 4];
}

void setVx(Chip8 emu, unsigned short val)
{
  assert(emu != NULL);
  emu->V[(emu->opcode & 0x0F00) >> 8] = val;
}


void execute_eq_reg_mem(Chip8 emu)
{
  assert(emu != NULL);
  unsigned short Vx = getVx(emu);
  unsigned short NN = getNN(emu);

  if (Vx == NN){
    skip_instruction(emu);
  } else{
    increment_pc(emu);
  }

}


void execute_neq_reg_mem(Chip8 emu)
{
  assert(emu != NULL);
  unsigned short Vx = getVx(emu);
  unsigned short NN = getNN(emu);

  if (Vx != NN){
    skip_instruction(emu);
  } else{
    increment_pc(emu);
  } 
}

void execute_eq_reg_reg(Chip8 emu)
{
  assert(emu != NULL);
  unsigned short Vx = getVx(emu);
  unsigned short Vy = getVy(emu);

  if (Vx == Vy){
    skip_instruction(emu);
  } else{
    increment_pc(emu);
  }

}

void execute_set_reg_mem(Chip8 emu)
{
   assert(emu != NULL); 
   unsigned short NN = getNN(emu);
   setVx(emu, NN);

   increment_pc(emu);
}

void execute_add_reg_mem(Chip8 emu)
{
   assert(emu != NULL);
   unsigned short Vx = getVx(emu);
   unsigned short NN = getNN(emu);
   setVx(emu, Vx + NN);

   increment_pc(emu);

}

void execute_set_reg_reg(Chip8 emu)
{
   assert(emu != NULL);
   unsigned short Vy = getVy(emu);
   setVx(emu, Vy);
   increment_pc(emu);
}

void execute_set_bitor_reg_reg(Chip8 emu)
{
   assert(emu != NULL);
   
   unsigned short Vx = getVx(emu);
   unsigned short Vy = getVy(emu);

   setVx(emu, Vx | Vy);

   increment_pc(emu);
}

void execute_set_bitand_reg_reg(Chip8 emu)
{
   assert(emu != NULL);
   unsigned short Vx = getVx(emu);
   unsigned short Vy = getVy(emu);

   setVx(emu, Vx & Vy);

   increment_pc(emu);

}

void execute_set_bitxor_reg_reg(Chip8 emu)
{
   assert(emu != NULL);
   unsigned short Vx = getVx(emu);
   unsigned short Vy = getVy(emu);

   setVx(emu, Vx ^ Vy);

   increment_pc(emu);
}

void execute_add_reg_reg(Chip8 emu)
{
   assert(emu != NULL);
   unsigned short Vx = getVx(emu);
   unsigned short Vy = getVy(emu);

   if (Vx + Vy > 0xFF)
     emu->V[0xF] = 1; // carry
   else
     emu->V[0xF] = 0;
   
   setVx(emu, Vx + Vy);
  
   increment_pc(emu);
}

void execute_dec_reg_reg(Chip8 emu)
{
   assert(emu != NULL);
   unsigned short Vx = getVx(emu);
   unsigned short Vy = getVy(emu);

 
   if (Vx < Vy)
    emu->V[0xF] = 0; // borrow
   else 
    emu->V[0xF] = 1;

   setVx(emu, Vx - Vy);

   increment_pc(emu);
}

void execute_lsr(Chip8 emu)
{
  assert(emu != NULL);
  
  unsigned short Vx = getVx(emu);

  // Stores the least significant bit of VX in VF
  emu->V[0xF] = Vx & 0x1; 
  

  setVx(emu, Vx >> 1); 
  increment_pc(emu);

}

void execute_lsl(Chip8 emu)
{
  assert(emu != NULL);
  unsigned short Vx = getVx(emu);

  // Stores the most significant bit of VX in VF
  emu->V[0xF] = Vx >> 7;

  setVx(emu, Vx << 1);
  increment_pc(emu);

}

void execute_sub_reg_reg(Chip8 emu)
{
   assert(emu != NULL);

   unsigned short Vx = getVx(emu);
   unsigned short Vy = getVy(emu);

   if (Vy < Vx)
    emu->V[0xF] = 0; // borrow
   else
    emu->V[0xF] = 1;

   setVx(emu, Vy - Vx);

   increment_pc(emu);
}

void execute_neq_reg_reg(Chip8 emu)
{
  assert(emu != NULL);
  unsigned short Vx = getVx(emu);
  unsigned short Vy = getVy(emu);

  if (Vx != Vy)
    skip_instruction(emu);

  else
    increment_pc(emu);

}

void execute_set_index_mem(Chip8 emu)
{
  assert(emu != NULL);
  emu->I = getNNN(emu);
  increment_pc(emu);
}

void execute_jump_v0_mem(Chip8 emu)
{
  assert(emu != NULL);
  emu->pc = emu->V[0x0] + getNNN(emu);
}

void execute_reg_rand(Chip8 emu)
{
  assert(emu != NULL);
  setVx(emu, (rand() % 0xFF) & getNN(emu));
  increment_pc(emu);
}

void execute_draw(Chip8 emu)
{
     // DXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
		 // Each row of 8 pixels is read as bit-coded starting from memory location I;
		 // I value doesn't change after the execution of this instruction.
		 // VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn,
		 // and to 0 if that doesn't happen
		  
      assert(emu != NULL);

			unsigned short Vx = getVx(emu);
			unsigned short Vy = getVy(emu);
			unsigned short height = getN(emu);
			unsigned short pixel;

			emu->V[0xF] = 0;

			for (int yline = 0; yline < height; yline++)
			{
				pixel = emu->memory[emu->I + yline];
				for(int xline = 0; xline < 8; xline++)
				{
					if((pixel & (0x80 >> xline)) != 0)
					{
						if(emu->gfx[(Vx + xline + ((Vy + yline) * 64))] == 1)
						{
							emu->V[0xF] = 1;
						}
						emu->gfx[Vx + xline + ((Vy + yline) * 64)] ^= 1;
					}
				}
			}

      emu->draw = true;
		 
      increment_pc(emu);  
}

void execute_skip_if_keypress(Chip8 emu)
{
  assert(emu != NULL);
  unsigned short Vx = getVx(emu);

  if (emu->key[Vx] != 0)
    skip_instruction(emu);
  else
    increment_pc(emu);
}

void execute_skip_if_not_keypress(Chip8 emu)
{
  assert(emu != NULL);
  unsigned short Vx = getVx(emu);

  if (emu->key[Vx] == 0)
    skip_instruction(emu);
  else
    increment_pc(emu);
}

void execute_reg_delay(Chip8 emu)
{
  assert(emu != NULL);
  
  setVx(emu, emu->delay_timer);
  increment_pc(emu);
}

void execute_await_key(Chip8 emu)
{
  assert(emu != NULL);
  bool key_press = false;

  for (int i = 0; i < 16; ++i){
	  if(emu->key[i] != 0){
      setVx(emu, i);
			key_press = true;
		}
	}

	// If we didn't received a keypress, skip this cycle and try again.
  if(!key_press)
	  return;

  increment_pc(emu);  
}

void execute_delay_timer(Chip8 emu)
{
   assert(emu != NULL);     
   emu->delay_timer = getVx(emu);
   increment_pc(emu);
}

void execute_sound_timer(Chip8 emu)
{
   assert(emu != NULL);
   emu->sound_timer = getVx(emu);
   increment_pc(emu);
}

void execute_add_index_reg(Chip8 emu)
{
  assert(emu != NULL);
  emu->I += getVx(emu);
  increment_pc(emu); 
}

void execute_set_index_sprite(Chip8 emu)
{
  assert(emu != NULL);
  unsigned short Vx = getVx(emu);
  emu->I = Vx * 0x5;
  increment_pc(emu); 
}

void execute_bcd(Chip8 emu)
{
  assert(emu != NULL);
  unsigned short Vx = getVx(emu);
  unsigned short I = emu->I;

  emu->memory[I] = Vx / 100;
  emu->memory[I + 1] = (Vx / 10) % 10;
	emu->memory[I + 2] = (Vx % 100) % 10;
  increment_pc(emu);
}

void execute_store(Chip8 emu)
{
  assert(emu != NULL);
  unsigned short X = getX(emu);

  for (int i = 0; i <= X ; ++i)
	  emu->memory[emu->I + i] = emu->V[i];

	emu->I += X + 1;
  increment_pc(emu);
}

void execute_load(Chip8 emu)
{
  assert(emu != NULL);
  
  unsigned short X = getX(emu);

  for (int i = 0; i <= X; ++i)
    emu->V[i] = emu->memory[emu->I + i];

  emu->I += X + 1;
	increment_pc(emu);
}

void decode_and_execute_opcode(Chip8 emu)
{
  assert(emu != NULL);
  unsigned short opcode = emu->opcode;

  switch(opcode & 0xF000){
    case 0x0000:
      switch(opcode & 0x000F){
          case 0x0000: 
            execute_clear_screen(emu);
            break; 

          case 0x000E: 
            execute_return(emu);
            break;
          
          default:
            break;
      }

      break;

    case 0x1000: 
      execute_goto(emu);
      break;
    
    case 0x2000: 
      execute_subroutine(emu);
      break;

    case 0x3000: 
      execute_eq_reg_mem(emu);
      break;
    
    case 0x4000:
      execute_neq_reg_mem(emu);
      break;

    case 0x5000:
      execute_eq_reg_reg(emu);
      break;

    case 0x6000:
      execute_set_reg_mem(emu);
      break;
    
    case 0x7000:
      execute_add_reg_mem(emu);
      break;

    case 0x8000:
      switch (opcode & 0x000F){
        case 0x0000:
          execute_set_reg_reg(emu); 
          break;
        
        case 0x0001:
          execute_set_bitor_reg_reg(emu);
          break;
        
        case 0x0002:
          execute_set_bitand_reg_reg(emu);
          break;

        case 0x0003:
          execute_set_bitxor_reg_reg(emu);
          break;
        
        case 0x0004:
          execute_add_reg_reg(emu);
          break;

        case 0x0005:
          execute_dec_reg_reg(emu);
          break;

        case 0x0006:
          execute_lsr(emu);
          break;

        case 0x0007:
          execute_sub_reg_reg(emu);
          break;

        case 0x000E:
          execute_lsl(emu);
          break;

        default:
          break;
      }
      break;

    case 0x9000:
     execute_neq_reg_reg(emu);
     break;

    case 0xA000:
      execute_set_index_mem(emu);
      break;

    case 0xB000:
     execute_jump_v0_mem(emu);
     break; 
    
    case 0xC000:
     execute_reg_rand(emu);
     break;
    
    case 0xD000:
     execute_draw(emu);
     break;

    case 0xE000:
     switch (opcode & 0x000F){
      case 0x000E:
        execute_skip_if_keypress(emu);
        break;

      case 0x0001:
       execute_skip_if_not_keypress(emu);
       break;

      default:
        break; 
     }

     break;
    
    case 0xF000:
     switch (opcode & 0x00FF){
      case 0x0007:
        execute_reg_delay(emu);
        break;

      case 0x000A:
        execute_await_key(emu);
        break;

      case 0x0015:
        execute_delay_timer(emu);
        break;

      case 0x0018:
        execute_sound_timer(emu);
        break;

      case 0x001E:
        execute_add_index_reg(emu);
        break;

      case 0x0029:
        execute_set_index_sprite(emu);
        break;

      case 0x0033:
        execute_bcd(emu);
        break;

      case 0x0055:
        execute_store(emu);
        break;

      case 0x0065:
        execute_load(emu);
        break;

      default:
        break;
     }

    default:
     break;
  }  
}


void update_timers(Chip8 emu)
{
  assert(emu != NULL);
  if (emu->delay_timer > 0)
    emu->delay_timer--;
  
  if (emu->sound_timer > 0){
    if (emu->sound_timer == 1){
      printf("beeeeep!\n");
      emu->sound_timer--;
    }
  } 
}


void emulation_cycle(Chip8 emu)
{
  fetch_opcode(emu);
  decode_and_execute_opcode(emu);
  update_timers(emu); 
}


bool load_ROM(Chip8 emu, const char * filename)
{
  assert(emu != NULL && filename != NULL);

	printf("Loading: %s\n", filename);

	// Open file
	FILE * pFile = fopen(filename, "rb");
	if (pFile == NULL)
	{
		fputs ("File error", stderr);
		return false;
	}

	// Check file size
	fseek(pFile , 0 , SEEK_END);
	long lSize = ftell(pFile);
  emu->size_ROM = lSize;

	rewind(pFile);
	printf("%d bytes loaded into memory.\n", (int)lSize);

	// Allocate memory to contain the whole file
	char * buffer = (char*)malloc(sizeof(char) * lSize);
	if (buffer == NULL)
	{
		fputs ("Memory error", stderr);
		return false;
	}

	// Copy the file into the buffer
	size_t result = fread (buffer, 1, lSize, pFile);
	if (result != lSize)
	{
		fputs("Reading error",stderr);
		return false;
	}

	// Copy buffer to Chip8 memory
	if((4096-512) > lSize)
	{
		for(int i = 0; i < lSize; ++i)
			emu->memory[i + 512] = buffer[i];
	}
	else
		printf("Error: ROM too big for memory");

	// Close file, free buffer
	fclose(pFile);
	free(buffer);

	return true;
} 
