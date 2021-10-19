#pragma once
#include "AnimationFrame.h"

class CAnimation
{
	std::vector<LPANIMATION_FRAME> frames;

	int defaultFrame;
	int currentFrame;
	DWORD lastFrameTime;

public:
	void Add(std::string id, DWORD time);
	void Render(Vector2 position, int alpha = 255);
};

typedef CAnimation* LPANIMATION;