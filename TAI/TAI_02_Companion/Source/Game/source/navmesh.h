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

struct OpenNode
{
	float myEstimatedCost;
	int myIndex;
	bool operator>(const OpenNode& aOther) const { return myEstimatedCost > aOther.myEstimatedCost; }
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
	int GetNodeIndexFromPoint(const Tga::Vector2f aPoint) const;
	Tga::Vector2f GetClosestPointOnMesh(const Tga::Vector2f& aPoint, int& outNodeIndex) const;

	//A*
	std::vector<int> FindPath(int aStartNode, int aEndNode) const;
	//Funnel
	std::vector<Tga::Vector2f> SmoothPath(const std::vector<int>& aNodePath, const Tga::Vector2f& aStart,
	                                      const Tga::Vector2f& aEnd, float aAgentRadius = 25.f) const;

private:
	//init
	void CreateNodes();
	void CalculateConnections();
	void MarkBorderVertices();

	//Helpers
	float GetNodeDistance(int aNodeA, int aNodeB) const
	{
		return Tga::Vector2f::Distance(myNodes[aNodeA].myCenter, myNodes[aNodeB].myCenter);
	}
	bool IsBorderVertex(int aVertexIndex) const;
	Tga::Vector2f ClosestPointOnSegment(const Tga::Vector2f& aSegmentStart, const Tga::Vector2f& aSegmentEnd,
	                                    const Tga::Vector2f& aTarget) const;

	float SignedArea2(const Tga::Vector2f& aA, const Tga::Vector2f& aB, const Tga::Vector2f& aC) const
	{
		return (aC - aA).Cross(aB - aA);
	}

	bool IsSamePoint(const Tga::Vector2f& aA, const Tga::Vector2f& aB) const
	{
		return (aB - aA).LengthSqr() < ourPointEpsilonSqr;
	}

	void InsetPortal(Tga::Vector2f& aLeft, Tga::Vector2f& aRight, int aLeftVertex, int aRightVertex,
	                 float aAgentRadius) const;

	static constexpr float ourPointEpsilonSqr = 0.001f * 0.001f;

	std::vector<Node> myNodes;
	std::vector<bool> myBorderVertices;
	Mesh myMesh;
};
