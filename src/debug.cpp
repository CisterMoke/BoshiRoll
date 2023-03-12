#include "debug.h"

void push_entity_info_render_command(Entity *e, FontSprite *font)
{
	int x = 5;
	int y = 5;
	std::stringstream s;

	s.str("");
	s << std::setprecision(4) << "Pos: " << e->pos << "\t\t\t\t\t" << "Theta: " << e->theta << "\n";
	s << std::setprecision(4) << "Vel: " << e->vel << "\t\t\t\t\t" << "Omega: " << e->omega << "\n";
	s << std::setprecision(4) << "Tforce: " << e->t_force << "\t\t\t\t\t" << "Rforce: " << e->r_force << "\n";
	if (Player* p = static_cast<Player*>(e); p != nullptr)
	{
		s << "Reel: " << p->get_tongue().get_reel() << "\n";
	}
	
	font->set_text(s.str());
	render_command_pool.push_back(new RenderSpriteCommand<FontSprite>(font, nullptr, Vec2(x, y), false));
	//font->renderAtLU(x, y);

}