#include <iostream>
#include <box2d.h>
#include <array>
#include <memory>

int main(int argc, char *argv[])
{
	std::unique_ptr<int> p1(new int(100));
	int *raw = p1.get();
	std::unique_ptr<int> p2 = std::make_unique<int>(*p1);
	p1.reset();

	std::cout << *p2 << std::endl;
	p2.reset();
	std::cout << *raw << std::endl;

	b2Vec2 gravity(0.0f, -10.0f);
	
	b2World world(gravity);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);
	b2Body *groundBody = world.CreateBody(&groundBodyDef);
	
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);
	
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	bodyDef.angle = 0.4;
	b2Body *body = world.CreateBody(&bodyDef);
	
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	body->CreateFixture(&fixtureDef);

	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	for (int32 i = 0; i < 120; ++i)
	{
		world.Step(timeStep, velocityIterations, positionIterations);
		b2Vec2 position = body->GetPosition();
		float angle = body->GetAngle();
		printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
	}
}