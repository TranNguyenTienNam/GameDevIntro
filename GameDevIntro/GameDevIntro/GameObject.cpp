#include "GameObject.h"
#include "Utils.h"

CGameObject::CGameObject()
{
	isEnabled = true;
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
	collider->PhysicsUpdate(coObjects);
}

void CGameObject::OnCollisionEnter(CCollider2D* selfCollider, std::vector<CCollisionEvent*> collisions)
{
}

void CGameObject::OnTriggerEnter(CCollider2D* selfCollider, std::vector<CCollisionEvent*> collisions)
{
}

void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR2 p(transform.position.x, transform.position.y);

	LPDIRECT3DTEXTURE9 bbox = CGame::GetInstance()->GetService<CTextures>()->Get("tex-bbox");

	RectF rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = collider->GetBoundingBox().right - collider->GetBoundingBox().left;
	rect.bottom = collider->GetBoundingBox().top - collider->GetBoundingBox().bottom;

	CGame::GetInstance()->Draw(p, bbox, rect.left, rect.top, rect.right, rect.bottom, 100);
}
