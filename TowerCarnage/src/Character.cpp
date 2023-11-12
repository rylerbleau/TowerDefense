#include "Character.h"
#include "SpriteSheet.h"
#include "StaticBody.h"
#include "KinematicSeek.h"
#include "KinematicSeperation.h"
#include "KinematicArrive.h"
#include "FollowAPath.h"

#include <random>
#include <vector>

static std::mt19937 randomEngine(time(nullptr));
static std::uniform_real_distribution<float> scaleGenerator(0.8f, 1.0f);

Character::~Character()
{
}

bool Character::OnCreate(Scene* scene_, Graph* graph_, Vec3 pos /*= Vec3(5.0f, 5.0f, 0.0f)*/)
{
	scene = scene_;
	scale = scaleGenerator(randomEngine);
	// Configure and instantiate the body to use for the demo
	if (!body)
	{
		float radius = 2.5;
		float orientation = 0.0f;
		float rotation = 0.0f;
		float angular = 0.0f;
		float maxSpeed = 1.0f;
		float maxAcceleration = 10.0f;
		float maxRotation = 2.0f;
		float maxAngular = 10.0f;
		body = new StaticBody(pos, orientation, maxSpeed, maxRotation);
	}

	if (!body)
	{
		return false;
	}
	graph = graph_;

	return true;
}

bool Character::setTextureWith(string file)
{
	SDL_Surface* image = IMG_Load(file.c_str());
	if (image == nullptr) {
		throw std::runtime_error("Incorrect filepath");
		return false;
	}
	SDL_Window* window = scene->getWindow();
	SDL_Renderer* renderer = SDL_GetRenderer(window);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
	if (!texture) 
	{
		throw std::runtime_error("Failed to create texture");
		return false;
	}
	body->setTexture(texture);
	return true;
}


void Character::Update(float deltaTime, std::vector<Character*> characters, int index)
{
	std::vector<StaticBody* > staticBodies;
	staticBodies.resize(characters.size());
	for (uint32_t i = 0; i < characters.size(); i++) {
		staticBodies[i] = characters[i]->body;
	}
	// create a new overall steering output
	KinematicSteeringOutput* steering = new KinematicSteeringOutput();
	// This creates the separation plus seek behaviour(might switch to arrive)
	SeekAndSeparationSteering(*steering, staticBodies, 1.5f, index);
	body->Update(deltaTime, steering);
	delete steering;
}

Node* Character::findNearestWalkableNode() {
	Node* closestNode = nullptr;
	float maxDistance = INT32_MAX;
		Vec3 characterPos = body->getPos();
		for (int i = 0; i < graph->NumNodes(); i++) {
			float tileDistance = VMath::distance(characterPos, graph->GetNode(i)->getPosition());
			if (tileDistance < maxDistance) {
				maxDistance = tileDistance;
				closestNode = graph->GetNode(i);
			}
		}
	
	return closestNode;
}

void Character::updatePath(Node* endNode_)
{
	if (path);
	delete path;

	path = new Path();
	Node* startNode = findNearestWalkableNode();
	endNode = endNode_;

	if (startNode && endNode) {
		int startIndex = startNode->GetLabel();
		int endIndex = endNode->GetLabel();

		std::vector<int> nodes = graph->Dijkstra(startIndex, endIndex);

		for (auto& node : nodes) {
			path->addNode(graph->GetNode(node));
		}
	}
}

void Character::SeekAndSeparationSteering(KinematicSteeringOutput& steering, std::vector<StaticBody* > staticBodies, float threshhold, int index)
{
	std::vector<KinematicSteeringOutput*> steering_outputs;

	
	// using the target, calculate and set values in the overall steering output
	FollowAPath* steering_algorithm = steering_algorithm = new FollowAPath(body, path);
	steering_outputs.push_back(steering_algorithm->getSteering());

	KinematicSeperation* separation = new KinematicSeperation(staticBodies, 1.5f, index);
	steering_outputs.push_back(separation->GetSteering());
	
	for (int i = 0; i < steering_outputs.size(); i++) {
		if (steering_outputs[i]) {
			steering += *steering_outputs[i];
		}
		delete steering_outputs[i];
	}
	if (steering_algorithm) {
		delete steering_algorithm;
	}
	if (separation) {
		delete separation;
	}
}


void Character::HandleEvents(const SDL_Event& event)
{

}

void Character::render()
{
	SDL_Renderer* renderer = scene->game->getRenderer();
	Matrix4 projectionMatrix = scene->getProjectionMatrix();

	SDL_Rect square;
	Vec3 screenCoords;
	int    w, h;

	// notice use of "body" in the following
	SDL_QueryTexture(body->getTexture(), nullptr, nullptr, &w, &h);
	w = static_cast<int>(w * scale);
	h = static_cast<int>(h * scale);
	screenCoords = projectionMatrix * body->getPos();
	square.x = static_cast<int>(screenCoords.x - 0.5f * w);
	square.y = static_cast<int>(screenCoords.y - 0.5f * h);
	square.w = w;
	square.h = h;

	// Convert character orientation from radians to degrees.
	float orientation = body->getOrientation() * 180.0f / M_PI;

	int numFrames = 0;
	int FRAME_SPEED = 50;

	SpriteSheet::QuerySpriteSheet(8, 3, body->getTexture());

	if (body->getVel().x > 0.0f && abs(body->getVel().x) > abs(body->getVel().y)) {
		direction = Direction::RIGHT;
	}
	else if (body->getVel().x < 0.0f && abs(body->getVel().x) > abs(body->getVel().y)) {
		direction = Direction::LEFT;
	}
	else if (body->getVel().y > 0.0f && abs(body->getVel().y) > abs(body->getVel().x)) {
		direction = Direction::FORWARD;
	}
	else if (body->getVel().y < 0.0f && abs(body->getVel().y) > abs(body->getVel().x)) {
		direction = Direction::BACKWARD;
	}
	int startPosX = 0;
	int tileIndexY = 0;
	int tileIndexX = 0;

	SpriteSheet::QuerySpriteSheet(8, 3, body->getTexture());
	switch (direction) {
	case Direction::RIGHT:
		numFrames = 4;
		startPosX = 4;
		tileIndexY = 1;
		tileIndexX = startPosX + ((SDL_GetTicks() / FRAME_SPEED) % numFrames);
		sourceRect = SpriteSheet::GetUVTile(tileIndexX, tileIndexY);
		break;

	case Direction::LEFT:
		numFrames = 4;
		startPosX = 0;
		tileIndexY = 1;
		tileIndexX = startPosX + ((SDL_GetTicks() / FRAME_SPEED) % numFrames);
		sourceRect = SpriteSheet::GetUVTile(tileIndexX, tileIndexY);
		break;

	case Direction::BACKWARD:
		numFrames = 4;
		startPosX = 4;
		tileIndexY = 2;
		tileIndexX = startPosX + ((SDL_GetTicks() / FRAME_SPEED) % numFrames);
		sourceRect = SpriteSheet::GetUVTile(tileIndexX, tileIndexY);
		break;

	case Direction::FORWARD:
		numFrames = 4;
		startPosX = 0;
		tileIndexY = 2;
		tileIndexX = startPosX + ((SDL_GetTicks() / FRAME_SPEED) % numFrames);
		sourceRect = SpriteSheet::GetUVTile(tileIndexX, tileIndexY);
		break;
	}
	SpriteSheet::drawPlayer(renderer, body->getTexture(), sourceRect, square, this->scale, true);
}

