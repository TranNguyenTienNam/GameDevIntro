#include "Grid.h"
#include <cmath>
#include <cstdlib>
#include "Utils.h"

CGrid::CGrid(int width, int height, int cellSize) : m_width(width), m_height(height), m_cellSize(cellSize)
{
	m_numXCells = ceil((float)m_width / m_cellSize);
	m_numYCells = ceil((float)m_height / m_cellSize);

	// Allocate all the cells
	m_cells.resize(m_numYCells * m_numXCells);
}

CGrid::~CGrid()
{
	
}

void CGrid::AddGameObject(CGameObject* gameObject)
{
	Cell* cell = GetCell(gameObject->GetPosition());
	cell->gameObjects.push_back(gameObject);
	gameObject->SetCell(cell);
	gameObject->SetCellVectorIndex(cell->gameObjects.size() - 1);
}

void CGrid::AddGameObject(CGameObject* gameObject, Cell* cell)
{
	cell->gameObjects.push_back(gameObject);
	gameObject->SetCell(cell);
	gameObject->SetCellVectorIndex(cell->gameObjects.size() - 1);
}

Cell* CGrid::GetCell(int x, int y)
{
	if (x < 0) x = 0;
	if (x >= m_numXCells) x = m_numXCells;
	if (y < 0) y = 0;
	if (y >= m_numYCells) y = m_numYCells;

	/*if (x < 0 || x >= m_numXCells || y < 0 || y >= m_numYCells) return nullptr;*/
	return &m_cells[y * m_numXCells + x];
}

Cell* CGrid::GetCell(const Vector2& pos)
{
	int cellX = (int)(pos.x / m_cellSize);
	int cellY = (int)(pos.y / m_cellSize);

	return GetCell(cellX, cellY);
}

Vector2 CGrid::GetCellCoord(const Vector2& pos)
{
	int cellX = (int)(pos.x / m_cellSize);
	int cellY = (int)(pos.y / m_cellSize);

	return Vector2(cellX, cellY);
}

void CGrid::RemoveGameObjectFromCell(CGameObject* gameObject)
{
	std::vector<CGameObject*>& gameObjects = gameObject->GetCell()->gameObjects;
	// Normal vector swap
	gameObjects[gameObject->GetCellVectorIndex()] = gameObjects.back();
	gameObjects.pop_back();
	// Update vector index
	if (gameObject->GetCellVectorIndex() < gameObjects.size())
	{
		gameObjects[gameObject->GetCellVectorIndex()]->SetCellVectorIndex(gameObject->GetCellVectorIndex());
	}
	// Set the index of game object to -1
	gameObject->SetCellVectorIndex(-1);
	gameObject->SetCell(nullptr);
}

void CGrid::RenderBoundingBox(int x, int y)
{
	D3DXVECTOR2 p(x * m_cellSize, y * m_cellSize);

	LPDIRECT3DTEXTURE9 bbox = CGame::GetInstance()->GetService<CTextures>()->Get("tex-bbox");
	LPDIRECT3DTEXTURE9 green_bbox = CGame::GetInstance()->GetService<CTextures>()->Get("tex-green-bbox");

	RectF rect;
	rect.left = 0;
	rect.top = m_cellSize - 0.5;
	rect.right = m_cellSize - 0.5;
	rect.bottom = 0;

	if (GetCell(x,y)->isActive)
		CGame::GetInstance()->Draw(p, green_bbox, rect.left, rect.top, rect.right, rect.bottom, 32);
	else CGame::GetInstance()->Draw(p, bbox, rect.left, rect.top, rect.right, rect.bottom, 32);
}

void CGrid::SetActiveCells(RectF rect)
{
	m_activeCells.clear();

	// Unshow all cells
	for (int i = 0; i < m_cells.size(); i++)
	{
		int x = i % m_numXCells;
		int y = i / m_numXCells;
		GetCell(x, y)->isActive = false;
	}

	int startX = floor(rect.left / m_cellSize);
	int endX = floor(rect.right / m_cellSize);
	int startY = floor(rect.bottom / m_cellSize);
	int endY = floor(rect.top / m_cellSize);
	
	for (int x = startX; x <= endX; x++)
	{
		if (x < 0 || x >= m_numXCells) continue;
		for (int y = startY; y <= endY; y++)
		{
			if (y < 0 || y >= m_numYCells) continue;
			GetCell(x, y)->isActive = true;
			m_activeCells.push_back(GetCell(x, y));
		}
	}
}
