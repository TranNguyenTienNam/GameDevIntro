#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_JUMP_SPEED_Y		0.53f
#define MARIO_GRAVITY			0.0026f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400

#define MARIO_ANI_IDLE_RIGHT		0
#define MARIO_ANI_IDLE_LEFT			1
#define MARIO_ANI_WALKING_RIGHT		2
#define MARIO_ANI_WALKING_LEFT		3

#define MARIO_WIDTH 15.0f
#define MARIO_HEIGHT 15.0f

class CMario : public CGameObject
{
protected:
	void InitAnimations();

public:
	CMario();
	~CMario();

	void Update(DWORD dt);
	void Render();
	void SetState(int state);

	void OnCollisionEnter(CCollider2D* selfCollider, std::vector<CCollisionEvent*> collisions);
	void OnTriggerEnter(CCollider2D* selfCollider, std::vector<CCollisionEvent*> collisions);
};