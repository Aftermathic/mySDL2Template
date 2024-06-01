#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <iostream>

#include "object.hpp"
#include "spritesheet.hpp"

Object::Object(float p_x, float p_y, Spritesheet p_spritesheet, float rate)
    :sprites(p_spritesheet) 
{
    frame = 0;

    x = p_x;
    y = p_y;
    animationRate = rate;
    w = sprites.getWidth();
    h = sprites.getHeight();

    originalHeight = sprites.getHeight();

    currentRect.x = x;
    currentRect.y = y;
    currentRect.w = w;
    currentRect.h = h;

    currentSprite = sprites.getSpritesheet();
}

float Object::getX() {
	return x;
}

float Object::getY() {
	return y;
}

void Object::setX(float p_x) {
	x = p_x;
}

void Object::setY(float p_y) {
	y = p_y;
}

int Object::getOriginalHeight() {
	return originalHeight;
}

void Object::nextFrame() {
    frame++;
    if (frame >= sprites.getNumberOfFrames()) {
        frame = 0;
    }

    currentRect.y = y + h * frame;
    currentSprite = sprites.getSpritesheet();
}

void Object::animate(float deltatime) {
	time = time + deltatime;
	if (time >= animationRate) {
		frame++;
		if (frame >= sprites.getNumberOfFrames()) {
			frame = 0;
		}

		time = 0;
	}

    currentRect.y = y + h * frame;
    currentSprite = sprites.getSpritesheet();
}

int Object::getCurrentFrame() {
	return frame;
}

void Object::changeSpritesheet(Spritesheet p_sprites) {
	sprites = p_sprites;
	frame = 0;

	w = sprites.getWidth();
	h = sprites.getHeight() * frame;

	currentRect.x = x;
	currentRect.y = y;
	currentRect.w = w;
	currentRect.h = h;

	currentSprite = sprites.getSpritesheet();
}

SDL_Texture* Object::getCurrentSprite() {
	return currentSprite;
}

SDL_Rect Object::getCurrentRect() {
	return currentRect;
}