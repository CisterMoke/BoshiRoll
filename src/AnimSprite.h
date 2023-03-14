#pragma once
#include <string>
#include <array>
#include <vector>
#include "BaseSprite.h"
#include <SDL.h>
#include <SDL_image.h>

using frame_dim_t = std::array<int, 2>;

class AnimSprite: public BaseSprite
{
public:
	AnimSprite(std::string path, frame_dim_t frame_dim, int mode = ALPHA | COLORKEY);

	bool load_from_file(std::string path, frame_dim_t frame_dim, int mode = ALPHA | COLORKEY);

	SDL_Rect get_frame_clip();

	void reset();

	void set_frame(int frame_num);
	void set_delay(int delay);
	void advance(int frames = 1);
	void rewind(int frames = 1);
	void sync(bool reverse = false);

private:
	int num_frames{ 0 };
	int curr_frame{ 0 };
	int anim_delay{ 1000 / glob::FPS };
	frame_dim_t frame_dim = { 0, 0 };
	std::vector<frame_dim_t> frame_rects{};
	bool load_from_file(std::string path, int mode);
};

