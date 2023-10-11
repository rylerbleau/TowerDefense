#pragma once
#include <SDL.h>
#include <SDL_image.h>
class SpriteSheet
{
private:
	SDL_Surface* surface; 
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	int w, h;
	int tilesX, tilesY;
public:
	SpriteSheet();
	~SpriteSheet();
	void init(SDL_Renderer* renderer_);
	void LoadMapSurface(const char* filename);
	SDL_Rect GetUVTile(int indexX, int indexY);
	SDL_Rect GetSourceRect() {
		return SDL_Rect{ 0, 0, w, h };
	}
	SDL_Texture* getTexture() const { return texture; };
};
