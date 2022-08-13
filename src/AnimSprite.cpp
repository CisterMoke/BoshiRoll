#include "AnimSprite.h"
using glob::g_renderer;


AnimSprite::AnimSprite() = default;

AnimSprite::AnimSprite(const AnimSprite &other): BaseSprite(other)
{
	num_frames = other.num_frames;
	curr_frame = other.curr_frame;
	anim_delay = other.anim_delay;
	frame_size = other.frame_size;
	frame_rects = other.frame_rects;
}


bool AnimSprite::load_from_file(std::string path, int mode)
{
	return BaseSprite::load_from_file(path, mode);
}

bool AnimSprite::load_from_file(std::string path, array<int, 2> _frameSize, int mode)
{
	if (!load_from_file(path, mode)) { return false; }
	frame_size = _frameSize;
	w = frame_size[0];
	h = frame_size[1];

	int horizontal = base_surf->w / w;
	int vertical = base_surf->h / h;
	num_frames = horizontal * vertical;
	frame_rects.reserve(num_frames);

	for (int i = 0; i < vertical; i++)
	{
		for (int j = 0; j < horizontal; j++)
		{
			int fx = j * w;
			int fy = i * h;
			int fn = i * horizontal + j;
			frame_rects.push_back({ fx, fy });
		}
	}
	return true;
}


SDL_Rect AnimSprite::get_frame_clip()
{
	return SDL_Rect(frame_rects[curr_frame][0], frame_rects[curr_frame][1], frame_size[0], frame_size[1]);
}

void AnimSprite::reset()
{
	BaseSprite::reset();
	frame_rects.erase(frame_rects.begin(), frame_rects.end());
	num_frames = 0;
	curr_frame = 0;
	anim_delay = 1000 / glob::FPS;
}

void AnimSprite::set_frame(int frameNum) { curr_frame = frameNum % num_frames; }
void AnimSprite::set_delay(int delay) { anim_delay = delay; }
void AnimSprite::advance(int frames) { curr_frame = (curr_frame + frames) % num_frames; }
void AnimSprite::rewind(int frames) { curr_frame = (((curr_frame - frames) % num_frames) + num_frames) % num_frames; }
void AnimSprite::sync(bool reverse)
{
	if (reverse)
	{
		int frame_num = -(int)(SDL_GetTicks()) / anim_delay;
		curr_frame = ((frame_num % num_frames) + num_frames) % num_frames;
	}
	else
	{
		int frame_num = SDL_GetTicks() / anim_delay;
		curr_frame = frame_num % num_frames;
	}
	
}