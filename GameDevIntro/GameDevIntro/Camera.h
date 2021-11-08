#pragma once
#include "Transform.h"

class CCamera
{
protected:
	Transform transform;

	Vector2 bbOffset;
	Vector2 bbSize;
public:
	Vector3 WorldToScreenPoint(Vector2 pos);
	CCamera();
	~CCamera();
	Vector2 GetPosition() { return this->transform.position; }
	void SetPosition(Vector2 pos) { this->transform.position = pos; }
	Vector2 GetScale() { return this->transform.scale; }
	void SetScale(Vector2 scale) { this->transform.scale = scale; }
	void SetBoundingBoxOffset(Vector2 offset) { this->bbOffset = offset; }
	void SetBoundingBoxSize(Vector2 boxSize) { this->bbSize = boxSize; }
	RectF GetBoundingBox();
	void Update();
	void RenderBoundingBox();
};