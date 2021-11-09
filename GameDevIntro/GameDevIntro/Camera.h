#pragma once
#include "Transform.h"

class CCamera
{
protected:
	Vector2 position;
	RectF boundary;
	Vector2 bbSize;
public:
	Vector2 WorldToScreenPoint(Vector2 pos);
	CCamera();
	~CCamera();
	Vector2 GetPosition() { return this->position; }
	void SetPosition(Vector2 pos) { this->position = pos; }
	void SetBoundingBoxSize(Vector2 boxSize) { this->bbSize = boxSize; }
	RectF GetBoundingBox();
	void GetBoundary(RectF boundary) { this->boundary = boundary; }

	void Update();
	void RenderBoundingBox();
};