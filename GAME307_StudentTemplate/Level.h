#pragma once
#include <string>
#include <vector>
#include <SDL.h>

class Tile {
	friend class Level;
private:
	SDL_Rect dims;
	SDL_Rect color;
};

class Level
{
private:
	std::vector<std::string> m_levelData;
	Tile tile;
public:
	Level();
	Level(const std::string& fileName);
	~Level();
	void draw(SDL_Renderer* renderer, const SDL_Rect& dims_, const SDL_Rect& color_);
	void drawTiles(SDL_Renderer* renderer, SDL_Window* window);
	std::vector<std::string> getTile();
	int getWidth() const { return m_levelData[0].size();}
	int getHeight() const { return m_levelData.size(); }
	const std::vector<std::string>& getLevelData() const { return m_levelData; }
};

