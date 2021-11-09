#include "Camera.h"
#include "Game.h"
#include "Scenes.h"
#include "PlayScene.h"
#include "Utils.h"

Vector2 CCamera::WorldToScreenPoint(Vector2 pos)
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
	RectF boundingBox;
	boundingBox.left = position.x;
	boundingBox.top = position.y;
	boundingBox.right = position.x + bbSize.x;
	boundingBox.bottom = position.y  - bbSize.y;
	return boundingBox;
}

void CCamera::Update()
{
	auto game = CGame::GetInstance();
	auto posPlayer = ((CPlayScene*)game->GetService<CScenes>()->GetCurrentScene())->GetPlayer()->GetPosition(); // target

	Vector2 vpPlayer = WorldToScreenPoint(posPlayer);
	DebugOut(L"wp %f %f\n", posPlayer.x, posPlayer.y);
	DebugOut(L"vp %f %f\n", vpPlayer.x, vpPlayer.y);

	position.x = posPlayer.x - bbSize.x / 2;
	position.y = posPlayer.y + bbSize.y / 2;
	
	if (vpPlayer.x <= 48 || vpPlayer.x >= 208)
		position.x = posPlayer.x - bbSize.x / 2;
	if (vpPlayer.y <= 64 || vpPlayer.y >= 192)
		position.y = posPlayer.y + bbSize.y / 2;

	// Camera co 2 kieu: static va follow target
	// Static la camera khong chuyen dong khi target di chuyen trong mot rect nhat dinh 
	// vpPlayer.x <= 48 || vpPlayer.x >= 208 (3 tile left right) (goi tat Viewport player)
	// vpPlayer.y <= 64 || vpPlayer.y >= 192 (4 tile bottom top)

	// Boundary block
	if (position.x <= boundary.left)
		position.x = boundary.left;
	if (position.y >= boundary.top)
		position.y = boundary.top;
	if (position.x + bbSize.x >= boundary.right)
		position.x = boundary.right - bbSize.x;
	if (position.y - bbSize.y <= boundary.bottom)
		position.y = boundary.bottom + bbSize.y;
}

void CCamera::RenderBoundingBox()
{
	LPDIRECT3DTEXTURE9 bbox = CGame::GetInstance()->GetService<CTextures>()->Get("tex-green-bbox");
	Vector2 translate_pivot = Vector2(bbSize.x / 2, -bbSize.y / 2);

	RectF rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = bbSize.x;
	rect.bottom = bbSize.y;

	CGame::GetInstance()->Draw(position + translate_pivot, 1, bbox, rect.left, rect.top, rect.right, rect.bottom, 32);
}