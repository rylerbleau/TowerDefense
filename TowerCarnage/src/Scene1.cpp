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
	level = new Level("assets/levels/Level2.txt", this);
	level->loadMap(12, 11, "assets/sprites/tilemap.png");

	/// Creating graph and connecting nodes
	graph = new Graph();
	auto walkableNodes = level->getWalkableTileNodes();

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

	endNode = graph->GetNode(graph->NumNodes() - 1);

	return true;
}

void Scene1::OnDestroy() {
	if (!characters.empty()) {
		for (auto& character : characters) {
			character->OnDestroy();
		}
	}
	delete graph;
	level->clear();
	delete level;
}

void Scene1::Update(const float deltaTime) {
	// Calculate and apply any steering for npc's
	for (uint32_t i = 0; i < characters.size(); i++) {
		characters[i]->Update(deltaTime, characters, i);
	}
	for (const auto& turret : turrets) {
		turret->Update(deltaTime, characters, turrets);
		//turret->GetTarget(characters);

	}
	//game->getPlayer()->Update(deltaTime);
}

void Scene1::Render() {
	// reset render colour
	SDL_SetRenderDrawColor(renderer, 210, 180, 140, 0);
	SDL_RenderClear(renderer);
	
	// Draw level and AI characters
	level->drawTiles();

	for (auto& character : characters) {
		character->render();
	}

	for (auto& t : turrets) {
		if (t->HasTarget()) {
			t->RenderBullet();
		}
	}

	// render the player

	SDL_RenderPresent(renderer);
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	level->levelHandleEvents(event);

	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT) {
			createNewCharacter();
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			endNode = findNode();
			for (auto& character : characters) {
				character->updatePath(endNode);
			}
		}
		if (event.button.button == SDL_BUTTON_MIDDLE) {
			placeTurret();
		}
		break;
	default:
		break;
	}
}

void Scene1::createNewCharacter() {
	for (const auto& tile : level->getTiles()) {
		if (level->canPlaceEntity() && level->isMouseOverTile(tile)) {

			float posX = static_cast<float>((tile->destCoords.x + tile->destCoords.w) * getxAxis() / game->getWindowWidth());
			float posY = getyAxis() - (static_cast<float>((tile->destCoords.y + 0.5 * tile->destCoords.h) * getyAxis() / game->getWindowHeight()));
			Vec3 position = { posX, posY, 0.0f };

			Character* character = new Character();
			character->OnCreate(this, graph, position);
			character->updatePath(endNode);
			character->setTextureWith("assets/sprites/hero.png");
			characters.push_back(character);
		}
	}
}

void Scene1::placeTurret()
{
	// have mouse x and y from mouse click
	// get tile position based on mouse pos
	// check if placement is legal

	// place turret
	for (const auto& tile : level->getTiles()) {
		if (level->canPlaceEntity() && level->isMouseOverTile(tile)) {

			Vec3 position = {
							static_cast<float>((tile->destCoords.x + tile->destCoords.w) * getxAxis()) / game->getWindowWidth(),
							getyAxis() - (static_cast<float>((tile->destCoords.y + 0.5 * tile->destCoords.h) * getyAxis()) / game->getWindowHeight()),
							0.0f
			};

			Turret* turret = new Turret("assets/sprites/tiles_packed.png", Vec2(6, 7), this, position);
			SpriteSheet::QuerySpriteSheet(12, 10, turret->m_turretTexture);
			SDL_Rect turretUV = SpriteSheet::GetUVTile(turret->uvCoords.x, turret->uvCoords.y);
			turret->turretUV = turretUV;

			turrets.push_back(turret);
			tile->tileTexture = turret->m_turretTexture;
			tile->letter = 'M';
			tile->uvCoords = turretUV;
			placeActor = false;
		}
	}

}

Node* Scene1::findNode() {
	Node* node = level->getTileNodeUnderMouse();
	if (node) return node;
	return endNode;
}
