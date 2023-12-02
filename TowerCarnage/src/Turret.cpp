#include "Turret.h"
#include <iostream>
#include "Scene.h"
#include <Vector.h>
#include "Character.h"
#include "StaticBody.h"
#include <limits>


double mapToRange(double degrees) {

	double r = fmod(degrees, 360.0);

	if (r < -180.0)
		r += 360.0;
	if (r >= 180.0)
		r -= 360.0;
	return r;

}


Turret::Turret(const char* filename, Scene* scene, Vec3 pos_, SDL_Rect destCoords_, Vec2 uvCoords_ /*= Vec2(6, 7)*/)
	:
	scene(scene),
	lerpT(0.0f),
	lerpPos(Vec3()),
	shooting(false),
	tIndex(0),
	target(nullptr),
	bulletScale(0.02f),
	destCoords(destCoords_),
	orientaton(Vec3(0.0f, -1.0f, 0.0f)),
	maxHP(10.0f),
	curHP(maxHP),
	body(StaticBody(pos_, 0.0f, 0.0f, 0.0f))
{
	SDL_Surface* turretSurface = IMG_Load(filename);
	if (turretSurface == nullptr) {
		throw std::runtime_error("Turret Surface was not created!");
	}
	m_turretTexture = SDL_CreateTextureFromSurface(scene->game->getRenderer(), turretSurface);
	if (m_turretTexture == nullptr) {
		throw std::runtime_error("Turret Texture was not created!");
	}
	SDL_FreeSurface(turretSurface);

	SDL_Surface* bulletSurface = IMG_Load("assets/sprites/tomato.png");
	if (bulletSurface == nullptr) {
		throw std::runtime_error("bullet Surface was not created!");
	}
	bulletTex = SDL_CreateTextureFromSurface(scene->game->getRenderer(), bulletSurface);
	SDL_FreeSurface(bulletSurface);

	SpriteSheet::QuerySpriteSheet(12, 10, m_turretTexture);
	turretUV = SpriteSheet::GetUVTile(uvCoords_.x, uvCoords_.y);
}

void Turret::GetTarget(const std::vector<Character*>& targets) {
	
	float leastDist = std::numeric_limits<float>::max();

	for (int i = 0; i < targets.size(); i++) {
		Vec3 targetPos = targets[i]->getBody()->getPos();
		float dist = VMath::distance(body.getPos(), targetPos);

		if (dist > 5.0f) {
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
	float ratio = body.getPos().x / turretUV.x;
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
		scaledPos = body.getPos() + offset;
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
	
	Vec3 targetOrientaton = Vec3{ 0.0f,-1.0f,0.0f };

	if (target) {
		targetOrientaton = Vec3{ target->getBody()->getPos().x -
			body.getPos().x, target->getBody()->getPos().y -
			body.getPos().y, 0.0f };
	}

	orientaton = VMath::lerp(orientaton, targetOrientaton, 0.025f);
	angle = std::atan2(orientaton.x, orientaton.y) * 180.0f / M_PI + 180;
	angle = mapToRange(angle);
	

	SDL_RenderCopyEx(renderer, m_turretTexture, &turretUV, &destCoords,
		angle, nullptr, SDL_FLIP_NONE);
	RenderUI();
}

void Turret::RenderUI()
{
	SDL_Renderer* renderer = scene->game->getRenderer();
	Matrix4 projectionMatrix = scene->getProjectionMatrix();

	SDL_Rect HPoutline;
	SDL_Rect HPfill;
	Vec3 screenCoords;
	float w, h;

	// notice use of "body" in the following

	w = static_cast<int>(destCoords.w);
	h = static_cast<int>(destCoords.h / 4.0f);
	screenCoords = projectionMatrix * body.getPos();

	HPoutline.y = static_cast<int>(screenCoords.y - h) - 20.0f;
	HPoutline.x = static_cast<int>(screenCoords.x - w);
	HPoutline.w = w;
	HPoutline.h = h;

	HPfill = HPoutline;
	HPfill.w = curHP / maxHP * w;
	// red for fill
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &HPfill);


	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &HPoutline);
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

