#include "Scene1.h"
#include "KinematicSeek.h"
#include "Graph.h"
#include "KinematicArrive.h"
#include "FollowAPath.h"
#include <Vector.h>

Scene1::Scene1(SDL_Window* sdlWindow_, GameManager* game_){
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;
	graph = nullptr;
	blinky = nullptr;
}

Scene1::~Scene1(){

}

bool Scene1::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);
	
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	/// Map and initial character set up 
	level = Level("assets/levels/Level2.txt", this);
	level.loadMap(12, 11, "assets/sprites/tilemap.png");

	/// Creating graph and connecting nodes
	graph = new Graph();
	auto walkableNodes = level.getWalkableTileNodes();
	graph->OnCreate(walkableNodes);
	for (int i = 0; i < walkableNodes.size(); i++) {
		Node* fromNode = walkableNodes[i];
		int from = fromNode->GetLabel();
		if (i > 0) {
			int to = walkableNodes[i - 1]->GetLabel();
			graph->AddWeightedConnection(from, to, 1.0f);
		}
		if (i < walkableNodes.size() - 1) {
			int to = walkableNodes[i + 1]->GetLabel();
			graph->AddWeightedConnection(from, to, 1.0f);
		}
	}

	return true;
}

void Scene1::OnDestroy() {
	/*if (!characters.empty()) {
		for (auto& character : characters) {
			character->OnDestroy();
		}
	}*/
	level.clear();
	delete graph;
}

void Scene1::Update(const float deltaTime) {
	// Calculate and apply any steering for npc's
	for (uint32_t i = 0; i < characters.size(); i++) {
		characters[i]->Update(deltaTime, characters, i);
	}
	//game->getPlayer()->Update(deltaTime);
}

void Scene1::Render() {
	// reset render colour
	SDL_SetRenderDrawColor(renderer, 210, 180, 140, 0);
	SDL_RenderClear(renderer);
	
	// Draw level and AI characters
	level.drawTiles();

	for (auto& character : characters) {
		character->render();
	}

	// render the player
	game->RenderPlayer(0.10f);
	SDL_RenderPresent(renderer);
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	game->getPlayer()->HandleEvents(event);
	level.levelHandleEvents(event);

	switch (event.type) {
	case SDL_MOUSEMOTION:
		mousePosX = event.motion.x;
		mousePosY = event.motion.y;
		break;
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT) {
			createNewCharacter();
		}
	}
}

void Scene1::createNewCharacter() {
	for (const auto& tile : level.getTiles()) {
		if (level.canPlaceCharacter(mousePosX, mousePosY) && level.isMouseOverTile(tile, mousePosX, mousePosY)) {
			float posX = static_cast<float>((tile->destCoords.x + tile->destCoords.w) * getxAxis() / game->getWindowWidth());
			float posY = getyAxis() - (static_cast<float>((tile->destCoords.y + 0.5 * tile->destCoords.h) * getyAxis() / game->getWindowHeight()));
			Vec3 position = { posX, posY, 0.0f };

			Character* character = new Character();
			character->OnCreate(this, graph, position);
			character->setTextureWith("assets/sprites/hero.png");
			characters.push_back(character);
		}
	}
}


