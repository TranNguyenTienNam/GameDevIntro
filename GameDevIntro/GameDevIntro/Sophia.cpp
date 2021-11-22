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
#include "Portal.h"

CSophia::CSophia()
{
	InitAnimation();
	stateWheel = new CWheelIdleState;
	stateDirection = new CSophiaIdleState;

	// Init collider
	stateDirection->UpdateColliders(*this, nx);

	// Player's settings
	controllable = true;
	sophia = this;
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
	velocity.y += -0.0026f * dt; // TODO: Need to adjust gravity
	/*if (abs(velocity.y) > 0.02) velocity.y = -0.02;*/

	if (controllable == false)
	{
		if (dynamic_cast<CWheelIdleState*>(stateWheel) == nullptr) stateWheel = new CWheelIdleState;
		if (dynamic_cast<CSophiaIdleState*>(stateDirection) == nullptr) stateDirection = new CSophiaIdleState;
		stateDirection->Update(dt, *this, nx);
		return;
	}

	/*velocity.x += acceleration.x * dt;*/

	// TODO: Move all changes of states into UpdateState() and the others into Update() of class State
	// TODO: Maybe combine Wheel State with Physical State 
	auto inputHandler = CGame::GetInstance()->GetService<CInputHandler>();

	if (inputHandler->IsKeyDown(PlayerKeySet::MOVE_RIGHT_KEY))
	{
		velocity.x = 0.15f;
		/*acceleration.x = 0.0002f;*/
		nx = 1;
		stateWheel = new CClockwiseState;
	}
	else if (inputHandler->IsKeyDown(PlayerKeySet::MOVE_LEFT_KEY))
	{
		velocity.x = -0.15f;
		/*acceleration.x = -0.0002f;*/
		nx = -1;
		stateWheel = new CCounterclockwiseState;
	}
	else
	{
		velocity.x = 0.0f;
		/*acceleration.x = 0.0f;*/
		stateWheel = new CWheelIdleState;
	}

	// Update gun's direction
	if (dynamic_cast<CSophiaUpwardState*>(stateDirection)) lastTimeToLowerGun = GetTickCount();

	if (inputHandler->IsKeyDown(PlayerKeySet::SOPHIA_UPWARD_KEY))
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

	if (inputHandler->OnKeyDown(PlayerKeySet::JUMPING_KEY) /*&& onGround == true*/)
	{
		onGround = false;
		velocity.y = 0.7f;

		DWORD now = GetTickCount();

		if (inputHandler->IsKeyDown(PlayerKeySet::JUMPING_KEY))
		{

		}
	}

	if (inputHandler->OnKeyDown(PlayerKeySet::SWITCH_CHARACTER_KEY) && 
		GetTickCount() - lastTimeSwitch > switchDelay)
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

		jason->SetEnable(true);
		jason->SetPosition(transform.position);
		jason->SetControllable(true);
		jason->SetDirect(nx);
		jason->SetState(JasonState::JASON_JUMPING);
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

void CSophia::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	if (dynamic_cast<CBrick*>(collision->obj))
	{
		if (onGround == false && collision->ny == 1) onGround = true;
	}
	else if (dynamic_cast<CPortal*>(collision->obj))
	{
		auto p = dynamic_cast<CPortal*>(collision->obj);
		CGame::GetInstance()->GetService<CScenes>()->SwitchScene(p->GetSceneId());
		return;
	}
}

void CSophia::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
}
