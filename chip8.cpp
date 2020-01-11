#include "chip8.h"
#include <time.h>

unsigned char chip8_fontset[FONTS] =
{
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

void chip8::opcode_00Ex_handle() {
	switch (opcode & 0x00FF)
	{
	case 0x00E0:
		opcode_00E0_handle();
		break;
	case 0x00EE:
		opcode_00EE_handle();
		break;
	}
}
void chip8::opcode_00E0_handle() {
	memset(graphics, 0, GRAPHICS_COLUMNS * GRAPHICS_ROWS);
	programCounter += 2;
}
void chip8::opcode_00EE_handle() {
	--stackPointer;
	programCounter = stack[stackPointer];
	programCounter += 2;
}
void chip8::opcode_1NNN_handle() {
	programCounter = opcode & 0x0FFF;
}
void chip8::opcode_2NNN_handle() {
	stack[stackPointer] = programCounter;
	++stackPointer;
	programCounter = opcode & 0x0FFF;
}
void chip8::opcode_3XNN_handle() {
	if (cpuRegisters[((opcode & 0x0F00) >> 8)] == (opcode & 0x00FF)) {
		programCounter += 4;
	}
	else {
		programCounter += 2;
	}
}
void chip8::opcode_4XNN_handle() {
	if (cpuRegisters[(opcode & 0x0F00) >> 8] != opcode & 0x00FF) {
		programCounter += 4;
	}
	else {
		programCounter += 2;
	}
}
void chip8::opcode_5XY0_handle() {
	if (cpuRegisters[(opcode & 0x0F00) >> 8] == cpuRegisters[(opcode & 0x00F0) >> 4]) {
		programCounter += 4;
	}
	else {
		programCounter += 2;
	}
}
void chip8::opcode_6XNN_handle() {
	cpuRegisters[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
	programCounter += 2;
}
void chip8::opcode_7XNN_handle() {
	cpuRegisters[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
	programCounter += 2;
}
void chip8::opcode_8XYx_handle() {
	switch (opcode & 0x000F)
	{
	case 0x0000:
		opcode_8XY0_handle();
		break;
	case 0x0001:
		opcode_8XY1_handle();
		break;
	case 0x0002:
		opcode_8XY2_handle();
		break;
	case 0x0003:
		opcode_8XY3_handle();
		break;
	case 0x0004:
		opcode_8XY4_handle();
		break;
	case 0x0005:
		opcode_8XY5_handle();
		break;
	case 0x0006:
		opcode_8XY6_handle();
		break;
	case 0x0007:
		opcode_8XY7_handle();
		break;
	case 0x000E:
		opcode_8XYE_handle();
		break;
	}
}
void chip8::opcode_8XY0_handle() {
	cpuRegisters[(opcode & 0x0F00) >> 8] = cpuRegisters[(opcode & 0x00F0) >> 4];
	programCounter += 2;
}
void chip8::opcode_8XY1_handle() {
	cpuRegisters[(opcode & 0x0F00) >> 8] |= cpuRegisters[(opcode & 0x00F0) >> 4];
	programCounter += 2;
}
void chip8::opcode_8XY2_handle() {
	cpuRegisters[(opcode & 0x0F00) >> 8] &= cpuRegisters[(opcode & 0x00F0) >> 4];
	programCounter += 2;
}
void chip8::opcode_8XY3_handle() {
	cpuRegisters[(opcode & 0x0F00) >> 8] ^= cpuRegisters[(opcode & 0x00F0) >> 4];
	programCounter += 2;
}
void chip8::opcode_8XY4_handle() {
	if (cpuRegisters[(opcode & 0x00F0) >> 4] > (0xFF - cpuRegisters[(opcode & 0x0F00) >> 8])) {
		cpuRegisters[0xF] = 1;
	}
	else {
		cpuRegisters[0xF] = 0;
	}
	cpuRegisters[(opcode & 0x0F00) >> 8] += cpuRegisters[(opcode & 0x00F0) >> 4];
	programCounter += 2;
}
void chip8::opcode_8XY5_handle() {
	if (cpuRegisters[(opcode & 0x00F0) >> 4] > cpuRegisters[(opcode & 0x0F00) >> 8]) {
		cpuRegisters[0xF] = 0;
	}
	else {
		cpuRegisters[0xF] = 1;
	}
	cpuRegisters[(opcode & 0x0F00) >> 8] -= cpuRegisters[(opcode & 0x00F0) >> 4];
	programCounter += 2;
}
void chip8::opcode_8XY6_handle() {
	cpuRegisters[0xF] = cpuRegisters[(opcode & 0x0F00) >> 8] & 0x1;
	cpuRegisters[(opcode & 0x0F00) >> 8] >>= 1;
	programCounter += 2;
}
void chip8::opcode_8XY7_handle() {
	if (cpuRegisters[(opcode & 0x00F0) >> 4] > cpuRegisters[(opcode & 0x0F00) >> 8]) {
		cpuRegisters[0xF] = 1;
	}
	else {
		cpuRegisters[0xF] = 0;
	}
	cpuRegisters[(opcode & 0x0F00) >> 8] = cpuRegisters[(opcode & 0x00F0) >> 4] - cpuRegisters[(opcode & 0x0F00) >> 8];
	programCounter += 2;
}
void chip8::opcode_8XYE_handle() {
	cpuRegisters[0xF] = cpuRegisters[(opcode & 0x0F00) >> 8] >> 7;
	cpuRegisters[(opcode & 0x0F00) >> 8] <<= 1;
	programCounter += 2;
}
void chip8::opcode_9XY0_handle() {
	if (cpuRegisters[(opcode & 0x0F00) >> 8] != cpuRegisters[(opcode & 0x00F0) >> 4]) {
		programCounter += 4;
	}
	else {
		programCounter += 2;
	}
}
void chip8::opcode_ANNN_handle() {
	indexRegister = opcode & 0x0FFF;
	programCounter += 2;
}
void chip8::opcode_BNNN_handle() {
	programCounter = (opcode & 0x0FFF) + cpuRegisters[0];
}
void chip8::opcode_CXNN_handle() {
	cpuRegisters[(opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (opcode & 0x00FF);
	programCounter += 2;
}
void chip8::opcode_DXYN_handle() {
	unsigned short x = cpuRegisters[(opcode & 0x0F00) >> 8];
	unsigned short y = cpuRegisters[(opcode & 0x00F0) >> 4];
	unsigned short height = opcode & 0x000F;
	unsigned short pixel;

	cpuRegisters[0xF] = 0;
	for (int yline = 0; yline < height; yline++)
	{
		pixel = memory[indexRegister + yline];
		for (int xline = 0; xline < 8; xline++)
		{
			if ((pixel & (0x80 >> xline)) != 0)
			{
				if (graphics[(x + xline + ((y + yline) * 64))] == 1)
				{
					cpuRegisters[0xF] = 1;
				}
				graphics[x + xline + ((y + yline) * 64)] ^= 1;
			}
		}
	}

	drawFlag = true;
	programCounter += 2;
}
void chip8::opcode_EX9x_handle() {
	switch (opcode & 0x00FF)
	{
	case 0x009E:
		opcode_EX9E_handle();
		break;
	case 0x00A1:
		opcode_EXA1_handle();
		break;
	}
}
void chip8::opcode_EX9E_handle() {
	if (key[cpuRegisters[(opcode & 0x0F00) >> 8]] != 0) {
		programCounter += 4;
	}
	else {
		programCounter += 2;
	}
}
void chip8::opcode_EXA1_handle() {
	if (key[cpuRegisters[(opcode & 0x0F00) >> 8]] == 0) {
		programCounter += 4;
	}
	else {
		programCounter += 2;
	}
}
void chip8::opcode_FXxx_handle() {
	switch (opcode & 0x00FF)
	{
	case 0x0007:
		opcode_FX07_handle();
		break;
	case 0x000A:
		opcode_FX0A_handle();
		break;
	case 0x0015:
		opcode_FX15_handle();
		break;
	case 0x0018:
		opcode_FX18_handle();
		break;
	case 0x001E:
		opcode_FX1E_handle();
		break;
	case 0x0029:
		opcode_FX29_handle();
		break;
	case 0x0033:
		opcode_FX33_handle();
		break;
	case 0x0055:
		opcode_FX55_handle();
		break;
	case 0x0065:
		opcode_FX65_handle();
		break;
	}
}
void chip8::opcode_FX07_handle() {
	cpuRegisters[(opcode & 0x0F00) >> 8] = delayTimer;
	programCounter += 2;
}
void chip8::opcode_FX0A_handle() {
	bool isKeyPressed = false;
	for (auto i = 0; i < KEYS && !isKeyPressed; ++i) {
		if (key[i] != 0) {
			cpuRegisters[(opcode & 0x0F00) >> 8] = i;
			isKeyPressed = true;
		}
	}
	if (!isKeyPressed) {
		return;
	}
	programCounter += 2;
}
void chip8::opcode_FX15_handle() {
	delayTimer = cpuRegisters[(opcode & 0x0F00) >> 8];
	programCounter += 2;
}
void chip8::opcode_FX18_handle() {
	soundTimer = cpuRegisters[(opcode & 0x0F00) >> 8];
	programCounter += 2;
}
void chip8::opcode_FX1E_handle() {
	if (indexRegister + cpuRegisters[(opcode & 0x0F00) >> 8] > 0xFFF) {
		cpuRegisters[0xF] = 1;
	}
	else {
		cpuRegisters[0xF] = 0;
	}
	indexRegister = indexRegister + cpuRegisters[(opcode & 0x0F00) >> 8];
	programCounter += 2;
}
void chip8::opcode_FX29_handle() {
	indexRegister = cpuRegisters[(opcode & 0x0F00) >> 8] * 0x5;
	programCounter += 2;
}
void chip8::opcode_FX33_handle() {
	memory[indexRegister] = cpuRegisters[(opcode & 0x0F00) >> 8] / 100;
	memory[indexRegister + 1] = (cpuRegisters[(opcode & 0x0F00) >> 8] / 10) % 10;
	memory[indexRegister + 2] = (cpuRegisters[(opcode & 0x0F00) >> 8] % 100) % 10;
	programCounter += 2;
}
void chip8::opcode_FX55_handle() {
	for (auto i = 0; i <= ((opcode & 0x0F00) >> 8); ++i) {
		memory[indexRegister + i] = cpuRegisters[i];
	}
	indexRegister += ((opcode & 0x0F00) >> 8) + 1;
	programCounter += 2;
}
void chip8::opcode_FX65_handle() {
	for (auto i = 0; i <= ((opcode & 0x0F00) >> 8); ++i) {
		cpuRegisters[i] = memory[indexRegister + i];
	}
	indexRegister += ((opcode & 0x0F00) >> 8) + 1;
	programCounter += 2;
}

void chip8::decodeopcode()
{
	switch (opcode & 0xF000)
	{
	case 0x0000:
		opcode_00Ex_handle();
		break;
	case 0x1000:
		opcode_1NNN_handle();
		break;
	case 0x2000:
		opcode_2NNN_handle();
		break;
	case 0x3000:
		opcode_3XNN_handle();
		break;
	case 0x4000:
		opcode_4XNN_handle();
		break;
	case 0x5000:
		opcode_5XY0_handle();
		break;
	case 0x6000:
		opcode_6XNN_handle();
		break;
	case 0x7000:
		opcode_7XNN_handle();
		break;
	case 0x8000:
		opcode_8XYx_handle();
		break;
	case 0x9000:
		opcode_9XY0_handle();
		break;
	case 0xA000:
		opcode_ANNN_handle();
		break;
	case 0xB000:
		opcode_BNNN_handle();
		break;
	case 0xC000:
		opcode_CXNN_handle();
		break;
	case 0xD000:
		opcode_DXYN_handle();
		break;
	case 0xE000:
		opcode_EX9x_handle();
		break;
	case 0xF000:
		opcode_FXxx_handle();
		break;
	}
}

void chip8::initializeFunctionPointers()
{
	//handlers[0] = &chip8::opcode_00Ex_handle;
	//handlers[1] = &chip8::opcode_1NNN_handle;
	//handlers[2] = &chip8::opcode_2NNN_handle;
	//handlers[3] = &chip8::opcode_3XNN_handle;
	//handlers[4] = &chip8::opcode_4XNN_handle;
	//handlers[5] = &chip8::opcode_5XY0_handle;
	//handlers[6] = &chip8::opcode_6XNN_handle;
	//handlers[7] = &chip8::opcode_7XNN_handle;
	//handlers[8] = &chip8::opcode_8XYx_handle;
	//handlers[9] = &chip8::opcode_9XY0_handle;
	//handlers[10] = &chip8::opcode_ANNN_handle;
	//handlers[11] = &chip8::opcode_BNNN_handle;
	//handlers[12] = &chip8::opcode_CXNN_handle;
	//handlers[13] = &chip8::opcode_DXYN_handle;
	//handlers[14] = &chip8::opcode_EX9x_handle;
	//handlers[15] = &chip8::opcode_FXxx_handle;
}

void chip8::debugRender()
{
	// Draw
	for (int y = 0; y < GRAPHICS_ROWS; ++y)
	{
		for (int x = 0; x < GRAPHICS_COLUMNS; ++x)
		{
			if (graphics[(y * GRAPHICS_COLUMNS) + x] == 0)
				printf("O");
			else
				printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}

bool chip8::loadApplication(const char* fileName)
{
	initialize();
	
	return loadFile(fileName);
}

bool chip8::loadFile(const char* fileName) {
	//Load Progam here
	FILE* fp = fopen(fileName, "rb");
	if (fp == nullptr) {
		fputs("Error loading program", stderr);
		return false;
	}

	//Get file size
	fseek(fp, 0, SEEK_END);
	long fSize = ftell(fp);
	rewind(fp);

	if (fSize > MEMORY_SIZE - ROM_RAM_START) {
		fputs("program is too big", stderr);
		fclose(fp);
		return false;
	}
	std::cout << "File Size: " << fSize << std::endl;

	auto readSize = fread(&memory[ROM_RAM_START], 1, fSize, fp);
	if (readSize != fSize) {
		fputs("Failed to read pprogram", stderr);
		fclose(fp);
		return false;
	}

	fclose(fp);
	return true;
}

void chip8::emulateCycle()
{
	//Fetch opcode
	opcode = (memory[programCounter] << 8) | memory[programCounter + 1];

	//Decode opcode
	decodeopcode();
}

void chip8::keyPressed(int k)
{
	key[k] = 1;
}

void chip8::keyRelease(int k)
{
	key[k] = 0;
}

void chip8::DecreaseTimers()
{
	if (delayTimer > 0) {
		--delayTimer;
	}
	if (soundTimer > 0) {
		if (soundTimer == 1) {
			std::cout << "BEEP" << std::endl;
		}
		--soundTimer;
	}
}

void chip8::initialize()
{
	programCounter = ROM_RAM_START;
	opcode = 0;
	indexRegister = 0;
	stackPointer = 0;
	delayTimer = 0;
	soundTimer = 0;

	//Clear display
	memset(graphics, 0, GRAPHICS_COLUMNS * GRAPHICS_ROWS);

	//Clear Memory
	memset(memory, 0, MEMORY_SIZE);

	//Clear Registers
	memset(cpuRegisters, 0, CPU_REGISTERS);

	//Clear Stack
	memset(stack, 0, STACK_SIZE);

	//Clear Key
	memset(key, 0, KEYS);

	//Set Font Set
	for (auto i = 0; i < FONTS; ++i) {
		memory[i] = chip8_fontset[i];
	}

	drawFlag = true;

	srand(time(NULL));

	initializeFunctionPointers();
}
