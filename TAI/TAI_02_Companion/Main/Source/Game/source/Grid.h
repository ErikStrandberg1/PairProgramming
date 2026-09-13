#pragma once
#include <span>
#include <vector>
#include "tge/Engine.h"
#include "InputMapper.h"
#include "tge/drawers/LineDrawer.h"
#include "GridTypes.h"

static constexpr float CELL_WIDTH = 40.f;
static constexpr float CELL_HEIGHT = 40.f;
static constexpr float CELL_HEIGHT_OFFSET = 20.f;

class Grid
{
public:
	Grid();
	~Grid();

	void Init();
	void Render();

	std::span<NodeData> GetGrid() { return myGrid; }
	int GetWidth() const { return myWidth; }
	int GetHeight() const { return myHeight; }

	Tga::Vector2f GetMousePos(InputManager& inputManager);
	bool GetCellCoord(Tga::Vector2f pixelPos, int& outX, int& outY);

private:
	void InitGrid();
	Tga::Color GetColor(Color color) const;

	Tga::LineDrawer myLineDrawer;
	Tga::Vector2f myResolution{};
	std::vector<NodeData> myGrid{};
	int myWidth = 0;
	int myHeight = 0;
};