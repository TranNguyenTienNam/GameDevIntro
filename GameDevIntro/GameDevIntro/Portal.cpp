#include "Portal.h"

CPortal::CPortal(float l, float t, float w, float h, int scene_id)
{
	this->scene_id = scene_id;
	transform.position.x = l;
	transform.position.y = t;
	width = w;
	height = h;
	GetCollider()->SetGameObject(this);
}

void CPortal::Update(DWORD dt)
{

}

void CPortal::Render()
{
	Vector2 p(transform.position);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CGame::GetInstance()->GetService<CTextures>()->Get("tex-bbox");

	RectF boundingBox = GetCollider()->GetBoundingBox();

	rect.left = 0;
	rect.top = 0;
	rect.right = boundingBox.right - boundingBox.left;
	rect.bottom = boundingBox.top - boundingBox.bottom;

	CGame::GetInstance()->Draw(p, bbox, rect.left, rect.top, rect.right, rect.bottom, 255);
}

void CPortal::UpdateBoundingBox()
{
	RectF boundingBox;
	boundingBox.left = transform.position.x;
	boundingBox.top = transform.position.y;
	boundingBox.right = transform.position.x + width;
	boundingBox.bottom = transform.position.y - height;
	collider->SetBoundingBox(boundingBox);
}