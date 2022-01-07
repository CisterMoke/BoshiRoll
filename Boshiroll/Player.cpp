#include "Player.h"


Player::Player(std::string spritePath, float zoom, int mode)
	: Entity(spritePath, zoom, mode), tongue(new Tongue(pos))
{}

Player::Player(std::string spritePath, Vec2 &pos, float zoom, int mode)
	: Entity(spritePath, pos, zoom, mode), tongue(new Tongue(&pos))
{}

Player::Player(std::string spritePath, float x, float y, float zoom, int mode)
	: Entity(spritePath, x, y, zoom, mode), tongue(new Tongue(pos))
{}

Player:: ~Player() { delete tongue; }

void Player::doAction(SDL_Event &event)
{
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
	{
		if (tongue->getState() == IDLE)
		{
			Vec2 dir = Vec2(event.button.x - glob::SCREEN_WIDTH / 2, event.button.y - glob::SCREEN_HEIGHT / 2);
			if (dir == Vec2(0.0f, 0.0f)) { dir = Vec2(1.0f, 0.0f); }
			tongue->shoot(dir);
		}
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT)
	{
		tongue->idle();
	}
	else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
	{
		tongue->release();
	}
	else
	{
		Entity::doAction(event);
	}
}

void Player::update()
{
	Entity::update();
	tongue->update();
}

void Player::render(SDL_Renderer *renderer, Vec2 const &orig, Vec2 const &offset, float phi, float zx, float zy)
{
	Entity::render(renderer, orig, offset, phi, zx, zy);
	if (tongue->getState() != IDLE)
	{
		for (Entity *e : tongue->parts) { e->render(renderer, orig, offset, phi, zx, zy); }
	}
}
