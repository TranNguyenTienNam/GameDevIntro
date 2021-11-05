#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <d3d9.h>
#include <d3dx9.h>

#include "Animation.h"
#include "Collider2D.h"
#include "Transform.h"

struct Cell;
class CCollider2D;
struct CCollisionEvent;

class CGameObject
{
protected:
	bool isEnabled;
	Transform transform;
	Vector2 velocity;
	int nx;
	int state;
	CCollider2D* collider;
	Cell* ownerCell;
	int cellVectorIndex = -1;
	std::unordered_map<std::string, LPANIMATION> animations;
public:
	CGameObject();
	~CGameObject();

	bool IsEnabled() { return this->isEnabled; }
	void Disable() { this->isEnabled = false; }
	void SetPosition(Vector2 pos) { this->transform.position = pos; }
	Vector2 GetPosition() { return this->transform.position; }
	void SetVelocity(Vector2 v) { this->velocity = v; }
	Vector2 GetVelocity() { return this->velocity; }
	int GetState() { return this->state; }
	void SetState(int state) { this->state = state; }

	CCollider2D* GetCollider() { return this->collider; }
	void SetCell(Cell* cell) { this->ownerCell = cell; }
	Cell* GetCell() { return this->ownerCell; }
	void SetCellVectorIndex(int index) { this->cellVectorIndex = index; }
	int GetCellVectorIndex() { return this->cellVectorIndex; }

	void AddAnimation(std::string stateName, LPANIMATION animation);

	virtual void UpdateBoundingBox() = 0;	// TODO: Bounding box cannot be updated according to animation frame size
	virtual void PhysicsUpdate(std::vector<CGameObject*>* coObjects);
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
	virtual void OnCollisionEnter(CCollider2D* selfCollider, std::vector<CCollisionEvent*> collisions);
	virtual void OnTriggerEnter(CCollider2D* selfCollider, std::vector<CCollisionEvent*> collisions);
	void RenderBoundingBox();
};

#endif