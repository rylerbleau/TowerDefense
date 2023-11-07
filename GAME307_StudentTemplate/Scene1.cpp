#include "Scene1.h"
#include "KinematicSeek.h"
#include "Graph.h"
#include "KinematicArrive.h"
#include "FollowAPath.h"


Scene1::Scene1(SDL_Window* sdlWindow_, GameManager* game_){
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;

	// create a NPC
	blinky = nullptr;
	
}

Scene1::~Scene1(){
	if (blinky)
	{
		blinky->OnDestroy();
		delete blinky;
	}
	if (!characters.empty()) {
		characters.clear();
	}

	level.clear();

}

bool Scene1::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);
	
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	
	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);


	/// Map set up begins here
	level = Level("Levels/Level2.txt", this);
	level.LoadMap(12, 11, "Sprites/tilemap.png");
	graph = new Graph();
	graph->OnCreate(level.getNodes());
	for (int i = 0; i < level.getNodes().size(); i++) {
		Node* fromNode = level.getNodes()[i];
		int from = fromNode->GetLabel();
		if (i > 0) {
			int to = level.getNodes()[i - 1]->GetLabel();
			graph->AddWeightedConnection(from, to, 1.0f);
		}
		if (i < level.getNodes().size() - 1) {
			int to = level.getNodes()[i + 1]->GetLabel();
			graph->AddWeightedConnection(from, to, 1.0f);
		}
	}

	std::vector<int> nodes = graph->Dijkstra(graph->NumNodes() - 1, 0);
	for (auto& node : nodes) {
		path.push_back(graph->GetNode(node));
	}

	// Set up characters, choose good values for the constructor
	// or use the defaults, like this
	blinky = new Character();
	if (!blinky->OnCreate(this) || !blinky->setTextureWith("Sprites/hero.png") )
	{
		return false;
	}

	characters.push_back(blinky);

	
	

	
	

	return true;
}

void Scene1::OnDestroy() {}

void Scene1::Update(const float deltaTime) {
	// Calculate and apply any steering for npc's
	for (uint32_t i = 0; i < characters.size(); i++) {
		characters[i]->Update(deltaTime, characters, i, path);
	}


	game->getPlayer()->Update(deltaTime);
}

void Scene1::Render() {
	// reset render colour
	SDL_SetRenderDrawColor(renderer, 210, 180, 140, 0);
	SDL_RenderClear(renderer);
	
	// Draw level and AI characters
	level.drawTiles(window, characters);
	for (auto& character : characters) {
		character->render(1.0f);
	}

	// render the player
	game->RenderPlayer(0.10f);
	SDL_RenderPresent(renderer);
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	game->getPlayer()->HandleEvents(event);
	level.levelHandleEvents(event);
}


