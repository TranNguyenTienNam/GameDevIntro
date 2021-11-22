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

#include "Sophia.h"
#include "Portal.h"
#include "Brick.h"
#include "Jason.h"

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

	int mapWidth = d["width"].GetInt();			// Calculate by tile
	int mapHeight = d["height"].GetInt();

	// Set boundary of camera
	RectF boundary;
	boundary.left = -tileWidth / 2;
	boundary.top = mapHeight * tileHeight + tileHeight / 2;
	boundary.right = mapWidth * tileWidth - tileWidth / 2;
	boundary.bottom = tileHeight / 2;
	mainCam->GetBoundary(boundary);

	// Init Grid
	m_mapWidth = mapWidth * tileWidth;
	m_mapHeight = mapHeight * tileHeight;
	quadtree = new CQuadtree(0, RectF(0, m_mapHeight, m_mapWidth, 0));
	quadtree->Reset(m_mapWidth, m_mapHeight);
	
	// Tileset texture settings
	int columns = d["tilesets"].GetArray()[0]["columns"].GetInt();
	int spacing = d["tilesets"].GetArray()[0]["spacing"].GetInt();
	/*auto image_path = ToWSTR(d["tilesets"].GetArray()[0]["image"].GetString());*/
	
	/*CGame::GetInstance()->GetService<CTextures>()->Add("tex-tileset", image_path.c_str(), D3DCOLOR_XRGB(0, 0, 0));*/

	auto layers = d["layers"].GetArray();

	for (auto& layer : layers)
	{
		auto layer_type = layer["type"].GetString();
		auto visible = layer["visible"].GetBool();

		// Tile Layer
		if (strcmp(layer_type, "tilelayer") == 0 && visible == true)
		{
			auto data = layer["data"].GetArray();

			for (int x = 0; x < mapWidth; x++)
			{
				for (int y = 0; y < mapHeight; y++)
				{
					int tilesetID = data[y * mapWidth + x].GetInt() - 1;

					// Get tile coordinate in tileset by id
					int tileX = tilesetID % columns;
					int tileY = tilesetID / columns;

					int left = tileX * (tileWidth + spacing);
					int top = tileY * (tileHeight + spacing);

					auto texTileset = CGame::GetInstance()->GetService<CTextures>()->Get("tex-tileset");

					int posX = x * tileWidth;
					int posY = (mapHeight - y) * tileHeight;

					Vector2 position = Vector2(posX, posY);

					auto newTile = new CTile(position, left, top, tileWidth, tileHeight, texTileset);

					tilemap.push_back(newTile);
				}
			}
		}
		// Object Layer
		else if (strcmp(layer_type, "objectgroup") == 0 && visible == true)
		{
			auto objects = layer["objects"].GetArray();

			for (int i = 0; i < objects.Size(); i++)
			{
				CGameObject* obj = new CBrick;
				int x = objects[i]["x"].GetInt();
				int y = objects[i]["y"].GetInt();

				obj->SetPosition(Vector2(x, m_mapHeight - y + 16));

				auto properties = objects[i]["properties"].GetArray();
				for (int j = 0; j < properties.Size(); j++)
				{
					bool filterX = true;
					bool filterY = true;

					if (strcmp(properties[j]["name"].GetString(), "FilterX"))
						filterX = properties[j]["value"].GetBool();
					else if (strcmp(properties[j]["name"].GetString(), "FilterY"))
						filterY = properties[j]["value"].GetBool();
					obj->GetColliders().at(0)->SetFilter(filterX, filterY);
				}

				this->objects.push_back(obj);
				this->quadtree->Insert(obj);
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

	if (object_type == "obj-jason")
	{
		obj = new CJason;
	}
	else if (object_type == "obj-sophia")
	{
		if (player != NULL)
		{
			DebugOut(L"[ERROR] SOPHIA object was created before!\n");
			return;
		}
		obj = new CSophia;
		player = (CSophia*)obj;
		mainCam->SetTarget(player);

		DebugOut(L"[INFO] Player object created!\n");
	}
	else if (object_type == "obj-brick") obj = new CBrick;
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

	quadtree->Insert(obj);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	auto game = CGame::GetInstance();

	// Init Camera
	mainCam = new CCamera();
	mainCam->SetBoundingBoxSize(Vector2(game->GetScreenWidth(), game->GetScreenHeight()));

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
	RectF bbMainCam = mainCam->GetBoundingBox();
	UpdateOnScreenTiles(bbMainCam);
	UpdatePotentialObjects(bbMainCam);
}

void CPlayScene::UpdateOnScreenTiles(RectF rect)
{
	onScreenTilemap.clear();

	rect.left -= 16;
	rect.top += 16;
	rect.right += 16;
	rect.bottom -= 16;

	for (auto tile : tilemap)
	{
		if (rect.Contain(tile->GetPosition()))
			onScreenTilemap.push_back(tile);
	}
}

void CPlayScene::UpdatePotentialObjects(RectF rect)
{
	potentials.clear();
	quadtree->Update(objects);
	quadtree->Retrieve(potentials, rect);
	DebugOut(L"potentials %d\n", potentials.size());
}

void CPlayScene::Update(DWORD dt)
{
	for (auto obj : potentials)
		if (obj->IsEnabled() == true) obj->PhysicsUpdate(&potentials);
	
	for (auto obj : potentials)
		if (obj->IsEnabled() == true) obj->Update(dt);

	if (mainCam != nullptr) mainCam->Update();
}

void CPlayScene::Render()
{
	for (auto tile : onScreenTilemap)
		tile->Draw(255);

	for (auto obj : potentials)
		if (obj->IsEnabled() == true) obj->Render();

	// RENDERING GIZMO
	for (auto obj : potentials)
		if (obj->IsEnabled() == true) obj->RenderBoundingBox();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (auto tile : tilemap)
		delete tile;
	tilemap.clear();

	onScreenTilemap.clear();
	 
	for (auto obj : objects)
		obj->SetDestroyed();
	objects.clear();

	player = NULL;
	
	if (quadtree != nullptr)
	{
		delete quadtree;
		quadtree = nullptr;
	}
	
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
		if (obj->IsDestroyed() == true) delete obj;
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CJason* jason = (CJason*)((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_A:
		CGame::GetInstance()->GetService<CScenes>()->SwitchScene(1);
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	
}