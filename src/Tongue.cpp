#include "Tongue.h"

TonguePart::TonguePart(std::shared_ptr<BaseSprite> sprite, b2World &world, const Vec2 &pos)
	:Entity(sprite, world, pos)
{
	add_to(world, pos);
}

b2Body *TonguePart::add_to(b2World &world, const Vec2 &pos)
{
	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.position.Set(pos.x, pos.y);


	b2CircleShape collider;
	collider.m_radius = sprite->get_width() / 2;

	b2FixtureDef fixdef;
	fixdef.shape = &collider;
	fixdef.density = density;
	fixdef.filter.maskBits = 0x0000;

	body = world.CreateBody(&bodydef);
	body->CreateFixture(&fixdef);
	return body;

}

b2DistanceJoint *Tongue::create_joint(b2Body *body_a, b2Body *body_b)
{
	b2DistanceJointDef def;
	def.bodyA = body_a;
	def.bodyB = body_b;
	def.length = rest_l;
	def.maxLength = 2 * rest_l;
	def.minLength = 0.0f;
	def.stiffness = k;
	def.damping = d;

	return (b2DistanceJoint *) world->CreateJoint(&def);
}

void Tongue::init_parts()
{
	std::shared_ptr<BaseSprite> tip_sprite(new BaseSprite(glob::TONGUE_TIP));
	BaseSprite body_sprite = BaseSprite(glob::TONGUE_BODY);
	tongue_end = std::shared_ptr<BaseSprite>(new BaseSprite(glob::TONGUE_END));
	parts[0] = std::make_unique<TonguePart>(tip_sprite, *world, *origin);
	parts[0]->body->SetType(b2_kinematicBody);
	parts[0]->body->SetEnabled(false);


	for (int i = 1; i < num_parts; i++)
	{
		parts[i] = std::make_unique<TonguePart>(
				std::shared_ptr<BaseSprite>(new BaseSprite(body_sprite)),
				*world, *origin
				);
		parts[i]->body->SetEnabled(false);
	}

	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.position.Set(origin->x, origin->y);
	bodydef.bullet = true;

	b2CircleShape collider;
	collider.m_radius = tip_sprite->get_width() / 2;

	b2FixtureDef fixdef;
	fixdef.shape = &collider;
	fixdef.density = parts[0]->density;
	fixdef.isSensor = true;
	fixdef.filter.groupIndex = glob::PLAYER_GROUP;

	sensor = world->CreateBody(&bodydef);
	sensor->CreateFixture(&fixdef);
	sensor->SetGravityScale(0.0f);
}

void Tongue::init_joints()
{
	for (int i = 0; i < num_parts - 1; i++)
	{
		tongue_joints[i] = create_joint(parts[i]->body, parts[i+1]->body);
	}
}

Tongue::Tongue(b2World &world, b2Body *player_body)
	: world(&world), player_body(player_body), origin((Vec2*) &player_body->GetWorldCenter())
{
	prev_origin = *origin;
	init_parts();
	init_joints();
}

TongueState Tongue::get_state() { return state; }
TonguePart &Tongue::get_tip() { return *parts[0]; }
int Tongue::get_reel() { return reel;  }

void Tongue::shoot(Vec2 const &dir)
{
	if (state != TongueState::IDLE) { return; }

	reel = 0;
	Vec2 shoot_dir = dir.normalize();
	Vec2 vel = shoot_dir * shoot_speed;
	vel += (Vec2 &)player_body->GetLinearVelocity();
	TonguePart &tip = get_tip();
	tip.body->SetEnabled(true);
	tip.body->SetTransform(*origin, 0.0f);
	tip.stop();
	tip.body->SetLinearVelocity(vel);
	clear_contacts();
	state = TongueState::SHOT;
}


void Tongue::anchor()
{
	if (state != TongueState::SHOT) { return; }
	for (int i = 1; i < reel + 1; i++)
	{
		parts[i]->stop();
		parts[i]->body->SetType(b2_dynamicBody);
	}

	get_tip().stop();
	player_joint = create_joint(parts[reel]->body, player_body);
	state = TongueState::ANCHORED;
}

void Tongue::release()
{
	if (state != TongueState::SHOT && state != TongueState::ANCHORED)
	{ 
		return;
	}
	else if (player_joint != nullptr)
	{
		world->DestroyJoint(player_joint);
		player_joint = nullptr;
	}
	int new_reel = reel;
	for (int i = reel; i >= 0; i--)
	{
		parts[i]->body->SetType(b2_dynamicBody);
		float dist = origin->dist(parts[i]->get_pos());

		if (dist < shoot_speed / glob::GAMETICKS)
		{
			parts[i]->stop();
			parts[i]->body->SetEnabled(false);
			new_reel--;
		}
	}

	if (new_reel < 0)
	{
		reel = 0;
		get_tip().body->SetType(b2_kinematicBody);
		idle();
		return;
	}
	reel = new_reel;
	parts[reel]->body->SetType(b2_kinematicBody);
	Vec2 return_dir = *origin - parts[reel]->get_pos();
	return_dir = return_dir.normalize();
	Vec2 vel = return_dir * shoot_speed;
	parts[reel]->body->SetLinearVelocity(vel);
	state = TongueState::RELEASED;
}

void Tongue::idle()
{
	for (int i = 0; i < reel + 1; i++)
	{
		b2Body *part_body = parts[i]->body;
		if (i == 0) { part_body->SetType(b2_kinematicBody); }
		else if (i == reel) { part_body->SetType(b2_dynamicBody); }
		part_body->SetEnabled(false);
		parts[i]->stop();
	}
	state = TongueState::IDLE;
}

void Tongue::reel_out()
{
	if (parts[reel]->get_pos().dist(*origin) > rest_l)
	{
		if (reel < parts.size() - 1)
		{
			b2Vec2 vel = parts[reel++]->body->GetLinearVelocity();
			b2Body *reel_body = parts[reel]->body;
			reel_body->SetType(b2_kinematicBody);
			reel_body->SetTransform(*origin, 0.0f);
			reel_body->SetLinearVelocity(vel);
			reel_body->SetEnabled(true);
		}
		else
		{
			release();
		}
	}
}

void Tongue::reel_in()
{
	const Vec2 &part_vel = (const Vec2&) parts[reel]->body->GetLinearVelocity();
	Vec2 return_dir = *origin - parts[reel]->get_pos();
	if (part_vel.dot(return_dir) < 0)
	{
		if (reel == 0)
		{
			idle();
			return;
		}
		else
		{
			parts[reel]->stop();
			parts[reel]->body->SetType(b2_dynamicBody);
			parts[reel]->body->SetEnabled(false);
			
			--reel;
			Vec2 return_dir = *origin - parts[reel]->get_pos();
			return_dir = return_dir.normalize();
			Vec2 vel = return_dir * shoot_speed;

			parts[reel]->stop();
			parts[reel]->body->SetType(b2_kinematicBody);
			parts[reel]->body->SetLinearVelocity(vel);
		}
	}

}

void Tongue::teleport(Vec2 &v)
{
	for (auto &e : parts)
	{
		e->teleport(v);
	}
}

void Tongue::push(Vec2 &f)
{
	for (auto &e : parts)
	{
		e->push(f);
	}
}

void Tongue::correct_pos(TonguePart &part)
{
	Vec2 off = *origin - prev_origin;
	part.teleport(part.get_pos() + off);
}

void Tongue::correct_angles()
{
	Vec2 dir;
	float angle;
	for (int i = 1; i < reel + 1; i++)
	{
		dir = parts[i - 1]->get_pos() - parts[i]->get_pos();
		angle = atan2f(dir.y, dir.x);
		parts[i]->set_theta(angle);
		if (i == 1) { parts[i - 1]->set_theta(angle); }
	}
	dir = parts[reel]->get_pos() - *origin;
	angle = atan2f(dir.y, dir.x);
	tongue_end->set_theta(angle);
}

bool Tongue::check_contacts()
{
	// contacts are set during last world update.
	bool contact = false;
	for (b2ContactEdge *ce = sensor->GetContactList(); ce; ce = ce->next)
	{
		b2Contact *c = ce->contact;
		if (c->IsEnabled() && c->IsTouching())
		{
			contact = true;
		}
		c->SetEnabled(false);
	}
	return contact;
}

void Tongue::clear_contacts()
{
	for (b2ContactEdge *ce = sensor->GetContactList(); ce; ce = ce->next)
	{
		b2Contact *c = ce->contact;
		c->SetEnabled(false);
	}

}

void set_player_joint()
{

}

void Tongue::update()
{
	sensor->SetTransform(get_tip().get_pos(), 0.0f);
	sensor->SetLinearVelocity(get_tip().body->GetLinearVelocity());
	switch (state)
	{
	case TongueState::IDLE:
		break;
	case TongueState::SHOT:
		if (check_contacts())
		{
			anchor();
		}
		else
		{
			reel_out();
		}
		if (state != TongueState::RELEASED)
		{ 
			break; // Fall through if released in this step.
		}
	case TongueState::ANCHORED:
		break;
	case TongueState::RELEASED:
		correct_pos(*parts[reel]);
		reel_in();
		break;
	}

	prev_origin = *origin;
	if (state != TongueState::IDLE)
	{
		correct_angles();
	}
}
