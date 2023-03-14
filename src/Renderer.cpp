#include "Renderer.h"


Renderer::Renderer(SDL_Window *window)
{
	//Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
	}
}

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