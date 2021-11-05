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

	// Init collider
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(Vector2(BRICK_WIDTH, BRICK_WIDTH));
	colliders.push_back(collider);
}

void CBrick::Update(DWORD dt)
{

}

void CBrick::Render()
{
	animations.at("Brick")->Render(transform.position);
}