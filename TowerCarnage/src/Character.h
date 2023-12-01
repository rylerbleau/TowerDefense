#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>
#include "Scene.h"
#include "StaticBody.h"
#include "Graph.h"

using namespace std;

class Turret;

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

	class DesicionTreeNode* desicionTree;
	std::vector<Turret*>* turrets;
	Turret* closestTurret;

	float maxHP;
	float curHP;
public:
	Character() :sourceRect{}
	{
		body = NULL;
		scene = NULL;
	};

	~Character();

	bool OnCreate(Scene* scene_, Graph* graph_, Vec3 pos, std::vector<Turret*>* turrets_);
	void OnDestroy() { /*delete body;*/ scene = nullptr; };
	bool setTextureWith(string file);
	void Update(float deltaTime, std::vector<Character* > characters, int index);
	void updatePath(Node* endNode);
	Node* findNearestWalkableNode();
	void render();
	void SeekAndSeparationSteering(KinematicSteeringOutput& steering, std::vector<Character*> characters, float threshhold, int index);
	void SteerToArrive(KinematicSteeringOutput& steering);
	StaticBody* getBody() { return body; }

	void TakeDamage(float dmg) { curHP -= dmg; };
	bool isDead(){ return (curHP <= 0) ? true : false; }
	void RenderUI();
	bool readDesicionTreeFromFile(const char* filename);
};

#endif

