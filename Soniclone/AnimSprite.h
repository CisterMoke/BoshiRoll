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
	void advance(int frames = 1);
	void rewind(int frames = 1);
	void renderAt(int x, int y);

private:
	int numFrames;
	int currFrame;
	array<int, 2> frameSize;
	vector<array<int,2>> frameRects;
	bool loadFromFile(std::string path, int mode);
};

