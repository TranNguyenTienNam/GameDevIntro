#pragma once
#include "SophiaState.h"

#define BOX_SOPHIA_UPWARD Vector2(24.0f, 32.0f)
#define OFFSET_SOPHIA_UPWARD Vector2(-3.0f, 12.0f)

#define BOX_SOPHIA_UPWARD_WHEELS Vector2(18.0f, 8.0f)
#define OFFSET_SOPHIA_UPWARD_WHEELS Vector2(0.0f, 0.0f)
#define BOX_SOPHIA_UPWARD_CABIN Vector2(19.0f, 16.0f)
#define OFFSET_SOPHIA_UPWARD_CABIN Vector2(-5.5f, 12.0f)

class CSophiaUpwardState : public ISophiaState
{
public:
	virtual void Shooting();
	virtual void UpdateColliders(CSophia& sophia, int nx);
	virtual void Update(DWORD dt, CSophia& sophia, int nx);
	virtual void Render();
};