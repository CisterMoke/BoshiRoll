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

	bool loadFromFile(std::string path, std::array<int, 2> _frameSize, int mode = 0x01);
	void reset();

	void setFrame(int frameNum);
	void setDelay(int delay);
	void advance(int frames = 1);
	void rewind(int frames = 1);
	void sync(bool reverse = false);
	void renderAt(int x, int y);
	void renderAt(Vec2 const &pos);

private:
	int numFrames = 0;
	int currFrame = 0;
	int animDelay = 1000 / glob::FPS;
	array<int, 2> frameSize = { 0, 0 };
	vector<array<int,2>> frameRects;
	bool loadFromFile(std::string path, int mode);
};

