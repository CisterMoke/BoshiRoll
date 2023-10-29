#include "LevelFactory.h"

Level LevelFactory::init_level(b2World &world)
{
	Level level{};
	std::shared_ptr<BaseSprite> background = std::make_shared<BaseSprite>(glob::BACKGROUND_IMG);
	background->zoom(640.0f / 2880 * 1.5f, 480.0f / 1920 * 1.5f);

	std::shared_ptr<BaseSprite> foreground = std::make_shared<BaseSprite>(glob::FOREGROUND_IMG);
	foreground->zoom(0.5f, 0.5f);

	level.set_background(background).set_foreground(foreground);

	std::shared_ptr<AnimSprite> yoshi_kart = std::make_shared<AnimSprite>(glob::YOSHI_KART, frame_dim_t{ 32, 32 }, ALPHA | COLORKEY);
	yoshi_kart->zoom(3, 3);
	std::shared_ptr<AnimSprite> yoshi_kart2 = std::make_shared<AnimSprite>(*yoshi_kart);
	yoshi_kart2->flip(SDL_FLIP_HORIZONTAL);

	level.add_enemy(Enemy(yoshi_kart, world));
	level.add_enemy(Enemy(yoshi_kart, world,
		Vec2(0.0f, (float)(glob::SCREEN_HEIGHT / glob::PX_PER_M - yoshi_kart->get_height()))
	));
	level.add_enemy(Enemy(yoshi_kart2, world,
		Vec2((float)(glob::SCREEN_WIDTH / glob::PX_PER_M - yoshi_kart2->get_width()), 0.0f)
	));
	level.add_enemy(Enemy(yoshi_kart2, world,
		Vec2((float)(glob::SCREEN_WIDTH / glob::PX_PER_M - yoshi_kart2->get_width()),
			(float)(glob::SCREEN_HEIGHT / glob::PX_PER_M - yoshi_kart2->get_height()))
	));

	std::vector<Vec2> floor{};
	floor.emplace_back(30.0f, -1.5f);
	floor.emplace_back(15.0f, -1.5f);
	floor.emplace_back(10.0f, -2.5f);
	floor.emplace_back(5.0f, -3.0f);
	floor.emplace_back(-20.0f, -3.0f);

	level.add_vgroup(floor, false);
	level.set_spawn(Vec2(
		glob::SCREEN_WIDTH / (2.0f*glob::PX_PER_M),
		glob::SCREEN_HEIGHT / (-2.0f*glob::PX_PER_M))
	);

	level.add_to(world);
	return level;
}