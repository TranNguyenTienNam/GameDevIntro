#include "GameObject.h"
#include "Utils.h"

CGameObject::CGameObject()
{
	isEnabled = true;
	nx = 1;
	transform.position = VectorZero();
	velocity = VectorZero();
}

CGameObject::~CGameObject()
{
	
}

void CGameObject::AddAnimation(std::string stateName, LPANIMATION animation)
{
	animations.insert(std::make_pair(stateName, animation));
}

void CGameObject::PhysicsUpdate(std::vector<CGameObject*>* coObjects)
{
	for (auto co : colliders)
		co->PhysicsUpdate(coObjects);
}

void CGameObject::OnCollisionEnter(CCollider2D* selfCollider, std::vector<CCollisionEvent*> collisions)
{
}

void CGameObject::OnTriggerEnter(CCollider2D* selfCollider, std::vector<CCollisionEvent*> collisions)
{
}

void CGameObject::RenderBoundingBox()
{
	for (auto co : colliders)
		co->RenderBoundingBox();
}
