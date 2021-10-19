#pragma once
#include "Animation.h"

class CAnimations : public CService
{
	std::unordered_map<std::string, LPANIMATION> animations;

public:
	void Add(std::string id, LPANIMATION anim);
	LPANIMATION Get(std::string id);
	void Clear();
};