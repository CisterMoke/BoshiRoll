#pragma once
#include <array>
#include <math.h>
#include <SDL.h>
#include "globals.h"
#include "Entity.h"
#include "Vec2.h"

enum class TongueState
{
	IDLE,
	SHOT,
	ANCHORED,
	RELEASED
};

class TonguePart : public Entity
{
	friend class Tongue;

public:	
	TonguePart(std::shared_ptr<BaseSprite> sprite, b2World &world, const Vec2 &pos = {});
	~TonguePart() = default;

	virtual b2Body *add_to(b2World &world, const Vec2 &pos);
	float density{ 0.1f };
};

class Tongue
{
private:

	friend class Player;

	static const int num_parts = 5;
	static constexpr float k = 0.005f;
	static constexpr float rest_l = 0.6f;
	static constexpr float d = 0.01f;
	static constexpr float s_mass = 0.1;
	static constexpr float shoot_speed = 20.0f;

	
	const Vec2 *const origin;
	Vec2 prev_origin;
	int reel = 0;

	TongueState state = TongueState::IDLE;
	std::array<b2DistanceJoint *, num_parts-1> tongue_joints{};

	b2World *world;
	b2Body *player_body;
	b2Body *sensor;
	b2DistanceJoint *player_joint = nullptr;

	Tongue(b2World &world, b2Body *player_body);

	b2DistanceJoint *create_joint(b2Body *body_a, b2Body *body_b);
	void init_parts();
	void init_joints();
	void correct_pos(TonguePart &part);
	void correct_angles();
	bool check_contacts();
	void clear_contacts();
	void set_player_joint();
	void clear_player_joint();

	void reel_out();
	void reel_in();

public:
	std::array<std::unique_ptr<TonguePart>, num_parts> parts{};
	std::shared_ptr<BaseSprite> tongue_end;


	TongueState get_state();
	TonguePart &get_tip();
	int get_reel();


	void shoot(Vec2 const &dir);
	void anchor();
	void release();
	void idle();
	void teleport(Vec2 &v);
	void push(Vec2 &f);
	void update();
};

