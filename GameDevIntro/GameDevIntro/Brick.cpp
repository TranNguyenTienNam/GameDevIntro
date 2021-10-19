#include "Animations.h"
#include "Brick.h"

void CBrick::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Brick", animations->Get("ani-brick"));
}

CBrick::CBrick() :CGameObject()
{
	InitAnimations();
	UpdateBoundingBox();
}

void CBrick::Update(DWORD dt)
{
	UpdateBoundingBox();
}

void CBrick::Render()
{
	animations.at("Brick")->Render(transform.position);
}

void CBrick::UpdateBoundingBox()
{
	RectF boundingBox;
	boundingBox.left = transform.position.x;
	boundingBox.top = transform.position.y;
	boundingBox.right = transform.position.x + BRICK_WIDTH;
	boundingBox.bottom = transform.position.y + BRICK_WIDTH;
	collider->SetBoundingBox(boundingBox);
}