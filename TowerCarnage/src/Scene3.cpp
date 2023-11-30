// Include necessary headers
#include "Scene3.h"
#include "KinematicSeek.h"
#include "KinematicSeperation.h"
#include "imgui.h"

// Scene3 constructor and destructor
Scene3::Scene3(SDL_Window* sdlWindow_, GameManager* game_) {
    window = sdlWindow_;
    game = game_;
    renderer = SDL_GetRenderer(window);
    xAxis = 25.0f;
    yAxis = 15.0f;

    // Initialize pointers
    blinky = nullptr;
    myNPC = nullptr;
}

Scene3::~Scene3() {
    // Clean up resources (e.g., NPCs)
    for (StaticBody* body : NPCs) {
        delete(body);
    }
}

// Scene3 initialization
bool Scene3::OnCreate() {
    // Create NPCs
    NPCs.push_back(new StaticBody(Vec3(5.0f, 5.0f, 0.0f), 0.0f, 2.0f, 1.0f));
    NPCs.push_back(new StaticBody(Vec3(7.0f, 10.0f, 0.0f), 0.0f, 2.0f, 1.0f));
    NPCs.push_back(new StaticBody(Vec3(10.0f, 5.0f, 0.0f), 0.0f, 2.0f, 1.0f));
    NPCs.push_back(new StaticBody(Vec3(5.0f, 10.0f, 0.0f), 0.0f, 2.0f, 1.0f));

    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    Matrix4 ndc = MMath::viewportNDC(w, h);
    Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
    projectionMatrix = ndc * ortho;

    // Turn on the SDL imaging subsystem
    IMG_Init(IMG_INIT_PNG);

    // Set player image to PacMan
    SDL_Surface* image;
    SDL_Texture* texture;

    image = IMG_Load("sprites/Pacman.png");
    texture = SDL_CreateTextureFromSurface(renderer, image);
    game->getPlayer()->setImage(image);
    game->getPlayer()->setTexture(texture);

    // Set up characters
    blinky = new Character();
    if (!blinky->OnCreate(this, {}) || !blinky->setTextureWith("Blinky.png")) {
        return false;
    }

    float orientation_ = 0.0f;
    float maxSpeed_ = 5.0f;
    float maxRotation_ = 1.0f;
    Vec3 position_(5.0f, 5.0f, 0.0f);

    image = IMG_Load("Clyde.png");
    texture = SDL_CreateTextureFromSurface(renderer, image);

    if (image == nullptr) {
        std::cerr << "Can't open Clyde.png" << std::endl;
        return false;
    }
    if (texture == nullptr) {
        std::cerr << "Can't create Clyde texture" << std::endl;
        return false;
    }

    for (StaticBody* body : NPCs) {
        body->setTexture(texture);
    }

    SDL_FreeSurface(image);

    return true;
}

void Scene3::OnDestroy() {
    // Clean up resources on scene destruction
    // ...
}

// Scene3 update function
void Scene3::Update(const float deltaTime) {
    HandleTheGUI();

    KinematicSeperation* steeringAlgorithm = nullptr;
    KinematicSeek* steeringAlgorithm2 = nullptr;
    KinematicSteeringOutput steering;

    Body* target;
    target = game->getPlayer();

    std::vector<Vec3> positions;
    positions.resize(NPCs.size());

    float threshold = 10.0f;

    for (uint32_t i = 0; i < NPCs.size(); i++) {
        steeringAlgorithm = new KinematicSeperation(NPCs, threshold, i);
        steeringAlgorithm2 = new KinematicSeek(NPCs[i], game->getPlayer());
        steering = *steeringAlgorithm->GetSteering() + *steeringAlgorithm2->GetSteering();

        NPCs[i]->Update(deltaTime, &steering);
        steeringAlgorithm = nullptr;
    }

    game->getPlayer()->Update(deltaTime);

    if (steeringAlgorithm) {
        delete steeringAlgorithm;
    }
}

// Scene3 rendering
void Scene3::Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

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

   

    SDL_RenderPresent(renderer);
}

// Handle SDL events
void Scene3::HandleEvents(const SDL_Event& event) {
    // Handle events for NPCs and player
    game->getPlayer()->HandleEvents(event);
}

// Handle ImGui GUI
void Scene3::HandleTheGUI() {
    ImGui::Begin("Clickable");
    ImGui::Text("Clickable");
    ImGui::End();
}
