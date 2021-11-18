#pragma once
#include <memory>

#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "InputHandler.h"
#include "Grid.h"
#include "Camera.h"
#include "Sprites.h"
#include "Quadtree.h"

const int CELL_SIZE = 128;

class CGameObject;

class CPlayScene : public CScene
{
protected:
	CGameObject* player;											// A play scene has to have player, right? 
	std::vector<CTile*> tilemap;
	std::vector<CTile*> onScreenTilemap;
	std::vector<CGameObject*> objects;
	std::vector<CGameObject*> potentials, destroyed;
	CQuadtree* quadtree;					// Quadtree for space partitioning
	CCamera* mainCam;

	float m_mapWidth;
	float m_mapHeight;

	void _ParseSection_BACKGROUND_COLOR(std::string line);
	void _ParseSection_TEXTURES(std::string line);
	void _ParseSection_SPRITES(std::string line);
	void _ParseSection_ANIMATIONS(std::string line);
	void _ParseSection_TILEMAP(std::string line);
	void _ParseSection_OBJECTS(std::string line);

public:
	CPlayScene(int id, LPCWSTR filePath);
	~CPlayScene();

	virtual void Load();
	virtual void PreUpdate();
	virtual void UpdateOnScreenTiles(RectF rect);
	virtual void UpdatePotentialObjects(RectF rect);
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void Clean();

	CGameObject* GetPlayer() { return player; }
	void SetPlayer(CGameObject* object) { this->player = object; }
	CCamera* GetCamera() { return mainCam; }

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