#pragma once

#define SDL_MAIN_HANDLED
#include <SDL.h>

//#include "InputHandler.h"

class Renderer
{
public:
	Renderer(const char* title, const unsigned int w_width, const unsigned int w_height);
	~Renderer();
	Renderer(Renderer&) = delete;
	//bool GetWindowEvent(InputHandler &inputHandler);
	void DrawRect(Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_Rect* rect) const;
	void Clear() const;
	void Present() const;

	//SDL_Window* GetWindow();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};