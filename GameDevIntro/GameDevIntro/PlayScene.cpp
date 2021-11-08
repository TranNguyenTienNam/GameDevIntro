#include <iostream>
#include <fstream>

#include "Scenes.h"
#include "PlayScene.h"
#include "Utils.h"
#include "Animations.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

#include "Portal.h"
#include "Brick.h"

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

CPlayScene::~CPlayScene()
{

}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_BACKGROUND_COLOR	1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_TILEMAP			5
#define SCENE_SECTION_OBJECTS			6

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_BACKGROUND_COLOR(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return;	// skip invalid lines

	int R = atoi(tokens[0].c_str());
	int G = atoi(tokens[1].c_str());
	int B = atoi(tokens[2].c_str());

	CGame::GetInstance()->SetBackgroundColor(D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	std::string texID = tokens[0].c_str();
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CGame::GetInstance()->GetService<CTextures>()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	std::string ID = tokens[0].c_str();
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	std::string texID = tokens[5].c_str();

	LPDIRECT3DTEXTURE9 tex = CGame::GetInstance()->GetService<CTextures>()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CGame::GetInstance()->GetService<CSprites>()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	std::string ani_id = tokens[0].c_str();

	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		std::string sprite_id = tokens[i].c_str();
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CGame::GetInstance()->GetService<CAnimations>()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_TILEMAP(std::string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1 || tokens[0] == "") return;

	string filePath = tokens[0];

	FILE* fp;
	errno_t err = fopen_s(&fp, filePath.c_str(), "r");

	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document d;
	d.ParseStream(is);

	int tileWidth = d["tilewidth"].GetInt();
	int tileHeight = d["tileheight"].GetInt();

	auto layers = d["layers"].GetArray();
	
	// Tileset texture settings
	int columns = d["tilesets"].GetArray()[0]["columns"].GetInt();
	int spacing = d["tilesets"].GetArray()[0]["spacing"].GetInt();
	wstring image_path = ToWSTR(d["tilesets"].GetArray()[0]["image"].GetString()).c_str();
	
	CGame::GetInstance()->GetService<CTextures>()->Add("tex-tileset", L"tileset.png", D3DCOLOR_XRGB(0, 0, 0)); // TODO: Fix cant read image file

	for (auto& layer : layers)
	{
		int mapWidth = layer["width"].GetInt();
		int mapHeight = layer["height"].GetInt();

		auto data = layer["data"].GetArray();

		for (int x = 0; x < mapWidth; x++)
		{
			for (int y = 0; y < mapHeight; y++)
			{
				int tilesetID = data[y * mapWidth + x].GetInt() - 1;
				/*DebugOut(L"tilesetID %d\n", tilesetID);*/

				// Get tile coordinate in tileset by id
				int tileX = tilesetID % columns;
				int tileY = tilesetID / columns;

				int left = tileX * (tileWidth + spacing);
				int top = tileY * (tileHeight + spacing);

				auto texTileset = CGame::GetInstance()->GetService<CTextures>()->Get("tex-tileset");
				
				int posX = x * tileWidth;
				int posY = (mapHeight - y) * tileHeight;
				/*float posY = y * tileHeight;*/

				/*DebugOut(L"index %d %d\n", tileX, tileY);*/
				Vector2 position = Vector2(posX, posY);

				auto newTile = new CTile(position, left, top, tileWidth, tileHeight, texTileset);

				/*DebugOut(L"pos %f %f, l %d, t %d, w %d, h %d\n", position.x, position.y, left, top, tileWidth, tileHeight);*/
				tilemap.push_back(newTile);
			}
		}
	}

	fclose(fp);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	string object_type = tokens[0].c_str();
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	Vector2 pos = Vector2(x, y);

	CGameObject* obj = NULL;

	if (object_type == "obj-mario")
	{
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario();
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object created!\n");
	}
	else if (object_type == "obj-brick") obj = new CBrick();
	else if (object_type == "obj-portal")
	{
		float w = atof(tokens[3].c_str());
		float h = atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, w, h, scene_id);
		DebugOut(L"[INFO] Portal object created!\n");
	}
	else
	{
		DebugOut(L"[ERR] Invalid object type: %s\n", ToWSTR(object_type).c_str());
		return;
	}

	// General object setup
	obj->SetPosition(pos);
	objects.push_back(obj);

	grid->AddGameObject(obj);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	// Init Camera
	mainCam = new CCamera();
	auto game = CGame::GetInstance();
	mainCam->SetBoundingBoxSize(Vector2(game->GetScreenWidth() / 2, game->GetScreenHeight() / 2));
	mainCam->SetBoundingBoxOffset(Vector2(game->GetScreenWidth() / 4, -game->GetScreenHeight() / 4));
	mainCam->SetScale(Vector2(2.4f, 2.4f));

	// Init Grid
	int screenWidth = CGame::GetInstance()->GetScreenWidth();
	int screenHeight = CGame::GetInstance()->GetScreenHeight();
	grid = std::make_unique<CGrid>(screenWidth * 10, screenHeight * 3, CELL_SIZE);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[BACKGROUND_COLOR]") { section = SCENE_SECTION_BACKGROUND_COLOR; continue; }
		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[TILEMAP]") { section = SCENE_SECTION_TILEMAP; continue; }
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_BACKGROUND_COLOR: _ParseSection_BACKGROUND_COLOR(line); break;
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_TILEMAP: _ParseSection_TILEMAP(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	game->GetService<CTextures>()->Add("tex-bbox", L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	game->GetService<CTextures>()->Add("tex-green-bbox", L"textures\\green-bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::PreUpdate()
{
	mainCam->Update();
	grid->SetActiveCells(mainCam->GetBoundingBox());

	UpdatePotentialObjects();
}

void CPlayScene::UpdatePotentialObjects()
{
	potentials.clear();

	for (auto cell : grid->m_activeCells)
	{
		for (auto obj : cell->gameObjects)
			potentials.push_back(obj);
	}

	for (auto obj : objects)
	{
		Cell* newCell = grid->GetCell(obj->GetPosition());
		if (newCell != nullptr && newCell != obj->GetCell())
		{
			grid->RemoveGameObjectFromCell(obj);
			grid->AddGameObject(obj, newCell);
		}
	}
}

void CPlayScene::Update(DWORD dt)
{
	for (auto obj : objects)
		if (obj->IsEnabled() == true) obj->PhysicsUpdate(&objects);
	
	for (auto obj : objects)
		if (obj->IsEnabled() == true) obj->Update(dt);
}

void CPlayScene::Render()
{
	for (auto tile : tilemap)
		tile->Draw(255);

	for (auto obj : objects)
		if (obj->IsEnabled() == true) obj->Render();

	// RENDERING GIZMO
	/*for (auto obj : potentials)
		if (obj->IsEnabled() == true) obj->RenderBoundingBox();*/

	// TODO: Something wrong terribly
	/*for (int i= 0; i < grid->m_cells.size(); i++)
	{
		int x = i % grid->m_numXCells;
		int y = i / grid->m_numXCells;
		grid->RenderBoundingBox(x, y);
	}*/

	mainCam->RenderBoundingBox();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (auto obj : objects)
		obj->Disable();

	objects.clear();

	player = NULL;
	grid.reset();
	
	if (mainCam != nullptr)
	{
		delete mainCam;
		mainCam = nullptr;
	}

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::Clean()
{
	for (auto obj : objects)
		if (obj->IsEnabled() == false) delete obj;
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_SPACE:
		mario->SetState(MARIO_STATE_JUMP);
		break;
	/*case DIK_A:
		mario->Reset();*/
	case DIK_A:
		CGame::GetInstance()->GetService<CScenes>()->SwitchScene(1);
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	auto game = CGame::GetInstance()->GetService<CInputHandler>();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		mario->SetState(MARIO_STATE_WALKING_LEFT);
	else
		mario->SetState(MARIO_STATE_IDLE);
}