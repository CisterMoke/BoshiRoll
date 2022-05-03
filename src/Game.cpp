#include "Game.h"

Game::Game(Player &player, Level &level)
	:player(std::make_shared<Player>(player)), currLevel(std::make_shared<Level>(level)) {}
Game::Game(std::shared_ptr<Player> player, std::shared_ptr<Level> level)
	: player(player), currLevel(level) {}

Game::~Game() {}

void Game::tick()
{
	applyAirFriction();
	player->update();

	checkCollisions();

	*player->t_force += Vec2(0, g);
}

bool Game::checkCollisions()
{
	Vec2 *cptr = new Vec2(0.0f, 0.0f);
	bool check = false;
	for (auto collider : currLevel->colliders)
	{
		if (collider->checkCollision(*player->collider, cptr))
		{
			Vec2 disp = collider->collisionDisp(*player->collider, cptr);
			player->teleport(disp);

			Vec2 n_disp = disp.normalize();
			Vec2 bounce = n_disp * player->vel->dot(n_disp) * (1 + bounciness);
			if (bounce.norm2() > 1e-4f)
			{
				*player->t_force -= bounce;
			}

			Vec2 tangent = n_disp.cross(1.0f);
			float grip = calculateGrip(fabsf(player->vel->dot(n_disp)));
			float slip = slipRatio(tangent);
			float Ft = tractionForce(slip, grip);
			*player->t_force += tangent * Ft;
			player->r_force += Ft * 180.0f / player->collider->r / M_PI;
			applyRollingFriction(grip);
			check = true;
			BREAKPOINT;
		}
		if (player->tongue->getState() == TongueState::SHOT
			&& collider->checkCollision(*player->tongue->getTip()->collider))
		{
			player->tongue->anchor();
		}
	}
	delete cptr;
	return check;
}

void Game::applyAirFriction()
{
	// Translational friction
	float vel_norm = player->vel->norm();
	if (fabsf(vel_norm) < 0.01f)
	{
		*player->vel = Vec2(0.0f, 0.0f);
		return;
	}
	*player->vel -= *player->vel * air_fric_t;

	// Rotational friction
	if (fabsf(player->omega) < 0.01f)
	{
		player->omega = 0.0f;
		return;
	}
	player->omega -= player->omega * air_fric_r;
}

void Game::applyRollingFriction(float grip)
{
	float vel_norm = player->vel->norm();
	if (fabsf(vel_norm) < 0.01f)
	{
		*player->vel = Vec2(0.0f, 0.0f);
		return;
	}
	*player->vel -= *player->vel * roll_fric * grip;

}

float Game::calculateGrip(float Fn, float sigma)
{
	if (sigma < 0) { sigma = 0.05f / (g + 0.0001f); }
	return 0.1f;
	//return fminf(1.0f, Fn * sigma);
}

float Game::slipRatio(Vec2 &dir)
{
	float vp = dir.dot(*player->vel);
	float omega = player->omega * M_PI / 180; // convert to radians
	float r = player->collider->r;

	return (omega * r + vp);// / (fabsf(vp) + fabsf(omega * r) + 1e-6f);
}

float Game::tractionForce(float slip, float grip)
{
	float sign = copysignf(1.0f, -slip);
	return sign * fminf(fabsf(slip * grip), 0.5f);
}
