#include "Game.h"

Game::Game(Entity &player, Level &level)
	:player(std::make_shared<Entity>(player)), currLevel(std::make_shared<Level>(level)) {}
Game::Game(std::shared_ptr<Entity> player, std::shared_ptr<Level> level)
	: player(player), currLevel(level) {}

Game::~Game() {}

void Game::tick()
{
	//applyAirFriction();
	player->update();

	checkCollisions();

	*player->t_force += Vec2(0, g);
}

void Game::checkCollisions()
{
	Vec2 *cptr = new Vec2(0.0f, 0.0f);
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

			std::array<float, 2> deltas = calculateGripForce(grip, tangent);
			*player->t_force += tangent * deltas[0];//copysign(1.0f, deltas[0]) * sqrtf(fabsf(deltas[0]));
			player->r_force += deltas[1];
			BREAKPOINT;
		}
	}
	delete cptr;
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

float Game::calculateGrip(float Fn, float sigma)
{
	if (sigma < 0) { sigma = 0.05f / (g + 0.0001f); }
	return 0.5f;
	//return fminf(1.0f, Fn * sigma);
}

std::array<float, 2> Game::calculateGripForce(float grip, Vec2 &dir)
{
	/*
	Calculate delta vp and delta omega based on grip (gamma) with 0 <= gamma <= 1
	We define grip such that upon impact we have that
	delta_v_rel_new = -gamma x v_rel_old with v_rel the relative velocity at the contact point.
	From the conservation of energy we then obtain a quadratic equation for delta vp (omega)
	which is solved analytically.
	*/
	std::array<float, 2> result;
    float vp = dir.dot(*player->vel);
	float omega = player->omega * M_PI / 180; // convert to radians
	float r = player->collider->r;
	float c = -grip * (vp + omega * r);

	//std::cout << "OMEGA: " << omega << std::endl;

	if (fabsf(c) < 1e-5)
	{
		result = { 0.0f, 0.0f };
		return result;
	}
	//std::cout << "c: " << c << std::endl;
	float I_hat = player->inertia;
	std::cout << "BEFORE: " << powf(vp, 2.0f) + I_hat * powf(omega * r, 2.0f) << ", ";

	// Skipped some steps to find simplified versions of A, B, C and D
	float A = 1 + I_hat;
	float B = - vp + I_hat * omega * r - c; // actually B/2
	float C = c * (c + 2 * vp);
	float D = powf(B, 2.0f) - A * C; // factor 4 omitted
	float sD = sqrtf(D);

	// Alright I've wasted more than enough time on this jeez.
	int sign = vp*omega <= 0 ? copysignf(1.0f, omega) : copysignf(1.0f, omega * r - vp);

	float delta_omega = (-B + sign * sD) / A / r;
	float delta_vp = (c + (B - sign * sD) / A);
	float omega1 = omega + delta_omega;
	float omega2 = omega + delta_omega - (2 * sign * sD / A / r);
	float vp1 = vp + delta_vp;
	float vp2 = vp + delta_vp + 2 * sign * sD / A;
	std::cout << "AFTER: " << powf(vp + delta_vp, 2.0f) + I_hat * powf((omega + delta_omega) * r, 2.0f) << std::endl;
	std::cout << "c: " << c << std::endl;
	std::cout << "SIGN: " << sign << std::endl;
	std::cout << "OMEGA " << omega << ", VP: " << vp << std::endl;
	std::cout << "omega1: " << omega1 << ",  vp1: " << vp1 << std::endl;
	std::cout << "omega2: " << omega2 << ",  vp2: " << vp2 << std::endl;
	std::cout << "--------------------" << std::endl;

	delta_omega *= 180 / M_PI; // convert to degrees
	result = { delta_vp, delta_omega };
	return result;
}
