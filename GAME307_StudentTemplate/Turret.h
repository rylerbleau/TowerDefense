#pragma once
#include <memory>
#include <vector>
#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "SpriteSheet.h"
#include "Vector.h"

class Scene;

class Turret
{
	friend class Level;
private:
	SDL_Texture* m_turretTexture;
	SDL_Rect turretUV;
	std::unique_ptr<Scene> scene;
	MATH::Vec2 uvCoords;
public:
	Turret(const char* filename, MATH::Vec2 _uvCoords, Scene* scene);
	~Turret();
	

	void Render();

};

