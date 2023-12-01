#include "Scene1.h"
#include "KinematicSeek.h"
#include "Graph.h"
#include "KinematicArrive.h"
#include "FollowAPath.h"
#include <Vector.h>
#include "imgui.h"

Scene1::Scene1(SDL_Window* sdlWindow_, GameManager* game_) : 
	window(sdlWindow_),
	renderer(SDL_GetRenderer(sdlWindow_)),
	xAxis(25.0f),
	yAxis(15.0f),
	graph(nullptr),
	blinky(nullptr),
	endNode(nullptr), 
	level(nullptr), 
	placeActor(false),
	usingUI(false), 
	paused(false)
{
	game = game_;
}

Scene1::~Scene1(){}

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
	graph->OnCreate(level->getWalkableTileNodes());
	
	int rows = level->getHeight();
	int cols = level->getWidth();
	int tileWidth = level->getTiles()[0]->destCoords.w;
	int tileHeight = level->getTiles()[0]->destCoords.h;

#define ADD_CONNECTION_IF_WALKABLE(x_check, y_check, weight) \
    if (level->getTile(x_check, y_check)->isWalkable) { \
        int to = level->getTile(x_check, y_check)->tileNode->GetLabel(); \
        graph->AddWeightedConnection(from, to, weight); \
    }

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			Node* fromNode = level->getTile(j, i)->tileNode;
			int from = fromNode->GetLabel();

			if (j > 0) {
				ADD_CONNECTION_IF_WALKABLE(j - 1, i, tileWidth);
			}
			if (j < cols - 1) {
				ADD_CONNECTION_IF_WALKABLE(j + 1, i, tileWidth);
			}
			if (i > 0) {
				ADD_CONNECTION_IF_WALKABLE(j, i - 1, tileHeight);
			}
			if (i < rows - 1) {
				ADD_CONNECTION_IF_WALKABLE(j, i + 1, tileHeight);
			}
		}
	}
#undef ADD_CONNECTION_IF_WALKABLE

	endNode = graph->GetNode(graph->NumNodes() - 1);

	level->sortTiles();
	usingUI = false;
	paused = false;

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
	if (paused) {
		return;
	}

	// Calculate and apply any steering for npc's
	for (const auto& turret : turrets) {
		turret->Update(deltaTime, characters, turrets);

	}
	for (uint32_t i = 0; i < characters.size(); i++) {
		characters[i]->Update(deltaTime, characters, i);
		if (characters[i]->isDead()) {

			Turret::ResetTargets(characters, turrets, i);
			delete characters[i];
			characters.erase(characters.begin()+i);
			
		}
	}
}

void Scene1::Render() {

	// reset render colour
	SDL_SetRenderDrawColor(renderer, 210, 180, 140, 0);
	SDL_RenderClear(renderer);
	
	// Draw level and AI characters
	level->drawTiles();

	for (auto& character : characters) {
		character->render();
		character->RenderUI();
		
	}

	for (auto& t : turrets) {
		t->render();
		if (t->HasTarget()) {
			t->RenderBullet();
		}
	}

	HandleTheGUI();
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	if (paused) {
		return;
	}
	level->levelHandleEvents(event);
	if (usingUI) {
		return;
	}
	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_RIGHT) {
			createNewCharacter();
		}
		if (event.button.button == SDL_BUTTON_MIDDLE) {
			endNode = findNode();
			for (auto& character : characters) {
				character->updatePath(endNode);
			}
		}
		if (event.button.button == SDL_BUTTON_LEFT) {
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
		if (level->canPlaceEntity() && level->isMouseOverTile(tile) && tile->letter == 'G') {

			Vec3 position = {
					static_cast<float>((tile->destCoords.x + tile->destCoords.w) * getxAxis()) / game->getWindowWidth(),
					getyAxis() - (static_cast<float>((tile->destCoords.y + 0.5 * tile->destCoords.h) * getyAxis()) / game->getWindowHeight()),
					0.0f
			};

			Turret* turret = new Turret("assets/sprites/tiles_packed.png", this, position, tile->destCoords);
			turrets.push_back(turret);
			tile->letter = 'T';
		}
	}
}

Node* Scene1::findNode() {
	Node* node = level->getTileNodeUnderMouse();
	if (graph->hasConnections(node->GetLabel())) {
		return node;
	}
	return endNode;
}

void Scene1::HandleTheGUI() {
	
	if (ImGui::Begin("Test", NULL, 
		ImGuiWindowFlags_NoResize			  |		ImGuiWindowFlags_NoCollapse   | 
		ImGuiWindowFlags_NoTitleBar			  |		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoScrollWithMouse	  |		ImGuiWindowFlags_NoScrollbar  |
		ImGuiWindowFlags_NoNavInputs		  |     ImGuiHoveredFlags_AnyWindow)) {

		if (ImGui::Button("pause", ImVec2(50, 30))) {
			paused = !paused;
		}
	}
	if (ImGui::IsWindowHovered()) {
		usingUI = true;
	}
	else {
		usingUI = false;
	}

	ImGui::End();
}