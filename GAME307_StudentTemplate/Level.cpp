#include "Level.h"
#include "fstream"
#include <iostream>
#include <random>
#include "SDL_image.h"


Level::Level(const std::string& fileName)
{
	std::ifstream file;
	file.open(fileName);

	//error checking
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


void Level::drawTiles(SDL_Renderer* renderer, SDL_Window* window)
{
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

	//m_tiles.resize(m_levelData.size() * m_levelData[0].size());
	for (int y = 0; y < m_levelData.size(); y++) {
		for (int x = 0; x < m_levelData[y].size(); x++) {
			
			char tile = m_levelData[y][x];

			SDL_Rect gridRect = worldTileCoords;
			gridRect.x *= x;
			gridRect.y *= y;

			switch (tile) {
			case 'P':
				//SpriteSheet::draw(renderer, texture, pathRect, gridRect);
				m_tiles.push_back(new Tile{ pathRect , gridRect , 1.0 , false });
				break;
			case 'G':
				SpriteSheet::draw(renderer, texture, grassRect, gridRect);
				break;
				break;
			case 'F':
				SpriteSheet::draw(renderer, texture, grassRect, gridRect);
				SpriteSheet::draw(renderer, texture, flowerRect, gridRect);
				break;
			case 'T':
				SpriteSheet::draw(renderer, texture, grassRect, gridRect);
				SpriteSheet::draw(renderer, texture, greenTreeRect, gridRect, 1, true);
				break;
			case 'O':
				SpriteSheet::draw(renderer, texture, grassRect, gridRect);
				SpriteSheet::draw(renderer, texture, orangeTreeRect, gridRect, 2 , true);
				break;
			case 'R':
				draw(renderer, rightGrassRect, gridRect, 1);
				break;
			case 'L':
				draw(renderer, leftGrassRect, gridRect, 1);
				break;
			case 'B':
				draw(renderer, bottomRect, gridRect, 1);
				break;
			case 'N':
				draw(renderer, topRect, gridRect, 1);
				break;
			case '1':
				draw(renderer, bottomLeftGrassCorner, gridRect, 1);
				break;
			case '2':
				draw(renderer, topRightGrassCorner, gridRect, 1);
				break;
			default:
				break;
			}
		}
		for (int x = 0; x < m_levelData[y].size(); x++) {

			char tile = m_levelData[y][x];

			SDL_Rect gridRect = worldTileCoords;
			gridRect.x *= x;
			gridRect.y *= y;

			switch (tile) {
			case 'C':
				draw(renderer, buildingRect, gridRect, 6, true);
				gridRect.x += 210;
				draw(renderer, doorRect, gridRect, 2, true);
				break;
			case 'S':
				draw(renderer, rockRect, gridRect, 1);
				break;
			case 'W':
				draw(renderer, rockRect, gridRect, 1);	
				draw(renderer, wellRect, gridRect, 3, true);
				break;
			case 'J':
				draw(renderer, rockRect, gridRect, 1);
				draw(renderer, stockRect, gridRect, 3, true);
				break;
			case 'V':
				draw(renderer, rockRect, gridRect, 1);
				draw(renderer, boardRect, gridRect);
				break;
			default:
				//printf("Unexpected symbol %c at (%d, %d)", tile, x, y);
				break;
			}
		}
	}

	for (auto& tile : m_tiles) {
		SpriteSheet::draw(renderer, texture, tile->uvCoords, tile->destCoords, tile->scale, tile->needsResizing);
	}
	m_tiles.clear();
	printf("%d", m_tiles.size());
}

char Level::getTile(int x, int y)
{
	return m_levelData[y][x];
}

void Level::levelHandleEvents(const SDL_Event& event)
{
	switch (event.type) {
	case SDL_MOUSEMOTION:
		int mousePosX = event.motion.x;
		int mousePosY = event.motion.y;

		//for (auto tile : m_tiles) {
		//	if (mousePosX >= tile->destCoords.x && mousePosX <= tile->destCoords.x + tile->destCoords.w &&
		//		mousePosY >= tile->destCoords.y && mousePosY <= tile->destCoords.y + tile->destCoords.h) {

		//		// Set color for the outline rectangle (for example white)
		//		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		//		// Draw the outline of the rectangle
		//		SDL_RenderDrawRect(renderer, &tile->destCoords);
		//	}
		//}
		break; // Don't forget to break after handling the case
	}

}
