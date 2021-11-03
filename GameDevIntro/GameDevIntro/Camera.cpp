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

void CCamera::UpdateCamPos(Vector2 pos)
{
	position = pos;

	position.x -= CGame::GetInstance()->GetScreenWidth() / 2;
	position.y += CGame::GetInstance()->GetScreenHeight() / 2;
	/*camPos.x = camPos.x < 0 ? 0 : camPos.x;
	camPos.y = camPos.y < 0 ? 0 : camPos.y;*/
}

void CCamera::UpdateBoundingBox()
{
	auto game = CGame::GetInstance();
	/*boundingBox.left = position.x;
	boundingBox.top = position.y;
	boundingBox.right = position.x + game->GetScreenWidth();
	boundingBox.bottom = position.y - game->GetScreenHeight();*/

	// Test
	boundingBox.left = position.x + game->GetScreenWidth() / 4;
	boundingBox.top = position.y - game->GetScreenHeight() / 4;
	boundingBox.right = position.x + game->GetScreenWidth() * 3 / 4;
	boundingBox.bottom = position.y - game->GetScreenHeight() * 3 / 4;
}

void CCamera::RenderBoundingBox()
{
	Vector2 pos;
	pos.x = position.x + CGame::GetInstance()->GetScreenWidth() / 4;
	pos.y = position.y - CGame::GetInstance()->GetScreenHeight() / 4;
	LPDIRECT3DTEXTURE9 blue_bbox = CGame::GetInstance()->GetService<CTextures>()->Get("tex-bbox");

	RectF rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = boundingBox.right - boundingBox.left;
	rect.bottom = boundingBox.top - boundingBox.bottom;

	CGame::GetInstance()->Draw(pos, blue_bbox, rect.left, rect.top, rect.right, rect.bottom, 32);
}