#pragma once
#include <string>
#include <vector>
#include <SDL.h>
#include "SpriteSheet.h"


class Level
{
private:
	std::vector<std::string> m_levelData;
	SpriteSheet* spriteSheet;
	
public:
	Level();
	Level(const std::string& fileName);
	~Level();
	void draw(SDL_Renderer* renderer, const SDL_Rect& dims_, const SDL_Rect& color_);
	void drawTiles(SDL_Renderer* renderer, SDL_Window* window);
	char getTile(int x, int y);
	SDL_Rect getWoorldCoordTile(const SDL_Rect& rect, const int& windowW, const int& windowH);
	
	int getWidth() const { return m_levelData[0].size();}
	int getHeight() const { return m_levelData.size(); }
	const std::vector<std::string>& getLevelData() const { return m_levelData; }
};

