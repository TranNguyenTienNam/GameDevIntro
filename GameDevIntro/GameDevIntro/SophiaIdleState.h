#pragma once
#include "SophiaState.h"

#define BOX_SOPHIA_IDLE Vector2(24.0f, 17.0f)
#define OFFSET_SOPHIA_IDLE Vector2(0.0f, 4.5f)

class CSophiaIdleState : public ISophiaState
{
public:
	virtual void Shooting();
	virtual void UpdateColliders(CSophia& sophia, int nx);
	virtual void Update(DWORD dt, CSophia& sophia, int nx);
	virtual void Render();
};