#pragma once
#include <SDL.h>

class SpriteSheet
{
private:
	SpriteSheet() {};
	~SpriteSheet() {};
public:
	static int w, h;
	static int tilesX, tilesY;

	static void QuerySpriteSheet(const int& tileSizeX, const int& tileSizeY, SDL_Texture* _texture);
	static SDL_Rect GetUVTile(int indexX, int indexY);
	static SDL_Rect GetSizedUVTile(int indexX, int indexY, int indexSizeX, int indexSizeY);
};

