#include "Turret.h"
#include <iostream>
#include "Scene.h"
#include <Vector.h>
#include "Character.h"
#include "StaticBody.h"
#include <limits>

Turret::Turret(const char* filename, Vec2 _uvCoords, Scene* scene, Vec3 pos_)
	: uvCoords(_uvCoords), scene(scene)
{
	SDL_Surface* turretSurface = IMG_Load(filename);
	if (turretSurface == nullptr) {
		throw std::runtime_error("Turret Surface was not created!");
	}
	m_turretTexture = SDL_CreateTextureFromSurface(scene->game->getRenderer(), turretSurface);
	if (m_turretTexture == nullptr) {
		throw std::runtime_error("Turret Texture was not created!");
	}


	pos = pos_;
	printf("pos: %f, %f\n", pos.x, pos.y);	
	SDL_FreeSurface(turretSurface);

	SDL_Surface* bulletSurface = IMG_Load("assets/sprites/tomato.png");
	if (bulletSurface == nullptr) {
		throw std::runtime_error("bullet Surface was not created!");
	}
	bulletTex = SDL_CreateTextureFromSurface(scene->game->getRenderer(), bulletSurface);
	SDL_FreeSurface(bulletSurface);

	bulletScale = 0.02f;

	curCharge = 0.0f;
	chargeTime = 5.0f;
	range = 5.0f;
	target = nullptr;


}

void Turret::GetTarget(const std::vector<Character*>& targets) {
	
	float leastDist = std::numeric_limits<float>::max();
	//target = nullptr;


	for (int i = 0; i < targets.size(); i++) {
		Vec3 targetPos = targets[i]->getBody()->getPos();
		float dist = VMath::distance(pos, targetPos);

		if (dist > range) {
			// target out of range

		}
		else if (dist < leastDist) {
			// get the closest target
			leastDist = dist;
			target = targets[i];
			tIndex = i;

		}
	}
	
}

void Turret::Update(float deltaTime, std::vector<Character*>& targets, std::vector <Turret*>& turrets) {
	float ratio = pos.x / turretUV.x;
	Vec3 offset(-0.2f * turretUV.w * ratio, -0.05f * turretUV.h * ratio, 0);

	
	if (shooting) {
		if (!target) {
			return;
		}
		// check finished
		if (lerpT >= 1.0f) {
			// finished, damage target then remove target
			
			target->TakeDamage(2.0f);
			RemoveTarget();

		}
		else {
			// continue lerping
			targetScaledPos = target->getBody()->getPos() + offset;

			lerpT += 0.02f;
			lerpPos = VMath::lerp(scaledPos, targetScaledPos, lerpT);
		}
	}


	else if (target) {
		// has target from GetTarget, but has not begun shooting yet
		shooting = true;
		lerpT = 0.0f;
		
		
		// values to offset bullet 
		

		scaledPos = pos + offset;
		lerpPos = scaledPos;

	}

	else {
		// no target, not shooting. try to find suitable target.
		GetTarget(targets);
	}

}


void Turret::RemoveTarget()
{
	// reset all values relavent to target
	target = nullptr;
	tIndex = 0;
	shooting = false;
	lerpT = 0.0f;
	lerpPos = Vec3();

}

void Turret::ResetTargets(std::vector<Character*>& targets, std::vector<Turret*>& turrets, int index)
{
	for (const auto& t : turrets) {
		t->RemoveTarget();
	}
}

void Turret::render() {
	SDL_Renderer* renderer = scene->game->getRenderer();
	Matrix4 projectionMatrix = scene->game->getProjectionMatrix();

	// square represents the position and dimensions for where to draw the image
	SDL_Rect square;
	Vec3 screenCoords;
	int    w, h;

	SDL_QueryTexture(m_turretTexture, nullptr, nullptr, &w, &h);
	// convert the position from game coords to screen coords

	w = static_cast<int>(w * pos.x);
	h = static_cast<int>(h * pos.y);
	screenCoords = projectionMatrix * lerpPos;
	square.x = static_cast<int>(screenCoords.x - 0.5 * w);
	square.y = static_cast<int>(screenCoords.y - 0.5 * h);
	square.w = w;
	square.h = h;

	Vec3 dir = targetScaledPos - lerpPos;
	float orientationDegrees = std::atan2(dir.x, dir.y) * 180.0f / M_PI + 180;


	SDL_RenderCopyEx(renderer, bulletTex, nullptr, &square,
		orientationDegrees, nullptr, SDL_FLIP_NONE);
}

void Turret::RenderBullet()
{
	SDL_Renderer* renderer = scene->game->getRenderer();
	Matrix4 projectionMatrix = scene->game->getProjectionMatrix();

	// square represents the position and dimensions for where to draw the image
	SDL_Rect square;
	Vec3 screenCoords;
	int    w, h;

	SDL_QueryTexture(bulletTex, nullptr, nullptr, &w, &h);
	// convert the position from game coords to screen coords
	
	w = static_cast<int>(w * bulletScale);
	h = static_cast<int>(h * bulletScale);
	screenCoords = projectionMatrix * lerpPos;
	square.x = static_cast<int>(screenCoords.x - 0.5 * w);
	square.y = static_cast<int>(screenCoords.y - 0.5 * h);
	square.w = w;
	square.h = h;

	Vec3 dir = targetScaledPos - lerpPos;
	float orientationDegrees = std::atan2(dir.x, dir.y) * 180.0f / M_PI + 180;


	SDL_RenderCopyEx(renderer, bulletTex, nullptr, &square,
		orientationDegrees, nullptr, SDL_FLIP_NONE);

}



Turret::~Turret()
{
	delete m_turretTexture;
	delete bulletTex;
}

