#include <algorithm>    
#include "Collider2D.h"
#include "Utils.h"
#include "Jason.h"

void CCollider2D::SweptAABB(
	RectF movingRect, RectF staticRect,
	float dx, float dy, 
	float& nx, float& ny, float& t)
{
	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry, t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0.0f;

	//
	// Broad-phase test 
	//

	RectF rBroadPhase;
	rBroadPhase.left = dx > 0 ? movingRect.left : movingRect.left + dx;
	rBroadPhase.top = dy > 0 ? movingRect.top + dy: movingRect.top;
	rBroadPhase.right = dx > 0 ? movingRect.right + dx : movingRect.right;
	rBroadPhase.bottom = dy > 0 ? movingRect.bottom : movingRect.bottom + dy;

	if (!rBroadPhase.Overlap(staticRect)) return;

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
		dy_entry = staticRect.bottom - movingRect.top;
		dy_exit = staticRect.top - movingRect.bottom;
	}
	else if (dy < 0)
	{
		dy_entry = staticRect.top - movingRect.bottom;
		dy_exit = staticRect.bottom - movingRect.top;
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
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CCollider2D::SweptAABBEx(CCollider2D* coOther)
{
	RectF staticRect;		// static object bbox
	RectF movingRect;		// moving object bbox
	float t, nx, ny;

	staticRect = coOther->GetBoundingBox();

	// deal with moving object: m speed = original m speed - collide object speed
	auto coObject = coOther->GetGameObject();
	float sdx = coObject->GetVelocity().x * CGame::GetDeltaTime();
	float sdy = coObject->GetVelocity().y * CGame::GetDeltaTime();

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = this->dx - sdx;
	float rdy = this->dy - sdy;

	movingRect = GetBoundingBox();

	SweptAABB(
		movingRect, staticRect,
		rdx, rdy, nx, ny, t);

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, coObject, coOther);
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
		if (object != coObjects->at(i) && coObjects->at(i)->IsEnabled() == true) {
			for (auto co : coObjects->at(i)->GetColliders())
			{
				LPCOLLISIONEVENT e = SweptAABBEx(co);

				if (e->t > 0 && e->t <= 1.0f)
					coEvents.push_back(e);
				else
					delete e;
			}
		}
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CCollider2D::FilterCollision(
	std::vector<LPCOLLISIONEVENT>& coEvents,
	LPCOLLISIONEVENT& colX,
	LPCOLLISIONEVENT& colY,
	bool filterX = true, 
	bool filterY = true)
{
	float min_tx, min_ty;

	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0 && filterX == true) {
			min_tx = c->t; min_ix = i;
		}

		if (c->t < min_ty && c->ny != 0 && filterY == true) {
			min_ty = c->t; min_iy = i;
		}
	}

	if (min_ix >= 0) colX = coEvents[min_ix];
	if (min_iy >= 0) colY = coEvents[min_iy];
}

void CCollider2D::PhysicsUpdate(std::vector<CGameObject*>* coObjects)
{
	// Reduce the number of check collision if game object is not enabled, is nullptr, static
	if (object == nullptr || isDynamic == false) return;

	auto dt = CGame::GetDeltaTime();
	auto pos = object->GetPosition();
	auto velocity = object->GetVelocity();

	this->dx = velocity.x * dt;
	this->dy = velocity.y * dt;

	coEvents.clear();
	coEventX = NULL;
	coEventY = NULL;

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		pos.x += dx;
		pos.y += dy;
		object->SetPosition(pos);
	}
	else
	{
		FilterCollision(coEvents, coEventX, coEventY);

		if (coEventX != NULL && coEventY != NULL)
		{
			// was collision on Y first ?
			if (coEventY->t < coEventX->t)
			{
				if (isTrigger == false)
				{
					pos.y += coEventY->t * dy + coEventY->ny * 0.4f;
					object->SetPosition(pos);

					if (coEventY->ny != 0) velocity.y = 0;
					object->SetVelocity(velocity);
				}

				if (coEventY->ny != 0)
					if (isTrigger == false) object->OnCollisionEnter(this, coEventY);
					else object->OnTriggerEnter(this, coEventY);

				//
				// see if after correction on Y, is there still a collision on X ? 
				//
				LPCOLLISIONEVENT colX_other = NULL;

				//
				// check again if there is true collision on X 
				//

				// replace with a new collision event using corrected location 
				coEvents.push_back(SweptAABBEx(coEventX->co));

				// re-filter on X only
				FilterCollision(coEvents, colX_other, coEventY, /*filterX=*/1, /*filterY=*/0);

				if (colX_other != NULL)
				{
					pos.x += colX_other->t * dx + colX_other->nx * 0.4f;
					/*object->SetPosition(pos);*/

					if (isTrigger == false) object->OnCollisionEnter(this, colX_other);
					else object->OnTriggerEnter(this, colX_other);
				}
				else
				{
					pos.x += dx;
					/*object->SetPosition(pos);*/
				}
			}
			// collision on X first
			else 
			{
				if (isTrigger == false)
				{
					pos.x += coEventX->t * dx + coEventX->nx * 0.4f;
					object->SetPosition(pos);

					if (coEventX->nx != 0) velocity.x = 0;
					object->SetVelocity(velocity);
				}

				if (coEventX->nx != 0)
					if (isTrigger == false) object->OnCollisionEnter(this, coEventX);
					else object->OnTriggerEnter(this, coEventX);

				//
				// see if after correction on X, is there still a collision on Y ? 
				//
				LPCOLLISIONEVENT colY_other = NULL;

				//
				// check again if there is true collision on X 
				//

				// replace with a new collision event using corrected location 
				coEvents.push_back(SweptAABBEx(coEventY->co));

				// re-filter on X only
				FilterCollision(coEvents, coEventX, colY_other, /*filterX=*/0, /*filterY=*/1);

				if (colY_other != NULL)
				{
					pos.y += colY_other->t * dy + colY_other->ny * 0.4f;
					/*object->SetPosition(pos);*/

					if (isTrigger == false) object->OnCollisionEnter(this, colY_other);
					else object->OnTriggerEnter(this, colY_other);
				}
				else
				{
					pos.y += dy;
					/*object->SetPosition(pos);*/
				}
			}
		}
		else
		{
			if (coEventX != NULL)
			{
				if (isTrigger == false)
				{
					pos.x += coEventX->t * dx + coEventX->nx * 0.4f;
					pos.y += dy;
					object->SetPosition(pos);

					if (coEventX->nx != 0) velocity.x = 0;
					object->SetVelocity(velocity);
				}

				if (isTrigger == false) object->OnCollisionEnter(this, coEventX);
				else object->OnTriggerEnter(this, coEventX);
			}
			else
			{
				if (coEventY != NULL)
				{
					if (isTrigger == false)
					{
						pos.x += dx;
						pos.y += coEventY->t * dy + coEventY->ny * 0.4f;
						object->SetPosition(pos);

						if (coEventY->ny != 0) velocity.y = 0;
						object->SetVelocity(velocity);
					}

					if (isTrigger == false) object->OnCollisionEnter(this, coEventY);
					else object->OnTriggerEnter(this, coEventY);
				}
				// both colX & colY are NULL 
				else 
				{
					pos.x += dx;
					pos.y += dy;
					/*object->SetPosition(pos);*/
				}
			}
		}

		object->SetPosition(pos);
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

RectF CCollider2D::GetBoundingBox()
{
	Vector2 posCollider = object->GetPosition() + offset;
	RectF boundingBox;
	boundingBox.left = posCollider.x - boxSize.x / 2;
	boundingBox.top = posCollider.y + boxSize.y / 2;
	boundingBox.right = posCollider.x + boxSize.x / 2;
	boundingBox.bottom = posCollider.y - boxSize.y / 2;
	return boundingBox;
}

void CCollider2D::RenderBoundingBox()
{
	Vector2 posCollider = object->GetPosition() + offset;

	LPDIRECT3DTEXTURE9 bbox = CGame::GetInstance()->GetService<CTextures>()->Get("tex-bbox");

	RectF rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = boxSize.x;
	rect.bottom = boxSize.y;

	CGame::GetInstance()->Draw(posCollider, -1, bbox, rect.left, rect.top, rect.right, rect.bottom, 100);
}
