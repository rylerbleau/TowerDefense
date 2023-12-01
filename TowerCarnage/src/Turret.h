#pragma once
#include <memory>
#include <vector>
#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "SpriteSheet.h"
#include "Vector.h"

class Scene;
class Character;

using namespace MATH;
class Turret
{
	friend class Scene1;
private:
	SDL_Rect turretUV;
	SDL_Rect destCoords;
	SDL_Texture* m_turretTexture;
	std::unique_ptr<Scene> scene;

	
	int tIndex;
	float range;
	float angle;
	float lerpT;
	float bulletScale;
	bool shooting;
	Vec3 pos;
	Vec3 lerpPos;
	Vec3 orientaton;
	Vec3 scaledPos;
	Vec3 targetScaledPos;
	Character* target;
	SDL_Texture* bulletTex;
public:
	Turret(const char* filename, Scene* scene, Vec3 pos_, SDL_Rect destCoords, Vec2 _uvCoords = Vec2(6, 7));
	~Turret();

	void GetTarget(const std::vector<Character*>& targets);
	bool HasTarget() { return (target == nullptr) ? false : true; }
	void Update(float deltaTime, std::vector<Character*>& targets, std::vector <Turret*>& turrets);
	void RenderBullet();
	const int GetTIndex() { return tIndex; }
	void RemoveTarget();
	static void ResetTargets(std::vector<Character*>& targets, std::vector <Turret*>& turrets, int index);
	void render();
		
};

