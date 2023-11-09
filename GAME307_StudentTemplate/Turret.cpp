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

	SDL_Surface* bulletSurface = IMG_Load("Clyde.png");
	if (bulletSurface == nullptr) {
		throw std::runtime_error("bullet Surface was not created!");
	}
	bulletTex = SDL_CreateTextureFromSurface(scene->game->getRenderer(), bulletSurface);
	SDL_FreeSurface(bulletSurface);

	bulletScale = 0.1f;

	curCharge = 0.0f;
	chargeTime = 5.0f;
	range = 5.0f;
	target = nullptr;

}

void Turret::GetTarget(const std::vector<Character*>& targets) {
	
	float leastDist = std::numeric_limits<float>::max();
	Character* tempTarget = nullptr;
	for (const auto& c : targets) {

		// dist from here to there
		Vec3 targetPos = c->getBody()->getPos();
		float dist = VMath::distance(pos, targetPos);

		if (dist > range) {
			// target out of range
			
		}
		else if (dist < leastDist) {
			// get the closest target
			leastDist = dist;
			tempTarget = c;
			
		}
	}
	
	target = tempTarget;
	
}

void Turret::Update(float deltaTime) {
	// TODO
	// if shooting , continue shooting
		// continue lerping towards target
		// if finished, finish 
		// else
	
	// else if no shooting, 
		// if within range
			// begin shoot
		// else
			// do nothing

	

	if (shooting) {
		if (lerpT >= 1.0f) {
			shooting = false;
		}
		lerpT += 0.02f;
		// update lerp

		// TODO make sure that when target goe out of scope
		lerpPos = VMath::lerp(pos, target->getBody()->getPos(), lerpT);
	}
	else if (target) {
		curCharge += deltaTime;

		printf("shoot\n");
		shooting = true;
		lerpT = 0.0f;
		curCharge = 0.0f;
		lerpPos = pos;
		if (curCharge >= chargeTime) {
		}
	}




	


}

void Turret::RenderBullet()
{
	SDL_Renderer* renderer = scene->game->getRenderer();
	
	//SDL_RenderDrawLine(renderer, pos.x, pos.y, target->getBody()->getPos().x, target->getBody()->getPos().y);
	//SDL_RenderCopyEx(renderer, bulletTex, NULL, &rect, 20, NULL, SDL_FLIP_NONE);
	


	Matrix4 projectionMatrix = scene->game->getProjectionMatrix();

	// square represents the position and dimensions for where to draw the image
	SDL_Rect square;
	Vec3 screenCoords;
	int    w, h;

	SDL_QueryTexture(bulletTex, nullptr, nullptr, &w, &h);
	// convert the position from game coords to screen coords
	screenCoords = projectionMatrix * lerpPos;
	w *= bulletScale;
	h *= bulletScale;
	square.x = static_cast<int>(screenCoords.x - 0.5f * w);
	square.y = static_cast<int>(screenCoords.y + 0.5f * h);
	square.w = static_cast<int>(w);
	square.h = static_cast<int>(h);

	// Convert character orientation from radians to degrees.
	float orientationDegrees = 0.0f * 180.0f / M_PI;

	SDL_RenderCopyEx(renderer, bulletTex, nullptr, &square,
		orientationDegrees, nullptr, SDL_FLIP_NONE);

}

Turret::~Turret()
{
	delete m_turretTexture;
}

