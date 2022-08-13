#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <array>
#include <vector>
#include "BaseSprite.h"
using std::array;
using std::vector;

class AnimSprite: public BaseSprite
{
public:
	AnimSprite();
	AnimSprite(const AnimSprite &other);

	bool load_from_file(std::string path, std::array<int, 2> _frameSize, int mode = ALPHA | COLORKEY);

	SDL_Rect get_frame_clip();

	void reset();

	void set_frame(int frame_num);
	void set_delay(int delay);
	void advance(int frames = 1);
	void rewind(int frames = 1);
	void sync(bool reverse = false);

private:
	int num_frames = 0;
	int curr_frame = 0;
	int anim_delay = 1000 / glob::FPS;
	array<int, 2> frame_size = { 0, 0 };
	vector<array<int,2>> frame_rects;
	bool load_from_file(std::string path, int mode);
};

