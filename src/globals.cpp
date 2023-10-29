#include "globals.h"

namespace glob
{
	extern SDL_Window *g_window{};
	extern SDL_Renderer *g_renderer{};
	extern SDL_Surface *g_screen{};
	extern SDL_GLContext g_context{};

	extern GLuint g_program_id = 0;
	extern GLint g_vertex_pos_2d_location = -1;
	extern GLuint g_vbo = 0;
	extern GLuint g_ibo = 0;


	extern DebugMode DEBUG_MODE{ DEBUG_OFF };
}