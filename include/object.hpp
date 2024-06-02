#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "spritesheet.hpp"

class Object {
    public:
        Object(float p_x, float p_y, Spritesheet p_spritesheet, float rate);

        float getX();
        float getY();

        void setX(float p_x);
        void setY(float p_y);
        float getdX();
        float getdY();

        int getCurrentFrame();

        void nextFrame();
        void animate(float deltatime);
        void changeSpritesheet(Spritesheet p_sprites);

        int getOriginalHeight();

        SDL_Texture* getCurrentSprite();
        SDL_Rect getCurrentRect();

    private:
        float x, y;
        float dstX, dstY;
        int w, h;
        float animationRate;

        float time;
        int frame;
        int originalHeight;

        SDL_Texture* currentSprite;
        SDL_Rect currentRect;

        Spritesheet sprites;
};
