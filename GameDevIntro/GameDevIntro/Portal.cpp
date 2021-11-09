#include "Portal.h"

CPortal::CPortal(float l, float t, float w, float h, int scene_id)
{
	this->scene_id = scene_id;
	transform.position.x = l;
	transform.position.y = t;
	width = w;
	height = h;

	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(Vector2(w, h));
	colliders.push_back(collider);
}

void CPortal::Update(DWORD dt)
{

}

void CPortal::Render()
{
	Vector2 p(transform.position);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CGame::GetInstance()->GetService<CTextures>()->Get("tex-bbox");

	RectF boundingBox = colliders.at(0)->GetBoundingBox();

	rect.left = 0;
	rect.top = 0;
	rect.right = boundingBox.right - boundingBox.left;
	rect.bottom = boundingBox.top - boundingBox.bottom;

	CGame::GetInstance()->Draw(p, 1, bbox, rect.left, rect.top, rect.right, rect.bottom, 255);
}