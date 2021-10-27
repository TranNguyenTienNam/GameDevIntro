#include "Sprites.h"
#include "Utils.h"

CSprite::CSprite(int left, int top, int width, int height, LPDIRECT3DTEXTURE9 texture)
{
	this->left = left;
	this->top = top;
	this->width = width;
	this->height = height;
	this->texture = texture;
}

void CSprite::Draw(Vector2 position, int alpha)
{
	CGame::GetInstance()->Draw(position, texture, left, top, left + width, top + height, alpha);
}

void CSprites::Add(std::string id, int left, int top, int width, int height, LPDIRECT3DTEXTURE9 texture)
{
	sprites.insert(make_pair(id, new CSprite(left, top, width, height, texture)));
	/*DebugOut(L"[INFO] sprite added: %s\t%d\t%d\t%d\t%d\n", ToWSTR(id).c_str(), left, top, height, width);*/
}

LPSPRITE CSprites::Get(std::string id)
{
	return sprites.at(id);
}

void CSprites::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}