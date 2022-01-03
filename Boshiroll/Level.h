#pragma once
#include <memory>
#include <vector>
#include "Colliders.h"
#include "Vec2.h"

class Level
{
public:
	std::vector<std::shared_ptr<BaseCollider>> colliders;
	Vec2 spawn = Vec2(0.0f, 0.0f);
	std::vector<Vec2> checkpoints;
	
	Level();
	~Level();
};

