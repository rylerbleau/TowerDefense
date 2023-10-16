#include "SpriteSheet.h"

int SpriteSheet::w = 0;
int SpriteSheet::h = 0;
int SpriteSheet::tilesX = 0;
int SpriteSheet::tilesY = 0;

void SpriteSheet::QuerySpriteSheet(const int& tileSizeX_, const int& tileSizeY_, SDL_Texture* _texture)
{
	SpriteSheet::tilesX = tileSizeX_;
	SpriteSheet::tilesY = tileSizeY_;
	SDL_QueryTexture(_texture, NULL, NULL, &w, &h);
}

SDL_Rect SpriteSheet::GetUVTile(int indexX, int indexY)
{
	return {
	(int)(ceil(((float)(indexX) / (float)tilesX) * (float)w)),
	(int)(ceil(((float)(abs(indexY - tilesY) - 1) / (float)tilesY) * (float)h)),
	(int)(floor((((float)(indexX + 1) / (float)tilesX) * (float)w) -
	((float)(indexX) / (float)tilesX) * (float)w)),
	(int)(floor ((((float)(abs(indexY - tilesY) + 1) / (float)tilesY) * (float)h) -
	((float)(abs(indexY - tilesY)) / (float)tilesY) * (float)h))
	};
}

SDL_Rect SpriteSheet::GetSizedUVTile(int indexX, int indexY, int indexSizeX, int indexSizeY)
{
	return {
	(int)(ceil(((float)(indexX) / (float)tilesX) * (float)w)),
	(int)(ceil(((float)(abs(indexY - tilesY) - indexSizeY) / (float)tilesY) * (float)h)),
	(int)(floor((((float)(indexX + indexSizeX) / (float)tilesX) * (float)w) -
	((float)(indexX) / (float)tilesX) * (float)w)),
	(int)(floor((((float)(abs(indexY - tilesY) + indexSizeY) / (float)tilesY) * (float)h) -
	((float)(abs(indexY - tilesY)) / (float)tilesY) * (float)h))
	};
}