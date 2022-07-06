#pragma once
#include <memory>
#include <string>
#include <vector>
#include "BaseSprite.h"
#include "Colliders.h"
#include "globals.h"
#include "Vec2.h"

class Level
{
public:
	std::vector<std::shared_ptr<BaseCollider>> colliders;
	Vec2 spawn = Vec2(0.0f, 0.0f);
	std::vector<Vec2> checkpoints;
	
	Level();
	~Level();

	bool setBackground(std::string path, int mode = 0);
	bool setForeground(std::string path, int mode = ALPHA);
	void renderBackground();
	void renderForeground(Vec2 orig, Vec2 off, Vec2 zoom);

private:
	void renderScenery(BaseSprite *scenery, Vec2 orig, Vec2 off, float depth, Vec2 zoom = Vec2(1.0f, 1.0f));
	BaseSprite *background;
	BaseSprite *foreground;

};

