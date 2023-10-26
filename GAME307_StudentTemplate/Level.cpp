#include "Level.h"
#include "fstream"
#include <iostream>
#include <random>
#include "SDL_image.h"
#include "Character.h"

Level::Level(const std::string& fileName)
{
	std::ifstream file;
	file.open(fileName);

	/// error checking
	if (file.fail()) {
		std::cerr << ("failed to open " + fileName) << std::endl;
	}

	std::string temp;
	std::getline(file, temp);
	while (std::getline(file, temp)) {
		m_levelData.push_back(temp);
	}
}

Level::~Level() {}

void Level::LoadMap(SDL_Renderer* renderer_, const int& tileSizeX, const int& tileSizeY, const char* filename)
{
	renderer = renderer_;

	this->tileSizeX = tileSizeX;
	this->tileSizeY = tileSizeY;

	surface = IMG_Load(filename);
	if (surface == nullptr) {
		throw std::runtime_error("Incorrect filepath");
	}

	if (texture == nullptr) {
		texture = SDL_CreateTextureFromSurface(renderer, surface);
	}

	SDL_FreeSurface(surface);
}

void Level::draw(SDL_Renderer* renderer, const SDL_Rect& uvRect, SDL_Rect destRect, float scale /*= 1.0f*/, bool needsResizing /*= false*/)
{

	if (needsResizing)
	{
		int originalWidth = destRect.w;
		int originalHeight = destRect.h;

		float aspectRatio = static_cast<float>(uvRect.w) / uvRect.h;
	
		destRect.w = static_cast<int>(originalHeight * aspectRatio * scale);
		destRect.h = static_cast<int>(originalHeight * scale);

		// Adjust y-position to make the texture scale upwards
		destRect.y -= (destRect.h - originalHeight);

		SDL_RenderCopy(renderer, texture, &uvRect, &destRect);
	}
	else {
		destRect.w *= scale;
		destRect.h *= scale;

		SDL_RenderCopy(renderer, texture, &uvRect, &destRect);
	}
}

void Level::clear() {
	for (auto tile : m_tiles) {
		delete tile;
	}
	m_tiles.clear();
}

void Level::drawTiles(SDL_Renderer* renderer, SDL_Window* window)
{
	clear();

	SpriteSheet::QuerySpriteSheet(tileSizeX, tileSizeY, texture);

	int width = 0;
	int height = 0;
	SDL_GetWindowSize(window, &width, &height);

	static SDL_Rect pathRect = SpriteSheet::GetUVTile(1, 8);
	static SDL_Rect grassRect = SpriteSheet::GetUVTile(1, 10);
	static SDL_Rect leftGrassRect = SpriteSheet::GetUVTile(0, 8);
	static SDL_Rect bottomLeftGrassCorner = SpriteSheet::GetUVTile(0, 7);
	static SDL_Rect topLeftGrassCorner = SpriteSheet::GetUVTile(0, 9);
	static SDL_Rect rightGrassRect = SpriteSheet::GetUVTile(2, 8);
	static SDL_Rect bottomRightGrassRect = SpriteSheet::GetUVTile(2, 7);
	static SDL_Rect topRightGrassCorner = SpriteSheet::GetUVTile(2, 9);
	static SDL_Rect bottomRect = SpriteSheet::GetUVTile(1, 7);
	static SDL_Rect topRect = SpriteSheet::GetUVTile(1, 9);

	static SDL_Rect flowerRect = SpriteSheet::GetUVTile(2, 10);
	static SDL_Rect greenTreeRect = SpriteSheet::GetUVTile(4, 8);
	static SDL_Rect orangeTreeRect = SpriteSheet::GetSizedUVTile(3, 9, 1, 2);
	static SDL_Rect buildingRect = SpriteSheet::GetSizedUVTile(0, 4, 4, 3);
	static SDL_Rect doorRect = SpriteSheet::GetUVTile(1, 3);
	static SDL_Rect rockRect = SpriteSheet::GetUVTile(7, 7);
	static SDL_Rect wellRect = SpriteSheet::GetSizedUVTile(8, 2, 1, 2);
	static SDL_Rect stockRect = SpriteSheet::GetSizedUVTile(8, 5, 3, 3);
	static SDL_Rect boardRect = SpriteSheet::GetUVTile(11, 4);

	static const SDL_Rect worldTileCoords{ ceil((float)width / (float)getWidth()),
							  ceil((float)height / (float)getHeight()),
							  ceil((float)width / (float)getWidth()),
							  ceil((float)height / (float)getHeight()) };
	/// <summary>
	/// Tile drawing happens here. When it detectsa character ir is going to scan through switch statement
	/// to determine the attributes for that tile that is going to be rendered at that grid location
	/// than it is pushed in the vector of Tile pointers after what i am going to sort the vector by its scale 
	/// so that sprites won't be overdrawn
	/// </summary>

	for (int y = 0; y < m_levelData.size(); y++) {
		for (int x = 0; x < m_levelData[y].size(); x++) {

			char tile = m_levelData[y][x];

			SDL_Rect gridRect = worldTileCoords;
			gridRect.x *= x;
			gridRect.y *= y;

			switch (tile) {
			case 'P':
				m_tiles.push_back(new Tile{ pathRect , gridRect , 1.0 , false });
				break;
			case 'G':
				m_tiles.push_back(new Tile{ grassRect , gridRect , 1.0 , false });
				break;
			case 'F':
				m_tiles.push_back(new Tile{ grassRect , gridRect , 1.0 , false });
				m_tiles.push_back(new Tile{ flowerRect , gridRect , 1.0 , false });
				break;
			case 'T':
				m_tiles.push_back(new Tile{ grassRect , gridRect , 1.0 , false });
				m_tiles.push_back(new Tile{ greenTreeRect , gridRect , 1.0 , true });
				break;
			case 'O':
				m_tiles.push_back(new Tile{ grassRect , gridRect , 1.0 , false });
				m_tiles.push_back(new Tile{ orangeTreeRect , gridRect , 2.0 , true });
				break;
			case 'R':
				m_tiles.push_back(new Tile{ rightGrassRect , gridRect , 1.0 , false });
				break;
			case 'L':
				m_tiles.push_back(new Tile{ leftGrassRect , gridRect , 1.0 , false });
				break;
			case 'B':
				m_tiles.push_back(new Tile{ bottomRect , gridRect , 1.0 , false });
				break;
			case 'N':
				m_tiles.push_back(new Tile{ topRect , gridRect , 1.0 , false });
				break;
			case '1':
				m_tiles.push_back(new Tile{ bottomLeftGrassCorner , gridRect , 1.0 , false });
				break;
			case '2':
				m_tiles.push_back(new Tile{ topRightGrassCorner , gridRect , 1.0 , false });
				break;
			case 'C':
				m_tiles.push_back(new Tile{ buildingRect , gridRect , 6.0 , true });
				gridRect.x += 210;
				m_tiles.push_back(new Tile{ doorRect , gridRect , 2.0 , true });
				break;
			case 'S':
				m_tiles.push_back(new Tile{ rockRect , gridRect , 1.0 , false });
				break;
			case 'W':
				m_tiles.push_back(new Tile{ rockRect, gridRect, 1.0, false });
				m_tiles.push_back(new Tile{ wellRect, gridRect, 3.0, true });
				break;
			case 'J':
				m_tiles.push_back(new Tile{ rockRect , gridRect , 1.0 , false });
				m_tiles.push_back(new Tile{ stockRect , gridRect , 3.0 , true });
				break;
			case 'V':
				m_tiles.push_back(new Tile{ rockRect , gridRect , 1.0 , false });
				m_tiles.push_back(new Tile{ boardRect , gridRect , 1.0 , false });
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

	for (auto& tile : m_tiles) {
		SpriteSheet::draw(renderer, texture, tile->uvCoords, tile->destCoords, tile->scale, tile->needsResizing);

		if (mousePosX >= tile->destCoords.x && mousePosX <= tile->destCoords.x + tile->destCoords.w &&
			mousePosY >= tile->destCoords.y && mousePosY <= tile->destCoords.y + tile->destCoords.h) {


			// Set color for the outline rectangle (for example white)
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

			// Draw the outline of the rectangle
			SDL_RenderDrawRect(renderer, &tile->destCoords);
		}
	}

}

char Level::getTile(int x, int y)
{
	return m_levelData[y][x];
}

void Level::levelHandleEvents(const SDL_Event& event, std::vector<Character*>& characters, Scene* scene)
{
	switch (event.type) {
	case SDL_MOUSEMOTION:
		 mousePosX = event.motion.x;
		 mousePosY = event.motion.y;
		break; 
	case SDL_MOUSEBUTTONDOWN:
		for (auto tile : m_tiles) {
			if (mousePosX >= tile->destCoords.x && mousePosX <= tile->destCoords.x + tile->destCoords.w &&
				mousePosY >= tile->destCoords.y && mousePosY <= tile->destCoords.y + tile->destCoords.h) {
				
				Character* character = new Character();
				character->OnCreate(scene);
				character->setTextureWith("Sprites/hero.png");
				characters.push_back(character);
			}
		}
	default:
		break;
	}
}
