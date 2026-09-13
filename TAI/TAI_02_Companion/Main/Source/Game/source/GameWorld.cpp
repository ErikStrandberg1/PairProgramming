#include "stdafx.h"

#include "GameWorld.h"
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include "tge\input\InputManager.h"
#include "navmesh.h"

#include <tge/graphics/dx11.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/DebugDrawer.h>
#include <tge/engine.h>
#include "UpdateContext.h"

#include <queue>
#include <cmath>
#include <functional>
#include <limits>
#include <algorithm>

GameWorld::GameWorld()
{
    myNavmesh = nullptr;
}

GameWorld::~GameWorld()
{
    delete myNavmesh;
}

void GameWorld::Init()
{
    myNavmesh = new Navmesh();
    myNavmesh->Init("navmesh/navmesh.obj");

    auto player = std::make_unique<Actor>();
    player->Init(
        "../data/sprites/hacker.png",
        0.2f,
        myControllerFactory.CreateController(AI::eControllerType::ePlayer),
        { 0.f, 0.5f }
    );
    myPlayer = player.get();
    myPlayer->SetPosition(ConvertToPos(myNavmesh->GetNodes()[0].myCenter));
    myTargetMousePos = myPlayer->GetPosition();

    myActors.push_back(std::move(player));

    auto enemy1 = std::make_unique<Actor>();
    enemy1->Init(
        "../data/sprites/killerRobo2.png",
        0.14f,
        myControllerFactory.CreateController(AI::eControllerType::eEvent),
        { .7f, .7f }
    );
    myEnemy = enemy1.get();
    enemy1.get()->SetPosition(ConvertToPos(myNavmesh->GetNodes()[10].myCenter));

    myActors.push_back(std::move(enemy1));

}

void GameWorld::Update(const UpdateContext& context)
{
    //auto aTimeDelta = context.myDeltaTime;
    auto anInput = context.myInputManager;

    bool mouseDown = anInput->IsKeyHeld(VK_LBUTTON);
    bool justClicked = mouseDown && !myMouseWasDown;  
    myMouseWasDown = mouseDown;

    if (justClicked)
    {
        const auto& engine = *Tga::Engine::GetInstance();

        myTargetMousePos.x = static_cast<float>(anInput->GetMousePosition().x);
        // flip Y
        myTargetMousePos.y = engine.GetRenderSize().y - static_cast<float>(anInput->GetMousePosition().y);

        int clickedNode = myNavmesh->GetNodeIndexFromPoint(myTargetMousePos);
        if (clickedNode == -1)
            return; // clicked outside the mesh

        if (myStartNode == -1)
        {
            myStartNode = clickedNode;
            myStartPosition = myTargetMousePos;
        }
        else if (myEndNode == -1)
        {
            if (clickedNode != myStartNode)
            {
                myEndNode = clickedNode;
                myEndPosition = myTargetMousePos;
                myPath = FindPath(myStartNode, myEndNode);
            }
        }
        else
        {
            myStartNode = clickedNode;
            myStartPosition = myTargetMousePos;
            myEndNode = -1;
            myEndPosition = { 0.f, 0.f };
            myPath.clear();
        }
    }

    //AI
    for (auto& actor : myActors)
    {
        actor->Update(context);
    }

    if(justClicked)
    {
        Node closestNode = myNavmesh->GetNodes()[0];
        for (const auto& node : myNavmesh->GetNodes())
        {
            if ((myTargetMousePos - node.myCenter).Length() <=
                (myTargetMousePos - closestNode.myCenter).Length())
            {
                closestNode = node;
            }
        }

        //std::cout << "closest node: " << closestNode.myCenter << "\n";
        const auto& engine = *Tga::Engine::GetInstance();

        Tga::Vector2f enemyPos = myEnemy->GetPosition();

        Tga::Vector2f navmeshPos =
        {
            enemyPos.x * engine.GetRenderSize().x,
            (1.0f - enemyPos.y) * engine.GetRenderSize().y
        };

        int currentNode = myNavmesh->GetNodeIndexFromPoint(navmeshPos);

        //std::cout << "\n currentNode x:" << myNavmesh->GetNodes()[currentNode].myCenter.X << "currentNode y:" << myNavmesh->GetNodes()[currentNode].myCenter.Y << "\n";
        //std::cout << "GetNodeIndex from closest node center: " << myNavmesh->GetNodeIndexFromPoint(closestNode.myCenter) << "\n";
        //std::cout << "Current Node: " << currentNode << "\n";
        if (currentNode == -1)
        {
            currentNode = 10;
        }
        auto path = FindPath(currentNode, myNavmesh->GetNodeIndexFromPoint(closestNode.myCenter));

        std::cout << "Current node: " << currentNode << "\n";
        std::cout << "Target node: "
            << myNavmesh->GetNodeIndexFromPoint(closestNode.myCenter)
            << "\n";

        std::cout << "Path: ";
        for (int node : path)
        {
            std::cout << node << " -> ";
        }
        std::cout << "\n";

        std::cout << path.size();
        for (const auto& node : myNavmesh->GetNodes())
        {
            if ((myTargetMousePos - node.myCenter).Length() <=
                (myTargetMousePos - closestNode.myCenter).Length())
            {
                closestNode = node;
            }
        }

        auto eventManager = AIEventManager::GetInstance();
        AIEvent event{};
        event.type = AIEvent::Type::PlayerMoved;
        event.pathPositions = PathToPositions(path);
        eventManager.SendEvent(event);
    }

}

void GameWorld::Render()
{
    const auto& engine = *Tga::Engine::GetInstance();
    Tga::DebugDrawer& debugDrawer = engine.GetDebugDrawer();


    //Tga::SpriteDrawer& spriteDrawer(engine.GetGraphicsEngine().GetSpriteDrawer());

    //{
    //    Tga::SpriteSharedData sharedData = {};
    //    sharedData.myTexture = myBackgroundTexture;

    //    Tga::Sprite2DInstanceData instanceData = {};
    //    instanceData.myPivot = { 0.0f, 1.0f };
    //    instanceData.myPosition = { 0.0f, 0.0f };
    //    instanceData.mySize = { 1.0f, 1.0f };

    //    spriteDrawer.Draw(sharedData, instanceData);
    //}

    myNavmesh->RenderNavmesh(debugDrawer);
    myNavmesh->RenderConnections(debugDrawer);

    if (!myPath.empty())
    {
        const std::vector<Node>& nodes = myNavmesh->GetNodes();
        for (int i = 0; i + 1 < ( int )myPath.size(); ++i)
        {
            Tga::Vector2f a = nodes[myPath[i]].myCenter;
            Tga::Vector2f b = nodes[myPath[i + 1]].myCenter;
            debugDrawer.DrawLine(a, b, Tga::Color(1.f, 1.f, 0.f, 1.f));
        }
    }

    if (myStartNode != -1)
    {
        debugDrawer.DrawCircle({ myStartPosition.x, myStartPosition.y }, 10.0f, Tga::Color(0, 1, 0, 1));
    }

    if (myEndNode != -1)
    {
        debugDrawer.DrawCircle({ myEndPosition.x, myEndPosition.y }, 20.0f, Tga::Color(0, 1, 1, 1));
    }

    //AI
    Tga::Vector2f resolution = Tga::Vector2f(( float )Tga::DX11::GetResolution().x, ( float )Tga::DX11::GetResolution().y);
    myScreenMin = { 0.f, 0.f };
    myScreenMax = { 1.0f , 1.0f };

    camera.SetOrtographicProjection(myScreenMin.x, myScreenMax.x, myScreenMin.y, myScreenMax.y, -1.0f, 1.0f);
    engine.GetGraphicsEngine().GetGraphicsStateStack().SetCamera(camera);

    for (auto& actor : myActors)
    {
        actor->Render();
    }
}

std::vector<int> GameWorld::FindPath(int startNode, int endNode)
{
    const std::vector<Node>& nodes = myNavmesh->GetNodes();
    int total = (int)nodes.size();

    auto dist = [&](int a, int b) -> float
    {
        Tga::Vector2f pa = nodes[a].myCenter;
        Tga::Vector2f pb = nodes[b].myCenter;
        float dx = pa.x - pb.x;
        float dy = pa.y - pb.y;
        return std::sqrt(dx * dx + dy * dy);
    };

    struct ANode
    {
        float f;
        int index;
        bool operator>(const ANode& o) const { return f > o.f; }
    };

    std::vector<float> gCost(total, std::numeric_limits<float>::infinity());
    std::vector<int>   parent(total, -1);
    std::vector<bool>  closed(total, false);
    std::priority_queue<ANode, std::vector<ANode>, std::greater<ANode>> open;

    gCost[startNode] = 0.f;
    open.push({ dist(startNode, endNode), startNode });

    while (!open.empty())
    {
        ANode current = open.top();
        open.pop();

        int idx = current.index;
        if (closed[idx]) continue;
        closed[idx] = true;

        if (idx == endNode) break;

        for (int n : nodes[idx].myConnections)
        {
            if (n == -1) continue;        // unused connection slot
            if (closed[n]) continue;

            float tentativeG = gCost[idx] + dist(idx, n);
            if (tentativeG < gCost[n])
            {
                gCost[n] = tentativeG;
                parent[n] = idx;
                open.push({ tentativeG + dist(n, endNode), n });
            }
        }
    }

    std::vector<int> path;
    if (parent[endNode] == -1 && endNode != startNode)
    {
        return path;        
    }

    for (int i = endNode; i != -1; i = parent[i])
    {
        path.push_back(i);
    }

    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<Tga::Vector2f> GameWorld::PathToPositions(
    const std::vector<int>& indices)
{
    std::vector<Tga::Vector2f> path;

    for (int index : indices)
    {
        auto center = myNavmesh->GetNodes()[index].myCenter;

        std::cout << "Node " << index
            << " center: "
            << center.x << ", "
            << center.y << "\n";

        path.push_back(ConvertToPos(center));
    }

    return path;
}

Tga::Vector2f ConvertToPos(Tga::Vector2f aPos)
{
    const auto& engine = *Tga::Engine::GetInstance();

    return {
        aPos.x / engine.GetRenderSize().x,
        (aPos.y / engine.GetRenderSize().y)
    };
}
