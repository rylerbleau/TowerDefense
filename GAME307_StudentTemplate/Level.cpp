#include "Level.h"
#include "fstream"
#include <iostream>
#include <random>


Level::Level(const std::string& fileName, SDL_Renderer* renderer_)
{
	spriteSheet = new SpriteSheet();

	spriteSheet->init(renderer_, 12, 11);
	spriteSheet->LoadMapSurface("Sprites/tilemap.png");
	
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

void Level::draw(SDL_Renderer* renderer, const SDL_Rect& uvRect, SDL_Rect destRect, float scale /*= 1.0f*/, bool needsResizing /*= false*/)
{
	if(needsResizing)
	{
		int originalWidth = destRect.w;
		int originalHeight = destRect.h;

		float aspectRatio = static_cast<float>(uvRect.w) / uvRect.h;

		destRect.w = static_cast<int>(originalHeight * aspectRatio * scale);
		destRect.h = static_cast<int>(originalHeight * scale);

		// Adjust y-position to make the texture scale upwards
		destRect.y -= (destRect.h - originalHeight);

		SDL_RenderCopy(renderer, spriteSheet->getTexture(), &uvRect, &destRect);
	}
	else {
		destRect.w *= scale;
		destRect.h *= scale;
		SDL_RenderCopy(renderer, spriteSheet->getTexture(), &uvRect, &destRect);
	}
}


void Level::drawTiles(SDL_Renderer* renderer, SDL_Window* window)
{
	int width = 0;
	int height = 0;
	SDL_GetWindowSize(window, &width, &height);

	SDL_Rect pathRect = spriteSheet->GetUVTile(1, 8);
	SDL_Rect grassRect = spriteSheet->GetUVTile(1, 10);
	SDL_Rect leftGrassRect = spriteSheet->GetUVTile(0, 8);
	SDL_Rect bottomLeftGrassCorner = spriteSheet->GetUVTile(0, 7);
	SDL_Rect topLeftGrassCorner = spriteSheet->GetUVTile(0, 9);
	SDL_Rect rightGrassRect = spriteSheet->GetUVTile(2, 8);
	SDL_Rect bottomRightGrassRect = spriteSheet->GetUVTile(2, 7);
	SDL_Rect topRightGrassCorner = spriteSheet->GetUVTile(2, 9);
	SDL_Rect bottomRect = spriteSheet->GetUVTile(1, 7);
	SDL_Rect topRect = spriteSheet->GetUVTile(1, 9);

	SDL_Rect flowerRect = spriteSheet->GetUVTile(2, 10);
	SDL_Rect greenTreeRect = spriteSheet->GetUVTile(4, 8);
	SDL_Rect orangeTreeRect = spriteSheet->GetSizedUVTile(3, 9, 1, 2);
	SDL_Rect buildingRect = spriteSheet->GetSizedUVTile(0, 4, 4, 3);
	SDL_Rect doorRect = spriteSheet->GetUVTile(1, 3);
	SDL_Rect rockRect = spriteSheet->GetUVTile(7, 7);
	SDL_Rect wellRect = spriteSheet->GetSizedUVTile(8, 2, 1, 2);
	SDL_Rect stockRect = spriteSheet->GetSizedUVTile(8, 5, 3, 3);
	SDL_Rect boardRect = spriteSheet->GetUVTile(11, 4);

	SDL_Rect worldTileCoords{ ceil((float)width / (float)getWidth()),
							  ceil((float)height / (float)getHeight()),
							  ceil((float)width / (float)getWidth()),
							  ceil((float)height / (float)getHeight()) };

	for (int y = 0; y < m_levelData.size(); y++) {
		for (int x = 0; x < m_levelData[y].size(); x++) {
			
			char tile = m_levelData[y][x];

			SDL_Rect gridRect = worldTileCoords;
			gridRect.x *= x;
			gridRect.y *= y;

			switch (tile) {
			case 'P':
				draw(renderer, pathRect, gridRect);
				break;
			case 'G':
				draw(renderer, grassRect, gridRect);
				break;
			case 'F':
				draw(renderer, grassRect, gridRect);
				draw(renderer, flowerRect, gridRect);
				break;
			case 'T':
				draw(renderer, grassRect, gridRect, 1);
				draw(renderer, greenTreeRect, gridRect, 1, true);
				break;
			case 'O':
				draw(renderer, grassRect, gridRect, 1);
				draw(renderer, orangeTreeRect, gridRect, 2, true);
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
}

char Level::getTile(int x, int y)
{
	return m_levelData[y][x];
}