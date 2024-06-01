#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <windows.h>
#include <cmath>
#include <vector>

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
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    Window window("Game", 1280, 720);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_MIXER_ERROR: " << Mix_GetError() << std::endl;
        std::cout << "Close this window, unless you want to continue without any sound." << std::endl;
        std::cin.get();
    }

    Mix_Chunk* clickSound = Mix_LoadWAV("res/click.wav");
    if (!clickSound) {
        std::cout << "SDL_MIXER_ERROR: " << Mix_GetError() << std::endl;
        std::cout << "Close this window, unless you want to continue without any sound." << std::endl;
        std::cin.get();
    }

    SDL_Texture* playbtnsprite = window.loadTexture("res/playbtn.png");
    if (!playbtnsprite) {
        std::cout << "Failed to load play button texture" << std::endl;
        return -1;
    }

    int playbtnwidth = 454;
    int playbtnheight = 186;

    Spritesheet playbtnsprites(playbtnsprite, playbtnwidth, playbtnheight, 8);
    Object playbtn(0, 0, playbtnsprites, 0.5);

    std::vector<Object> allobjects = {playbtn};

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

                std::cout << "Mouse clicked at (" << mouseX << ", " << mouseY << ")" << std::endl;

                if (isMouseInside(playbtn.getCurrentRect(), mouseX, mouseY)) {
                    clickedPlay = true;
                    std::cout << "Play button area clicked." << std::endl;
                    if (clickSound) {
                        Mix_PlayChannel(-1, clickSound, 0);
                    }
                }
            }
        }

        if (clickedPlay) {
            std::cout << "Play button clicked!" << std::endl;
            clickedPlay = false;
        }

        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = static_cast<double>(NOW - LAST) / SDL_GetPerformanceFrequency();
        framerate = 1.0 / deltaTime;

        window.clear();

        for (Object& o : allobjects) {
            window.render(o);
            o.animate(deltaTime);
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
