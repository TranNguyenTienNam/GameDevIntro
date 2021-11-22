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
	LPCOLLISIONEVENT coEventX;
	LPCOLLISIONEVENT coEventY;
protected:
	float dx;
	float dy;

	bool isTrigger = false;
	bool isDynamic = false;
	bool filterX;
	bool filterY;
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
		LPCOLLISIONEVENT & colX, LPCOLLISIONEVENT & colY, 
		bool filterX, bool filterY);
	void PhysicsUpdate(std::vector<CGameObject*>* coObjects);

	bool IsDynamic() { return this->isDynamic; }
	void SetDynamic(bool value) { this->isDynamic = value; }
	void SetTrigger(bool value) { this->isTrigger = value; }
	void SetFilter(bool valueX, bool valueY) { this->filterX = valueX; this->filterY = valueY; }
	CGameObject* GetGameObject() { return this->object; }
	void SetGameObject(CGameObject* gameObject) { this->object = gameObject; }

	void SetOffset(Vector2 offset) { this->offset = offset; }
	void SetBoxSize(Vector2 size) { this->boxSize = size; }
	RectF GetBoundingBox();
	void SetBoundingBox(const RectF& boundingBox) { this->boundingBox = boundingBox; }
	void RenderBoundingBox();
};