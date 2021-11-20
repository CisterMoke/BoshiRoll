#include "Game.h"

Game::Game(Entity &player, Level &level)
	:player(std::make_shared<Entity>(player)), currLevel(std::make_shared<Level>(level)) {}
Game::Game(std::shared_ptr<Entity> player, std::shared_ptr<Level> level)
	: player(player), currLevel(level) {}

Game::~Game() {}

void Game::checkCollisions()
{
	Vec2 *cptr = new Vec2(0.0f, 0.0f);
	for (auto collider : currLevel->colliders)
	{
		if (collider->checkCollision(*player->collider, cptr))
		{
			Vec2 disp = collider->collisionDisp(*player->collider, cptr);
			player->push(disp);
		}
	}
	delete cptr;
}

void Game::tick()
{
	checkCollisions();
	player->update();
}

