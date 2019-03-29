#include "Renderer.h"

Renderer::Renderer(const char* title, const unsigned int w_width, const unsigned int w_height)
{
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		w_width, w_height, 0);
	if (!window)
	{

	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Renderer::DrawRect(Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_Rect* rect) const
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderDrawRect(renderer, rect);
}

void Renderer::Clear() const
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

void Renderer::Present() const
{
	SDL_RenderPresent(renderer);
}