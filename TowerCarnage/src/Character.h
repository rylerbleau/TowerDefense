#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>

#include "Scene.h"
#include "steering/KinematicBody.h"
#include "Seek.h"
#include "Flee.h"
#include "StaticBody.h"

using namespace std;
enum class Direction {
	RIGHT,
	LEFT,
	FORWARD,
	BACKWARD,
	IDLE
};
class Character
{
private:
	class StaticBody* body;
	class Scene* scene;
	SDL_Rect sourceRect;
	Direction direction = Direction::IDLE;
	float scale = 1.0f;
	
public:
	Character() :sourceRect{}
	{
		body = NULL;
		scene = NULL;
	};

	~Character()
	{};

	bool OnCreate(Scene* scene_, Path* path = nullptr, Vec3 pos = Vec3(15.0f, 10.0f, 0.0f));
	void OnDestroy() { delete body; };
	bool setTextureWith(string file);
	void Update(float deltaTime, std::vector<Character* > characters, int index, Path* path_);
	void HandleEvents(const SDL_Event& event);
	void render(float scale = 1.0f);
	void SeekAndSeparationSteering(KinematicSteeringOutput& steering, std::vector<StaticBody*> staticBodies, float threshhold, int index, Path* path_);
	StaticBody* getBody() { return body; }
};

#endif

