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

	BaseSprite *get_background();
	BaseSprite *get_foreground();
	bool set_background(std::string path, Vec2 zoom = { 1.0f, 1.0f }, int mode = 0);
	bool set_foreground(std::string path, Vec2 zoom = { 1.0f, 1.0f }, int mode = ALPHA);

private:
	BaseSprite *background;
	BaseSprite *foreground;

};

