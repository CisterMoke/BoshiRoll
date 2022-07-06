#include "Level.h"

Level::Level()
	:colliders(std::vector<std::shared_ptr<BaseCollider>>()), spawn(Vec2(0.0f, 0.0f)), checkpoints(std::vector<Vec2>()),
	background(new BaseSprite()), foreground(new BaseSprite()) {}

Level::~Level() {}

bool Level::setBackground(std::string path, int mode)
{
	return background->loadFromFile(path, mode);
}

bool Level::setForeground(std::string path, int mode)
{
	return foreground->loadFromFile(path, mode);
}

void Level::renderBackground()
{
	renderScenery(background, Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f), 0.0f);
}

void Level::renderForeground(Vec2 orig, Vec2 off, Vec2 zoom)
{
	renderScenery(foreground, orig, off, 1.0f, zoom);
}

void Level::renderScenery(BaseSprite *scenery, Vec2 orig, Vec2 off, float depth, Vec2 zoom)
{
	float rx = glob::SCREEN_WIDTH / (float) scenery->getWidth();
	float ry = glob::SCREEN_HEIGHT / (float) scenery->getHeight();
	float h = glob::SCREEN_WIDTH * zoom.y;
	float top = -orig.y * zoom.y * depth;
	if (top > glob::SCREEN_HEIGHT - off.y || top + h < -off.y) { return; }
	float w = glob::SCREEN_WIDTH * zoom.x;
	float left = -orig.x * zoom.x * depth;
	float right = left + w;
	int start = -ceilf((off.x + left) / (w));
	int stop = fmaxf(ceilf((glob::SCREEN_WIDTH - off.x - right) / (w)), start+1);
	std::cout << "START: " << start << "\t|\tSTOP: " << stop << std::endl;
	for (int i = start; i<=stop; i++)
	{
		scenery->renderAtLU((Vec2(i * glob::SCREEN_WIDTH, 0.0f) - orig) / Vec2(rx, ry) * depth, off, 0.0f, rx * zoom.x, ry * zoom.y);
	}
}