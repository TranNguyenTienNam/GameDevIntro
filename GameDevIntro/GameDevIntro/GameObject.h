#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <d3d9.h>
#include <d3dx9.h>

#include "Animation.h"
#include "Collider2D.h"
#include "Transform.h"

class CCollider2D;

class CGameObject
{
protected:
	Transform transform;
	Vector2 velocity;
	
	int nx;

	int state;

	CCollider2D* collider;

	std::unordered_map<std::string, LPANIMATION> animations;

public:
	CGameObject();
	~CGameObject();

	void SetPosition(Vector2 pos) { this->transform.position = pos; }
	Vector2 GetPosition() { return this->transform.position; }
	void SetSpeed(Vector2 v) { this->velocity = v; }
	Vector2 GetSpeed() { return this->velocity; }
	int GetState() { return this->state; }
	void SetState(int state) { this->state = state; }
	
	CCollider2D* GetCollider() { return this->collider; }

	void AddAnimation(std::string stateName, LPANIMATION animation);

	virtual void UpdateBoundingBox() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
};

#endif