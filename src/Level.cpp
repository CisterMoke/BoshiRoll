#include "Level.h"

Level::Level()
	:colliders(std::vector<std::shared_ptr<BaseCollider>>()), spawn(Vec2(0.0f, 0.0f)), checkpoints(std::vector<Vec2>()),
	background(new BaseSprite()), foreground(new BaseSprite()) {}

Level::~Level() {}

BaseSprite *Level::get_background()
{
	return background;
}

BaseSprite *Level::get_foreground()
{
	return foreground;
}

bool Level::set_background(std::string path, Vec2 zoom, int mode)
{
	bool success = background->load_from_file(path, mode);
	background->zoom(zoom.x, zoom.y);
	return success;
}

bool Level::set_foreground(std::string path, Vec2 zoom, int mode)
{
	bool success = foreground->load_from_file(path, mode);
	foreground->zoom(zoom.x, zoom.y);
	return success;
}