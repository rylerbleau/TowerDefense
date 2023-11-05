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
	Tile* chlid;
	SDL_Texture* tileTexture;
	SDL_Rect uvCoords;
	SDL_Rect destCoords;
	float scale = 1.0f;
	bool needsResizing = false;
	bool isWalkable = true;
	void resizeTile();
};

class Level
{
private:
	Scene* scene;
	std::vector<Tile*> m_tiles;
	bool isChildTileHovered = false;
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
	void drawTiles(SDL_Window* window, std::vector<Character*>& characters);
	bool isMouseOverTile(const Tile* tile, int mouseX, int mouseY);
	void levelHandleEvents(const SDL_Event& event);

	bool canPlaceCharacter(int mouseX, int mouseY);

	void drawTopTileOutline(SDL_Renderer* renderer, int mouseX, int mouseY);

	int getWidth() const { return m_levelData[0].size();}
	int getHeight() const { return m_levelData.size(); }
	const std::vector<std::string>& getLevelData() const { return m_levelData; }
};