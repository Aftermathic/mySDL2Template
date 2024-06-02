#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cmath>

#include "window.hpp"
#include "object.hpp"

Window::Window(const char* p_title, int p_w, int p_h)
    : window(NULL), renderer(NULL)
{
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, p_w, p_h, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        std::cout << "ERROR: " << SDL_GetError() << std::endl;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        std::cout << "ERROR: " << SDL_GetError() << std::endl;
    }
}

SDL_Texture* Window::loadTexture(const char* p_filePath) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, p_filePath);

    if (texture == NULL) {
        std::cout << "Failed to load texture: " << IMG_GetError() << std::endl;
    }

    return texture;
}

void Window::render(Object& p_object) {
    SDL_Rect src;
    src.x = 0;
    src.y = p_object.getCurrentRect().y;
    src.w = p_object.getCurrentRect().w;
    src.h = p_object.getCurrentRect().h;

    SDL_Rect dst;
    dst.x = static_cast<int>(p_object.getdX());
    dst.y = static_cast<int>(p_object.getdY());
    dst.w = p_object.getCurrentRect().w;
    dst.h = p_object.getCurrentRect().h;

    SDL_RenderCopy(renderer, p_object.getCurrentSprite(), &src, &dst);
}

void Window::clear() {
    SDL_RenderClear(renderer);
}

void Window::display() {
    SDL_RenderPresent(renderer);
}

void Window::setTitle(const char* p_title) {
    SDL_SetWindowTitle(window, p_title);
}

void Window::close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
