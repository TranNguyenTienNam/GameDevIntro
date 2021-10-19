#pragma once
#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "Service.h"
#include "KeyEventHandler.h"

#define DIRECTINPUT_VERSION 0x0800
#define KEYBOARD_BUFFER_SIZE 1024

class CInputHandler : public CService
{
public:
	void SetHandleWindow(HWND hWnd) { this->hWnd = hWnd; }
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { this->keyHandler = handler; }

	void Initialize();
	void ProcessKeyboard();
	int IsKeyDown(int keyCode);

private:
	HWND hWnd;
	LPDIRECTINPUT8 di;										// The DirectInput object 
	LPDIRECTINPUTDEVICE8 didv;								// The keyboard device 

	LPKEYEVENTHANDLER keyHandler;

	BYTE keyStates[256];									// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data
};

#endif 