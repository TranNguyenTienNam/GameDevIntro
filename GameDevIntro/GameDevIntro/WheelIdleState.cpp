#include "WheelIdleState.h"
#include "Utils.h"

void CWheelIdleState::Render(CSophia& sophia)
{
	auto animLeft = sophia.GetAnimations().at("Left-Wheel");
	animLeft->SetIsPaused(true);

	auto animRight = sophia.GetAnimations().at("Right-Wheel");
	animRight->SetIsPaused(true);
}
