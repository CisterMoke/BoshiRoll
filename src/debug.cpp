#include "debug.h"

void push_entity_info_render_command(Entity *e, FontSprite *font)
{
	int x = 5;
	int y = 5;
	std::stringstream s;
	EntityData e_data = e->get_data();
	s.str("");
	s << std::setprecision(4) << "Pos: " << e_data.pos << "\t\t\t\t\t" << "Theta: " << e_data.theta << "\n";
	s << std::setprecision(4) << "Vel: " << e_data.vel << "\t\t\t\t\t" << "Omega: " << e_data.omega << "\n";
	//s << std::setprecision(4) << "Tforce: " << e->t_force << "\t\t\t\t\t" << "Rforce: " << e->r_force << "\n";
	if (Player* p = static_cast<Player*>(e); p != nullptr)
	{
		Tongue &t = p->get_tongue();
		s << "Reel: " << t.get_reel() << "\t\t\t\t\t" << "State: " << (int) t.get_state() << "\n";
	}
	
	font->set_text(s.str());
	render_command_pool.push_back(new RenderSpriteCommand<FontSprite>(font, nullptr, Vec2(x, y), false));
}

void print_program_log(GLuint program)
{
    //Make sure name is shader
    if (glIsProgram(program))
    {
        //Program log length
        int info_log_length = 0;
        int max_length = info_log_length;

        //Get info string length
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

        //Allocate string
        char *info_log = new char[max_length];

        //Get info log
        glGetProgramInfoLog(program, max_length, &info_log_length, info_log);
        if (info_log_length > 0)
        {
            //Print Log
            printf("%s\n", info_log);
        }

        //Deallocate string
        delete[] info_log;
    }
    else
    {
        printf("Name %d is not a program\n", program);
    }
}

void print_shader_log(GLuint shader)
{
    //Make sure name is shader
    if (glIsShader(shader))
    {
        //Shader log length
        int info_log_length = 0;
        int max_length = info_log_length;

        //Get info string length
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

        //Allocate string
        char *info_log = new char[max_length];

        //Get info log
        glGetShaderInfoLog(shader, max_length, &info_log_length, info_log);
        if (info_log_length > 0)
        {
            //Print Log
            printf("%s\n", info_log);
        }

        //Deallocate string
        delete[] info_log;
    }
    else
    {
        printf("Name %d is not a shader\n", shader);
    }
}