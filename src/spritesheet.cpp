#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <iostream>

#include "spritesheet.hpp"

Spritesheet::Spritesheet(SDL_Texture* p_spritesheet, int p_width, int p_height, int p_frames) {
	spritesheet = p_spritesheet;
	width = p_width;
	height = p_height;

	frames = p_frames;
}

SDL_Texture* Spritesheet::getSpritesheet() {
	return spritesheet;
}

int Spritesheet::getWidth() {
	return width;
}

int Spritesheet::getHeight() {
	return height;
}

int Spritesheet::getNumberOfFrames() {
	return frames;
}