#include "stdafx.h"
#include "navmesh.h"
#include <assert.h>
#include <fstream>
#include <string>

#include <queue>
#include <limits>
#include <algorithm>
#include <cmath>

static float Sign(Tga::Vector2f p1, Tga::Vector2f p2, Tga::Vector2f p3)
{
	return (p1.myX - p3.myX) * (p2.myY - p3.myY) - (p2.myX - p3.myX) * (p1.myY - p3.myY);
}

static float Cross(const Tga::Vector2f& u, const Tga::Vector2f& v)
{
	return u.x * v.y - u.y * v.x;
}

// Signed area * 2. Positive means c is to the left of the line a->b.
static float TriArea2(const Tga::Vector2f& a, const Tga::Vector2f& b, const Tga::Vector2f& c)
{
	const float ax = b.x - a.x;
	const float ay = b.y - a.y;
	const float bx = c.x - a.x;
	const float by = c.y - a.y;
	return bx * ay - ax * by;
}

static bool VEqual(const Tga::Vector2f& a, const Tga::Vector2f& b)
{
	const float eps = 0.001f * 0.001f;
	const float dx = b.x - a.x;
	const float dy = b.y - a.y;
	return (dx * dx + dy * dy) < eps;
}

static Tga::Vector2f ClosestPointOnSegment(const Tga::Vector2f& a,
                                           const Tga::Vector2f& b,
                                           const Tga::Vector2f& p)
{
	const float abx = b.x - a.x;
	const float aby = b.y - a.y;
	const float lenSq = abx * abx + aby * aby;

	if (lenSq < 0.0001f)
		return a;

	float t = ((p.x - a.x) * abx + (p.y - a.y) * aby) / lenSq;
	t = std::clamp(t, 0.f, 1.f);

	return {a.x + abx * t, a.y + aby * t};
}

Navmesh::Navmesh()
{
}
Navmesh::~Navmesh()
{
}

void Navmesh::Init(const char* aObjFile)
{
	myMesh = LoadMesh(aObjFile);
	CreateNodes();
	CalculateConnections();
}

void Navmesh::RenderNavmesh(Tga::DebugDrawer& debugDrawer)
{
	for (int nodeIndex = 0; nodeIndex < myMesh.myIndices.size(); nodeIndex += 3)
	{
		Tga::Vector2f nodeCenter(myNodes[nodeIndex / 3].myCenter.myX, myNodes[nodeIndex / 3].myCenter.myY);
		debugDrawer.DrawCircle(nodeCenter, 5.f, Tga::Color(1.f, 0, 0, 1.f));

		int index1 = myMesh.myIndices[nodeIndex + 0];
		int index2 = myMesh.myIndices[nodeIndex + 1];
		int index3 = myMesh.myIndices[nodeIndex + 2];
		Tga::Vector2f from = { myMesh.myVertices[index1].myX , myMesh.myVertices[index1].myY };
		Tga::Vector2f to = { myMesh.myVertices[index2].myX, myMesh.myVertices[index2].myY };
		debugDrawer.DrawLine(from, to);

		from = {myMesh.myVertices[index1].myX, myMesh.myVertices[index1].myY};
		to = { myMesh.myVertices[index3].myX, myMesh.myVertices[index3].myY };
		debugDrawer.DrawLine(from, to);

		from = { myMesh.myVertices[index2].myX, myMesh.myVertices[index2].myY };
		to = { myMesh.myVertices[index3].myX, myMesh.myVertices[index3].myY };
		debugDrawer.DrawLine(from, to);
	}
}

void Navmesh::RenderConnections(Tga::DebugDrawer& debugDrawer)
{
	for (int nodeIndex = 0; nodeIndex < (int)myNodes.size(); ++nodeIndex)
	{
		const Tga::Vector2f from = myNodes[nodeIndex].myCenter;

		for (const Portal& portal : myNodes[nodeIndex].myPortals)
		{
			if (portal.myNeighbour == -1) continue;

			const Tga::Vector2f to = myNodes[portal.myNeighbour].myCenter;
			debugDrawer.DrawLine(from, to, Tga::Color(0.3f, 0.3f, 0.f, 0.5f));
		}
	}
}

const std::vector<Node>& Navmesh::GetNodes() const
{
	return myNodes;
}

int Navmesh::GetNodeIndexFromPoint(const Tga::Vector2f aPoint) const
{
	for (int i = 0; i < (int)myNodes.size(); ++i)
	{
		Tga::Vector2f a = myMesh.myVertices[myNodes[i].myIndices[0]];
		Tga::Vector2f b = myMesh.myVertices[myNodes[i].myIndices[1]];
		Tga::Vector2f c = myMesh.myVertices[myNodes[i].myIndices[2]];

		float d1 = Sign(aPoint, a, b);
		float d2 = Sign(aPoint, b, c);
		float d3 = Sign(aPoint, c, a);

		bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
		bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

		if (!(hasNeg && hasPos)) 
			return i;
	}
	return -1;
}

void Navmesh::CreateNodes()
{
	for (int nodeIndex = 0; nodeIndex < (int)myMesh.myIndices.size(); nodeIndex += 3)
	{
		Node newNode;
		newNode.myIndices[0] = myMesh.myIndices[nodeIndex + 0];
		newNode.myIndices[1] = myMesh.myIndices[nodeIndex + 1];
		newNode.myIndices[2] = myMesh.myIndices[nodeIndex + 2];
		newNode.myCenter = (myMesh.myVertices[newNode.myIndices[0]]
			+ myMesh.myVertices[newNode.myIndices[1]]
			+ myMesh.myVertices[newNode.myIndices[2]]) / 3.f;
		myNodes.push_back(newNode);
	}
}

void Navmesh::CalculateConnections()
{
	for (int i = 0; i < (int)myNodes.size(); ++i)
	{
		for (int edge = 0; edge < 3; ++edge)
		{
			const int a = myNodes[i].myIndices[edge];
			const int b = myNodes[i].myIndices[(edge + 1) % 3];

			myNodes[i].myPortals[edge].myVertexA = a;
			myNodes[i].myPortals[edge].myVertexB = b;
			myNodes[i].myPortals[edge].myNeighbour = -1;

			for (int j = 0; j < (int)myNodes.size(); ++j)
			{
				if (i == j) continue;

				bool hasA = false;
				bool hasB = false;
				for (int v = 0; v < 3; ++v)
				{
					if (myNodes[j].myIndices[v] == a) hasA = true;
					if (myNodes[j].myIndices[v] == b) hasB = true;
				}

				if (hasA && hasB)
				{
					myNodes[i].myPortals[edge].myNeighbour = j;
					break;
				}
			}
		}
	}
}

Mesh LoadMesh(const char* aObjFile)
{
	Mesh mesh;
	std::fstream meshLoader;
	meshLoader.open(aObjFile, std::ios_base::in);
	assert(meshLoader.is_open() == true && "Error! Couldn't open file!");
	int sizeMultiplier = 700;

	while (meshLoader.eof() == false)
	{
		std::string reader;
		meshLoader >> reader;
		if (reader == "v")
		{
			float x, y, z;
			meshLoader >> x;
			meshLoader >> y;
			meshLoader >> z;
			x *= sizeMultiplier;
			y *= sizeMultiplier;
			z *= sizeMultiplier;
			mesh.myVertices.push_back({ x, z });
		}
		else if (reader == "f")
		{
			int index1, index2, index3;
			int unusedUVIndex;
			char divider;
			meshLoader >> index1;
			meshLoader >> divider;
			meshLoader >> unusedUVIndex;

			meshLoader >> index2;
			meshLoader >> divider;
			meshLoader >> unusedUVIndex;

			meshLoader >> index3;
			meshLoader >> divider;
			meshLoader >> unusedUVIndex;

			mesh.myIndices.push_back(index1 - 1);//Just as LUA, it thinks it should start with index 1 
			mesh.myIndices.push_back(index2 - 1);//Just as LUA, it thinks it should start with index 1 
			mesh.myIndices.push_back(index3 - 1);//Just as LUA, it thinks it should start with index 1 
		}
	}

	return mesh;
}

Tga::Vector2f Navmesh::GetClosestPointOnMesh(const Tga::Vector2f& aPoint, int& outNodeIndex) const
{
	outNodeIndex = GetNodeIndexFromPoint(aPoint);
	if (outNodeIndex != -1)
		return aPoint;

	float bestDistSq = std::numeric_limits<float>::infinity();
	Tga::Vector2f best = aPoint;

	for (int i = 0; i < (int)myNodes.size(); ++i)
	{
		for (int e = 0; e < 3; ++e)
		{
			const Tga::Vector2f a = myMesh.myVertices[myNodes[i].myIndices[e]];
			const Tga::Vector2f b = myMesh.myVertices[myNodes[i].myIndices[(e + 1) % 3]];

			const Tga::Vector2f candidate = ClosestPointOnSegment(a, b, aPoint);
			const float dx = candidate.x - aPoint.x;
			const float dy = candidate.y - aPoint.y;
			const float distSq = dx * dx + dy * dy;

			if (distSq < bestDistSq)
			{
				bestDistSq = distSq;
				best = candidate;
				outNodeIndex = i;
			}
		}
	}

	return best;
}

std::vector<int> Navmesh::FindPath(int aStartNode, int aEndNode) const
{
	std::vector<int> path;

	const int total = (int)myNodes.size();
	if (aStartNode < 0 || aEndNode < 0 || aStartNode >= total || aEndNode >= total)
		return path;

	if (aStartNode == aEndNode)
	{
		path.push_back(aStartNode);
		return path;
	}

	auto dist = [&](int a, int b) -> float
	{
		const float dx = myNodes[a].myCenter.x - myNodes[b].myCenter.x;
		const float dy = myNodes[a].myCenter.y - myNodes[b].myCenter.y;
		return std::sqrt(dx * dx + dy * dy);
	};

	struct ANode
	{
		float f;
		int index;
		bool operator>(const ANode& o) const { return f > o.f; }
	};

	std::vector<float> gCost(total, std::numeric_limits<float>::infinity());
	std::vector<int> parent(total, -1);
	std::vector<bool> closed(total, false);
	std::priority_queue<ANode, std::vector<ANode>, std::greater<ANode>> open;

	gCost[aStartNode] = 0.f;
	open.push({dist(aStartNode, aEndNode), aStartNode});

	bool found = false;

	while (!open.empty())
	{
		const int idx = open.top().index;
		open.pop();

		if (closed[idx]) continue;
		closed[idx] = true;

		if (idx == aEndNode)
		{
			found = true;
			break;
		}

		for (const Portal& portal : myNodes[idx].myPortals)
		{
			const int n = portal.myNeighbour;
			if (n == -1) continue;
			if (closed[n]) continue;

			const float tentativeG = gCost[idx] + dist(idx, n);
			if (tentativeG < gCost[n])
			{
				gCost[n] = tentativeG;
				parent[n] = idx;
				open.push({tentativeG + dist(n, aEndNode), n});
			}
		}
	}

	if (!found)
		return path;

	for (int i = aEndNode; i != -1; i = parent[i])
		path.push_back(i);

	std::reverse(path.begin(), path.end());
	return path;
}

std::vector<Tga::Vector2f> Navmesh::SmoothPath(const std::vector<int>& aNodePath,
                                               const Tga::Vector2f& aStart,
                                               const Tga::Vector2f& aEnd) const
{
	std::vector<Tga::Vector2f> result;

	if (aNodePath.empty())
	{
		result.push_back(aStart);
		result.push_back(aEnd);
		return result;
	}
	struct PortalEdge
	{
		Tga::Vector2f left;
		Tga::Vector2f right;
	};
	std::vector<PortalEdge> portals;
	portals.push_back({aStart, aStart});

	for (size_t i = 0; i + 1 < aNodePath.size(); ++i)
	{
		const int current = aNodePath[i];
		const int next = aNodePath[i + 1];

		const Portal* shared = nullptr;
		for (const Portal& p : myNodes[current].myPortals)
		{
			if (p.myNeighbour == next)
			{
				shared = &p;
				break;
			}
		}
		if (shared == nullptr) continue;

		const Tga::Vector2f vA = myMesh.myVertices[shared->myVertexA];
		const Tga::Vector2f vB = myMesh.myVertices[shared->myVertexB];

		const Tga::Vector2f origin = myNodes[current].myCenter;
		const Tga::Vector2f dir = myNodes[next].myCenter - origin;

		if (Cross(dir, vA - origin) > 0.f)
			portals.push_back({vA, vB});
		else
			portals.push_back({vB, vA});
	}

	portals.push_back({aEnd, aEnd});

	Tga::Vector2f portalApex = portals[0].left;
	Tga::Vector2f portalLeft = portals[0].left;
	Tga::Vector2f portalRight = portals[0].right;

	size_t apexIndex = 0;
	size_t leftIndex = 0;
	size_t rightIndex = 0;

	result.push_back(portalApex);

	for (size_t i = 1; i < portals.size(); ++i)
	{
		const Tga::Vector2f left = portals[i].left;
		const Tga::Vector2f right = portals[i].right;

		if (TriArea2(portalApex, portalRight, right) <= 0.f)
		{
			if (VEqual(portalApex, portalRight) || TriArea2(portalApex, portalLeft, right) > 0.f)
			{
				portalRight = right;
				rightIndex = i;
			}
			else
			{
				result.push_back(portalLeft);
				portalApex = portalLeft;
				apexIndex = leftIndex;

				portalLeft = portalApex;
				portalRight = portalApex;
				leftIndex = apexIndex;
				rightIndex = apexIndex;

				i = apexIndex;
				continue;
			}
		}

		if (TriArea2(portalApex, portalLeft, left) >= 0.f)
		{
			if (VEqual(portalApex, portalLeft) || TriArea2(portalApex, portalRight, left) < 0.f)
			{
				portalLeft = left;
				leftIndex = i;
			}
			else
			{
				result.push_back(portalRight);
				portalApex = portalRight;
				apexIndex = rightIndex;

				portalLeft = portalApex;
				portalRight = portalApex;
				leftIndex = apexIndex;
				rightIndex = apexIndex;

				i = apexIndex;
				continue;
			}
		}
	}

	if (result.empty() || !VEqual(result.back(), aEnd))
		result.push_back(aEnd);

	return result;
}
