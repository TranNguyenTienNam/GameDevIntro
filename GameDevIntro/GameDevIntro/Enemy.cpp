#include "Enemy.h"
#include "Scenes.h"
#include "PlayScene.h"

CGameObject* CEnemy::target = nullptr;

CEnemy::CEnemy()
{
	auto player = ((CPlayScene*)CGame::GetInstance()->GetService<CScenes>()->GetCurrentScene())->GetPlayer();
	if (player != nullptr) target = player;
}

CEnemy::~CEnemy()
{
}
