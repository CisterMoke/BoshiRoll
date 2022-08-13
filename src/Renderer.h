#pragma once
#include <SDL.h>
#include "Camera.h"
#include "globals.h"
#include "RenderCommands.h"

class Renderer
{

public:
	SDL_Renderer *renderer;

	Renderer();
	Renderer(SDL_Window *window);
	~Renderer();
	void render_all();

};