#include <algorithm>    
#include "Collider2D.h"
#include "Utils.h"

void CCollider2D::SweptAABB(
	RectF movingRect, RectF staticRect,
	float dx, float dy, 
	Vector2& vNormal, float& t)
{
	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry, t_exit;

	t = -1.0f;			// no collision
	vNormal = VectorZero();

	//
	// Broad-phase test 
	//

	RectF rBroadPhase;
	rBroadPhase.left = dx > 0 ? movingRect.left : movingRect.left + dx;
	rBroadPhase.top = dy > 0 ? movingRect.top : movingRect.top + dy;
	rBroadPhase.right = dx > 0 ? movingRect.right + dx : movingRect.right;
	rBroadPhase.bottom = dy > 0 ? movingRect.bottom + dy : movingRect.bottom;

	if (!rBroadPhase.Overlap(staticRect) && !rBroadPhase.Contain(staticRect)) return;

	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = staticRect.left - movingRect.right;
		dx_exit = staticRect.right - movingRect.left;
	}
	else if (dx < 0)
	{
		dx_entry = staticRect.right - movingRect.left;
		dx_exit = staticRect.left - movingRect.right;
	}

	if (dy > 0)
	{
		dy_entry = staticRect.top - movingRect.bottom;
		dy_exit = staticRect.bottom - movingRect.top;
	}
	else if (dy < 0)
	{
		dy_entry = staticRect.bottom - movingRect.top;
		dy_exit = staticRect.top - movingRect.bottom;
	}

	if (dx == 0)
	{
		tx_entry = -VectorInfinity().x;
		tx_exit = VectorInfinity().x;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -VectorInfinity().y;
		ty_exit = VectorInfinity().y;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}

	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		if (dx > 0)
		{
			vNormal = Vector2(-1.0f, 0.0f);
		}
		else
		{
			vNormal = Vector2(1.0f, 0.0f);
		}
	}
	else
	{
		if (dy > 0)
		{
			vNormal = Vector2(0.0f, -1.0f);
		}
		else
		{
			vNormal = Vector2(0.0f, 1.0f);
		}
	}
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CCollider2D::SweptAABBEx(CGameObject* coO)
{
	RectF staticRect;		// static object bbox
	RectF movingRect;		// moving object bbox
	Vector2 vNormal;
	float t;

	staticRect = coO->GetCollider()->GetBoundingBox();

	// deal with moving object: m speed = original m speed - collide object speed
	float svx = coO->GetSpeed().x;
	float svy = coO->GetSpeed().y;

	float sdx = svx * CGame::GetDeltaTime();
	float sdy = svy * CGame::GetDeltaTime();

	float mvx = object->GetSpeed().x;
	float mvy = object->GetSpeed().y;

	float dx = mvx * CGame::GetDeltaTime() - sdx;
	float dy = mvy * CGame::GetDeltaTime() - sdy;

	movingRect = GetBoundingBox();

	SweptAABB(
		movingRect, staticRect,
		dx, dy, vNormal, t);

	CCollisionEvent* e = new CCollisionEvent(t, vNormal, coO);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CCollider2D::CalcPotentialCollisions(
	std::vector<CGameObject*>* coObjects,
	std::vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		//if (coObjects->at(i) == object) return; // Don't check collision with itself

		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CCollider2D::FilterCollision(
	std::vector<LPCOLLISIONEVENT>& coEvents,
	std::vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	Vector2& vNormal)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	vNormal = VectorZero();

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->vNormal.x != 0) {
			min_tx = c->t; vNormal.x = c->vNormal.x; min_ix = i;
		}

		if (c->t < min_ty && c->vNormal.y != 0) {
			min_ty = c->t; vNormal.y = c->vNormal.y; min_iy = i;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}