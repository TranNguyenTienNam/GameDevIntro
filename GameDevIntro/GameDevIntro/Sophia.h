#pragma once
#include "GameObject.h"
#include "SophiaState.h"
#include "Player.h"

class ISophiaState;
class CPlayer;

class CSophia : public CGameObject, public CPlayer
{
private:
	ISophiaState* stateWheel;
	ISophiaState* stateDirection;
	ISophiaState* statePhysical;
	//ISophiaState* equipment; // Dont need to code this if Ms.Trinh not require

	DWORD lastTimeToLiftGun;
	DWORD lastTimeToLowerGun;

	// Power, Hover in class Player
	void InitAnimation(); // If CSophia inherits from CPlayer, this is virtual function
public:
	CSophia();
	~CSophia();

	Vector2 posLeftWheel;
	Vector2 posRightWheel;
	LPSPRITE sprMiddle;
	Vector2 posMiddle;
	LPSPRITE sprCabin;
	Vector2 posCabin;
	LPSPRITE sprGun;
	Vector2 posGun;

	virtual void Update(DWORD dt);
	virtual void Render();

	void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};