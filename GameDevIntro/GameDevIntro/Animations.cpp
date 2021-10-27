#include "Animations.h"
#include "Utils.h"

void CAnimations::Add(std::string id, LPANIMATION anim)
{
	animations.insert(make_pair(id, anim));
	/*DebugOut(L"[INFO] animation added: %s\n", ToWSTR(id).c_str());*/
}

LPANIMATION CAnimations::Get(std::string id)
{
	return animations.at(id);
}

void CAnimations::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION s = x.second;
		delete s;
	}

	animations.clear();
}