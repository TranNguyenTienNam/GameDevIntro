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
	GetCollider()->SetGameObject(this);
	UpdateBoundingBox();
}

void CMario::Update(DWORD dt)
{
	UpdateBoundingBox();

	// simple fall down
	velocity.y += MARIO_GRAVITY * dt;

	std::vector<LPCOLLISIONEVENT> coEvents;
	std::vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	auto current_scene = CGame::GetInstance()->GetService<CScenes>()->GetCurrentScene();
	auto coObjects = dynamic_cast<CPlayScene*>(current_scene)->GetGameObjects();

	if (state != MARIO_STATE_DIE)
		collider->CalcPotentialCollisions(&coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		transform.position.x += velocity.x * dt;
		transform.position.y += velocity.y * dt;
	}
	else
	{
		float min_tx, min_ty;
		float nx, ny;
		nx = ny = 0;

		collider->FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		transform.position.x += min_tx * velocity.x * dt + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		transform.position.y += min_ty * velocity.y * dt + ny * 0.4f;

		if (nx != 0) velocity.x = 0;
		if (ny != 0) velocity.y = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->GetService<CScenes>()->SwitchScene(p->GetSceneId());
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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