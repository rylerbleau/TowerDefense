#include "Scene3.h"
#include "KinematicSeek.h"
#include "KinematicSeperation.h"

#include "imgui.h"



Scene3::Scene3(SDL_Window* sdlWindow_, GameManager* game_) {
	window = sdlWindow_;
	game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;

	// create a NPC
	blinky = nullptr;
	myNPC = nullptr;
}

Scene3::~Scene3() {
	for (StaticBody* body : NPCs) {
		delete(body);
	}

}

bool Scene3::OnCreate() {

	NPCs.push_back(new StaticBody(Vec3(5.0f, 5.0f, 0.0f), 0.0f, 5.0f, 1.0f));
	NPCs.push_back(new StaticBody(Vec3(10.0f, 10.0f, 0.0f), 0.0f, 5.0f, 1.0f));
	NPCs.push_back(new StaticBody(Vec3(10.0f, 5.0f, 0.0f), 0.0f, 5.0f, 1.0f));
	NPCs.push_back(new StaticBody(Vec3(5.0f, 10.0f, 0.0f), 0.0f, 5.0f, 1.0f));

	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	// Set player image to PacMan

	SDL_Surface* image;
	SDL_Texture* texture;

	image = IMG_Load("pacman.png");
	texture = SDL_CreateTextureFromSurface(renderer, image);
	game->getPlayer()->setImage(image);
	game->getPlayer()->setTexture(texture);

	// Set up characters, choose good values for the constructor
	// or use the defaults, like this
	blinky = new Character();
	if (!blinky->OnCreate(this) || !blinky->setTextureWith("Blinky.png"))
	{
		return false;
	}

	float orientation_ = 0.0f;
	float maxSpeed_ = 5.0f;
	float maxRotation_ = 1.0f;
	Vec3 position_(5.0f, 5.0f, 0.0f);
	//myNPC = new StaticBody(position_, orientation_, maxSpeed_, maxRotation_);

	image = IMG_Load("Clyde.png");
	texture = SDL_CreateTextureFromSurface(renderer, image);

	if (image == nullptr) {
		std::cerr << "Can't open Clyde.png" << endl;
		return false;
	}
	if (texture == nullptr) {
		std::cerr << "Can't create Clyde texture" << endl;
		return false;
	}

	for (StaticBody* body : NPCs) {
		body->setTexture(texture);

	}
	//myNPC->setTexture(texture);
	SDL_FreeSurface(image);


	// end of character set ups

	return true;
}

void Scene3::OnDestroy() {

}

void Scene3::Update(const float deltaTime) {

	HandleTheGUI();
	// Calculate and apply any steering for npc's


	//blinky->Update(deltaTime);

	

	KinematicSeperation* steeringAlgorithm = nullptr;
	KinematicSteeringOutput* steering;

	Body* target;
	target = game->getPlayer();
	
	std::vector<Vec3> positions;

	positions.resize(NPCs.size());

	float threshold = 10.0f;

	for (int i = 0; i < NPCs.size();  i++) {

		steeringAlgorithm = new KinematicSeperation(NPCs[i], NPCs, threshold, i);
		steering = steeringAlgorithm->GetSteering();
	

		NPCs[i]->Update(deltaTime, steering);
		steeringAlgorithm = nullptr;
		steering = nullptr;
	}


	//myNPC->Update(deltaTime, steering);

	game->getPlayer()->Update(deltaTime);


	if (steeringAlgorithm) {
		delete steeringAlgorithm;
	}

	// Update player
}

void Scene3::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	//// render any npc's
	//blinky->render(0.15f);

	//SDL_Rect rect;
	//Vec3 screenCoords;
	//int w, h;

	//screenCoords = projectionMatrix * myNPC->getPos();
	////float scale = 0.15f;
	//SDL_QueryTexture(myNPC->getTexture(), nullptr, nullptr, &w, &h);

	//rect.w = static_cast<int>(w * scale);
	//rect.h = static_cast<int>(h * scale);
	//rect.x = static_cast<int>(screenCoords.x - (0.5f * rect.w));
	//rect.y = static_cast<int>(screenCoords.y - (0.5f * rect.h));

	//float orientation = myNPC->getOrientation();
	//float orientationDeg = orientation * 180.0f / M_PI;
	//SDL_RenderCopyEx(renderer, myNPC->getTexture(), nullptr, &rect, orientationDeg, nullptr, SDL_FLIP_NONE);

	for (StaticBody* body : NPCs) {
		SDL_Rect bodyRect;
		Vec3 screenCoords;
		int w, h;
		screenCoords = projectionMatrix * body->getPos();
		float scale = 0.15f;

		SDL_QueryTexture(body->getTexture(), nullptr, nullptr, &w, &h);

		bodyRect.w = static_cast<int>(w * scale);
		bodyRect.h = static_cast<int>(h * scale);
		bodyRect.x = static_cast<int>(screenCoords.x - (0.5f * bodyRect.w));
		bodyRect.y = static_cast<int>(screenCoords.y - (0.5f * bodyRect.h));

		float orientation = body->getOrientation();
		float orientationDeg = orientation * 180.0f / M_PI;
		SDL_RenderCopyEx(renderer, body->getTexture(), nullptr, &bodyRect, orientationDeg, nullptr, SDL_FLIP_NONE);


	}

	// render the player
	game->RenderPlayer(0.10f);

	//SDL_RenderPresent(renderer);
}

void Scene3::HandleEvents(const SDL_Event& event)
{
	// send events to npc's as needed

	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
}

void Scene3::HandleTheGUI() {
	ImGui::Begin("Clickable");
	ImGui::Text("Clickable");
	ImGui::End();


}

