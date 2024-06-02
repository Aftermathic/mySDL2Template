#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <windows.h>
#include <cmath>
#include <vector>
#include <random>

#include "window.hpp"
#include "spritesheet.hpp"
#include "object.hpp"

void HideConsole() {
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void ShowConsole() {
    ::ShowWindow(::GetConsoleWindow(), SW_SHOWMAXIMIZED);
}

bool isMouseInside(Object ob, SDL_Rect rect, int mouseX, int mouseY) {
    return mouseX >= ob.getdX() && mouseX <= ob.getdX() + rect.w &&
           mouseY >= ob.getdY() && mouseY <= ob.getdY() + rect.h;
}

int main(int argc, char* args[]) {
    ShowConsole();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        std::cin.get();

        return -1;
    }

    Window window("Game", 1280, 720);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_MIXER_ERROR: " << Mix_GetError() << std::endl;
        std::cout << "Close this window, unless you want to continue without any sound (press enter to continue)." << std::endl;
        std::cin.get();
    }

    Mix_Chunk* clickSound = Mix_LoadWAV("res/click.wav");
    if (!clickSound) {
        std::cout << "SDL_MIXER_ERROR: " << Mix_GetError() << std::endl;
        std::cout << "Close this window, unless you want to continue without any sound (press enter to continue)." << std::endl;
        std::cin.get();
    }

    SDL_Texture* playbtnsprite = window.loadTexture("res/playbtn.png");
    if (!playbtnsprite) {
        std::cout << "Failed to load play button texture (press enter to end all)" << std::endl;
        std::cin.get();

        return -1;
    }

    HideConsole();

    int playbtnwidth = 454;
    int playbtnheight = 186;

    Spritesheet playbtnsprites(playbtnsprite, playbtnwidth, playbtnheight, 8);
    Object playbtn(0, 0, playbtnsprites, 0.5);

    std::vector<Object> allobjects = {playbtn};

    bool gameRunning = true;
    bool clickedPlay = false;
    bool playbuttonGone = false;

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;

    SDL_Event event;

    float velocityX = 500;
    float velocityY = 400;

    while (gameRunning) {
    	SDL_PumpEvents();
    	
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameRunning = false;
            }

            if (! clickedPlay) {
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                std::cout << "Mouse clicked at (" << mouseX << ", " << mouseY << ")" << std::endl;

                int num = 0;
                for (Object& o : allobjects) {
                	if (num == 0) {
                		if (isMouseInside(o, o.getCurrentRect(), mouseX, mouseY)) {
                			clickedPlay = true;
                			std::cout << "Clicked!" << std::endl;

                			SDL_Delay(1000);

                			if (clickSound) {
                				Mix_PlayChannel(-1, clickSound, 0);
                			}
                		}
                	}
                }
            }

            }

            SDL_PumpEvents();
        }

        if (clickedPlay && !playbuttonGone) {
            std::cout << "Play button clicked!" << std::endl;
            playbuttonGone = true;
        }

        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = static_cast<double>(NOW - LAST) / SDL_GetPerformanceFrequency();

        window.clear();

        int num = 0;
        for (Object& o : allobjects) {
            if (!(clickedPlay && num == 0)) {
                window.render(o);
                o.animate(deltaTime);
            }

            if (num == 0) {
                float newX = o.getdX() + velocityX * deltaTime;
                float newY = o.getdY() + velocityY * deltaTime;

                if (newX <= 0 || newX >= 1280 - playbtnwidth) {
                    velocityX = -velocityX;
                    newX = std::max(0.0f, std::min(static_cast<float>(1280 - playbtnwidth), newX));
                }

                if (newY <= 0 || newY >= 720 - playbtnheight) {
                    velocityY = -velocityY;
                    newY = std::max(0.0f, std::min(static_cast<float>(720 - playbtnheight), newY));
                }

                o.setX(static_cast<int>(newX));
                o.setY(static_cast<int>(newY));
            }

            num++;
        }

        window.display();
    }

    if (clickSound) {
        Mix_FreeChunk(clickSound);
    }

    Mix_CloseAudio();

    window.close();
    SDL_Quit();

    return 0;
}
