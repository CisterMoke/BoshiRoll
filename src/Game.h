#pragma once
#include <array>
#include <memory>
#include "globals.h"
#include "Colliders.h"
#include "Player.h"
#include "Level.h"
#include "RenderCommands.h"
#include "Sprites.h"

class Game
{
public:
	std::unique_ptr<Player> player;
	std::unique_ptr<Level> currLevel;
	Camera &camera;

	// Physics
	float g = 12.0f / glob::GAMETICKS;
	float air_fric_t = 0.2f / glob::GAMETICKS;
	float air_fric_r = 0.5f / glob::GAMETICKS;
	float roll_fric = 0.1f / glob::GAMETICKS;

	float bounciness = 0.2f;


	Game(const Player &player, const Level &level, Camera &camera);

	bool is_paused();
	bool is_over();

	void reset();
	void pause();
	void resume();
	void end();

	void tick();
	void push_render_commands();

private:
	bool paused = false;
	bool over = false;

	bool check_collisions();
	void apply_air_friction(Entity &e);
	void apply_rolling_friction(float grip);
	float calculate_grip(float Fn, float sigma = -1.0f);
	float slip_ratio(Vec2 &dir);
	float traction_force(float slip, float grip);
};

