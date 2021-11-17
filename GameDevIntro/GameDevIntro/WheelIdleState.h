#pragma once
#include "SophiaState.h"

class CWheelIdleState : public ISophiaState
{
public:
	void Render(CSophia& sophia);
};