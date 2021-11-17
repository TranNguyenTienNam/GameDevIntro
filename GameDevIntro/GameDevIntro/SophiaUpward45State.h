#pragma once
#include "SophiaState.h"

#define BOX_SOPHIA_UPWARD_45 Vector2(23.0f, 29.0f)
#define OFFSET_SOPHIA_UPWARD_45 Vector2(-0.5f, 10.5f)

class CSophiaUpward45State : public ISophiaState
{
public:
	virtual void Shooting();
	virtual void UpdateColliders(CSophia& sophia, int nx);
	virtual void Update(DWORD dt, CSophia& sophia, int nx);
	virtual void Render();
};