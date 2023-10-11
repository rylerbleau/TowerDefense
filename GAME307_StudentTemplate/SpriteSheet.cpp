#include "SpriteSheet.h"
#include <iostream>



SpriteSheet::SpriteSheet():
	renderer(nullptr),
	surface(nullptr),
	texture(nullptr),
	tilesX(0),
	tilesY(0),
	w(0),
	h(0)
{
}

SpriteSheet::~SpriteSheet()
{
	SDL_FreeSurface(surface);
}

void SpriteSheet::init(SDL_Renderer* renderer_ , const int& tileSizeX, const int& tileSizeY) {
	renderer = renderer_;
	tilesX = tileSizeX;
	tilesY = tileSizeY;
}

void SpriteSheet::LoadMapSurface(const char* filename)
{
	surface = IMG_Load(filename);
	if (surface == nullptr) {
		throw std::runtime_error("Incorrect filepath");
	}

	if(texture == nullptr){
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	}
}

SDL_Rect SpriteSheet::GetUVTile(int indexX, int indexY)
{
	return {
	(int)(ceil(((float)(indexX) / (float)tilesX) * (float)w)),
	(int)(ceil(((float)(abs(indexY - tilesY) - 1) / (float)tilesY) * (float)h)),
	(int)(ceil((((float)(indexX + 1) / (float)tilesX) * (float)w) -
	((float)(indexX) / (float)tilesX) * (float)w)),
	(int)(ceil((((float)(abs(indexY - tilesY) + 1) / (float)tilesY) * (float)h) -
	((float)(abs(indexY - tilesY)) / (float)tilesY) * (float)h))
	};
}