#pragma once
#include <SDL.h>
#include <SDL2_rotozoom.h>
#include <string>
#include "BaseSprite.h"

class SpinSprite: public BaseSprite
{
public:
	SpinSprite();

	void reset() override;
	void revert();
	void rotate(double angle, int smoothing = 0);
	void renderAt(int x, int y, double zoom = 1);

protected:
	double theta;
	
};

