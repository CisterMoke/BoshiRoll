#include "Player.h"
#include "Player.h"

b2Body *Player::add_to(b2World &world, const Vec2 &pos)
{
	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.position.Set(pos.x, pos.y);

	
	b2CircleShape collider;
	collider.m_radius = sprite->get_width() / 2.0f;

	b2FixtureDef fixdef;
	fixdef.shape = &collider;
	fixdef.density = density;
	fixdef.friction = 1.0f;
	fixdef.restitution = 0.3f;
	fixdef.filter.groupIndex = glob::PLAYER_GROUP;
	
	body = world.CreateBody(&bodydef);
	body->CreateFixture(&fixdef);
	mass = body->GetMass();
	inertia = body->GetInertia();
	return body;
}

Player::Player(std::shared_ptr<BaseSprite> sprite, b2World &world, const Vec2 &pos)
	: Entity(sprite, world, pos)
{
	add_to(world, pos);
	tongue = std::unique_ptr<Tongue>(new Tongue(world, body));
}

Tongue &Player::get_tongue() { return *tongue; }

void Player::do_action(SDL_Event &event)
{
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
	{
		if (tongue->get_state() == TongueState::IDLE)
		{
			Vec2 dir = Vec2(event.button.x - glob::SCREEN_WIDTH / 2, -event.button.y + glob::SCREEN_HEIGHT / 2);
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
			push(Vec2(0.0f, 5.0f) * mass);
			break;

		case SDLK_DOWN:
			body->SetAngularVelocity(0.0f);
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
			spin(0.5f * inertia);
			break;

		case SDLK_RIGHT:
			spin(-0.5f * inertia);
			break;
		}
	}
	Entity::update();
	tongue->update();
}