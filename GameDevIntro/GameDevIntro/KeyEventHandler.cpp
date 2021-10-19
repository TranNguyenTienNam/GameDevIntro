#include "KeyEventHandler.h"
#include "InputHandler.h"
#include "Game.h"
#include "Utils.h"

//void CSampleKeyHandler::OnKeyDown(int KeyCode)
//{
//	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
//	switch (KeyCode)
//	{
//	case DIK_SPACE:
//		target->SetState(MARIO_STATE_JUMP);
//		break;
//	}
//}
//
//void CSampleKeyHandler::OnKeyUp(int KeyCode)
//{
//	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
//}
//
//void CSampleKeyHandler::KeyState(BYTE* states)
//{
//	auto handler = CGame::GetInstance()->GetService<CInputHandler>();
//	if (handler->IsKeyDown(DIK_RIGHT))
//		target->SetState(MARIO_STATE_WALKING_RIGHT);
//	else if (handler->IsKeyDown(DIK_LEFT))
//		target->SetState(MARIO_STATE_WALKING_LEFT);
//	else target->SetState(MARIO_STATE_IDLE);
//}