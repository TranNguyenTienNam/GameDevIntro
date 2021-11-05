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
	CPortal(float l, float t, float w, float h, int scene_id);
	void Update(DWORD dt);
	void Render();
	int GetSceneId() { return scene_id; }
};