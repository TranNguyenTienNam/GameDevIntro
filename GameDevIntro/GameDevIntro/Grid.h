#pragma once
#include "GameObject.h"

struct Cell
{
	std::vector<CGameObject*> gameObjects;
};

class CGrid
{
	friend class CPlayScene;
public:
	CGrid(int width, int height, int cellSize);
	~CGrid();
	void AddGameObject(CGameObject* gameObject);				// Add a game object and determine which cell it belongs to
	void AddGameObject(CGameObject* gameObject, Cell* cell);	// Add a game object to the specified cell
	Cell* GetCell(int x, int y);								// Get cell based on cell coordinates
	Cell* GetCell(const Vector2& pos);							// Get cell based on window coordinates
	Vector2 GetCellCoord(const Vector2& pos);
	void RemoveGameObjectFromCell(CGameObject* gameObject);

	void RenderBoundingBox(int x, int y);
	std::vector<CGameObject*> GetPotentialObjects(CGameObject* object);
private:
	std::vector<Cell> m_cells;
	int m_cellSize;
	int m_width;
	int m_height;
	int m_numXCells;
	int m_numYCells;
};