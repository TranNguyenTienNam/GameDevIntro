#pragma once
#include "Transform.h"

class CCamera
{
protected:
	Vector2 position;
	RectF boundingBox;
public:
	Vector2 WorldPointToScreenPoint(Vector2 pos);
	CCamera();
	~CCamera();
	Vector2 GetCamPos() { return this->position; }
	void SetCamPos(Vector2 pos) { this->position = pos; }
	RectF GetBoundingBox() { return this->boundingBox; }
	void UpdateCamPos(Vector2 pos);
	void UpdateBoundingBox();
	void RenderBoundingBox();
};