#pragma once
#include <iomanip>
#include <memory>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <SDL.h>
#include "DebugMode.h"
#include "Camera.h"
#include "Entity.h"
#include "Player.h"
#include "FontSprite.h"
#include "RenderCommands.h"


void push_entity_info_render_command(Entity *e, FontSprite *font);
void print_program_log(GLuint program);
void print_shader_log(GLuint shader);
