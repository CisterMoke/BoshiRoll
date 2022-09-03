#pragma once
#include <iomanip>
#include <memory>
#include <sstream>
#include <vector>
#include <SDL.h>
#include "DebugMode.h"
#include "Camera.h"
#include "Entity.h"
#include "Player.h"
#include "FontSprite.h"
#include "RenderCommands.h"


void push_entity_info_render_command(Entity *e, FontSprite *font);

