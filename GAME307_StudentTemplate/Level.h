#pragma once
#include <string>
#include <vector>
#include <SDL.h>
#include "SpriteSheet.h"
#include <unordered_map>

class Scene;
class Character;
class Turret;

struct Tile {
	SDL_Texture* tileTexture;
	SDL_Rect uvCoords;
	SDL_Rect destCoords;
	float scale = 1.0f;
	bool needsResizing = false;
	char letter;
};

class Level
{
private:
	Scene* scene;
	std::vector<Tile*> m_tiles;
	std::vector<std::string> m_levelData;
	int mousePosX = 0;
	int mousePosY = 0;
	int width = 0;
	int height = 0;
	bool placeActor = false;
public:
	Level() = default;
	Level(const std::string& fileName, Scene* scene);
	~Level();

	void LoadMap(const int& tileSizeX, const int& tileSizeY, const char* filename);
	void clear();
	void drawTiles(SDL_Window* window, std::vector<Character*>& characters, std::vector<Turret*>& turrets);
	void placeTurret(SDL_Window* window, std::vector<Turret*>& turrets, Tile* tile);
	char getTile(int x, int y);
	bool compareTile(SDL_Rect r1, SDL_Rect r2);
	void levelHandleEvents(const SDL_Event& event);

	int getWidth() const { return m_levelData[0].size();}
	int getHeight() const { return m_levelData.size(); }
	const std::vector<std::string>& getLevelData() const { return m_levelData; }
};