#include "Camera.h"
#include "Game.h"
#include "Scenes.h"
#include "PlayScene.h"

Vector2 CCamera::WorldPointToScreenPoint(Vector2 pos)
{
	return Vector2(floor(pos.x - position.x), floor(-pos.y + position.y));
}

CCamera::CCamera()
{
}

CCamera::~CCamera()
{
}

RectF CCamera::GetBoundingBox()
{
	Vector2 posBoundingBox = position + bbOffset;

	RectF boundingBox;
	boundingBox.left = position.x + bbSize.x / 2;
	boundingBox.top = position.y - bbSize.y / 2;
	boundingBox.right = position.x + bbSize.x * 3/2;
	boundingBox.bottom = position.y - bbSize.y *3/2;
	return boundingBox;
}

void CCamera::Update(Vector2 pos)
{
	position = pos;
	position.x -= bbSize.x;
	position.y += bbSize.y;
	/*camPos.x = camPos.x < 0 ? 0 : camPos.x;
	camPos.y = camPos.y < 0 ? 0 : camPos.y;*/
}

void CCamera::RenderBoundingBox()
{
	Vector2 pos = position + bbOffset;
	LPDIRECT3DTEXTURE9 blue_bbox = CGame::GetInstance()->GetService<CTextures>()->Get("tex-bbox");

	RectF rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = bbSize.x;
	rect.bottom = bbSize.y;

	CGame::GetInstance()->Draw(pos, blue_bbox, rect.left, rect.top, rect.right, rect.bottom, 32);
}