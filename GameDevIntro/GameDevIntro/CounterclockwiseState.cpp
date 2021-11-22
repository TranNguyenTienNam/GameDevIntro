#include "CounterclockwiseState.h"
#include "Utils.h"

void CCounterclockwiseState::Render(CSophia& sophia)
{
	auto animLeft = sophia.GetAnimations().at("Left-Wheel");
	animLeft->SetIsReversed(false);
	animLeft->SetIsPaused(false);

	auto animRight = sophia.GetAnimations().at("Right-Wheel");
	animRight->SetIsReversed(false);
	animRight->SetIsPaused(false);
}
