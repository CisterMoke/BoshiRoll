#pragma once
#include <iomanip>
#include <memory>
#include <sstream>
#include <vector>
#include <SDL.h>
#include "Camera.h"
#include "Entity.h"
#include "Player.h"
#include "FontSprite.h"
#include "RenderCommands.h"

enum DebugMode
{
	DEBUG_OFF = 0x0,
	DEBUG_INFO = 0x1,
	DEBUG_DRAW = 0x2,
	DEBUG_ALL = 0x3,
};
void push_entity_info_render_command(Entity *e, FontSprite *font);

