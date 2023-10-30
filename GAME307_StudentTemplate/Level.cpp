#include "Level.h"
#include "fstream"
#include <iostream>
#include <random>
#include "SDL_image.h"
#include "Character.h"
#include "Turret.h"

Level::Level(const std::string& fileName, Scene* scene)
    :scene(scene)
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
}

Level::~Level() {}

void Level::LoadMap(const int& tileSizeX, const int& tileSizeY, const char* filename)
{
    SDL_Surface* surface = IMG_Load(filename);
    if (surface == nullptr) {
        throw std::runtime_error("Incorrect filepath");
    }
   
    SDL_Texture* mapTexture = SDL_CreateTextureFromSurface(scene->game->getRenderer(), surface);
    if (mapTexture == nullptr) {
        throw std::runtime_error("Map Texture was not created!");
    }

    SDL_FreeSurface(surface);

    SpriteSheet::QuerySpriteSheet(tileSizeX, tileSizeY, mapTexture);

    SDL_GetWindowSize(scene->game->getWindow(), &width, &height);

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

    static const SDL_Rect worldTileCoords{ ceil((float)width / (float)getWidth()),
                              ceil((float)height / (float)getHeight()),
                              ceil((float)width / (float)getWidth()),
                              ceil((float)height / (float)getHeight()) };
    /// <summary>
    /// Here I pupulate the m_tiles vector. When it detectsa character ir is going to scan through switch statement
    /// to determine the attributes for that tile that is going to be rendered at that grid location
    /// than it is pushed in the vector of Tile pointers after what i am going to sort the vector by its scale 
    /// so that sprites won't be overdrawn
    /// </summary>

    for (size_t y = 0; y < m_levelData.size(); y++) {
        for (size_t x = 0; x < m_levelData[y].size(); x++) {

            char tile = m_levelData[y][x];

            SDL_Rect gridPosition = worldTileCoords;
            gridPosition.x *= x;
            gridPosition.y *= y;

            switch (tile) {
            case 'P':
                m_tiles.push_back(new Tile{ mapTexture, pathRect , gridPosition , 1.0 , false});
                break;
            case 'G':
                m_tiles.push_back(new Tile{ mapTexture, grassRect , gridPosition , 1.0 , false });
                break;
            case 'F':
                m_tiles.push_back(new Tile{ mapTexture, grassRect , gridPosition , 1.0 , false });
                m_tiles.push_back(new Tile{ mapTexture, flowerRect , gridPosition , 1.0 , false });
                break;
            case 'T':
                m_tiles.push_back(new Tile{ mapTexture, grassRect , gridPosition , 1.0 , false });
                m_tiles.push_back(new Tile{ mapTexture, greenTreeRect , gridPosition , 1.0 , true });
                break;
            case 'O':
                m_tiles.push_back(new Tile{ mapTexture, grassRect , gridPosition , 1.0 , false });
                m_tiles.push_back(new Tile{ mapTexture, orangeTreeRect , gridPosition , 2.0 , true });
                break;
            case 'R':
                m_tiles.push_back(new Tile{ mapTexture, rightGrassRect , gridPosition , 1.0 , false });
                break;
            case 'L':
                m_tiles.push_back(new Tile{ mapTexture, leftGrassRect , gridPosition , 1.0 , false });
                break;
            case 'B':
                m_tiles.push_back(new Tile{ mapTexture, bottomRect , gridPosition , 1.0 , false });
                break;
            case 'N':
                m_tiles.push_back(new Tile{ mapTexture, topRect , gridPosition , 1.0 , false });
                break;
            case '1':
                m_tiles.push_back(new Tile{ mapTexture, bottomLeftGrassCorner , gridPosition , 1.0 , false });
                break;
            case '2':
                m_tiles.push_back(new Tile{ mapTexture, topRightGrassCorner , gridPosition , 1.0 , false });
                break;
            case 'C':
                m_tiles.push_back(new Tile{ mapTexture, buildingRect , gridPosition , 6.0 , true });
                gridPosition.x += 210;
                m_tiles.push_back(new Tile{ mapTexture, doorRect , gridPosition , 2.0 , true });
                break;
            case 'S':
                m_tiles.push_back(new Tile{ mapTexture, rockRect , gridPosition , 1.0 , false });
                break;
            case 'W':
                m_tiles.push_back(new Tile{ mapTexture, rockRect, gridPosition, 1.0, false });
                m_tiles.push_back(new Tile{ mapTexture, wellRect, gridPosition, 3.0, true });
                break;
            case 'J':
                m_tiles.push_back(new Tile{ mapTexture, rockRect , gridPosition , 1.0 , false });
                m_tiles.push_back(new Tile{ mapTexture, stockRect , gridPosition , 3.0 , true });
                break;
            case 'V':
                m_tiles.push_back(new Tile{ mapTexture, rockRect , gridPosition , 1.0 , false });
                m_tiles.push_back(new Tile{ mapTexture, boardRect , gridPosition , 1.0 , false });
                break;
            default:
                printf("Unexpected symbol %c at (%d, %d)", tile, x, y);
                break;
            }
        }
    }

    std::sort(m_tiles.begin(), m_tiles.end(), [](const Tile* tile1, const Tile* tile2) {
        return tile1->scale < tile2->scale;
    });
}

void Level::clear() {
    for (auto tile : m_tiles) {
        delete tile;
    }
    m_tiles.clear();
}

void Level::drawTiles(SDL_Window* window, std::vector<Character*>& characters)
{

    for (const auto& tile : m_tiles) {
        SpriteSheet::draw(scene->game->getRenderer(), tile->tileTexture, tile->uvCoords, tile->destCoords, tile->scale, tile->needsResizing);

        if (mousePosX >= tile->destCoords.x && mousePosX <= tile->destCoords.x + tile->destCoords.w &&
            mousePosY >= tile->destCoords.y && mousePosY <= tile->destCoords.y + tile->destCoords.h) {

            // Set color for the outline rectangle (for example white)
            SDL_SetRenderDrawColor(scene->game->getRenderer(), 255, 255, 255, 255);

            // Draw the outline of the rectangle
            SDL_RenderDrawRect(scene->game->getRenderer(), &tile->destCoords);
            if(placeActor)
            {
                Character* character = new Character();
                Vec3 position = {
                    static_cast<float>((tile->destCoords.x + tile->destCoords.w) * scene->getxAxis()) / width,
                    scene->getyAxis() - (static_cast<float>((tile->destCoords.y + 0.5 * tile->destCoords.h) * scene->getyAxis()) / height),
                    0.0f
                };

                character->OnCreate(scene, position);
                character->setTextureWith("Sprites/hero.png");
                characters.push_back(character);
                placeActor = false;
               /* Turret* turret = new Turret("Sprites/tiles_packed.png", Vec2(6, 7), scene);
                SpriteSheet::QuerySpriteSheet(12, 10, turret->m_turretTexture);
                SDL_Rect turretUV = SpriteSheet::GetUVTile(turret->uvCoords.x, turret->uvCoords.y);
                tile->tileTexture = turret->m_turretTexture;
                tile->uvCoords = turretUV;
                placeActor = false;*/
            }
        }
    }
}

char Level::getTile(int x, int y)
{
    return m_levelData[y][x];
}

void Level::levelHandleEvents(const SDL_Event& event)
{
    switch (event.type) {
    case SDL_MOUSEMOTION:
        mousePosX = event.motion.x;
        mousePosY = event.motion.y;
        break;
    case SDL_MOUSEBUTTONDOWN:
        if(event.button.button == SDL_BUTTON_LEFT)
        placeActor = true;
        break;
    case SDL_MOUSEBUTTONUP:
        placeActor = false;
        break;
    default:
        break;
    }
}
