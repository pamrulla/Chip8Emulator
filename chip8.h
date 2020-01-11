#pragma once
#include <iostream>

#define MEMORY_SIZE 4096
#define CPU_REGISTERS 16
#define STACK_SIZE 16
#define GRAPHICS_COLUMNS 64
#define GRAPHICS_ROWS 32
#define KEYS 16

#define FONT_START 0x50
#define ROM_RAM_START 0x200
#define FONTS 80

class chip8
{
public:
    bool drawFlag;
    unsigned char graphics[GRAPHICS_COLUMNS * GRAPHICS_ROWS];

    bool loadApplication(const char* fileName);
	bool loadFile(const char* fileName);
    void emulateCycle();
	void keyPressed(int k);
	void keyRelease(int k);
	void DecreaseTimers();

private:
	void initialize();
	void opcode_00Ex_handle();
	void opcode_00E0_handle();
	void opcode_00EE_handle();
	void opcode_1NNN_handle();
	void opcode_2NNN_handle();
	void opcode_3XNN_handle();
	void opcode_4XNN_handle();
	void opcode_5XY0_handle();
	void opcode_6XNN_handle();
	void opcode_7XNN_handle();
	void opcode_8XYx_handle();
	void opcode_8XY0_handle();
	void opcode_8XY1_handle();
	void opcode_8XY2_handle();
	void opcode_8XY3_handle();
	void opcode_8XY4_handle();
	void opcode_8XY5_handle();
	void opcode_8XY6_handle();
	void opcode_8XY7_handle();
	void opcode_8XYE_handle();
	void opcode_9XY0_handle();
	void opcode_ANNN_handle();
	void opcode_BNNN_handle();
	void opcode_CXNN_handle();
	void opcode_DXYN_handle();
	void opcode_EX9x_handle();
	void opcode_EX9E_handle();
	void opcode_EXA1_handle();
	void opcode_FXxx_handle();
	void opcode_FX07_handle();
	void opcode_FX0A_handle();
	void opcode_FX15_handle();
	void opcode_FX18_handle();
	void opcode_FX1E_handle();
	void opcode_FX29_handle();
	void opcode_FX33_handle();
	void opcode_FX55_handle();
	void opcode_FX65_handle();

	void decodeopcode();

	void initializeFunctionPointers();

	void debugRender();

	//typedef void(*FunctionopcodeHandlers)();
	//void(chip8::*handlers[16])();
	
	unsigned short opcode;
	unsigned short indexRegister;
	unsigned short programCounter;
	unsigned char memory[MEMORY_SIZE];
	unsigned char cpuRegisters[CPU_REGISTERS];
	unsigned short stack[STACK_SIZE];
	unsigned short stackPointer;
	unsigned char delayTimer;
	unsigned char soundTimer;
	unsigned char key[KEYS];
};

