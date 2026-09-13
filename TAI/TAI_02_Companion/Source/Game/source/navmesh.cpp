#include "stdafx.h"
#include "navmesh.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <limits>
#include <queue>
#include <string>

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
	MarkBorderVertices();
}

void Navmesh::RenderNavmesh(Tga::DebugDrawer& debugDrawer)
{
	for (int nodeIndex = 0; nodeIndex < (int)myMesh.myIndices.size(); nodeIndex += 3)
	{
		const Tga::Vector2f& nodeCenter = myNodes[nodeIndex / 3].myCenter;
		debugDrawer.DrawCircle(nodeCenter, 5.f, Tga::Color(1.f, 0.f, 0.f, 1.f));

		const int index1 = myMesh.myIndices[nodeIndex + 0];
		const int index2 = myMesh.myIndices[nodeIndex + 1];
		const int index3 = myMesh.myIndices[nodeIndex + 2];

		debugDrawer.DrawLine(myMesh.myVertices[index1], myMesh.myVertices[index2]);
		debugDrawer.DrawLine(myMesh.myVertices[index1], myMesh.myVertices[index3]);
		debugDrawer.DrawLine(myMesh.myVertices[index2], myMesh.myVertices[index3]);
	}
	RenderConnections(debugDrawer);
}

void Navmesh::RenderConnections(Tga::DebugDrawer& debugDrawer)
{
	for (int nodeIndex = 0; nodeIndex < (int)myNodes.size(); ++nodeIndex)
	{
		const Tga::Vector2f& from = myNodes[nodeIndex].myCenter;

		for (const Portal& portal : myNodes[nodeIndex].myPortals)
		{
			if (portal.myNeighbour == -1) continue;

			const Tga::Vector2f& to = myNodes[portal.myNeighbour].myCenter;
			debugDrawer.DrawLine(from, to, Tga::Color(0.3f, 0.3f, 0.f, 0.5f));
		}
	}
}

const std::vector<Node>& Navmesh::GetNodes() const
{
	return myNodes;
}

bool Navmesh::IsBorderVertex(int aVertexIndex) const
{
	if (aVertexIndex < 0 || aVertexIndex >= (int)myBorderVertices.size())
	{
		return false;
	}

	return myBorderVertices[aVertexIndex];
}

int Navmesh::GetNodeIndexFromPoint(const Tga::Vector2f aPoint) const
{
	for (int i = 0; i < (int)myNodes.size(); ++i)
	{
		const Tga::Vector2f a = myMesh.myVertices[myNodes[i].myIndices[0]];
		const Tga::Vector2f b = myMesh.myVertices[myNodes[i].myIndices[1]];
		const Tga::Vector2f c = myMesh.myVertices[myNodes[i].myIndices[2]];

		const float d1 = (aPoint - b).Cross(a - b);
		const float d2 = (aPoint - c).Cross(b - c);
		const float d3 = (aPoint - a).Cross(c - a);

		const bool hasNegative = (d1 < 0.f) || (d2 < 0.f) || (d3 < 0.f);
		const bool hasPositive = (d1 > 0.f) || (d2 > 0.f) || (d3 > 0.f);

		if (!(hasNegative && hasPositive))
		{
			return i;
		}
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
					if (myNodes[j].myIndices[v] == a)
					{
						hasA = true;
					}
					if (myNodes[j].myIndices[v] == b)
					{
						hasB = true;
					}
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

void Navmesh::MarkBorderVertices()
{
	myBorderVertices.assign(myMesh.myVertices.size(), false);

	for (const Node& node : myNodes)
	{
		for (const Portal& portal : node.myPortals)
		{
			if (portal.myNeighbour != -1)
			{
				continue;
			}
			myBorderVertices[portal.myVertexA] = true;
			myBorderVertices[portal.myVertexB] = true;
		}
	}
}

Tga::Vector2f Navmesh::ClosestPointOnSegment(const Tga::Vector2f& aSegmentStart,
                                             const Tga::Vector2f& aSegmentEnd,
                                             const Tga::Vector2f& aTarget) const
{
	const Tga::Vector2f segment = aSegmentEnd - aSegmentStart;
	const float lengthSqr = segment.LengthSqr();

	if (lengthSqr < 0.0001f)
	{
		return aSegmentStart;
	}

	const float t = std::clamp((aTarget - aSegmentStart).Dot(segment) / lengthSqr, 0.f, 1.f);
	return aSegmentStart + segment * t;
}

void Navmesh::InsetPortal(Tga::Vector2f& aLeft, Tga::Vector2f& aRight, int aLeftVertex, int aRightVertex,
                          float aOffset) const
{
	if (aOffset <= 0.f)
	{
		return;
	}

	float leftInset = IsBorderVertex(aLeftVertex) ? aOffset : 0.f;
	float rightInset = IsBorderVertex(aRightVertex) ? aOffset : 0.f;

	const float totalInset = leftInset + rightInset;
	if (totalInset <= 0.f)
	{
		return;
	}

	const Tga::Vector2f delta = aRight - aLeft;
	const float length = delta.Length();
	if (length < 0.0001f)
	{
		return;
	}

	// Too narrow, collapse both ends onto the same point instead of
	// letting them cross over.
	if (totalInset > length)
	{
		const float scale = length / totalInset;
		leftInset *= scale;
		rightInset *= scale;
	}

	const Tga::Vector2f direction = delta.GetNormalized();
	aLeft = aLeft + direction * leftInset;
	aRight = aRight - direction * rightInset;
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
			mesh.myVertices.push_back({x, z});
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

			mesh.myIndices.push_back(index1 - 1);
			mesh.myIndices.push_back(index2 - 1);
			mesh.myIndices.push_back(index3 - 1);
		}
	}

	return mesh;
}

Tga::Vector2f Navmesh::GetClosestPointOnMesh(const Tga::Vector2f& aPoint, int& outNodeIndex) const
{
	outNodeIndex = GetNodeIndexFromPoint(aPoint);
	if (outNodeIndex != -1)
	{
		return aPoint;
	}

	float bestDistanceSqr = std::numeric_limits<float>::infinity();
	Tga::Vector2f best = aPoint;

	for (int i = 0; i < (int)myNodes.size(); ++i)
	{
		for (int edge = 0; edge < 3; ++edge)
		{
			const Tga::Vector2f a = myMesh.myVertices[myNodes[i].myIndices[edge]];
			const Tga::Vector2f b = myMesh.myVertices[myNodes[i].myIndices[(edge + 1) % 3]];

			const Tga::Vector2f candidate = ClosestPointOnSegment(a, b, aPoint);
			const float distanceSqr = (candidate - aPoint).LengthSqr();

			if (distanceSqr < bestDistanceSqr)
			{
				bestDistanceSqr = distanceSqr;
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
	{
		return path;
	}

	if (aStartNode == aEndNode)
	{
		path.push_back(aStartNode);
		return path;
	}

	std::vector<float> gCost(total, std::numeric_limits<float>::infinity());
	std::vector<int> parent(total, -1);
	std::vector<bool> closed(total, false);
	std::priority_queue<OpenNode, std::vector<OpenNode>, std::greater<OpenNode>> open;

	gCost[aStartNode] = 0.f;
	open.push({GetNodeDistance(aStartNode, aEndNode), aStartNode});

	bool found = false;

	while (!open.empty())
	{
		const int current = open.top().myIndex;
		open.pop();

		if (closed[current]) continue;
		closed[current] = true;

		if (current == aEndNode)
		{
			found = true;
			break;
		}

		for (const Portal& portal : myNodes[current].myPortals)
		{
			const int neighbour = portal.myNeighbour;
			if (neighbour == -1) continue;
			if (closed[neighbour]) continue;

			const float tentativeG = gCost[current] + GetNodeDistance(current, neighbour);
			if (tentativeG < gCost[neighbour])
			{
				gCost[neighbour] = tentativeG;
				parent[neighbour] = current;
				open.push({tentativeG + GetNodeDistance(neighbour, aEndNode), neighbour});
			}
		}
	}

	if (!found)
	{
		return path;
	}

	for (int i = aEndNode; i != -1; i = parent[i])
	{
		path.push_back(i);
	}

	std::reverse(path.begin(), path.end());
	return path;
}

std::vector<Tga::Vector2f> Navmesh::SmoothPath(const std::vector<int>& aNodePath, const Tga::Vector2f& aStart,
                                               const Tga::Vector2f& aEnd, float aAgentRadius) const
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
		Tga::Vector2f myLeft;
		Tga::Vector2f myRight;
	};

	std::vector<PortalEdge> portals;
	portals.push_back({aStart, aStart});

	for (size_t i = 0; i + 1 < aNodePath.size(); ++i)
	{
		const int current = aNodePath[i];
		const int next = aNodePath[i + 1];

		const Portal* shared = nullptr;
		for (const Portal& portal : myNodes[current].myPortals)
		{
			if (portal.myNeighbour == next)
			{
				shared = &portal;
				break;
			}
		}
		if (shared == nullptr) continue;

		const Tga::Vector2f vertexA = myMesh.myVertices[shared->myVertexA];
		const Tga::Vector2f vertexB = myMesh.myVertices[shared->myVertexB];

		const Tga::Vector2f origin = myNodes[current].myCenter;
		const Tga::Vector2f direction = myNodes[next].myCenter - origin;

		PortalEdge edge;
		int leftVertex = 0;
		int rightVertex = 0;

		if (direction.Cross(vertexA - origin) > 0.f)
		{
			edge = {vertexA, vertexB};
			leftVertex = shared->myVertexA;
			rightVertex = shared->myVertexB;
		}
		else
		{
			edge = {vertexB, vertexA};
			leftVertex = shared->myVertexB;
			rightVertex = shared->myVertexA;
		}

		InsetPortal(edge.myLeft, edge.myRight, leftVertex, rightVertex, aAgentRadius);
		portals.push_back(edge);
	}

	portals.push_back({aEnd, aEnd});

	Tga::Vector2f portalApex = portals[0].myLeft;
	Tga::Vector2f portalLeft = portals[0].myLeft;
	Tga::Vector2f portalRight = portals[0].myRight;

	int apexIndex = 0;
	int leftIndex = 0;
	int rightIndex = 0;

	result.push_back(portalApex);

	for (int i = 1; i < portals.size(); ++i)
	{
		const Tga::Vector2f left = portals[i].myLeft;
		const Tga::Vector2f right = portals[i].myRight;

		if (SignedArea2(portalApex, portalRight, right) <= 0.f)
		{
			if (IsSamePoint(portalApex, portalRight) || SignedArea2(portalApex, portalLeft, right) > 0.f)
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

		if (SignedArea2(portalApex, portalLeft, left) >= 0.f)
		{
			if (IsSamePoint(portalApex, portalLeft) || SignedArea2(portalApex, portalRight, left) < 0.f)
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

	if (result.empty() || !IsSamePoint(result.back(), aEnd))
	{
		result.push_back(aEnd);
	}

	return result;
}
