#include "ClockwiseState.h"
#include "Utils.h"

void CClockwiseState::Render(CSophia& sophia)
{
	auto animLeft = sophia.GetAnimations().at("Left-Wheel");
	animLeft->SetIsReversed(true);
	animLeft->SetIsPaused(false);

	auto animRight = sophia.GetAnimations().at("Right-Wheel");
	animRight->SetIsReversed(true);
	animRight->SetIsPaused(false);
}
