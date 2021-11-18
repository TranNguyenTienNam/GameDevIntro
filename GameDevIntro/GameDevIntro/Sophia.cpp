#include "Sophia.h"
#include "Animations.h"
#include "WheelIdleState.h"
#include "ClockwiseState.h"
#include "CounterclockwiseState.h"
#include "SophiaIdleState.h"
#include "SophiaUpward45State.h"
#include "SophiaUpwardState.h"
#include "InputHandler.h"
#include "Utils.h"
#include "Brick.h"
#include "Scenes.h"
#include "PlayScene.h"
#include "Jason.h"

CSophia::CSophia()
{
	InitAnimation();
	stateWheel = new CWheelIdleState;
	stateDirection = new CSophiaIdleState;

	// Init collider
	stateDirection->UpdateColliders(*this, nx);

	// Player
	controllable = true;
	CPlayer::sophia = this;
}

CSophia::~CSophia()
{	
}

void CSophia::InitAnimation()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Left-Wheel", animations->Get("ani-sophia-left-wheel"));
	AddAnimation("Right-Wheel", animations->Get("ani-sophia-right-wheel"));

	sprMiddle = CGame::GetInstance()->GetService<CSprites>()->Get("spr-sophia-middle");
}

void CSophia::Update(DWORD dt)
{
	if (controllable == false)
	{
		if (dynamic_cast<CWheelIdleState*>(stateWheel) == nullptr) stateWheel = new CWheelIdleState;
		if (dynamic_cast<CSophiaIdleState*>(stateDirection) == nullptr) stateDirection = new CSophiaIdleState;
		stateDirection->Update(dt, *this, nx);
		return;
	}

	// TODO: Move all changes of states into UpdateState() and the others into Update() of class State
	// TODO: Maybe combine Wheel State with Physical State 
	auto inputHandler = CGame::GetInstance()->GetService<CInputHandler>();
	if (inputHandler->IsKeyDown(DIK_RIGHT))
	{
		velocity.x = 0.15f;
		nx = 1;
		stateWheel = new CClockwiseState;
	}
	else if (inputHandler->IsKeyDown(DIK_LEFT))
	{
		velocity.x = -0.15f;
		nx = -1;
		stateWheel = new CCounterclockwiseState;
	}
	else
	{
		velocity.x = 0.0f;
		stateWheel = new CWheelIdleState;
	}

	if (dynamic_cast<CSophiaUpwardState*>(stateDirection)) lastTimeToLowerGun = GetTickCount();

	if (inputHandler->IsKeyDown(DIK_UP))
	{
		DWORD now = GetTickCount();
		if (dynamic_cast<CSophiaIdleState*>(stateDirection))
		{
			stateDirection = new CSophiaUpward45State;
		}
		else if (now - lastTimeToLiftGun > 200 && dynamic_cast<CSophiaUpward45State*>(stateDirection))
		{
			stateDirection = new CSophiaUpwardState;
		}
	}
	else
	{
		lastTimeToLiftGun = GetTickCount();
		DWORD now = lastTimeToLiftGun;
		if (dynamic_cast<CSophiaUpwardState*>(stateDirection))
		{
			stateDirection = new CSophiaUpward45State;
		}
		else if (now - lastTimeToLowerGun > 200 && dynamic_cast<CSophiaUpward45State*>(stateDirection))
		{
			stateDirection = new CSophiaIdleState;
		}
	}

	stateDirection->Update(dt, *this, nx);
	stateDirection->UpdateColliders(*this, nx);

	if (inputHandler->OnKeyDown(DIK_X) && onGround == true)
	{
		onGround = false;
		velocity.y = 0.7f;

		DWORD now = GetTickCount();

		if (inputHandler->IsKeyDown(DIK_X))
		{

		}
	}

	if (inputHandler->OnKeyDown(DIK_LSHIFT) && GetTickCount() - lastTimeSwitch > switchDelay)
	{
		lastTimeSwitch = GetTickCount();
		// Sophia is not controllable, collider is trigger, animation is idle
		controllable = false;
		for (auto co : colliders)
			co->SetTrigger(true);

		// Enable Jason, set jason's position, state is jumping
		auto scene = (CPlayScene*)CGame::GetInstance()->GetService<CScenes>()->GetCurrentScene();
		scene->SetPlayer(jason);
		scene->GetCamera()->SetTarget(jason);

		jason->SetPosition(transform.position);
		jason->SetControllable(true);
		jason->SetState(JasonState::JASON_JUMPING); // TODO: Set nx yet
	}
}

void CSophia::Render()
{
	stateWheel->Render(*this);
	animations.at("Left-Wheel")->Render(transform.position + posLeftWheel, 1);
	animations.at("Right-Wheel")->Render(transform.position + posRightWheel, 1);
	sprMiddle->Draw(transform.position + posMiddle, nx, 255);
	sprCabin->Draw(transform.position + posCabin, nx, 255);
	sprGun->Draw(transform.position + posGun, nx, 255);
}

void CSophia::OnCollisionEnter(CCollider2D* selfCollider, std::vector<CCollisionEvent*> collisions)
{
	for (UINT i = 0; i < collisions.size(); i++)
	{
		LPCOLLISIONEVENT e = collisions[i];

		if (dynamic_cast<CBrick*>(e->obj))
		{
			if (onGround == false && e->ny == 1) onGround = true;
		}
	}
}

void CSophia::OnTriggerEnter(CCollider2D* selfCollider, std::vector<CCollisionEvent*> collisions)
{
}
