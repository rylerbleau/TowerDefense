#ifndef SCENE1_H
#define SCENE1_H

#include <MMath.h>
#include <VMath.h>
#include <array>
#include "Scene.h"
#include "Character.h"
#include "StaticBody.h"
#include "Level.h"
#include "SpriteSheet.h"
#include "Turret.h"

class Graph;

using namespace MATH;
class Scene1 : public Scene {
private:
	SDL_Window *window;
	float xAxis = 25.0f;
	float yAxis = 15.0f;

	SDL_Renderer* renderer;
	Matrix4 projectionMatrix;
    Matrix4 inverseProjection;
	Character* blinky;
	std::vector<Character*> characters;
	std::vector<Turret*> turrets;
	std::vector<Node*> path;
	Graph* graph;

	Level level;

public:
	Scene1(SDL_Window* sdlWindow, GameManager* game_);
	~Scene1();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
    void HandleEvents(const SDL_Event &event);

	float getxAxis() { return xAxis; }
	float getyAxis() { return yAxis; }
	SDL_Window* getWindow() { return window; }
    Matrix4 getProjectionMatrix() { return projectionMatrix; }
	Matrix4 getInverseMatrix() { return inverseProjection; }

	std::vector<Node*> getPath() { return  path; }
};

#endif
