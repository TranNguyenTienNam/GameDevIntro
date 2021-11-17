#include "Animation.h"
#include "Utils.h"

void CAnimation::Add(std::string id, DWORD time)
{
	LPSPRITE newSprite = CGame::GetInstance()->GetService<CSprites>()->Get(id);
	LPANIMATION_FRAME frame = new CAnimationFrame(newSprite, time);
	frames.push_back(frame);
	/*DebugOut(L"[INFO] animation frame added: %s\t%d\n", ToWSTR(id).c_str(), frames.size());*/
}

void CAnimation::Render(Vector2 position, int nx, int alpha)
{
	DWORD now = GetTickCount();

	if (isReversed == false)
	{
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
				if (isPaused == false) currentFrame++;
				lastFrameTime = now;
				if (isLooped == true && currentFrame == frames.size()) currentFrame = 0;
			}
		}
	}
	else
	{
		if (currentFrame == -1)
		{
			currentFrame = frames.size() - 1;
			lastFrameTime = now;
		}
		else
		{
			DWORD t = frames[currentFrame]->GetTime();
			if (now - lastFrameTime > t)
			{
				if (isPaused == false) currentFrame--;
				lastFrameTime = now;
				if (isLooped == true && currentFrame == -1) currentFrame = frames.size() - 1;
			}
		}
	}

	frames[currentFrame]->GetSprite()->Draw(position, nx, alpha);
}