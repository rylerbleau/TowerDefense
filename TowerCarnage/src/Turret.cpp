#include "Turret.h"
#include <iostream>
#include "Scene.h"
#include <Vector.h>

Turret::Turret(const char* filename, Vec2 _uvCoords, Scene* scene)
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

	SDL_FreeSurface(turretSurface);
}

Turret::~Turret()
{
	delete m_turretTexture;
}

