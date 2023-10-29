#include "Level.h"


b2ChainShape Level::get_chain(const std::vector<Vec2> &vertices, bool loop)
{
	b2ChainShape chain;
	int n = vertices.size();
	if (loop)
	{
		chain.CreateLoop((b2Vec2 *)vertices.data(), n);
	}
	else
	{
		b2Vec2 prev = vertices[0] * 2 - vertices[1];
		b2Vec2 next = vertices[n - 1] * 2 - vertices[n - 2];
		chain.CreateChain((b2Vec2 *)vertices.data(), n, prev, next);
	}
	return chain;
}

b2Body *Level::add_to(b2World &world)
{
	// All these shenanigans should be moved to the LevelFactory
	//***********************************************************************
	//***********************************************************************

	b2BodyDef level_geometry_def;
	b2Body *level_geometry = world.CreateBody(&level_geometry_def);

	for (int i = 0; i < vertex_groups.size(); i++)
	{
		b2ChainShape chain = Level::get_chain(vertex_groups[i], loop_flags[i]);
		b2FixtureDef fixdef;
		fixdef.shape = &chain;
		fixdef.density = 0.0f;
		fixdef.friction = 1.0f;
		level_geometry->CreateFixture(&fixdef);
	}
	return level_geometry;
}

const Vec2 Level::get_spawn() const
{
	return spawn;
}

void Level::set_spawn(Vec2 spawn)
{
	this->spawn = spawn;
}

const std::vector<Vec2> Level::get_checkpoints() const
{
	return checkpoints;
}

void Level::add_checkpoint(Vec2 checkpoint)
{
	checkpoints.push_back(checkpoint);
}

const std::list<Enemy> &Level::get_enemies()
{
	return enemies;
}

void Level::add_enemy(Enemy enemy)
{
	enemies.push_back(enemy);
}

void Level::add_vgroup(std::vector<Vec2> vgroup, bool loop)
{
	vertex_groups.push_back(vgroup);
	loop_flags.push_back(loop);
}

BaseSprite &Level::get_background()
{
	return *background;
}

BaseSprite &Level::get_foreground()
{
	return *foreground;
}

Level &Level::set_background(std::shared_ptr<BaseSprite> background)
{
	this->background = background;
	return *this;
}

Level &Level::set_foreground(std::shared_ptr<BaseSprite> foreground)
{
	this->foreground = foreground;
	return *this;
}