#pragma once
#include <iomanip>
#include <memory>
#include <sstream>
#include <vector>
#include <SDL.h>
#include "Camera.h"
#include "Entity.h"
#include "FontSprite.h"

enum DebugMode
{
	DEBUG_OFF = 0x0,
	DEBUG_INFO = 0x1,
	DEBUG_DRAW = 0x2,
	DEBUG_ALL = 0x3,
};
void drawCollider(SDL_Renderer *renderer, BaseCollider *collider, Camera *cam, const Vec2 &offset, const SDL_Color &color = SDL_Color(180, 0, 180));
void renderText(Entity *e, FontSprite *font);

