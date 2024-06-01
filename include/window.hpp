#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "object.hpp"

class Window {
	public:
		Window(const char* p_title, int p_w, int p_h);
		SDL_Texture* loadTexture(const char* p_filePath);

		void close();
		void setTitle(const char* p_title);

		void render(Object& p_object);
		void display();

		void clear();

	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
};