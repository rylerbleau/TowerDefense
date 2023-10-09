#include "Level.h"
#include "fstream"
#include <iostream>


Level::Level()
{
}

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

Level::~Level()
{
}

void Level::draw(SDL_Renderer* renderer, const SDL_Rect& dims_, const SDL_Rect& color_)
{
	SDL_SetRenderDrawColor(renderer, color_.x, color_.y, color_.w, color_.h);
	
	SDL_RenderFillRect(renderer, &dims_);
}

void Level::drawTiles(SDL_Renderer* renderer, SDL_Window* window)
{
	
	int w = 0;
	int h = 0;
	SDL_GetWindowSize(window, &w, &h);
	for (int y = 0; y < m_levelData.size(); y++) {
		for (int x = 0; x < m_levelData[y].size(); x++) {
			
			char tile = m_levelData[y][x];
			 float a = ((float)w / (float)getWidth());
			 int b = round(a) * x;
			
			 SDL_Rect destRect{ x * round((float)w / (float)getWidth()),
								y * round((float)h / (float)getHeight()),
								round((float)w / (float)getWidth()),
								round((float)h / (float)getHeight())};

			SDL_Rect whiteColor{ 255,177,84,255 };
			SDL_Rect blackColor{ 53,75,143,0 };

			switch (tile) {
			case 'R':
				draw(renderer,
					destRect,
					whiteColor);
				break;
			case 'G':
				draw(renderer,
					destRect,
					blackColor);
				break;
			case 'B':
			case 'L':
				draw(renderer,
					destRect,
					blackColor);
				break;
			case '@':
				draw(renderer,
					destRect,
					whiteColor);
				break;
			case 'Z':
				draw(renderer,
					destRect,
					blackColor);
				break;
			case '.':
				draw(renderer,
					destRect,
					whiteColor);
				break;
			default:
				printf("Unexpected symbol %c at (%d, %d)", tile, x, y);
				break;
			}
		}
	}
}

std::vector<std::string> Level::getTile()
{
	return std::vector<std::string>();
}
