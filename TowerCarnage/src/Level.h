#pragma once

#include <string>
#include <vector>
#include <SDL.h>
#include "SpriteSheet.h"

class Scene;
class Turret;
class Graph;
class Node;
class Path;

// Tile struct declaration 
struct Tile {
    Tile* child;
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
    Tile* topTile;
    std::vector<Tile*> m_tiles;
    std::vector<Node*> walkableTileNodes;
    std::vector<std::string> m_levelData;
    int mousePosX;
    int mousePosY;
public:
    Level() = default;
    Level(const std::string& fileName, Scene* scene);
    ~Level();

    void loadMap(const int& tileSizeX, const int& tileSizeY, const char* filename);
    void clear();
    void drawTiles();
    bool isMouseOverTile(const Tile* tile);
    Node* getTileNodeUnderMouse();
    void levelHandleEvents(const SDL_Event& event);
    bool canPlaceEntity();
    void drawTopTileOutline();

    int getWidth() const { return m_levelData[0].size(); }
    int getHeight() const { return m_levelData.size(); }
    const std::vector<Node*>& getWalkableTileNodes() const { return walkableTileNodes; }
    const std::vector<Tile*>& getTiles() const { return m_tiles; }
};
