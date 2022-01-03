#include "debug.h"

void drawCollider(SDL_Renderer *renderer, BaseCollider *collider, Camera *cam, const Vec2 &offset, const SDL_Color &color)
{
	collider->draw(renderer, color, cam->getOrigin(), offset, cam->getTransform());
}

void renderText(Entity *e, FontSprite *font)
{
	int x = 5;
	int y = 5;
	std::stringstream s;

	s.str("");
	s << std::setprecision(4) << "Pos: " << *e->pos << "\t\t\t\t\t" << "Theta: " << *e->theta << "\n";
	s << std::setprecision(4) << "Vel: " << *e->vel << "\t\t\t\t\t" << "Omega: " << e->omega << "\n";
	s << std::setprecision(4) << "Tforce: " << *e->t_force << "\t\t\t\t\t" << "Rforce: " << e->r_force;
	auto pairs = font->parseText(s.str());
	for (auto pair : pairs)
	{
		Vec2 off = pair.first;
		std::string text = pair.second;
		font->setText(text);
		font->renderAtLU(x, y, off);
	}

}