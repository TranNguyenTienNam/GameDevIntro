#pragma once
#include "GameObject.h"
#include "Player.h"

#define JASON_WALKING_SPEED		0.1f
#define JASON_JUMP_SPEED_Y		0.53f
#define JASON_GRAVITY			0.0026f

#define JASON_WIDTH 8.0f
#define JASON_HEIGHT 16.0f

enum JasonState
{
	JASON_IDLE,
	JASON_MOVING_LEFT,
	JASON_MOVING_RIGHT,
	JASON_JUMPING
};

class CPlayer;

class CJason : public CGameObject, public CPlayer
{
private:
	JasonState state;
	LPANIMATION animation;
protected:
	void InitAnimations();
public:
	CJason();
	~CJason();

	void SetState(JasonState state);

	void Update(DWORD dt);
	void Render();

	void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};