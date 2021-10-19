#pragma once
#include "GameObject.h"
#include "Transform.h"

class CGameObject;

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;
struct CCollisionEvent
{
	CGameObject* obj;
	Vector2 vNormal;
	float t;
	CCollisionEvent(float t, Vector2 vNormal, CGameObject* obj = NULL) { this->t = t; this->vNormal = vNormal; this->obj = obj; }

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};

class CCollider2D
{
protected:
	CGameObject* object;
	RectF boundingBox;
public:
	static void SweptAABB(
		RectF movingRect, RectF staticRect,
		float dx, float dy, 
		Vector2& vNormal, float& deltaTime);
	LPCOLLISIONEVENT SweptAABBEx(CGameObject* coO);
	void CalcPotentialCollisions(std::vector<CGameObject*>* coObjects, std::vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(
		std::vector<LPCOLLISIONEVENT>& coEvents,
		std::vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		Vector2& vNormal);

	CGameObject* GetGameObject() { return this->object; }
	void SetGameObject(CGameObject* gameObject) { this->object = gameObject; }

	RectF GetBoundingBox() { return this->boundingBox; }
	void SetBoundingBox(const RectF& boundingBox) { this->boundingBox = boundingBox; }
};