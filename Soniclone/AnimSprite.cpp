#include "AnimSprite.h"
using glob::gRenderer;


AnimSprite::AnimSprite() = default;


bool AnimSprite::loadFromFile(std::string path, int mode)
{
	return BaseSprite::loadFromFile(path, mode);
}

bool AnimSprite::loadFromFile(std::string path, array<int, 2> _frameSize, int mode)
{
	if (!loadFromFile(path, mode)) { return false; }
	frameSize = _frameSize;
	w = frameSize[0];
	h = frameSize[1];

	int horizontal = baseSurf->w / w;
	int vertical = baseSurf->h / h;
	numFrames = horizontal * vertical;
	frameRects.reserve(numFrames);

	for (int i = 0; i < vertical; i++)
	{
		for (int j = 0; j < horizontal; j++)
		{
			int fx = j * w;
			int fy = i * h;
			int fn = i * horizontal + j;
			frameRects.push_back({ fx, fy });
		}
	}
	return true;
}

void AnimSprite::reset()
{
	BaseSprite::reset();
	frameRects.erase(frameRects.begin(), frameRects.end());
	numFrames = 0;
	currFrame = 0;
	animDelay = 1000 / glob::FPS;
}

void AnimSprite::setFrame(int frameNum) { currFrame = frameNum % numFrames; }
void AnimSprite::setDelay(int delay) { animDelay = delay; }
void AnimSprite::advance(int frames) { currFrame = (currFrame + frames) % numFrames; }
void AnimSprite::rewind(int frames) { currFrame = (((currFrame - frames) % numFrames) + numFrames) % numFrames; }
void AnimSprite::sync(bool reverse)
{
	if (reverse)
	{
		int frameNum = -(int)(SDL_GetTicks()) / animDelay;
		currFrame = ((frameNum % numFrames) + numFrames) % numFrames;
	}
	else
	{
		int frameNum = SDL_GetTicks() / animDelay;
		currFrame = frameNum % numFrames;
	}
	
}

void AnimSprite::renderAt(int x, int y, Vec2 const &off, float phi, float zx, float zy)
{
	SDL_Rect clip = { frameRects[currFrame][0], frameRects[currFrame][1], frameSize[0], frameSize[1] };
	BaseSprite::renderAt(x, y, off, phi, zx, zy, &clip);
}

void AnimSprite::renderAt(Vec2 const &pos, Vec2 const &off, float phi, float zx, float zy)
{
	renderAt(pos.x, pos.y, off, phi, zx, zy);
}