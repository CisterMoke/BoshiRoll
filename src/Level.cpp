#include "Level.h"


bool Level::is_init()
{
	return initialized;
}

void Level::init()
{
	std::shared_ptr<BaseSprite> background = std::make_shared<BaseSprite>(glob::BACKGROUND_IMG);
	background->zoom(640.0f / 2880 * 1.5f, 480.0f / 1920 * 1.5f);

	std::shared_ptr<BaseSprite> foreground = std::make_shared<BaseSprite>(glob::FOREGROUND_IMG);
	foreground->zoom(0.5f, 0.5f);

	set_background(background).set_foreground(foreground);

	std::shared_ptr<AnimSprite> yoshi_kart = std::make_shared<AnimSprite>(glob::YOSHI_KART, frame_dim_t{ 32, 32 }, ALPHA | COLORKEY);
	yoshi_kart->zoom(3, 3);
	std::shared_ptr<AnimSprite> yoshi_kart2 = std::make_shared<AnimSprite>(*yoshi_kart);
	yoshi_kart2->flip(SDL_FLIP_HORIZONTAL);

	enemies.emplace_back(yoshi_kart, Vec2(0.0f, 0.0f));
	enemies.emplace_back(yoshi_kart, Vec2(0.0f, glob::SCREEN_HEIGHT - yoshi_kart->get_height()));
	enemies.emplace_back(yoshi_kart2, Vec2(glob::SCREEN_WIDTH - yoshi_kart2->get_width(), 0.0f));
	enemies.emplace_back(yoshi_kart2, Vec2(glob::SCREEN_WIDTH - yoshi_kart2->get_width(), glob::SCREEN_HEIGHT - yoshi_kart2->get_height()));

	colliders.emplace_back(new LineCollider(
		std::shared_ptr<Vec2>(new Vec2(-10.5 * glob::SCREEN_WIDTH, glob::SCREEN_HEIGHT * 0.75f)),
		std::shared_ptr<Vec2>(new Vec2(20.5 * glob::SCREEN_WIDTH, glob::SCREEN_HEIGHT * 0.75f))
	));
	colliders.emplace_back(new RampCollider(
		std::shared_ptr<Vec2>(new Vec2(1600.0f, glob::SCREEN_HEIGHT * 0.75f - 250)), 250, Quadrant::IV
	));

	spawn = Vec2(glob::SCREEN_WIDTH / 2, glob::SCREEN_HEIGHT / 2);
}

const Vec2 Level::get_spawn() const
{
	return spawn;
}

const std::list<Enemy> &Level::get_enemies()
{
	return enemies;
}

const std::vector<BaseCollider*> Level::get_colliders() const
{
	std::vector<BaseCollider*> v{};
	for (auto &collider : colliders)
	{
		v.push_back(collider.get());
	}
	return v;
}

BaseSprite &Level::get_background()
{
	return *background;
}

BaseSprite &Level::get_foreground()
{
	return *foreground;
}

Level &Level::set_background(std::shared_ptr<BaseSprite> background)
{
	this->background = background;
	return *this;
}

Level &Level::set_foreground(std::shared_ptr<BaseSprite> foreground)
{
	this->foreground = foreground;
	return *this;
}