#pragma once
#include "GameObject.h"

class CEnemy
{
protected:
	static CGameObject* target;
public:
	CEnemy();
	~CEnemy();
};