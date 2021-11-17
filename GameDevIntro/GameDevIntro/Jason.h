#pragma once
#include "GameObject.h"

#define JASON_WALKING_SPEED		0.1f
#define JASON_JUMP_SPEED_Y		0.53f
#define JASON_GRAVITY			0.0026f

#define JASON_STATE_IDLE			10
#define JASON_STATE_WALK_LEFT		20
#define JASON_STATE_WALK_RIGHT		21
#define JASON_STATE_JUMP			30
#define JASON_STATE_DIE				40
#define JASON_STATE_CROUCH_LEFT		50
#define JASON_STATE_CROUCH_RIGHT	51
#define JASON_STATE_LIE				60

#define JASON_WIDTH 8.0f
#define JASON_HEIGHT 16.0f

enum JasonState
{
	JASON_IDLE,
	JASON_WALKING_LEFT,
	JASON_WALKING_RIGHT,
	JASON_JUMPING
};

class CJason : public CGameObject
{
private:
	bool onGround;
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

	void OnCollisionEnter(CCollider2D* selfCollider, std::vector<CCollisionEvent*> collisions);
	void OnTriggerEnter(CCollider2D* selfCollider, std::vector<CCollisionEvent*> collisions);
};