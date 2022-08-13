#include "Game.h"

Game::Game(Player &player, Level &level)
	:player(std::make_shared<Player>(player)), currLevel(std::make_shared<Level>(level)), camera(new Camera()) {}
Game::Game(std::shared_ptr<Player> player, std::shared_ptr<Level> level)
	: player(player), currLevel(level), camera(new Camera()) {}

Game::~Game() {}

void Game::init()
{
	*player->pos = currLevel->spawn;
	camera->set_origin(*player->pos);
	camera->rx = 0.5; camera->ry = 0.5;
}

void Game::tick()
{
	apply_air_friction(*player);
	player->update();

	check_collisions();

	*player->t_force += Vec2(0, g * player->mass);
	player->tongue->apply_gravity(g);
}

void Game::push_render_commands()
{
	RenderParallaxCommand *backgroundMsg = new RenderParallaxCommand(currLevel->get_background(), camera, Vec2(0.0f, 0.0f), 0.0f, true);
	RenderParallaxCommand *foregroundMsg = new RenderParallaxCommand(currLevel->get_foreground(), camera, Vec2(0.0f, 100.0f), 1.0f);
	RenderSpriteCommand<BaseSprite> *playerMsg = new RenderSpriteCommand(player->sprite, camera, *player->pos);


	render_command_pool.push_front(backgroundMsg);
	render_command_pool.push_back(playerMsg);
	
	if (player->tongue->get_state() != TongueState::IDLE)
	{
		for (int i = 0; i < player->tongue->get_reel() + 1; i++)
		{
			Entity *e = player->tongue->parts[i];
			render_command_pool.push_back( new RenderSpriteCommand(e->sprite, camera, *e->pos));
		}
		render_command_pool.push_back(new RenderSpriteCommand<BaseSprite>(player->tongue->tongue_end, camera, *player->pos));
	}
	render_command_pool.push_back(foregroundMsg);


}

bool Game::check_collisions()
{
	Vec2 *cptr = new Vec2(0.0f, 0.0f);
	bool check = false;
	for (auto collider : currLevel->colliders)
	{
		if (collider->check_collision(*player->collider, cptr))
		{
			Vec2 disp = collider->collision_disp(*player->collider, cptr);
			player->teleport(disp);

			Vec2 n_disp = disp.normalize();
			Vec2 bounce = n_disp * player->vel->dot(n_disp) * (1 + bounciness);
			if (bounce.norm2() > 1e-4f)
			{
				*player->t_force -= bounce;
			}

			Vec2 tangent = n_disp.cross(1.0f);
			float grip = calculate_grip(fabsf(player->vel->dot(n_disp)));
			float slip = slip_ratio(*player, tangent);
			float Ft = traction_force(slip, grip);
			*player->t_force += tangent * Ft;
			player->r_force += Ft * 180.0f / player->collider->r / M_PI;
			apply_rolling_friction(*player, grip);
			check = true;
			BREAKPOINT;
		}
		if (player->tongue->get_state() == TongueState::SHOT
			&& collider->check_collision(*player->tongue->get_tip()->collider))
		{
			player->tongue->anchor();
		}
	}
	delete cptr;
	return check;
}

void Game::apply_air_friction(Entity &e)
{
	// Translational friction
	float vel_norm = e.vel->norm();
	if (fabsf(vel_norm) < 0.01f)
	{
		*e.vel = Vec2(0.0f, 0.0f);
		return;
	}
	*e.vel -= *e.vel * air_fric_t/e.mass;

	// Rotational friction
	if (fabsf(e.omega) < 0.01f)
	{
		e.omega = 0.0f;
		return;
	}
	e.omega -= e.omega * air_fric_r/e.mass;
}

void Game::apply_rolling_friction(Entity &e, float grip)
{
	float vel_norm = e.vel->norm();
	if (fabsf(vel_norm) < 0.01f)
	{
		*e.vel = Vec2(0.0f, 0.0f);
		return;
	}
	*e.vel -= *e.vel * roll_fric * grip;

}

float Game::calculate_grip(float Fn, float sigma)
{
	if (sigma < 0) { sigma = 0.05f / (g + 0.0001f); }
	return 0.1f;
	//return fminf(1.0f, Fn * sigma);
}

float Game::slip_ratio(Entity &e, Vec2 &dir)
{
	float vp = dir.dot(*player->vel);
	float omega = e.omega * M_PI / 180; // convert to radians
	float r = e.collider->r;

	return (omega * r + vp);// / (fabsf(vp) + fabsf(omega * r) + 1e-6f);
}

float Game::traction_force(float slip, float grip)
{
	float sign = copysignf(1.0f, -slip);
	return sign * fminf(fabsf(slip * grip), 0.5f);
}
