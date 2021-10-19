#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 

	int width;
	int height;
public:
	CPortal(float l, float t, float r, float b, int scene_id);
	void Update(DWORD dt);
	void Render();
	void UpdateBoundingBox();
	int GetSceneId() { return scene_id; }
};