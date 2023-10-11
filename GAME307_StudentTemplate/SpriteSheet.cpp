#include "SpriteSheet.h"
#include <iostream>

const size_t TILE_SIZE_X = 12;
const size_t TILE_SIZE_Y = 11;

SpriteSheet::SpriteSheet():
	renderer(nullptr),
	surface(nullptr),
	texture(nullptr),
	tilesX(TILE_SIZE_X),
	tilesY(TILE_SIZE_Y)
{
}

SpriteSheet::~SpriteSheet()
{
	SDL_FreeSurface(surface);
}

void SpriteSheet::init(SDL_Renderer* renderer_) {
	renderer = renderer_;
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
	int tileWidth = w / tilesX;
	int tileHeight = h / tilesY;

	return {
	(int)(ceil(((float)(indexX) / (float)tilesX) * (float)w)),
	(int)(ceil(((float)(indexY) / (float)tilesY) * (float)h)),
	(int)(ceil((((float)(indexX + 1) / (float)tilesX) * (float)w) -
	((float)(indexX) / (float)tilesX) * (float)w)),
	(int)(ceil((((float)(indexY + 1) / (float)tilesY) * (float)h) -
	((float)(indexY) / (float)tilesY) * (float)h))
	};
	
}