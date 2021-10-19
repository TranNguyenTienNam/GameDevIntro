#pragma once
#ifndef SPRITES_H
#define SPRITES_H

#include <unordered_map>
#include "Textures.h"

class CSprite
{
	int left;
	int top;
	int width;
	int height;

	LPDIRECT3DTEXTURE9 texture;

public:
	CSprite(int left, int top, int witdh, int height, LPDIRECT3DTEXTURE9 texture);

	void Draw(Vector2 position, int alpha);
};

typedef CSprite* LPSPRITE;

class CSprites : public CService
{
	std::unordered_map<std::string, LPSPRITE> sprites;

public:
	void Add(std::string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 texture);
	LPSPRITE Get(std::string id);
	void Clear();
};

#endif