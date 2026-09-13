#include "Grid.h"
#include <span>
#include <limits>
#include "tge/primitives/LinePrimitive.h"

Grid::Grid() {}
Grid::~Grid() {}

void Grid::Init()
{
	myResolution = (Tga::Vector2f)Tga::Engine::GetInstance()->GetRenderSize();
	myLineDrawer.Init();
	InitGrid();
}

void Grid::Render()
{
	if (myGrid.empty()) return;

	std::vector<Tga::Vector3f> fromPositions;
	std::vector<Tga::Vector3f> toPositions;
	std::vector<Tga::Color> colors;

	for (auto& nodeData : myGrid)
	{
		auto& cell = nodeData.cell;
		Tga::Color color = GetColor(nodeData.color);

		float halfW = CELL_WIDTH * 0.5f;
		float halfH = CELL_HEIGHT * 0.5f;
		float inset = 1.0f;

		Tga::Vector3f topLeft = { cell.pos.x - halfW + inset, cell.pos.y + halfH - inset, 0.f };
		Tga::Vector3f topRight = { cell.pos.x + halfW - inset, cell.pos.y + halfH - inset, 0.f };
		Tga::Vector3f backLeft = { cell.pos.x - halfW + inset, cell.pos.y - halfH + inset, 0.f };
		Tga::Vector3f backRight = { cell.pos.x + halfW - inset, cell.pos.y - halfH + inset, 0.f };

		fromPositions.push_back(topLeft);
		fromPositions.push_back(backLeft);
		fromPositions.push_back(topLeft);
		fromPositions.push_back(topRight);

		toPositions.push_back(topRight);
		toPositions.push_back(backRight);
		toPositions.push_back(backLeft);
		toPositions.push_back(backRight);

		colors.push_back(color);
		colors.push_back(color);
		colors.push_back(color);
		colors.push_back(color);

		if (nodeData.color != Color::White && nodeData.color != Color::OffWhite)
		{
			float fi = 4.f;
			Tga::Vector3f ftl = { cell.pos.x - halfW + fi, cell.pos.y + halfH - fi, 0.f };
			Tga::Vector3f ftr = { cell.pos.x + halfW - fi, cell.pos.y + halfH - fi, 0.f };
			Tga::Vector3f fbl = { cell.pos.x - halfW + fi, cell.pos.y - halfH + fi, 0.f };
			Tga::Vector3f fbr = { cell.pos.x + halfW - fi, cell.pos.y - halfH + fi, 0.f };

			fromPositions.push_back(ftl); toPositions.push_back(fbr); colors.push_back(color);
			fromPositions.push_back(ftr); toPositions.push_back(fbl); colors.push_back(color);

			Tga::Vector3f ml = { cell.pos.x - halfW + fi, cell.pos.y, 0.f };
			Tga::Vector3f mr = { cell.pos.x + halfW - fi, cell.pos.y, 0.f };
			Tga::Vector3f mt = { cell.pos.x, cell.pos.y + halfH - fi, 0.f };
			Tga::Vector3f mb = { cell.pos.x, cell.pos.y - halfH + fi, 0.f };
			fromPositions.push_back(ml); toPositions.push_back(mr); colors.push_back(color);
			fromPositions.push_back(mt); toPositions.push_back(mb); colors.push_back(color);
		}
	}

	const unsigned int MAX_LINES = 1000;
	for (unsigned int start = 0; start < fromPositions.size(); start += MAX_LINES)
	{
		unsigned int count = std::min(MAX_LINES, (int)fromPositions.size() - start);
		Tga::LineMultiPrimitive multi;
		multi.fromPositions = fromPositions.data() + start;
		multi.toPositions = toPositions.data() + start;
		multi.colors = colors.data() + start;
		multi.count = count;
		myLineDrawer.Draw(multi);
	}
}

void Grid::InitGrid()
{
	myWidth = (int)(myResolution.x / CELL_WIDTH);
	myHeight = (int)(myResolution.y / CELL_HEIGHT);

	for (int y = 0; y < myHeight; ++y)
		for (int x = 0; x < myWidth; ++x)
		{
			NodeData node{};
			node.cell.pos.x = CELL_WIDTH * x + CELL_WIDTH * 0.5f;
			node.cell.pos.y = CELL_HEIGHT_OFFSET + CELL_HEIGHT * y + CELL_HEIGHT * 0.5f;
			myGrid.emplace_back(node);
		}
}

Tga::Vector2f Grid::GetMousePos(InputManager& inputManager)
{
	return Tga::Vector2f{
		(float)inputManager.GetMousePosition().x,
		myResolution.y - (float)inputManager.GetMousePosition().y
	};
}

Tga::Color Grid::GetColor(Color color) const
{
	switch (color)
	{
	case Color::OffWhite: return { 1.f, 1.f, 1.f, 0.5f };
	case Color::White: return { 1.f, 1.f, 1.f, 1.f };
	case Color::Green: return { 0.f, 1.f, 0.f, 1.f };
	case Color::Purple: return { 0.6f, 0.f, 1.f, 1.f };
	case Color::Cyan: return { 0.f, 1.f, 1.f, 1.f };
	case Color::Blue: return { 0.f, 0.2f, 1.f, 1.f };
	default: return { 1.f, 1.f, 1.f, 1.f };
	}
}

bool Grid::GetCellCoord(Tga::Vector2f pixelPos, int& outX, int& outY)
{
	outX = (int)floor(pixelPos.x / CELL_WIDTH);
	outY = (int)floor((pixelPos.y - CELL_HEIGHT_OFFSET) / CELL_HEIGHT);
	return outX >= 0 && outX < myWidth && outY >= 0 && outY < myHeight;
}