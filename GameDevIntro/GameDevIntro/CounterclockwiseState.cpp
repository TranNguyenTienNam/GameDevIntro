#include "CounterclockwiseState.h"
#include "Utils.h"

void CCounterclockwiseState::Render(CSophia& sophia)
{
	auto anim = sophia.GetAnimations().at("Left-Wheel");
	anim->SetIsReversed(false);
	anim->SetIsPaused(false);

	auto anim1 = sophia.GetAnimations().at("Right-Wheel");
	anim1->SetIsReversed(false);
	anim1->SetIsPaused(false);
}
