#include "SophiaUpwardState.h"

void CSophiaUpwardState::Shooting()
{
}

void CSophiaUpwardState::UpdateColliders(CSophia& sophia, int nx)
{
	auto colliders = sophia.GetColliders();

	colliders.clear();

	auto collider = new CCollider2D;
	collider->SetGameObject(&sophia);
	collider->SetOffset(Vector2(-3.0f * nx, 12.0f));
	collider->SetBoxSize(BOX_SOPHIA_UPWARD);
	collider->SetDynamic(true);
	colliders.push_back(collider);

	sophia.SetColliders(colliders);
}

void CSophiaUpwardState::Update(DWORD dt, CSophia& sophia, int nx)
{
	sophia.posLeftWheel = Vector2(-5.0f, 0.0f);
	sophia.posRightWheel = Vector2(5.0f, 0.0f);
	sophia.posMiddle = Vector2(0.0f, 7.0f);
	sophia.posCabin = Vector2(-7.0f * nx, 13.0f);
	sophia.posGun = Vector2(-3.0f * nx, 24.0f);

	auto sprites = CGame::GetInstance()->GetService<CSprites>();
	sophia.sprCabin = sprites->Get("spr-sophia-cabin-45");
	sophia.sprGun = sprites->Get("spr-sophia-gun-90");
}

void CSophiaUpwardState::Render()
{
	
}
