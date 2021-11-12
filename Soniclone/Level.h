#pragma once
#include <vector>
#include "Colliders.h"
#include "Vec2.h"

class Level
{
public:
	CollVec colliders;
	Vec2 spawn = Vec2(0.0f, 0.0f);
	std::vector<Vec2> checkpoints;
	
	Level();
	~Level();
};

