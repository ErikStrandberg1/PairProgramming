#pragma once
#include <vector>
#include <array>
#include <tge/drawers/DebugDrawer.h>

struct Portal
{
	int myNeighbour = -1;
	int myVertexA = -1;
	int myVertexB = -1;
};

struct Node
{
	Tga::Vector2f myCenter;
	std::array<Portal, 3> myPortals;
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

	// Returns aPoint if it is inside the mesh. Otherwise returns the closest
	// point on any triangle edge. outNodeIndex is the triangle that point sits on.
	Tga::Vector2f GetClosestPointOnMesh(const Tga::Vector2f& aPoint, int& outNodeIndex) const;

	std::vector<int> FindPath(int aStartNode, int aEndNode) const;

	std::vector<Tga::Vector2f> SmoothPath(const std::vector<int>& aNodePath,
	                                      const Tga::Vector2f& aStart,
	                                      const Tga::Vector2f& aEnd) const;
private:
	void CreateNodes();
	void CalculateConnections();
	std::vector<Node> myNodes;
	Mesh myMesh;
};
