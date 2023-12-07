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

    
}

Scene3::~Scene3() {
    
}

// Scene3 initialization
bool Scene3::OnCreate() {
    

    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    Matrix4 ndc = MMath::viewportNDC(w, h);
    Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
    projectionMatrix = ndc * ortho;

    // Turn on the SDL imaging subsystem
    IMG_Init(IMG_INIT_PNG);

    

    // Set up characters
  

    float orientation_ = 0.0f;
    float maxSpeed_ = 5.0f;
    float maxRotation_ = 1.0f;
    Vec3 position_(5.0f, 5.0f, 0.0f);

    


    return true;
}

void Scene3::OnDestroy() {
    // Clean up resources on scene destruction
    // ...
}

// Scene3 update function
void Scene3::Update(const float deltaTime) {

}

// Scene3 rendering
void Scene3::Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    
    HandleTheGUI();

}

// Handle SDL events
void Scene3::HandleEvents(const SDL_Event& event) {
    // Handle events for NPCs and player
}

// Handle ImGui GUI
void Scene3::HandleTheGUI() {
    // pause menu
    if (ImGui::Begin("Pause", NULL,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoNavInputs | ImGuiHoveredFlags_AnyWindow)) {
        if (ImGui::Button("Play", ImVec2(185, 50))) {
            SDL_Event e;
            e.type = SDL_EventType::SDL_USEREVENT;
            e.user.code = 1;
            SDL_PushEvent(&e);
        }
        
        if (ImGui::Button("Quit", ImVec2(185, 50))) {
            SDL_Event e;
            e.type = SDL_EventType::SDL_QUIT;
            SDL_PushEvent(&e);
            //paused = !paused;
        }

    }
    ImGui::End();
}
