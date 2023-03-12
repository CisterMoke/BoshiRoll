#include "Level.h"

Level::Level() {}

Level::Level(const Level &other)
	: background(other.background), foreground(other.foreground), spawn(other.spawn), enemies(other.enemies),
	colliders(other.colliders), checkpoints(other.checkpoints) {}

Level Level::init_level()
{
	Level lvl = Level();

	std::shared_ptr<BaseSprite> background(new BaseSprite(glob::BACKGROUND_IMG));
	background->zoom(640.0f / 2880 * 1.5f, 480.0f / 1920 * 1.5f);

	std::shared_ptr<BaseSprite> foreground(new BaseSprite(glob::FOREGROUND_IMG));
	foreground->zoom(0.5f, 0.5f);

	lvl.set_background(background)->set_foreground(foreground);

	std::shared_ptr<AnimSprite> yoshi_kart(new AnimSprite(glob::YOSHI_KART, { 32, 32 }, ALPHA | COLORKEY));
	yoshi_kart->zoom(3, 3);
	std::shared_ptr<AnimSprite> yoshi_kart2(new AnimSprite(*yoshi_kart));
	yoshi_kart2->flip(SDL_FLIP_HORIZONTAL);

	lvl.enemies.emplace_back(new Enemy(yoshi_kart, Vec2(0.0f, 0.0f)));
	lvl.enemies.emplace_back(new Enemy(yoshi_kart, Vec2(0.0f, glob::SCREEN_HEIGHT - yoshi_kart->get_height())));
	lvl.enemies.emplace_back(new Enemy(yoshi_kart2, Vec2(glob::SCREEN_WIDTH - yoshi_kart2->get_width(), 0.0f)));
	lvl.enemies.emplace_back(new Enemy(yoshi_kart2, Vec2(glob::SCREEN_WIDTH - yoshi_kart2->get_width(), glob::SCREEN_HEIGHT - yoshi_kart2->get_height())));

	lvl.colliders.emplace_back(new LineCollider(
		std::shared_ptr<Vec2>(new Vec2(-10.5 * glob::SCREEN_WIDTH, glob::SCREEN_HEIGHT * 0.75f)),
		std::shared_ptr<Vec2>(new Vec2(20.5 * glob::SCREEN_WIDTH, glob::SCREEN_HEIGHT * 0.75f))
	));
	lvl.colliders.emplace_back(new RampCollider(
		std::shared_ptr<Vec2>(new Vec2(1600.0f, glob::SCREEN_HEIGHT * 0.75f - 250)), 250, Quadrant::IV
	));

	lvl.spawn = Vec2(glob::SCREEN_WIDTH / 2, glob::SCREEN_HEIGHT / 2);

	return lvl;
}

BaseSprite &Level::get_background()
{
	return *background;
}

BaseSprite &Level::get_foreground()
{
	return *foreground;
}

Level *Level::set_background(std::shared_ptr<BaseSprite> background)
{
	this->background = background;
	return this;
}

Level *Level::set_foreground(std::shared_ptr<BaseSprite> foreground)
{
	this->foreground = foreground;
	return this;
}