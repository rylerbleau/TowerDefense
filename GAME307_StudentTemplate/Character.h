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
	class KinematicBody* body;
	class Scene* scene;
	SDL_Rect sourceRect;
	Direction direction = Direction::IDLE;
public:
	Character()
	{
		body = NULL;
		scene = NULL;
	};

	~Character()
	{
		if (body) delete body;
	};

	bool OnCreate(Scene* scene_);
	void OnDestroy() {};
	bool setTextureWith(string file);
	void Update(float time);
	void HandleEvents(const SDL_Event& event);
	void render(float scale = 1.0f);

	void SteerToSeekPlayer(SteeringOutput& steering);
	void SteerToFleePlayer(SteeringOutput& steering);


};

#endif

