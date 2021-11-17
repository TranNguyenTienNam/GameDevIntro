#pragma once
#include "Transform.h"
#include "GameObject.h"

class CCamera
{
protected:
	Vector2 position;
	RectF boundary;
	Vector2 bbSize;
	CGameObject* target;
public:
	Vector2 WorldToScreenPoint(Vector2 pos);
	CCamera();
	~CCamera();
	Vector2 GetPosition() { return this->position; }
	void SetPosition(Vector2 pos) { this->position = pos; }
	void SetBoundingBoxSize(Vector2 boxSize) { this->bbSize = boxSize; }
	RectF GetBoundingBox();
	void GetBoundary(RectF boundary) { this->boundary = boundary; }
	CGameObject* GetTarget() { return this->target; }
	void SetTarget(CGameObject* target) { this->target = target; }

	void Update();
	void RenderBoundingBox();
};