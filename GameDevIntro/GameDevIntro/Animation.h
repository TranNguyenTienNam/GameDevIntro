#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

#include "AnimationFrame.h"

class CAnimation
{
	std::vector<LPANIMATION_FRAME> frames;

	int defaultFrame;
	int currentFrame;
	DWORD lastFrameTime;

	bool isLooped = true;
	bool isPaused = false;
	bool isReversed = false;

public:
	void Add(std::string id, DWORD time);
	void Render(Vector2 position, int nx, int alpha = 255);

	void SetIsLooped(bool value) { this->isLooped = value; }
	void SetIsPaused(bool value) { this->isPaused = value; }
	void SetIsReversed(bool value) { this->isReversed = value; }
};

typedef CAnimation* LPANIMATION;

#endif