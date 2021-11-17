#include "ClockwiseState.h"
#include "Utils.h"

void CClockwiseState::Render(CSophia& sophia)
{
	auto anim = sophia.GetAnimations().at("Left-Wheel");
	anim->SetIsReversed(true);
	anim->SetIsPaused(false);

	auto anim1 = sophia.GetAnimations().at("Right-Wheel");
	anim1->SetIsReversed(true);
	anim1->SetIsPaused(false);
}
