#pragma once
#include <memory>
#include <box2d.h>

#include "globals.h"
#include "AnimSprite.h"
#include "BaseSprite.h"
#include "Enemy.h"
#include "Level.h"
#include "Vec2.h"

//#include "Level.h"

class LevelFactory
{
public:
	static Level init_level(b2World &world);
};
