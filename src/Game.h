#pragma once
#include <array>
#include <memory>
#include <box2d.h>
#include "globals.h"
#include "Colliders.h"
#include "Player.h"
#include "Level.h"
#include "LevelFactory.h"
#include "RenderCommands.h"
#include "Sprites.h"

class Game
{
private:
	static b2World create_world();
	static const int velocity_iterations{ 6 };
	static const int position_iterations{ 2 };

	b2World world; // Must be declared before player and level.

	bool paused = false;
	bool over = false;

	bool check_collisions();
	float traction_force(float slip, float grip);

public:
	Player player;
	Level curr_level;
	Camera &camera;

	// Physics
	float air_fric_t = 0.2f / glob::GAMETICKS;
	float air_fric_r = 0.5f / glob::GAMETICKS;
	float roll_fric = 0.1f / glob::GAMETICKS;

	float bounciness = 0.2f;


	Game(Camera &cam);
	~Game() = default;

	bool is_paused();
	bool is_over();

	void reset();
	void pause();
	void resume();
	void end();

	void tick();
	void push_render_commands();
};

