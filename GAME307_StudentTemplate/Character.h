#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>

#include "Scene.h"
#include "KinematicBody.h"
#include "Seek.h"
#include "Flee.h"


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
public:
	Character() :sourceRect{}
	{
		body = NULL;
		scene = NULL;
	};

	~Character()
	{
		if (body) delete body;
	};

	bool OnCreate(Scene* scene_, Vec3 pos = Vec3(5.0f, 5.0f, 0.0f));
	void OnDestroy() {};
	bool setTextureWith(string file);
	void Update(float deltaTime, std::vector<Character*> characters, int index);
	void HandleEvents(const SDL_Event& event);
	void render(float scale = 1.0f);

	void SeekAndSeparationSteering(KinematicSteeringOutput& steering, std::vector<StaticBody*> staticBodies, float threshhold, int index);
	void SteerToFleePlayer(SteeringOutput& steering);
	StaticBody* getBody() { return body; }
};

#endif

