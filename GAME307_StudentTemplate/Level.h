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
	Level() = default;
	Level(const std::string& fileName, SDL_Renderer* renderer_);
	~Level();
	void draw(SDL_Renderer* renderer, const SDL_Rect& uvRect, SDL_Rect destRect, float scale = 1.0f, bool needsResizing = false);
	void drawTiles(SDL_Renderer* renderer, SDL_Window* window);
	char getTile(int x, int y);
	
	int getWidth() const { return m_levelData[0].size();}
	int getHeight() const { return m_levelData.size(); }
	const std::vector<std::string>& getLevelData() const { return m_levelData; }
};

