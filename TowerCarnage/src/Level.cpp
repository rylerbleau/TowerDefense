#include "Level.h"
#include "fstream"
#include <iostream>
#include <random>
#include "SDL_image.h"
#include "Scene.h"
#include "Turret.h"

Level::Level(const std::string& fileName, Scene* scene)
    :scene(scene), mousePosX(0), mousePosY(0), topTile(nullptr)
{
    std::ifstream file;
    file.open(fileName);

    // Error checking
    if (file.fail()) {
        std::cerr << "Failed to open " + fileName << std::endl;
    }

    std::string temp;
    std::getline(file, temp);
    while (std::getline(file, temp)) {
        m_levelData.push_back(temp);
    }
    file.close();

}

Level::~Level() {
   
}

void Level::loadMap(const int& tileSizeX, const int& tileSizeY, const char* filename)
{
    SDL_Surface* surface = IMG_Load(filename);
    if (surface == nullptr) {
        throw std::runtime_error("Incorrect filepath");
    }

    mapTexture = SDL_CreateTextureFromSurface(scene->game->getRenderer(), surface);
    if (mapTexture == nullptr) {
        throw std::runtime_error("Map Texture was not created!");
    }

    SDL_FreeSurface(surface);
    /// Quering sprite sheet to get dimentions and pass how many tiles along x and y are there
    /// needed for sprite calculation later
    SpriteSheet::QuerySpriteSheet(tileSizeX, tileSizeY, mapTexture);

    /// <summary>
    /// GetUVTile function gets the individual texture from the spritesheet. Just pass the coordinates of the tile and
    /// it will return the sdl rectangle that is used to extract a tile from the spritesheet 
    /// </summary>
    /// <param name="indexX"> depending on the spritesheet pass in the coordinate of the tile along X</param>
    /// <param name="indexY"> depending on the spritesheet pass in the coordinate of the tile along Y</param>
    SDL_Rect pathRect = SpriteSheet::GetUVTile(1, 8);
    SDL_Rect grassRect = SpriteSheet::GetUVTile(1, 10);
    SDL_Rect leftGrassRect = SpriteSheet::GetUVTile(0, 8);
    SDL_Rect bottomLeftGrassCorner = SpriteSheet::GetUVTile(0, 7);
    SDL_Rect topLeftGrassCorner = SpriteSheet::GetUVTile(0, 9);
    SDL_Rect rightGrassRect = SpriteSheet::GetUVTile(2, 8);
    SDL_Rect bottomRightGrassRect = SpriteSheet::GetUVTile(2, 7);
    SDL_Rect topRightGrassCorner = SpriteSheet::GetUVTile(2, 9);
    SDL_Rect bottomRect = SpriteSheet::GetUVTile(1, 7);
    SDL_Rect topRect = SpriteSheet::GetUVTile(1, 9);

    SDL_Rect flowerRect = SpriteSheet::GetUVTile(2, 10);
    SDL_Rect greenTreeRect = SpriteSheet::GetUVTile(4, 8);
    SDL_Rect orangeTreeRect = SpriteSheet::GetSizedUVTile(3, 9, 1, 2);
    SDL_Rect buildingRect = SpriteSheet::GetSizedUVTile(0, 4, 4, 3);
    SDL_Rect doorRect = SpriteSheet::GetUVTile(1, 3);
    SDL_Rect rockRect = SpriteSheet::GetUVTile(7, 7);
    SDL_Rect wellRect = SpriteSheet::GetSizedUVTile(8, 2, 1, 2);
    SDL_Rect stockRect = SpriteSheet::GetSizedUVTile(8, 5, 3, 3);
    SDL_Rect boardRect = SpriteSheet::GetUVTile(11, 4);

    static const SDL_Rect worldTileCoords{ ceil((float)scene->game->getWindowWidth() / (float)getWidth()),
                              ceil((float)scene->game->getWindowHeight() / (float)getHeight()),
                              ceil((float)scene->game->getWindowWidth() / (float)getWidth()),
                              ceil((float)scene->game->getWindowHeight() / (float)getHeight()) };

    /// Here I pupulate the m_tiles vector. When it detects a character it is going to scan through switch statement
    /// to determine the attributes for that tile that is going to be rendered at that grid location
    /// than it is pushed in the vector of Tile pointers after what i am going to sort the vector by its scale 
    /// so that sprites won't be overdrawn
    int label = 0;
    float positionX = 0;
    float positionY = 0;

    MATH::Vec3 position = {};

    for (size_t y = 0; y < m_levelData.size(); y++) {
        for (size_t x = 0; x < m_levelData[y].size(); x++) {

            char tile = m_levelData[y][x];

            SDL_Rect gridPosition = worldTileCoords;
            gridPosition.x *= x;
            gridPosition.y *= y;
            Tile* newTile = nullptr;
            Tile* childTile = nullptr;
            Node* newNode = nullptr;

            switch (tile) {
            case 'P':
                /// Node position is the centre of that tile that is within the orthographic dimensions
                positionX = static_cast<float>((gridPosition.x + gridPosition.w) * scene->getxAxis()) / scene->game->getWindowWidth();
                positionY = scene->getyAxis() - (static_cast<float>((gridPosition.y + 0.5 * gridPosition.h) * scene->getyAxis()) / scene->game->getWindowHeight());
                position = { positionX, positionY, 0.0f };
                newNode = new Node{ label, position };
                newTile = new Tile{ nullptr, mapTexture, pathRect , gridPosition , 1.0 , false, true, newNode };
                m_tiles.push_back(newTile);
                label++;
                break;
            case 'G':
                newTile = new Tile{ nullptr, mapTexture, grassRect , gridPosition , 1.0 , false, true, nullptr, tile};
                m_tiles.push_back(newTile);
                break;
            case 'F':
                newTile = new Tile{ nullptr, mapTexture, flowerRect , gridPosition , 1.0 , false };
                m_tiles.push_back(newTile);
                break;
            case 'T':
                childTile = new Tile{ nullptr, mapTexture, greenTreeRect, gridPosition, 1.2, true, false };
                newTile = new Tile{ childTile, mapTexture, grassRect, gridPosition, 1.0, false, false };
                m_tiles.push_back(newTile);
                m_tiles.push_back(childTile);
                break;
            case 'O':
                childTile = new Tile{ nullptr, mapTexture, orangeTreeRect, gridPosition, 2.0, true, false };
                newTile = new Tile{ childTile, mapTexture, grassRect, gridPosition, 1.0, false, false };
                m_tiles.push_back(newTile);
                m_tiles.push_back(childTile);
                break;
            case 'R':
                newTile = new Tile{ nullptr, mapTexture, rightGrassRect , gridPosition , 1.0 , false };
                m_tiles.push_back(newTile);
                break;
            case 'L':
                newTile = new Tile{ nullptr, mapTexture, leftGrassRect , gridPosition , 1.0 , false };
                m_tiles.push_back(newTile);
                break;
            case 'B':
                newTile = new Tile{ nullptr, mapTexture, bottomRect , gridPosition , 1.0 , false };
                m_tiles.push_back(newTile);
                break;
            case 'N':
                newTile = new Tile{ nullptr, mapTexture, topRect , gridPosition , 1.0 , false };
                m_tiles.push_back(newTile);
                break;
            case '1':
                newTile = new Tile{ nullptr, mapTexture, bottomLeftGrassCorner , gridPosition , 1.0 , false };
                m_tiles.push_back(newTile);
                break;
            case '2':
                newTile = new Tile{ nullptr, mapTexture, topRightGrassCorner , gridPosition , 1.0 , false };
                m_tiles.push_back(newTile);
                break;
            case 'C':
                newTile = new Tile{ nullptr, mapTexture, buildingRect , gridPosition , 6.0 , true, false };
                m_tiles.push_back(newTile);
                gridPosition.x += 210;
                m_tiles.push_back(new Tile{ newTile, mapTexture, doorRect , gridPosition , 2.0 , true, false });
                break;
            case 'S':
                newTile = new Tile{ nullptr, mapTexture, rockRect , gridPosition , 1.0 , false };
                m_tiles.push_back(newTile);
                break;
            case 'W':
                childTile = new Tile{ nullptr, mapTexture, wellRect, gridPosition, 3.0, true, false };
                newTile = new Tile{ childTile, mapTexture, rockRect, gridPosition, 1.0, false, false };
                m_tiles.push_back(newTile);
                m_tiles.push_back(childTile);
                break;
            case 'J':
                childTile = new Tile{ nullptr, mapTexture, stockRect, gridPosition, 3.0, true, false };
                newTile = new Tile{ childTile, mapTexture, rockRect, gridPosition, 1.0, false, false };
                m_tiles.push_back(newTile);
                m_tiles.push_back(childTile);
                break;
            case 'V':
                childTile = new Tile{ nullptr, mapTexture, boardRect, gridPosition, 1.2, false, false };
                newTile = new Tile{ childTile, mapTexture, rockRect, gridPosition, 1.0, false, false };
                m_tiles.push_back(newTile);
                m_tiles.push_back(childTile);
                break;
            default:
                printf("Unexpected symbol %c at (%d, %d)", tile, x, y);
                break;
            }
        }
    }

    /// resizing tile so that they are not stretched to tile dimension(especially if scale is more than 1)
    for (auto& tile : m_tiles) {
        tile->resizeTile();
        if (tile->tileNode != nullptr) {
            walkableTileNodes.emplace_back(tile->tileNode);
        }
    }

    std::sort(m_tiles.begin(), m_tiles.end(), [](const Tile* tile1, const Tile* tile2) {
        return tile1->scale < tile2->scale;
    });


}

void Level::clear() {
    for (auto& tile : m_tiles) {
        delete tile;
    }
    for (auto& nodes : walkableTileNodes) {
        delete nodes;
    }
    m_tiles.clear();
    walkableTileNodes.clear();

   
}

void Level::drawTiles()
{
    SDL_SetRenderDrawColor(scene->game->getRenderer(), 255, 255, 255, 255);

    for (size_t i = 0; i < m_tiles.size(); ++i) {
        SpriteSheet::draw(scene->game->getRenderer(), m_tiles[i]->tileTexture, m_tiles[i]->uvCoords, m_tiles[i]->destCoords, m_tiles[i]->scale, m_tiles[i]->needsResizing);
    }
    drawTopTileOutline();
}
       

bool Level::isMouseOverTile(const Tile* tile) {
    const Tile& tempTile = (tile->child) ? *(tile->child) : *tile;

    SDL_Rect adjustedRect = tempTile.destCoords;

    if (tempTile.needsResizing) {
        float aspectRatio = static_cast<float>(tempTile.uvCoords.w) / tempTile.uvCoords.h;
        adjustedRect.w = static_cast<int>(tempTile.destCoords.h * aspectRatio);
    }

    //check if the mouse position is within the bounds of the rectangle
    bool withinXBounds = mousePosX >= adjustedRect.x && mousePosX <= adjustedRect.x + adjustedRect.w;
    bool withinYBounds = mousePosY >= adjustedRect.y && mousePosY <= adjustedRect.y + adjustedRect.h;

    return withinXBounds && withinYBounds;
}

Node* Level::getTileNodeUnderMouse() {
    for (auto& tile : m_tiles) {
        if (isMouseOverTile(tile)) {
            return tile->tileNode;
        }
    }
    return nullptr;
}


void Level::levelHandleEvents(const SDL_Event& event)
{
    switch (event.type) {
    case SDL_MOUSEMOTION:
        mousePosX = event.motion.x;
        mousePosY = event.motion.y;
        break;
    default:
        break;
    }
}

bool Level::canPlaceEntity() {
    // Can place a character if the topmost tile under the mouse is walkable
    return topTile != nullptr && topTile->isWalkable;
}

void Level::drawTopTileOutline() {
    Tile* drawTile = nullptr;

    // iterating backwards to make fewer iterations because biggest scaled tiles are at the back of the vector
    for (auto it = m_tiles.rbegin(); it != m_tiles.rend(); ++it) {
        Tile* currentTile = *it;
        if (isMouseOverTile(*it)) {
            if (drawTile == nullptr || currentTile->scale > drawTile->scale)
            drawTile = *it;
            topTile = *it;
            break;  // Break because no other tile can be above this one
        }
    }

    if (drawTile != nullptr) {
        SDL_SetRenderDrawColor(scene->game->getRenderer(), 255, 255, 255, 255); // white
        SDL_Rect outlineRect = drawTile->destCoords;  //draw the outline
        SDL_RenderDrawRect(scene->game->getRenderer(), &outlineRect);
    }

    if (drawTile != nullptr && drawTile->scale > 1.0f) {
        SDL_SetRenderDrawColor(scene->game->getRenderer(), 255, 0, 0, 255); // red
        SDL_Rect outlineRect = drawTile->destCoords;  //draw the outline
        SDL_RenderDrawRect(scene->game->getRenderer(), &outlineRect);
    }
}


void Tile::resizeTile()
{
   
    if (needsResizing)
    {
        int originalWidth = destCoords.w;
        int originalHeight = destCoords.h;

        float aspectRatio = static_cast<float>(uvCoords.w) / uvCoords.h;

        destCoords.w = static_cast<int>(originalHeight * aspectRatio * scale);
        destCoords.h = static_cast<int>(originalHeight * scale);

        // Adjust y-position to make the texture scale upwards
        destCoords.y -= (destCoords.h - originalHeight);
    }
    else {
        destCoords.w *= scale;
        destCoords.h *= scale;
    }
}
