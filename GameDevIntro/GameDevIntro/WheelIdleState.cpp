#include "WheelIdleState.h"
#include "Utils.h"

void CWheelIdleState::Render(CSophia& sophia)
{
	auto anim = sophia.GetAnimations().at("Left-Wheel");
	anim->SetIsPaused(true);

	auto anim1 = sophia.GetAnimations().at("Right-Wheel");
	anim1->SetIsPaused(true);
}
