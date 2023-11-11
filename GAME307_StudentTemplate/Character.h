#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>

#include "Scene.h"
#include "KinematicBody.h"
#include "Seek.h"
#include "Flee.h"
#include "Scene1.h"
#include "Path.h"
#include "FollowAPath.h"

class Node;


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
	Path* path;
	FollowAPath* steering_algorithm;
public:
	Character() :sourceRect{}
	{
		body = NULL;
		scene = NULL;
	};

	~Character()
	{};

	bool OnCreate(Scene* scene_, Vec3 pos = Vec3(5.0f, 5.0f, 0.0f));
	void OnDestroy() { if (body != nullptr) { delete body; } };
	bool setTextureWith(string file);
	void Update(float deltaTime, std::vector<Character*> characters, int index, Path* path_);
	void UpdateKinematic(float deltaTime, KinematicSteeringOutput* steering);
	void HandleEvents(const SDL_Event& event);
	void render(float scale = 1.0f);
	void SeekAndSeparationSteering(KinematicSteeringOutput& steering, std::vector<StaticBody*> staticBodies, float threshhold, int index, Path* path_);
	void SteerToFleePlayer(SteeringOutput& steering);
	StaticBody* getBody() { return body; }
};

#endif

