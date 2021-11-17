#include "Jason.h"
#include "Animations.h"
#include "Portal.h"
#include "Scenes.h"

void CJason::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Idle", animations->Get("ani-jason-idle"));
	AddAnimation("Walk", animations->Get("ani-jason-walk"));
	AddAnimation("Jump", animations->Get("ani-jason-jump"));
	AddAnimation("Die", animations->Get("ani-jason-die"));
	AddAnimation("Crouch", animations->Get("ani-jason-crouch"));
	AddAnimation("Lie", animations->Get("ani-jason-lie"));
}

CJason::CJason() :CGameObject()
{
	InitAnimations();

	// Init collider
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(Vector2(JASON_WIDTH, JASON_HEIGHT));
	collider->SetDynamic();
	colliders.push_back(collider);
}

CJason::~CJason()
{
	
}

void CJason::Update(DWORD dt)
{
	if (velocity.x > JASON_WALKING_SPEED) velocity.x = JASON_WALKING_SPEED;
	else if (velocity.x < -JASON_WALKING_SPEED) velocity.x = - JASON_WALKING_SPEED;
	/*if (abs(velocity.x) - 0.0001f * dt < 0) velocity.x = 0;*/
	if (state == JASON_STATE_IDLE)
	{
		/*if (velocity.x * acceleration.x < 0)
			if (acceleration.x < 0 && velocity.x < 0) velocity.x = 0;
			else*/
	}
}

void CJason::Render()
{
	Vector2 pos = transform.position;
	if (velocity.x == 0)
		animations.at("Idle")->Render(pos, nx);
	else animations.at("Walk")->Render(pos, nx);
}

void CJason::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case JASON_STATE_WALK_RIGHT:
		velocity.x = JASON_WALKING_SPEED;
		/*acceleration.x = 0.0002f;*/
		nx = 1;
		break;
	case JASON_STATE_WALK_LEFT:
		velocity.x = -JASON_WALKING_SPEED;
		/*acceleration.x = -0.0002f;*/
		nx = -1;
		break;
	case JASON_STATE_JUMP:
		velocity.y = JASON_JUMP_SPEED_Y;
	case JASON_STATE_IDLE:
		acceleration.x = 0;
		velocity.x = 0;
		break;
	}
}

void CJason::OnCollisionEnter(CCollider2D* selfCollider, std::vector<CCollisionEvent*> collisions)
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

void CJason::OnTriggerEnter(CCollider2D* selfCollider, std::vector<CCollisionEvent*> collisions)
{
}
