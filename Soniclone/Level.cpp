#include "Level.h"

Level::Level()
	:colliders(std::vector<std::shared_ptr<BaseCollider>>()), spawn(Vec2(0.0f, 0.0f)), checkpoints(std::vector<Vec2>()) {}

Level::~Level() {}