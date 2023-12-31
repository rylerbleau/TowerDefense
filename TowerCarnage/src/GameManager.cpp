#include "GameManager.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer2.h"


GameManager::GameManager() {
	windowPtr = nullptr;
	timer = nullptr;
	isRunning = true;
	currentScene = nullptr;
    player = nullptr;
    show_ui = true;
    changeSceneEventType = 0;
}

bool GameManager::OnCreate() {
    
   //const int SCREEN_WIDTH = 1280;
   //const int SCREEN_HEIGHT = 720;

    windowPtr = new Window(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (windowPtr == nullptr) {
		OnDestroy();
		return false;
	}
	if (windowPtr->OnCreate() == false) {
		OnDestroy();
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) {
		OnDestroy();
		return false;
	}

    // select scene for specific assignment

    currentScene = new Scene1(windowPtr->GetSDL_Window(), this);

    // need to create Player before validating scene
    if (!ValidateCurrentScene()) {
        OnDestroy();
        return false;
    }
    
    // create some user defined event
    changeSceneEventType = SDL_RegisterEvents(1);
    if ( changeSceneEventType == ((Uint32) - 1 ) )
    {
        OnDestroy();
        return false;
    }
       


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    
    //ImGui_ImplSDL2_InitForOpenGL(windowPtr->getWindow(), window->getContext());
    //ImGui_ImplOpenGL3_Init("#version 450");
    io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Arial.ttf", 18.0f);
    ImGui_ImplSDL2_InitForSDLRenderer(getWindow(), getRenderer());
    ImGui_ImplSDLRenderer2_Init(getRenderer());
 


	return true;
}


/// Here's the whole game loop
void GameManager::Run() {
    
	timer->Start();
    // control if current scene's update() is called each tick
    
	while (isRunning)
	{
		handleEvents();


        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

     
		timer->UpdateFrameTicks();

		currentScene->Update(timer->GetDeltaTime());
           

		currentScene->Render();

        if (show_ui) {
            ImGui::Render();
            ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        }
        
        SDL_RenderPresent(getRenderer());



		/// Keep the event loop running at a proper rate
		SDL_Delay(timer->GetSleepTime(60)); ///60 frames per sec
	}
    //std::cout << "endMem: " << curMem << std::endl;
    //std::cout << "maxMem: " << maxMem << std::endl;
    //std::cout << "allocs: " << totalAllocations << std::endl;
    //std::cout << "deallocs: " << totalDeallocations << std::endl;

}

void GameManager::handleEvents()
{
	SDL_Event event;

        // Let's add mouse movement and position
        // https://wiki.libsdl.org/SDL_GetMouseState

        SDL_PumpEvents();  // make sure we have the latest mouse state.

        //https://www.youtube.com/watch?v=SYrRMr4BaD4&list=PLM7LHX-clszBIGsrh7_3B2Pi74AhMpKhj&index=3
        while( SDL_PollEvent(&event) )
        {

            ImGui_ImplSDL2_ProcessEvent(&event);
            if( event.type == SDL_QUIT )
            {
                isRunning = false;
            }
            else if( event.type == SDL_KEYDOWN )
            {
                switch ( event.key.keysym.scancode )
                {
                    case SDL_SCANCODE_ESCAPE:
                        isRunning = false;
                        break;
                    case SDL_SCANCODE_Q:
                        isRunning = false;
                        break;
                    case SDL_SCANCODE_DELETE:
                        isRunning = false;
                        break;
                    case SDL_SCANCODE_L:
                        //toggle launched with L key)
                        launched = !launched;
                        break;
                    case SDL_SCANCODE_1:
                        LoadScene( 1 );
                        break;
                    case SDL_SCANCODE_2:
                        LoadScene(2);
                        break;
                    case SDL_SCANCODE_3:
                        LoadScene(3);
                        break;
                    case SDL_SCANCODE_SPACE:
                        TogglePauseGame();
                        break;
                    default:
                        break;
                }
            }
            currentScene->HandleEvents( event );
        }
}

GameManager::~GameManager() {

    if (currentScene) {
        currentScene->OnDestroy();
        delete currentScene;
        currentScene = nullptr;
    }

    if (timer) {
        delete timer;
        timer = nullptr;
    }

    if (windowPtr) {
        delete windowPtr;
        windowPtr = nullptr;
    }

    

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

}

void GameManager::OnDestroy(){
	if (windowPtr) delete windowPtr;
	if (timer) delete timer;
	if (currentScene) delete currentScene;
}

 float const GameManager::getSceneHeight()
{ return  currentScene->getyAxis(); }

float const GameManager::getSceneWidth()
{ return currentScene->getxAxis(); }

float const GameManager::getWindowHeight()
{
    return SCREEN_HEIGHT;
}

float const GameManager::getWindowWidth()
{
    return SCREEN_WIDTH;
}

Matrix4 GameManager::getProjectionMatrix() 
{ return currentScene->getProjectionMatrix(); }

void GameManager::TogglePauseGame()
{

}

SDL_Renderer* GameManager::getRenderer()
{
    // [TODO] might be missing some SDL error checking
    SDL_Window* window = currentScene->getWindow();
    SDL_Renderer* renderer = SDL_GetRenderer(window);
    return renderer;
}

SDL_Window* GameManager::getWindow() {

    SDL_Window* window = currentScene->getWindow();
    return window;
}

void GameManager::LoadScene( int i )
{
    // cleanup of current scene before loading anothe one
    currentScene->OnDestroy();
    delete currentScene;
    switch ( i )
    {
        case 1:
            currentScene = new Scene1( windowPtr->GetSDL_Window(), this);
            break;
        case 2:
            currentScene = new Scene2(windowPtr->GetSDL_Window(), this);
            break;
        case 3:
            currentScene = new Scene3(windowPtr->GetSDL_Window(), this);
            break;
        default:
            currentScene = new Scene3( windowPtr->GetSDL_Window(), this );
            break;
    }
    // using ValidateCurrentScene() to safely run OnCreate
    if (!ValidateCurrentScene())
    {
        isRunning = false;
    }
    launched = true;
}

bool GameManager::ValidateCurrentScene()
{
    if (currentScene == nullptr) {
        return false;
    }
    if (currentScene->OnCreate() == false) {
        return false;
    }
    return true;
}
