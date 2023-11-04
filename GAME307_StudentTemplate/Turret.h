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
	friend class Level;
private:
	SDL_Texture* m_turretTexture;
	SDL_Rect turretUV;
	std::unique_ptr<Scene> scene;
	MATH::Vec2 uvCoords;

	Vec3 pos;
	
	float range;
	float curCharge;
	float chargeTime;
	float lerpT;
	Vec3 lerpPos;
	Character* target;
	bool shooting;
	
	SDL_Texture* bulletTex;
	float bulletScale;
public:
	Turret(const char* filename, MATH::Vec2 _uvCoords, Scene* scene, Vec3 pos_);
	~Turret();
	

	void GetTarget(const std::vector<Character*>& targets);
	bool HasTarget() { return (target == nullptr) ? false : true; }

	void Update(float deltaTime);
	void RenderBullet();

};

