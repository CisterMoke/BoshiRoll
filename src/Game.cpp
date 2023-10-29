#include "Game.h"

b2World Game::create_world()
{
	b2Vec2 gravity = { 0.0f, -10.0f };
	return b2World(gravity);
}

Game::Game(Camera &cam)
	: player(std::make_shared<BaseSprite>(glob::BOSHI_IMG_BMP), world),
	curr_level(LevelFactory::init_level(world)), camera(cam), world(Game::create_world())
{
	player.grow(0.5f);
	player.teleport(curr_level.get_spawn());
	camera.set_origin(player.get_pos());
	camera.rx = 0.5; camera.ry = 0.5;
}

bool Game::is_paused() { return paused; }

bool Game::is_over() { return over; }

void Game::reset()
{
	player.stop();
	player.teleport(curr_level.get_spawn());
}

void Game::pause() { paused = true; }

void Game::resume() { paused = false; }

void Game::end() { over = true; }


void Game::tick()
{
	player.update();
	world.Step(1.0f / glob::GAMETICKS, velocity_iterations, position_iterations);
}

void Game::push_render_commands()
{
	RenderParallaxCommand *background_msg = new RenderParallaxCommand(&curr_level.get_background(), &camera, Vec2(0.0f, 0.0f), 0.0f, true);
	render_command_pool.emplace_front(background_msg);

	for (auto &enemy : curr_level.get_enemies())
	{
		RenderSpriteCommand<AnimSprite> *enemy_msg = new RenderSpriteCommand(enemy.sprite.get(), &camera, enemy.get_pos().to_pixels());
		render_command_pool.push_back(enemy_msg);
	}

	RenderSpriteCommand<BaseSprite> *player_msg = new RenderSpriteCommand(player.sprite.get(), &camera, player.get_pos().to_pixels());
	render_command_pool.push_back(player_msg);
	
	if (player.get_tongue().get_state() != TongueState::IDLE)
	{
		for (int i = 0; i < player.get_tongue().get_reel() + 1; i++)
		{
			TonguePart &tp = *player.get_tongue().parts[i];
			render_command_pool.push_back( new RenderSpriteCommand(tp.sprite.get(), &camera, tp.get_pos().to_pixels()));
		}
		render_command_pool.push_back(new RenderSpriteCommand(player.get_tongue().tongue_end.get(), &camera, player.get_pos().to_pixels()));
	}

	RenderParallaxCommand *foreground_msg = new RenderParallaxCommand(&curr_level.get_foreground(), &camera, Vec2(0.0f, 0.0f).to_pixels(), 1.0f);
	render_command_pool.push_back(foreground_msg);
}

bool Game::check_collisions()
{
	Vec2 *cptr = new Vec2(0.0f, 0.0f);
	bool check = false;
	delete cptr;
	return check;
}

float Game::traction_force(float slip, float grip)
{
	float sign = copysignf(1.0f, -slip);
	return sign * fminf(fabsf(slip * grip), 0.5f);
}
