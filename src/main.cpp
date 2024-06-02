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

    double counter1 = 0; // max is pi  changes by 0.03
    double counter2 = 0; // max is pi/2  changes by 0.015

    bool gameRunning = true;
    bool clickedPlay = false;
    bool playbuttonGone = false;

    bool goback1 = false;
    bool goback2 = false;

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;

    SDL_Event event;

    while (gameRunning) {
        SDL_PumpEvents();

        // button stuff
        if (counter1 >= M_PI) {
            goback1 = true;
        }

        if (counter2 >= M_PI / 2) {
            goback2 = true;
        }

        if (counter1 <= 0) {
            goback1 = false;
        }

        if (counter2 <= 0) {
            goback2 = false;
        }

        if (goback1) {
            counter1 -= 0.001 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (0.01 - 0.001));
        } 
        else {
            counter1 += 0.001 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (0.01 - 0.001));
        }

        if (goback2) {
            counter2 -= 0.001 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (0.01 - 0.001));
        } 
        else {
            counter2 += 0.001 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (0.01 - 0.001));
        }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameRunning = false;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                std::cout << "Mouse clicked at (" << mouseX << ", " << mouseY << ")" << std::endl;

                if (isMouseInside(playbtn, playbtn.getCurrentRect(), mouseX, mouseY)) {
                    clickedPlay = true;
                    std::cout << "Play button area clicked." << std::endl;
                    
                    if (clickSound) {
                        Mix_PlayChannel(-1, clickSound, 0);
                    }
                }
            }
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

            // more button stuff

            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist(1, 5);

            int random_number = dist(rng);

            if (num == 0) {
                o.setX(std::round(1280 * std::sin(counter1 * random_number)));
                o.setY(std::round(720 * std::sin(counter2 * random_number)));
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
