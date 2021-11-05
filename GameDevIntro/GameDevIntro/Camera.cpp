#include "Camera.h"
#include "Game.h"
#include "Scenes.h"
#include "PlayScene.h"

Vector3 CCamera::WorldToScreenPoint(Vector2 pos)
{
	return Vector3(floor(pos.x - position.x), floor(-pos.y + position.y), 0);
}

CCamera::CCamera()
{
}

CCamera::~CCamera()
{
}

RectF CCamera::GetBoundingBox()
{
	Vector2 posBB = position + bbOffset;
	RectF boundingBox;
	boundingBox.left = posBB.x;
	boundingBox.top = posBB.y;
	boundingBox.right = posBB.x + bbSize.x;
	boundingBox.bottom = posBB.y - bbSize.y;
	return boundingBox;
}

void CCamera::Update(Vector2 pos)
{
	position = pos;
	/*camPos.x = camPos.x < 0 ? 0 : camPos.x;
	camPos.y = camPos.y < 0 ? 0 : camPos.y;*/
}

void CCamera::RenderBoundingBox()
{
	LPDIRECT3DTEXTURE9 blue_bbox = CGame::GetInstance()->GetService<CTextures>()->Get("tex-bbox");

	RectF rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = bbSize.x;
	rect.bottom = bbSize.y;

	CGame::GetInstance()->Draw(position + bbOffset, blue_bbox, rect.left, rect.top, rect.right, rect.bottom, 32);
}