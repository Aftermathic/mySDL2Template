#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <windows.h>
#include <cmath>

#include "window.hpp"
#include "spritesheet.hpp"
#include "object.hpp"

void HideConsole() {
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void ShowConsole() {
    ::ShowWindow(::GetConsoleWindow(), SW_SHOWMAXIMIZED);
}

bool isMouseInside(SDL_Rect rect, int mouseX, int mouseY) {
    return mouseX >= rect.x && mouseX <= rect.x + rect.w && mouseY >= rect.y && mouseY <= rect.y + rect.h;
}

int main(int argc, char* args[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	//HideConsole();

	Window window("Game", 1280, 720);

	Mix_Chunk* clickSound = Mix_LoadWAV("res/click.wav");
    if (!clickSound) {
        std::cout << "SDL_MIXER_ERROR: " << Mix_GetError() << std::endl;
    }

	SDL_Texture* playbtnsprite = window.loadTexture("res/playbtn.png");
	int playbtnwidth = 454;
	int playbtnheight = 186;

	Spritesheet playbtnsprites(playbtnsprite, playbtnwidth, playbtnheight, 8);
	Object playbtn(0, 0, playbtnsprites, 0.5);

	std::vector<Object> allobjects = {};
	allobjects.push_back(playbtn);

	bool gameRunning = true;
	bool clickedPlay = false;

	Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;

	SDL_Event event;

	float framerate;	
	while (gameRunning) {
		SDL_PumpEvents();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				gameRunning = false;
			}

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
    
                if (isMouseInside(playbtn.getCurrentRect(), mouseX, mouseY)) {
                    clickedPlay = true;
                    Mix_PlayChannel(-1, clickSound, 0);
                }
            }
		}

        if (clickedPlay) {
            std::cout << "Play button clicked!" << std::endl;
            clickedPlay = false;
        }

        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = (NOW - LAST) / (double)SDL_GetPerformanceFrequency();
        framerate = 1/deltaTime;

		window.clear();

		for (Object& o : allobjects) {
			window.render(o);
			o.animate(deltaTime);
		}

		window.display();
	}

	Mix_FreeChunk(clickSound);
    Mix_CloseAudio();

	window.close();
	SDL_Quit();

	return 0;
}