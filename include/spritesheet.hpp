#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

class Spritesheet {
    public:
    	Spritesheet(SDL_Texture* p_spritesheet, int p_width, int p_height, int p_frames);

    	SDL_Texture* getSpritesheet();
    	int getWidth();
    	int getHeight();

    	int getNumberOfFrames();

    private:
    	SDL_Texture* spritesheet;
    	int width;
    	int height;

        int frames;
};