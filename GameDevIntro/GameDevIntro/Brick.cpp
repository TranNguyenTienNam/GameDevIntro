#include "Animations.h"
#include "Brick.h"

CBrick::CBrick() :CGameObject()
{
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
	
}