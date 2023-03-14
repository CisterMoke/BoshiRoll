#pragma once
#include "globals.h"
#include "Camera.h"
#include "RenderCommands.h"
#include <SDL.h>

class Renderer
{

public:
	SDL_Renderer *renderer{};

	Renderer() = default;
	Renderer(SDL_Window *window);
	void render_all();
};