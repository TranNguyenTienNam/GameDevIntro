#pragma once
#include "GameObject.h"
#include "Transform.h"

class CGameObject;
class CCollider2D;

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;
struct CCollisionEvent
{
	CGameObject* obj;
	CCollider2D* co;
	float nx, ny, t;
	CCollisionEvent(float t, float nx, float ny, CGameObject* obj = NULL, CCollider2D* co = NULL) 
	{
		this->t = t; this->nx = nx; this->ny = ny; this->obj = obj; this->co = co;
	}

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
	Vector2 offset;
	Vector2 boxSize;
public:
	static void SweptAABB(
		RectF movingRect, RectF staticRect,
		float dx, float dy, 
		float& nx, float& ny, float& deltaTime);
	LPCOLLISIONEVENT SweptAABBEx(CCollider2D* coOther);
	void CalcPotentialCollisions(std::vector<CGameObject*>* coObjects, std::vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(
		std::vector<LPCOLLISIONEVENT>& coEvents,
		std::vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx, float& min_ty,
		float& nx, float& ny);
	void PhysicsUpdate(std::vector<CGameObject*>* coObjects);

	bool IsDynamic() { return this->isDynamic; }
	void SetDynamic(bool value) { this->isDynamic = value; }
	void SetTrigger(bool value) { this->isTrigger = value; }
	CGameObject* GetGameObject() { return this->object; }
	void SetGameObject(CGameObject* gameObject) { this->object = gameObject; }

	void SetOffset(Vector2 offset) { this->offset = offset; }
	void SetBoxSize(Vector2 size) { this->boxSize = size; }
	RectF GetBoundingBox();
	void SetBoundingBox(const RectF& boundingBox) { this->boundingBox = boundingBox; }
	void RenderBoundingBox();
};