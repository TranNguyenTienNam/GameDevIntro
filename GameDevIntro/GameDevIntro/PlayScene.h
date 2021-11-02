#pragma once
#include <memory>

#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "InputHandler.h"
#include "Mario.h"
#include "Grid.h"

const int CELL_SIZE = 128;

class CGameObject;

class CPlayScene : public CScene
{
protected:
	CMario* player;							// A play scene has to have player, right? 
	std::vector<CGameObject*> objects;
	std::unique_ptr<CGrid> grid;			// Grid for space partitioning for collision

	void _ParseSection_BACKGROUND_COLOR(std::string line);
	void _ParseSection_TEXTURES(std::string line);
	void _ParseSection_SPRITES(std::string line);
	void _ParseSection_ANIMATIONS(std::string line);
	void _ParseSection_OBJECTS(std::string line);


public:
	CPlayScene(int id, LPCWSTR filePath);
	~CPlayScene();
	std::vector<CGameObject*> GetGameObjects() { return objects; }

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario* GetPlayer() { return player; }

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};