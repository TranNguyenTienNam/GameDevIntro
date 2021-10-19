#pragma once
//#include <d3d9.h>
#include <d3dx9.h>

typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;

class Transform
{
public:
	Vector2 position;
	Vector2 rotation;
	float scale;
};

struct RectF
{
	float left, top, right, bottom;
	bool Overlap(const RectF& other);
	bool Contain(const RectF& other);
	bool Contain(const Vector2& point);
	RectF RectZero() { return { 0, 0, 0, 0 }; }
};

Vector2 VectorInfinity();
Vector2 VectorZero();
Vector2 NormalizedVector();