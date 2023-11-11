#include "Scene2.h"

Scene2::Scene2(SDL_Window* sdlWindow, GameManager* game_)
{
	window = sdlWindow;
	game = game_;
	renderer = SDL_GetRenderer(window);
}

Scene2::~Scene2()
{
}

bool Scene2::OnCreate()
{
    printf("hello scene4\n");

    int count = 15; // Increase the number of nodes to 15
    sceneNodes.resize(count);

    for (int i = 0; i < count; i++) {
        sceneNodes[i] = new Node(i);
    }

    graph = new Graph();
    if (!graph->OnCreate(sceneNodes)) {
        std::cerr << "couldn't create graph" << std::endl;
        return false;
    }

    // Complex graph with 15 nodes and various weighted edges
    // This is a non-exhaustive setup; edges are selectively chosen for complexity
    graph->AddWeightedConnection(0, 1, 4.5f);
    graph->AddWeightedConnection(0, 4, 2.0f);
    graph->AddWeightedConnection(1, 2, 1.5f);
    graph->AddWeightedConnection(1, 5, 3.2f);
    graph->AddWeightedConnection(2, 3, 2.2f);
    graph->AddWeightedConnection(2, 6, 4.1f);
    graph->AddWeightedConnection(3, 7, 1.0f);
    graph->AddWeightedConnection(4, 5, 2.7f);
    graph->AddWeightedConnection(5, 8, 3.3f);
    graph->AddWeightedConnection(5, 6, 2.0f);
    graph->AddWeightedConnection(6, 7, 1.5f);
    graph->AddWeightedConnection(6, 10, 3.8f);
    graph->AddWeightedConnection(7, 11, 2.6f);
    graph->AddWeightedConnection(8, 12, 4.0f);
    graph->AddWeightedConnection(9, 13, 2.1f);
    graph->AddWeightedConnection(10, 0, 1.9f);
    graph->AddWeightedConnection(10, 11, 2.3f);
    graph->AddWeightedConnection(11, 0, 3.4f);
    graph->AddWeightedConnection(12, 13, 1.1f);
    graph->AddWeightedConnection(13, 14, 2.8f);

    std::vector<int> path = graph->Dijkstra(0, 14);

    for (auto& node : path) {
        std::cout << node << std::endl;
    }


	return true;
}

void Scene2::OnDestroy()
{
	// cleanup mem
}

void Scene2::Update(const float time)
{
}

void Scene2::Render()
{
}

void Scene2::HandleEvents(const SDL_Event& event)
{
}
