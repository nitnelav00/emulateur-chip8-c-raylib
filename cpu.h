#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <stdlib.h>
#include "render.h"

#define uint unsigned int
#define uchar unsigned char
#define ushort unsigned short

#define MEMORY_LEN 4096

int translate_keys(uchar in) {
    switch (in) {
    case 0x1:
        return KEY_ONE;
    case 0x2:
        return KEY_TWO;
    case 0x3:
        return KEY_THREE;
    case 0xC:
        return KEY_FOUR;
    case 0x4:
        return KEY_Q;
    case 0x5:
        return KEY_W;
    case 0x6:
        return KEY_E;
    case 0xD:
        return KEY_R;
    case 0x7:
        return KEY_A;
    case 0x8:
        return KEY_S;
    case 0x9:
        return KEY_D;
    case 0xE:
        return KEY_F;
    case 0xA:
        return KEY_Z;
    case 0x0:
        return KEY_X;
    case 0xB:
        return KEY_C;
    case 0xF:
        return KEY_V;
    }
    return 0;
}

uchar translate_keys_rev(uchar in) {
    switch (in) {
    case KEY_ONE:
        return 0x1;
    case KEY_TWO:
        return 0x2;
    case KEY_THREE:
        return 0x3;
    case KEY_FOUR:
        return 0xC;
    case KEY_Q:
        return 0x4;
    case KEY_W:
        return 0x5;
    case KEY_E:
        return 0x6;
    case KEY_R:
        return 0xD;
    case KEY_A:
        return 0x7;
    case KEY_S:
        return 0x8;
    case KEY_D:
        return 0x9;
    case KEY_F:
        return 0xE;
    case KEY_Z:
        return 0xA;
    case KEY_X:
        return 0x0;
    case KEY_C:
        return 0xB;
    case KEY_V:
        return 0xF;
    }
    return 0;
}

typedef struct Node {
    ushort data;
    struct Node *next;
} Node;

typedef struct {
    Render *render;
    uchar memory[MEMORY_LEN];
    uchar registers[16];
    ushort i;
    uchar delay_timer;
    uchar sound_timer;
    ushort pc;
    Node *_stack;
    bool paused;
    uchar key_reg;
    uint speed;
} CPU;

void PushStack(CPU *self, ushort val) {
    Node *nptr = malloc(sizeof(Node));
    nptr->data = val;
    nptr->next = self->_stack;
    self->_stack = nptr;
}

ushort PopStack(CPU *self) {
    if (self->_stack == NULL) {
        printf("Stack is empty");
        exit(1);
    }
    ushort val = self->_stack->data;
    Node *temp = self->_stack;
    self->_stack = self->_stack->next;
    free(temp);
    return val;
}

void FreeCPU(CPU *self){
    while (self->_stack != NULL){
        ushort a = PopStack(self);
        }
    free(self);
}

void UpdateTimers(CPU *self){
    if (self->delay_timer > 0)
        self->delay_timer--;
    if (self->sound_timer > 0)
        self->sound_timer--;
}

void LoadProgramIntoMemory(CPU *self, uchar prog[MEMORY_LEN - 0x200]) {
    for (int i = 0; i < MEMORY_LEN - 0x200; i++)
        self->memory[i + 0x200] = prog[i];
}

void LoadRom(CPU *self, char* rom_path) {
    FILE *in = fopen(rom_path, "rb");
    if (in == NULL) {
        printf("le fichier ne peut pas etre ouvert\n");
        exit(-1);
    }
    uchar rom_file[MEMORY_LEN - 0x200];
    fread(rom_file, sizeof(rom_file), 1, in);
    LoadProgramIntoMemory(self, rom_file);
}

void LoadSpriteIntoMemory(CPU *self) {
    const uchar sprites[] = {
                0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                0x20, 0x60, 0x20, 0x20, 0x70, // 1
                0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    for (int i = 0; i < 80; i++)
        self->memory[i] = sprites[i];
}

CPU *InitCPU(Render *render, char *rom_path, uint speed) {
    CPU *cpu;
    cpu = calloc(1, sizeof(CPU));
    cpu->render = render;
    cpu->speed = speed;
    cpu->_stack = NULL;
    LoadSpriteIntoMemory(cpu);
    LoadRom(cpu, rom_path);
    cpu->pc = 0x200;
    printf("%04X", cpu->pc);
    return cpu;
}

void ExecuteInstruction(CPU *self, ushort opcode) {
    self->pc += 2;
    self->pc = self->pc % MEMORY_LEN;
    ushort x = (opcode & 0x0F00) >> 8;
    ushort y = (opcode & 0x00F0) >> 4;
    // printf("%04x ", opcode);

    switch (opcode & 0xF000) {
    case 0x000:
        switch(opcode) {
        case 0x00E0:
            ClearRender(self->render);
            break;
        case 0x00EE:
            self->pc = PopStack(self);
            printf("%04x ", self->pc);
            break;
        }
        break;
    case 0x1000:
        self->pc = opcode & 0xFFF;
        break;
    case 0x2000:
        PushStack(self, self->pc);
        self->pc = opcode & 0xFFF;
        break;
    case 0x3000:
        if (self->registers[x] == (opcode & 0xFF))
            self->pc += 2;
        break;
    case 0x4000:
        if (self->registers[x] != (opcode & 0xFF))
            self->pc += 2;
        break;
    case 0x5000:
        if (self->registers[x] == self->registers[y])
            self->pc += 2;
        break;
    case 0x6000:
        self->registers[x] = (opcode & 0xFF);
        break;
    case 0x7000:
        self->registers[x] += (opcode & 0xFF);
        break;
    case 0x8000:
        switch (opcode & 0xF) {
        case 0x0:
            self->registers[x] = self->registers[y];
            break;
        case 0x1:
            self->registers[0xF] = 0;
            self->registers[x] |= self->registers[y];
            break;
        case 0x2:
            self->registers[0xF] = 0;
            self->registers[x] &= self->registers[y];
            break;
        case 0x3:
            self->registers[0xF] = 0;
            self->registers[x] ^= self->registers[y];
            break;
        case 0x4:
            ushort sum = self->registers[x] + self->registers[y];
            self->registers[0xF] = 0;
            if (sum > 0xFF)
                self->registers[0xF] = 1;
            self->registers[x] = sum & 0xFF;
            break;
        case 0x5:
            self->registers[0xF] = 0;
            if (self->registers[x] > self->registers[y])
                self->registers[0xF] = 1;
            self->registers[x] -= self->registers[y];
            break;
        case 0x6:
            self->registers[0xF] = self->registers[x] & 1;
            self->registers[x] >>= 1;
            break;
        case 0x7:
            self->registers[0xF] = 0;
            if (self->registers[y] > self->registers[x])
                self->registers[0xF] = 1;
            self->registers[x] = self->registers[y] - self->registers[x];
            break;
        case 0xE:
            self->registers[0xF] = self->registers[x] >> 7;
            self->registers[x] <<= 1;
            break;
        }
    case 0x9000:
        if (self->registers[x] != self->registers[y])
            self->pc += 2;
        break;
    case 0xA000:
        self->i = opcode & 0xFFF;
        break;
    case 0xB000:
        self->pc = (opcode & 0xFFF) + self->registers[0];
        break;
    case 0xC000:
        uchar rd = rand() % 100;
        self->registers[x] = rd & opcode & 0xFF;
        break;
    case 0xD000:
        ushort width = 8;
        ushort height = opcode & 0xF;
        self->registers[0xF] = 0;
        for (int row = 0; row < height; row++) {
            uchar sprite = self->memory[self->i + row];
            for (int col = 0; col < width; col++) {
                if ((sprite & 0x80) > 0) {
                    uchar xpos = self->registers[x] + col;
                    uchar ypos = self->registers[y] + row;
                    if ((xpos >= 0 && xpos < 64) && (ypos >= 0 && ypos < 32)) {
                        bool pixel_res = SetPixelRender(self->render, xpos, ypos);
                        if (pixel_res) self->registers[0xF] = 1;
                    }
                }
                sprite <<= 1;
            }
        }
        break;
    case 0xE000:
        switch (opcode & 0xFF) {
        case 0x9E:
            int key1 = translate_keys(self->registers[x]);
            if (IsKeyDown(key1))
                self->pc += 2;
            break;
        case 0xA1:
            int key2 = translate_keys(self->registers[x]);
            if (!IsKeyDown(key2))
                self->pc += 2;
            break;
        }
        break;
    case 0xF000:
        switch (opcode & 0xFF) {
        case 0x07:
            self->registers[x] = self->delay_timer;
            break;
        case 0x0A:
            self->paused = true;
            self->key_reg = x;
            break;
        case 0x15:
            self->delay_timer = self->registers[x];
            break;
        case 0x18:
            self->sound_timer = self->registers[x];
            break;
        case 0x1E:
            self->i += self->registers[x];
            break;
        case 0x29:
            self->i = self->registers[x] *5;
            break;
        case 0x33:
            self->memory[self->i] = self->registers[x] / 100;
            self->memory[self->i + 1] = (self->registers[x] % 100) / 10;
            self->memory[self->i + 2] = self->registers[x] % 100;
            break;
        case 0x55:
            for (int index = 0; index <= x; index ++)
                self->memory[self->i + index] = self->registers[index];
            break;
        case 0x65:
            for (int index = 0; index <= x; index ++)
                self->registers[index] = self->memory[self->i + index];
            break;
        }
        break;
    }
}

void CycleCPU(CPU *self){
    for(int i = 0; i < self->speed; i++) {
        if(self->paused) break;
        ushort opcode = self->memory[self->pc] << 8 | self->memory[self->pc + 1];
        ExecuteInstruction(self, opcode);
    }

    if(!self->paused) {
        UpdateTimers(self);
    }
    if (self->paused && GetKeyPressed() != 0) {
        self->registers[self->key_reg] = translate_keys_rev(GetKeyPressed());
        self->paused = false;
    }

    UpdateRender(self->render);
}

#endif // CPU_H_INCLUDED
