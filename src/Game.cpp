#include "Game.h"

Game::Game(const Player &player, const Level &level, Camera &camera)
	:player(std::make_unique<Player>(player)), currLevel(std::make_unique<Level>(level)), camera(camera)
{
	this->player->pos = currLevel->spawn;
	this->camera.set_origin(this->player->pos);
	this->camera.rx = 0.5; this->camera.ry = 0.5;
}

bool Game::is_paused() { return paused; }

bool Game::is_over() { return over; }

void Game::reset()
{
	this->player->pos = currLevel->spawn;
}

void Game::pause() { paused = true; }

void Game::resume() { paused = false; }

void Game::end() { over = true; }


void Game::tick()
{
	apply_air_friction(*player);
	player->update();

	check_collisions();

	player->t_force += Vec2(0, g * player->mass);
	player->get_tongue().apply_gravity(g);
}

void Game::push_render_commands()
{
	RenderParallaxCommand *background_msg = new RenderParallaxCommand(&currLevel->get_background(), &camera, Vec2(0.0f, 0.0f), 0.0f, true);
	render_command_pool.emplace_front(background_msg);

	for (auto enemy : currLevel->enemies)
	{
		RenderSpriteCommand<AnimSprite> *enemy_msg = new RenderSpriteCommand(enemy->sprite.get(), &camera, enemy->pos);
		render_command_pool.push_back(enemy_msg);
	}

	RenderSpriteCommand<BaseSprite> *player_msg = new RenderSpriteCommand(player->sprite.get(), &camera, player->pos);
	render_command_pool.push_back(player_msg);
	
	if (player->get_tongue().get_state() != TongueState::IDLE)
	{
		for (int i = 0; i < player->get_tongue().get_reel() + 1; i++)
		{
			TonguePart &tp = *player->get_tongue().parts[i];
			render_command_pool.push_back( new RenderSpriteCommand(tp.sprite.get(), &camera, tp.pos));
		}
		render_command_pool.push_back(new RenderSpriteCommand(player->get_tongue().tongue_end.get(), &camera, player->pos));
	}

	RenderParallaxCommand *foreground_msg = new RenderParallaxCommand(&currLevel->get_foreground(), &camera, Vec2(0.0f, 100.0f), 1.0f);
	render_command_pool.push_back(foreground_msg);
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
			Vec2 bounce = n_disp * player->vel.dot(n_disp) * (1 + bounciness);
			if (bounce.norm2() > 1e-4f)
			{
				player->t_force -= bounce;
			}

			Vec2 tangent = n_disp.cross(1.0f);
			float grip = calculate_grip(fabsf(player->vel.dot(n_disp)));
			float slip = slip_ratio(tangent);
			float Ft = traction_force(slip, grip);
			player->t_force += tangent * Ft;
			player->r_force += Ft * 180.0f / player->collider->r / M_PI;
			apply_rolling_friction(grip);
			check = true;
		}
		if (player->get_tongue().get_state() == TongueState::SHOT
			&& collider->check_collision(*player->get_tongue().get_tip().collider))
		{
			player->get_tongue().anchor();
		}
	}
	delete cptr;
	return check;
}

void Game::apply_air_friction(Entity &e)
{
	// Translational friction
	float vel_norm = e.vel.norm();
	if (fabsf(vel_norm) < 0.01f)
	{
		e.vel = Vec2(0.0f, 0.0f);
		return;
	}
	e.vel -= e.vel * air_fric_t/e.mass;

	// Rotational friction
	if (fabsf(e.omega) < 0.01f)
	{
		e.omega = 0.0f;
		return;
	}
	e.omega -= e.omega * air_fric_r/e.mass;
}

void Game::apply_rolling_friction(float grip)
{
	float vel_norm = player->vel.norm();
	if (fabsf(vel_norm) < 0.01f)
	{
		player->vel = Vec2(0.0f, 0.0f);
		return;
	}
	player->vel -= player->vel * roll_fric * grip;

}

float Game::calculate_grip(float Fn, float sigma)
{
	if (sigma < 0) { sigma = 0.05f / (g + 0.0001f); }
	return 0.1f;
	//return fminf(1.0f, Fn * sigma);
}

float Game::slip_ratio(Vec2 &dir)
{
	float vp = dir.dot(player->vel);
	float omega = player->omega * M_PI / 180; // convert to radians
	float r = player->collider->r;

	return (omega * r + vp);// / (fabsf(vp) + fabsf(omega * r) + 1e-6f);
}

float Game::traction_force(float slip, float grip)
{
	float sign = copysignf(1.0f, -slip);
	return sign * fminf(fabsf(slip * grip), 0.5f);
}
