#include <iostream>
#include "SDL.h"
#include "SDL_opengl.h"
#include "chip8.h"

static const int modifier = 10;

static int displayWidth = GRAPHICS_COLUMNS * modifier;
static int displayHeight = GRAPHICS_ROWS * modifier;

chip8 c8;

void HandleInput(const SDL_Event& event) {
	if (event.type == SDL_KEYDOWN) {
		int key = -1;
		switch (event.key.keysym.sym)
		{
		case SDLK_x: key = 0; break;
		case SDLK_1: key = 1; break;
		case SDLK_2: key = 2; break;
		case SDLK_3: key = 3; break;
		case SDLK_q: key = 4; break;
		case SDLK_w: key = 5; break;
		case SDLK_e: key = 6; break;
		case SDLK_a: key = 7; break;
		case SDLK_s: key = 8; break;
		case SDLK_d: key = 9; break;
		case SDLK_z: key = 10; break;
		case SDLK_c: key = 11; break;
		case SDLK_4: key = 12; break;
		case SDLK_r: key = 13; break;
		case SDLK_f: key = 14; break;
		case SDLK_v: key = 15; break;
		default: break;
		}
		if (key != -1)
		{
			c8.keyPressed(key);
		}
	}
	else if (event.type == SDL_KEYUP)
	{
		int key = -1;
		switch (event.key.keysym.sym)
		{
		case SDLK_x: key = 0; break;
		case SDLK_1: key = 1; break;
		case SDLK_2: key = 2; break;
		case SDLK_3: key = 3; break;
		case SDLK_q: key = 4; break;
		case SDLK_w: key = 5; break;
		case SDLK_e: key = 6; break;
		case SDLK_a: key = 7; break;
		case SDLK_s: key = 8; break;
		case SDLK_d: key = 9; break;
		case SDLK_z: key = 10; break;
		case SDLK_c: key = 11; break;
		case SDLK_4: key = 12; break;
		case SDLK_r: key = 13; break;
		case SDLK_f: key = 14; break;
		case SDLK_v: key = 15; break;
		default: break;
		}
		if (key != -1)
		{
			c8.keyRelease(key);
		}
	}
}

SDL_Window* SetupDisplay() {

	SDL_Init(SDL_INIT_EVERYTHING);
	auto* window = SDL_CreateWindow("Chip8 Emulator", 100, 100, displayWidth, displayHeight, SDL_WINDOW_OPENGL);
	SDL_GL_CreateContext(window);

	glViewport(0, 0, displayWidth, displayHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0, displayWidth, displayHeight, 0, -1.0, 1.0);
	glClearColor(0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_FLAT);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DITHER);
	glDisable(GL_BLEND);
	return window;
}

void RenderFrame(SDL_Window* window) {
	if (c8.drawFlag) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glRasterPos2i(-1, 1);
		glPixelZoom(1, -1);
		BYTE		m_ScreenData[320][640][3] = { 0 };
		unsigned char color = 0;
		for (int y = 0; y < GRAPHICS_ROWS; ++y)
		{
			for (int x = 0; x < GRAPHICS_COLUMNS; ++x)
			{
				if (c8.graphics[(y * GRAPHICS_COLUMNS) + x] == 1)
					color = 255;
				else
					color = 0;

				for (auto i = y * modifier; i < (y * modifier) + modifier; ++i) {
					for (auto j = x * modifier; j < (x * modifier) + modifier; ++j) {
						for (auto k = 0; k < 3; ++k) {
							m_ScreenData[i][j][k] = color;
						}
					}
				}

			}
		}
		glDrawPixels(displayWidth, displayHeight, GL_RGB, GL_UNSIGNED_BYTE, m_ScreenData);
		SDL_GL_SwapWindow(window);
		glFlush();
		c8.drawFlag = false;
	}
}

int main(int argc, char* argv[]) {

	int opcodesPerSoncond = 400;
	int fps = 60;
	int opcodesPerFrame = opcodesPerSoncond / fps;
	float interval = 1000.0f / fps;
	auto startTime = SDL_GetTicks();

	if (argc < 2) {
		fputs("Please pass program to run", stderr);
		return 1;
	}

	if (!c8.loadApplication(argv[1])) {
		return 1;
	}

	auto window = SetupDisplay();

	bool isQuit = false;
	SDL_Event event;
	
	while (!isQuit) {
		while (SDL_PollEvent(&event)) {
			HandleInput(event);
			if (event.type == SDL_QUIT) {
				isQuit = true;
			}
		}

		auto currentTime = SDL_GetTicks();
		if ((startTime + interval) < currentTime) {

			c8.DecreaseTimers();
			for (auto i = 0; i < opcodesPerFrame; ++i) {
				c8.emulateCycle();
			}

			startTime = currentTime;

			RenderFrame(window);
		}
	}

	SDL_Quit();
	SDL_DestroyWindow(window);
	return 0;
}
