#pragma once
#include "GameObject.h"
#include "Transform.h"

class CGameObject;

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;
struct CCollisionEvent
{
	CGameObject* obj;
	float nx, ny, t;
	CCollisionEvent(float t, float nx, float ny, CGameObject* obj = NULL) { this->t = t; this->nx = nx; this->ny = ny; this->obj = obj; }

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};

class CCollider2D
{
private:
	std::vector<LPCOLLISIONEVENT> coEvents;
	std::vector<LPCOLLISIONEVENT> coEventsResult;
protected:
	float dx;
	float dy;

	bool isTrigger = false;
	bool isDynamic = false;
	CGameObject* object;
	RectF boundingBox;
public:
	static void SweptAABB(
		RectF movingRect, RectF staticRect,
		float dx, float dy, 
		float& nx, float& ny, float& deltaTime);
	LPCOLLISIONEVENT SweptAABBEx(CGameObject* coO);
	void CalcPotentialCollisions(std::vector<CGameObject*>* coObjects, std::vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(
		std::vector<LPCOLLISIONEVENT>& coEvents,
		std::vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx, float& min_ty,
		float& nx, float& ny);
	void PhysicsUpdate(std::vector<CGameObject*>* coObjects);

	CGameObject* GetGameObject() { return this->object; }
	void SetGameObject(CGameObject* gameObject) { this->object = gameObject; }

	RectF GetBoundingBox() { return this->boundingBox; }
	void SetBoundingBox(const RectF& boundingBox) { this->boundingBox = boundingBox; }

	void SetDynamic() { this->isDynamic = true; }
	void SetStatic() { this->isDynamic = false; }
};