#pragma once
#include <vector>
#include <array>
#include <tge/drawers/DebugDrawer.h>

struct Node
{
	Tga::Vector2f myCenter;
	std::array<int, 3> myConnections;
	std::array<int, 3> myIndices;
};

struct Mesh
{
	std::vector<Tga::Vector2f> myVertices;
	std::vector<int> myIndices;
};

Mesh LoadMesh(const char* aObjFile);

class Navmesh
{
public:
	Navmesh();
	~Navmesh();
	void Init(const char* aObjFile);
	void RenderNavmesh(Tga::DebugDrawer& debugDrawer);
	void RenderConnections(Tga::DebugDrawer& debugDrawer);
	const std::vector<Node>& GetNodes() const;
	int	GetNodeIndexFromPoint(const Tga::Vector2f aPoint) const;

private:
	void CreateNodes();
	void CalculateConnections();
	std::vector<Node> myNodes;
	Mesh myMesh;
};
