#pragma once
#include <dinput.h>
//#include "Mario.h"

class CKeyEventHandler
{
public:
	virtual void KeyState(BYTE* state) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
};

typedef CKeyEventHandler* LPKEYEVENTHANDLER;

//class CSampleKeyHandler : public CKeyEventHandler
//{
//private:
//	CMario* target;
//public:
//	void SetTarget(CMario* target) { this->target = target; }
//
//	virtual void KeyState(BYTE* states);
//	virtual void OnKeyDown(int KeyCode);
//	virtual void OnKeyUp(int KeyCode);
//};