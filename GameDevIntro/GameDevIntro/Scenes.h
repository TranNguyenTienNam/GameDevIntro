#pragma once
#ifndef SCENES_H
#define SCENES_H
#include "Service.h"
#include "Scene.h"
#include <unordered_map>

#define MAX_GAME_LINE 1024

#define GAME_FILE_SECTION_UNKNOWN -1
#define GAME_FILE_SECTION_SETTINGS 1
#define GAME_FILE_SECTION_SCENES 2

class CScenes : public CService
{
protected:
	std::unordered_map<int, LPSCENE> scenes;
	int current_scene;
public:
	void _ParseSection_SETTINGS(std::string line);
	void _ParseSection_SCENES(std::string line);
	int GetCurrentSceneID() { return current_scene; }
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void Load(LPCWSTR gameFile);
	void SwitchScene(int scene_id);
};

#endif 
