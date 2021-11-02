#include "Animations.h"
#include "Mario.h"
#include "Utils.h"
#include "Transform.h"
#include "PlayScene.h"
#include "Scenes.h"
#include "Portal.h"

void CMario::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Walk_Left",	animations->Get("ani-small-mario-walk-left"));
	AddAnimation("Walk_Right",	animations->Get("ani-small-mario-walk-right"));
	AddAnimation("Idle_Left",	animations->Get("ani-small-mario-idle-left"));
	AddAnimation("Idle_Right",	animations->Get("ani-small-mario-idle-right"));
}

CMario::CMario() :CGameObject()
{
	InitAnimations();
	collider->SetGameObject(this);
	collider->SetDynamic();
}

void CMario::Update(DWORD dt)
{
	
}

void CMario::Render()
{
	Vector2 pos = transform.position;
	if (velocity.x == 0)
	{
		if (nx > 0) animations.at("Idle_Right")->Render(pos);
		else animations.at("Idle_Left")->Render(pos);
	}
	else if (velocity.x > 0)
		animations.at("Walk_Right")->Render(pos);
	else animations.at("Walk_Left")->Render(pos);
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		velocity.x = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		velocity.x = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		velocity.y = -MARIO_JUMP_SPEED_Y * 1.2;
	case MARIO_STATE_IDLE:
		velocity.x = 0;
		break;
	}
}

void CMario::UpdateBoundingBox()
{
	RectF boundingBox;
	boundingBox.left = transform.position.x;
	boundingBox.top = transform.position.y;
	boundingBox.right = transform.position.x + MARIO_WIDTH;
	boundingBox.bottom = transform.position.y + MARIO_HEIGHT;
	collider->SetBoundingBox(boundingBox);
}

void CMario::OnCollisionEnter(CCollider2D* selfCollider, std::vector<CCollisionEvent*> collisions)
{
	for (UINT i = 0; i < collisions.size(); i++)
	{
		LPCOLLISIONEVENT e = collisions[i];

		if (dynamic_cast<CPortal*>(e->obj))
		{
			auto p = dynamic_cast<CPortal*>(e->obj);
			CGame::GetInstance()->GetService<CScenes>()->SwitchScene(p->GetSceneId());
			return;
		}
	}
}

void CMario::OnTriggerEnter(CCollider2D* selfCollider, std::vector<CCollisionEvent*> collisions)
{

}