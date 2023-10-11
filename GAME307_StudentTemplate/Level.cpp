//#include "Level.h"
//#include "fstream"
//#include <iostream>
//
//
//Level::Level()
//{
//}
//
//Level::Level(const std::string& fileName)
//{
//	std::ifstream file;
//	file.open(fileName);
//	
//	//error checking
//	if (file.fail()) {
//		std::cerr << ("failed to open " + fileName) << std::endl;
//	}
//
//	std::string temp;
//
//	std::getline(file, temp);
//
//	while (std::getline(file, temp)) {
//		m_levelData.push_back(temp);
//	}
//}
//
//Level::~Level()
//{
//}
//
//void Level::draw(SDL_Renderer* renderer, const SDL_Rect& dims_, const SDL_Rect& color_)
//{
//	SDL_SetRenderDrawColor(renderer, color_.x, color_.y, color_.w, color_.h);
//	
//	SDL_RenderFillRect(renderer, &dims_);
//}
//
//void Level::drawTiles(SDL_Renderer* renderer, SDL_Window* window)
//{
//	
//	int w = 0;
//	int h = 0;
//	SDL_GetWindowSize(window, &w, &h);
//	for (int y = 0; y < m_levelData.size(); y++) {
//		for (int x = 0; x < m_levelData[y].size(); x++) {
//			
//			char tile = m_levelData[y][x];
//			 float a = ((float)w / (float)getWidth());
//			 int b = round(a) * x;
//			
//			SDL_Rect destRect  {x * ceil((float)w / (float)getWidth()),
//								y * ceil((float)h / (float)getHeight()),
//								ceil((float)w / (float)getWidth()),
//								ceil((float)h / (float)getHeight())};
//
//			SDL_Rect whiteColor{ 255,177,84,255 };
//			SDL_Rect blackColor{ 53,75,143,0 };
//
//			switch (tile) {
//			case 'W':
//				draw(renderer,
//					destRect,
//					whiteColor);
//				break;
//			case 'G':
//				draw(renderer,
//					destRect,
//					blackColor);
//				break;
//			case 'B':
//			case 'L':
//				draw(renderer,
//					destRect,
//					blackColor);
//				break;
//			case '@':
//				draw(renderer,
//					destRect,
//					whiteColor);
//				break;
//			case 'Z':
//				draw(renderer,
//					destRect,
//					blackColor);
//				break;
//			case '.':
//				draw(renderer,
//					destRect,
//					whiteColor);
//				break;
//			default:
//				printf("Unexpected symbol %c at (%d, %d)", tile, x, y);
//				break;
//			}
//		}
//	}
//}
//
//char Level::getTile(int x, int y)
//{
//	return m_levelData[y][x];
//}






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
	
	spriteSheet = new SpriteSheet();

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
	int width = 0;
	int height = 0;
	SDL_GetWindowSize(window, &width, &height);

	spriteSheet->init(renderer);
	spriteSheet->LoadMapSurface("Sprites/tilemap.png");

	SDL_Rect whiteColor{ 255,177,84,255 };
	SDL_Rect blackColor{ 53,75,143,0 };
	SDL_Rect rect = spriteSheet->GetUVTile(0, 0);
	SDL_Rect source = spriteSheet->GetSourceRect();
	

	for (int y = 0; y < m_levelData.size(); y++) {
		for (int x = 0; x < m_levelData[y].size(); x++) {
			
			char tile = m_levelData[y][x];

			SDL_Rect destRect{ x * ceil((float)rect.w / (float)width),
					  y * ceil((float)rect.h / (float)height),
					  ceil((float)rect.w / (float)height),
					  ceil((float)rect.h / (float)height) };
			

			switch (tile) {
			case 'W':
				/*draw(renderer,
					destRect,
					whiteColor);*/
				//SDL_RenderCopy(renderer, spriteSheet->getTexture(), nullptr, &spriteSheet->GetUVTile(0, 1));
				break;
			case 'G':
				/*draw(renderer,
					destRect,
					blackColor);*/
				SDL_RenderCopy(renderer, spriteSheet->getTexture(), &rect, &destRect);

				break;
			default:
				printf("Unexpected symbol %c at (%d, %d)", tile, x, y);
				break;
			}
		}
	}
}

char Level::getTile(int x, int y)
{
	return m_levelData[y][x];
}

SDL_Rect Level::getWoorldCoordTile(const SDL_Rect& rect, const int& windowW, const int& windowH) {
	SDL_Rect destRect{rect.x * ceil((float)rect.w / (float)windowW),
					  rect.y* ceil((float)rect.h / (float)windowH),
					  ceil((float)rect.w / (float)windowW),
					  ceil((float)rect.h / (float)windowH) };
	return destRect;
}