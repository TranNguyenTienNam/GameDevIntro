#pragma once
#include <Windows.h>

class CJason;
class CSophia;

class CPlayer
{
protected:
	bool controllable;
	bool onGround;

	static CJason* jason;
	static CSophia* sophia;
	static DWORD switchDelay;
	static DWORD lastTimeSwitch;
public:
	void SetControllable(bool value) { this->controllable = value; }
};