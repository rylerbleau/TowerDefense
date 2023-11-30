#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>

#include "Scene.h"
#include "steering/KinematicBody.h"
#include "Seek.h"
#include "Flee.h"
#include "StaticBody.h"
#include "Graph.h"

using namespace std;



enum class Direction {
	RIGHT,
	LEFT,
	FORWARD,
	BACKWARD,
	IDLE
};
class Character : public I_HasUI
{
private:
	class StaticBody* body;
	class Scene* scene;
	SDL_Rect sourceRect;
	Direction direction = Direction::IDLE;
	float scale = 1.0f;
	Graph* graph;
	Path* path;
	Node* endNode;
	Node* startNode;

	float maxHP;
	float curHP;

	
public:
	Character() :sourceRect{}
	{
		body = NULL;
		scene = NULL;
	};

	~Character();

	bool OnCreate(Scene* scene_, Graph* graph, Vec3 pos = Vec3(15.0f, 10.0f, 0.0f));
	void OnDestroy() { /*delete body*/ };
	bool setTextureWith(string file);
	void Update(float deltaTime, std::vector<Character* > characters, int index);
	void updatePath(Node* endNode);
	Node* findNearestWalkableNode();
	void HandleEvents(const SDL_Event& event);
	void render();
	void SeekAndSeparationSteering(KinematicSteeringOutput& steering, std::vector<StaticBody*> staticBodies, float threshhold, int index);
	StaticBody* getBody() { return body; }





	bool TakeDamage(float dmg);
	bool isDead(){ return (curHP <= 0) ? true : false; }
	void RenderUI();

};

#endif

