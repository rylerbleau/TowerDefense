#include "Scene1.h"
#include "KinematicSeek.h"



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

}

bool Scene1::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);
	
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	
	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);
	
	// Set player image to PacMan
	SDL_Surface* image;
	SDL_Texture* texture;
	image = IMG_Load("Pacman.png");
	texture = SDL_CreateTextureFromSurface(renderer, image);
	game->getPlayer()->setImage(image);
	game->getPlayer()->setTexture(texture);

	// Set up characters, choose good values for the constructor
	// or use the defaults, like this
	blinky = new Character();
	if (!blinky->OnCreate(this) || !blinky->setTextureWith("Sprites/hero.png") )
	{
		return false;
	}

	characters.push_back(blinky);
	
	/// Map set up begins here
	level = Level("Levels/Level2.txt");
	level.LoadMap(renderer, 12, 11, "Sprites/tilemap.png");

	return true;
	float orientation_ = 0.0f;
	float maxSpeed_ = 5.0f;
	float maxRotation_ = 1.0f;
	Vec3 position_(5.0f, 5.0f, 0.0f);
}

void Scene1::OnDestroy() {}

void Scene1::Update(const float deltaTime) {
	// Calculate and apply any steering for npc's
	for (int i = 0; i < characters.size(); i++) {
		characters[i]->Update(deltaTime, characters, i);
	}
	game->getPlayer()->Update(deltaTime);
}

void Scene1::Render() {
	// reset render colour
	SDL_SetRenderDrawColor(renderer, 210, 180, 140, 0);
	SDL_RenderClear(renderer);
	
	// Draw level and AI characters
	level.drawTiles(renderer, window);
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
	level.levelHandleEvents(event, characters, this);

	
}


