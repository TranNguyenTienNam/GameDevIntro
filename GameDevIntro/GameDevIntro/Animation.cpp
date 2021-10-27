#include "Animation.h"
#include "Utils.h"

void CAnimation::Add(std::string id, DWORD time)
{
	LPSPRITE newSprite = CGame::GetInstance()->GetService<CSprites>()->Get(id);
	LPANIMATION_FRAME frame = new CAnimationFrame(newSprite, time);
	frames.push_back(frame);
	/*DebugOut(L"[INFO] animation frame added: %s\t%d\n", ToWSTR(id).c_str(), frames.size());*/
}

void CAnimation::Render(Vector2 position, int alpha)
{
	DWORD now = GetTickCount();

	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}
	}

	frames[currentFrame]->GetSprite()->Draw(position, alpha);
}