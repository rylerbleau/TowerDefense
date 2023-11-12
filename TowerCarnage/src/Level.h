#pragma once

#include <string>
#include <vector>
#include <SDL.h>
#include "SpriteSheet.h"

class Scene;
class Turret;
class Graph;
class Character;
class Node;
class Path;

// Tile struct declaration 
struct Tile {
    Tile* chlid;
    SDL_Texture* tileTexture;
    SDL_Rect uvCoords;
    SDL_Rect destCoords;
    float scale = 1.0f;
    bool needsResizing = false;
    bool isWalkable = true;
    Node* tileNode = nullptr;
    void resizeTile();
};

class Level {
private:
    Scene* scene;
    std::vector<Tile*> m_tiles;
    std::vector<std::string> m_levelData;
    Node* endNode = nullptr;
    Graph* graph;
    int mousePosX;
    int mousePosY;
    int width;
    int height;
    bool placeActor;

public:
    Level() = default;
    Level(const std::string& fileName, Scene* scene);
    ~Level();

    void LoadMap(const int& tileSizeX, const int& tileSizeY, const char* filename);
    void clear();
    void drawTiles(SDL_Window* window, std::vector<Character*>& characters);
    bool isMouseOverTile(const Tile* tile, int mouseX, int mouseY);
    Node* getTileNodeUnderMouse();
    void levelHandleEvents(const SDL_Event& event);
    bool canPlaceCharacter(int mouseX, int mouseY);
    void drawTopTileOutline(int mouseX, int mouseY);

    int getWidth() const { return m_levelData[0].size(); }
    int getHeight() const { return m_levelData.size(); }
    const std::vector<std::string>& getLevelData() const { return m_levelData; }
};
