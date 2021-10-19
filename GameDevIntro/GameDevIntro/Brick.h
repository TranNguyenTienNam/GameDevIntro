#pragma once
#include "GameObject.h"

#define BRICK_WIDTH 16.0f

class CBrick : public CGameObject
{
protected:
	void InitAnimations();

public:
	CBrick();
	void Update(DWORD dt);
	void Render();

	void UpdateBoundingBox();
};