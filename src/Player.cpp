#include "Player.h"
#include "Player.h"


Player::Player(std::shared_ptr<BaseSprite> sprite, const Vec2 &pos)
	: Entity(sprite, pos), collider(new CircleCollider(std::shared_ptr<Vec2>(&this->pos, [](Vec2*) {}), sprite->get_width() / 2)), tongue(new Tongue(&this->pos)){}

Player::Player(std::shared_ptr<BaseSprite> sprite, float x, float y)
	: Player(sprite, Vec2(0.0f, 0.0f)) {}

Player::Player(const Player &other)
	: Player(other.sprite, other.pos) {}

Tongue &Player::get_tongue() { return *tongue; }

void Player::do_action(SDL_Event &event)
{
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
	{
		if (tongue->get_state() == TongueState::IDLE)
		{
			Vec2 dir = Vec2(event.button.x - glob::SCREEN_WIDTH / 2, event.button.y - glob::SCREEN_HEIGHT / 2);
			if (dir == Vec2(0.0f, 0.0f)) { dir = Vec2(1.0f, 0.0f); }
			tongue->shoot(dir, vel);
		}
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT)
	{
		tongue->idle();
	}
	else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
	{
		if (tongue->get_state() != TongueState::RELEASED && tongue->get_state() != TongueState::IDLE)
		{
			tongue->release();
		}
	}
	else if (event.type == SDL_KEYDOWN)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_SPACE:
			break;

		case SDLK_UP:
			t_force += Vec2(0.0f, -5.0f) * mass;
			break;

		case SDLK_DOWN:
			t_force += Vec2(0.0f, 5.0f) * mass;
			break;

		case SDLK_LEFT:
			held_key = SDLK_LEFT;
			break;

		case SDLK_RIGHT:
			held_key = SDLK_RIGHT;
			break;
		}
	}

	else if (event.type == SDL_KEYUP)
	{
		if (event.key.keysym.sym == held_key)
		{
			held_key = SDLK_UNKNOWN;
		}
	}
}

void Player::update()
{
	if (held_key)
	{
		switch (held_key)
		{
		case SDLK_LEFT:
			r_force -= 0.2f * mass;
			break;

		case SDLK_RIGHT:
			r_force += 0.2f * mass;
			break;
		}
	}
	if (tongue->get_state() == TongueState::ANCHORED)
	{
		Vec2 disp = pos - tongue->parts[tongue->get_reel()]->pos;
		Vec2 F = tongue->spring_force(disp, vel, mass, 0);
		push(F);
	}
	Entity::update();
	tongue->update();
}