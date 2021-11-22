#include "Jason.h"
#include "Animations.h"
#include "Portal.h"
#include "Scenes.h"
#include "InputHandler.h"
#include "Brick.h"
#include "Utils.h"
#include "PlayScene.h"
#include "Sophia.h"

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

	isEnabled = false;

	// Init collider
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(Vector2(JASON_WIDTH, JASON_HEIGHT));
	collider->SetDynamic(true);
	colliders.push_back(collider);

	// Player's settings
	onGround = true;
	controllable = false;
	jason = this;
	SetState(JasonState::JASON_IDLE);
}

CJason::~CJason()
{
	
}

void CJason::SetState(JasonState state)
{
	switch (state)
	{
	case JASON_IDLE:
		velocity.x = 0.0f;
		/*acceleration.x = 0.0f;*/
		if (onGround == true) animation = animations.at("Idle");
		break;
	case JASON_MOVING_LEFT:
		/*velocity.x = -JASON_WALKING_SPEED;*/
		acceleration.x = -0.0002f;
		nx = -1;
		if (onGround == true) animation = animations.at("Walk");
		break;
	case JASON_MOVING_RIGHT:
		/*velocity.x = JASON_WALKING_SPEED;*/
		acceleration.x = 0.0002f;
		nx = 1;
		if (onGround == true) animation = animations.at("Walk");
		break;
	case JASON_JUMPING:
		onGround = false;
		velocity.y = JASON_JUMP_SPEED_Y;
		animation = animations.at("Jump");
		break;
	default:
		break;
	}
}

void CJason::Update(DWORD dt)
{
	velocity.y += -0.0026f * dt;
	velocity.x += acceleration.x * dt;
	

	// TODO: Limit velocity
	/*if (velocity.x > JASON_WALKING_SPEED) velocity.x = JASON_WALKING_SPEED;
	else if (velocity.x < -JASON_WALKING_SPEED) velocity.x = -JASON_WALKING_SPEED;*/

	auto inputHandler = CGame::GetInstance()->GetService<CInputHandler>();
	if (controllable == false)
	{
		SetState(JasonState::JASON_IDLE);
		return;
	}

	/*velocity.x += acceleration.x * dt;*/

	if (inputHandler->IsKeyDown(PlayerKeySet::MOVE_RIGHT_KEY))
	{
		SetState(JasonState::JASON_MOVING_RIGHT);
	}
	else if (inputHandler->IsKeyDown(PlayerKeySet::MOVE_LEFT_KEY))
	{
		SetState(JasonState::JASON_MOVING_LEFT);
	}
	else
	{
		SetState(JasonState::JASON_IDLE);
	}

	if (inputHandler->OnKeyDown(PlayerKeySet::JUMPING_KEY) && onGround == true)
	{
		SetState(JasonState::JASON_JUMPING);
	}

	if (inputHandler->OnKeyDown(PlayerKeySet::SWITCH_CHARACTER_KEY) && 
		GetTickCount() - lastTimeSwitch > switchDelay && 
		sophia->GetColliders().at(0)->GetBoundingBox().Contain(colliders.at(0)->GetBoundingBox())) // TODO: Fix if size of colliders is greater than one
	{
		lastTimeSwitch = GetTickCount();
		controllable = false;
		isEnabled = false;

		auto scene = (CPlayScene*)CGame::GetInstance()->GetService<CScenes>()->GetCurrentScene();
		scene->SetPlayer(sophia);
		scene->GetCamera()->SetTarget(sophia);

		sophia->SetControllable(true);
		for (auto co : sophia->GetColliders())
			co->SetTrigger(false);
	}
}

void CJason::Render()
{
	animation->Render(transform.position, nx);
}

void CJason::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	if (dynamic_cast<CBrick*>(collision->obj))
	{
		if (onGround == false && collision->ny == 1) onGround = true;
		// TODO: Collise with wall, then hold idle state
	}
	else if (dynamic_cast<CPortal*>(collision->obj))
	{
		auto p = dynamic_cast<CPortal*>(collision->obj);
		CGame::GetInstance()->GetService<CScenes>()->SwitchScene(p->GetSceneId());
		return;
	}
}

void CJason::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collisions)
{
}
