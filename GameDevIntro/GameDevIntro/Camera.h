#pragma once
#include "Transform.h"

class CCamera
{
protected:
	Vector2 position;
	Vector2 bbOffset;
	Vector2 bbSize;
public:
	Vector2 WorldPointToScreenPoint(Vector2 pos);
	CCamera();
	~CCamera();
	Vector2 GetCamPos() { return this->position; }
	void SetCamPos(Vector2 pos) { this->position = pos; }
	void SetBoundingBoxOffset(Vector2 offset) { this->bbOffset = offset; }
	void SetBoundingBoxSize(Vector2 boxSize) { this->bbSize = boxSize; }
	RectF GetBoundingBox();
	void Update(Vector2 pos);
	void RenderBoundingBox();
};