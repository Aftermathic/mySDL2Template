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

        int getCurrentFrame();

    	void nextFrame();
        void animate(float deltatime);
        void changeSpritesheet(Spritesheet p_sprites);

        int getOriginalHeight();

    	SDL_Texture* getCurrentSprite();
    	SDL_Rect getCurrentRect();

    private:
    	float x, y;
        int w, h;
        float animationRate;

        float time = 0;
        int originalHeight;

    	int frame = 0;
    	SDL_Texture* currentSprite;
    	SDL_Rect currentRect;

    	Spritesheet sprites;
};