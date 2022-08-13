#include "Renderer.h"


Renderer::Renderer() : renderer(nullptr) {} // SHOULD BE REMOVED LATER ON
Renderer::Renderer(SDL_Window *window)
{
	//Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
	}
}


Renderer::~Renderer() {} // Renderer gets deleted by sdl stuff.

void Renderer::render_all()
{
	while (!render_command_pool.empty())
	{
		BaseRenderCommand *front = render_command_pool.front();
		front->execute(renderer);
		render_command_pool.pop_front();
		delete front;
	}
}