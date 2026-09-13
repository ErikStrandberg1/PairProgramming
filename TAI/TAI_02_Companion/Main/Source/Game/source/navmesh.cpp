#include "stdafx.h"
#include "navmesh.h"
#include <assert.h>
#include <fstream>
#include <string>
#include <assert.h>

Navmesh::Navmesh()
{
}
Navmesh::~Navmesh()
{
}

static float Sign(Tga::Vector2f p1, Tga::Vector2f p2, Tga::Vector2f p3)
{
	return (p1.myX - p3.myX) * (p2.myY - p3.myY) - (p2.myX - p3.myX) * (p1.myY - p3.myY);
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


		from = { myMesh.myVertices[index1].myX, myMesh.myVertices[index1].myY };
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
		Tga::Vector2f from = { myNodes[nodeIndex].myCenter.myX, myNodes[nodeIndex].myCenter.myY };
		for (int c = 0; c < (int)myNodes[nodeIndex].myConnections.size(); ++c)
		{
			int connected = myNodes[nodeIndex].myConnections[c];
			if (connected == -1) continue;

			Tga::Vector2f to = { myNodes[connected].myCenter.myX, myNodes[connected].myCenter.myY };
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
	for (int nodeIndex = 0; nodeIndex < myMesh.myIndices.size(); nodeIndex += 3)
	{
		Node newNode;
		newNode.myIndices[0] = myMesh.myIndices[nodeIndex + 0];
		newNode.myIndices[1] = myMesh.myIndices[nodeIndex + 1];
		newNode.myIndices[2] = myMesh.myIndices[nodeIndex + 2];
		newNode.myCenter = (myMesh.myVertices[newNode.myIndices[0]] + myMesh.myVertices[newNode.myIndices[1]] + myMesh.myVertices[newNode.myIndices[2]]) / 3.f;
		newNode.myConnections = { -1, -1, -1 };   
		myNodes.push_back(newNode);
	}
}

void Navmesh::CalculateConnections()
{
	for (int i = 0; i < (int)myNodes.size(); ++i)
	{
		int slot = 0;
		for (int j = 0; j < (int)myNodes.size(); ++j)
		{
			if (i == j) continue;

			int shared = 0;
			for (int a = 0; a < 3; ++a)
			{
				for (int b = 0; b < 3; ++b){
					if (myNodes[i].myIndices[a] == myNodes[j].myIndices[b])
					{
						++shared;
					}
				}
			}

			if (shared >= 2 && slot < 3)
			{
				myNodes[i].myConnections[slot++] = j;
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
