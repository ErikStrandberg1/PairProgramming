#pragma once
#include "tge/math/Vector.h"

enum class Color
{
	OffWhite,
	White,
	Red,
	Green,
	Blue,
	Cyan,
	Purple,
};

enum class Terrain
{
	Impassable,
	Passable
};

struct Cell
{
	Tga::Vector2f pos;
	Tga::Vector2f size{ 40.f, 40.f };
};

struct NodeData
{
	Cell cell;
	Color color{ Color::White };
	Terrain terrain{ Terrain::Passable };

	float gCost = 0.f;
	float hCost = 0.f;
	float FCost() const { return gCost + hCost; }
	int parentIndex = -1;
};