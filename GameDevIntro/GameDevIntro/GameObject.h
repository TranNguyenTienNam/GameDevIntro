#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <d3d9.h>
#include <d3dx9.h>

#include "Animation.h"
#include "Collider2D.h"
#include "Transform.h"
#include "Quadtree.h"

struct Cell;

class CQuadtree;
class CCollider2D;
struct CCollisionEvent;

class CGameObject
{
protected:
	bool isEnabled;

	Transform transform;
	std::vector<CCollider2D*>  colliders;

	int nx;
	Vector2 velocity;
	Vector2 acceleration;
	int state;

	Cell* ownerCell;				// Grid
	int cellVectorIndex = -1;		//

	CQuadtree* ownerQuadtree;		// Quadtree
	int inNodesIndex = -1;			//

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
	void SetAcceleration(Vector2 a) { this->acceleration = a; }
	Vector2 GetAcceleration() { return this->acceleration; }

	std::vector<CCollider2D*> GetColliders() { return this->colliders; }
	void SetColliders(std::vector<CCollider2D*> colliders) { this->colliders = colliders; }

	CQuadtree* GetQuadtree() { return this->ownerQuadtree; }
	void SetQuadtree(CQuadtree* quadtree) { this->ownerQuadtree = quadtree; }
	int GetInNodesIndex() { return this->inNodesIndex; }
	void SetInNodesIndex(int index) { this->inNodesIndex = index; }

#pragma region Grid Space Partitioning
	void SetCell(Cell* cell) { this->ownerCell = cell; }
	Cell* GetCell() { return this->ownerCell; }
	void SetCellVectorIndex(int index) { this->cellVectorIndex = index; }
	int GetCellVectorIndex() { return this->cellVectorIndex; }
#pragma endregion

	void AddAnimation(std::string stateName, LPANIMATION animation);
	std::unordered_map<std::string, LPANIMATION> GetAnimations() { return this->animations; }

	virtual void PhysicsUpdate(std::vector<CGameObject*>* coObjects);
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
	virtual void OnCollisionEnter(CCollider2D* selfCollider, std::vector<CCollisionEvent*> collisions);
	virtual void OnTriggerEnter(CCollider2D* selfCollider, std::vector<CCollisionEvent*> collisions);

	void RenderBoundingBox();
};

#endif