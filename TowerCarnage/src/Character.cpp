#include "Character.h"
#include "SpriteSheet.h"
#include "StaticBody.h"
#include "KinematicSeek.h"
#include "KinematicSeperation.h"
#include "KinematicSeek.h"
#include "FollowAPath.h"
#include <random>
#include "Action.h"
#include "Desicion.h"
#include "Turret.h"

static std::mt19937 randomEngine(time(nullptr));
static std::uniform_real_distribution<float> scaleGenerator(0.8f, 1.0f);

Character::~Character()
{
}

bool Character::OnCreate(Scene* scene_, Graph* graph_, Vec3 pos)
{
	scene = scene_;
	scale = scaleGenerator(randomEngine);
	maxHP = 8.0f;
	curHP = maxHP;

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
	
	// create a new overall steering output
	KinematicSteeringOutput* steering = nullptr;

	Action* action = static_cast<Action*>(desicionTree->makeDecision());
	switch (action->getLabel()) {
	case ACTION_SET::FIND_PATH:
		// This creates the separation plus seek behaviour(might switch to arrive)
		SeekAndSeparationSteering(steering, characters, 1.5f, index);
		break;
	case ACTION_SET::ARRIVE:
		SteerToArrive(steering);
		if (closestTurret && VMath::distance(body->getPos(), closestTurret->getBody()->getPos()) < closestTurret->getBody()->getRadius()) {
			closestTurret->TakeDamage(0.02f);
		}
		break;
	case ACTION_SET::DO_NOTHING:
		break;
	default:
		break;
	}

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
	Node* endNode = endNode_;

	if (startNode && endNode) {
		int startIndex = startNode->GetLabel();
		int endIndex = endNode->GetLabel();

		std::vector<int> nodes = graph->Dijkstra(startIndex, endIndex);

		for (auto& node : nodes) {
			path->addNode(graph->GetNode(node));
		}
	}
}

void Character::SeekAndSeparationSteering(KinematicSteeringOutput*& steering, std::vector<Character*> characters, float threshhold, int index)
{
	steering = new KinematicSteeringOutput();

	std::vector<StaticBody* > staticBodies;
	staticBodies.resize(characters.size());
	for (uint32_t i = 0; i < characters.size(); i++) {
		staticBodies[i] = characters[i]->body;
	}

	std::vector<KinematicSteeringOutput*> steering_outputs;
	// using the target, calculate and set values in the overall steering output
	FollowAPath* steering_algorithm = steering_algorithm = new FollowAPath(body, path);
	steering_outputs.push_back(steering_algorithm->getSteering());

	KinematicSeperation* separation = new KinematicSeperation(staticBodies, 1.5f, index);
	steering_outputs.push_back(separation->GetSteering());
	
	for (int i = 0; i < steering_outputs.size(); i++) {
		if (steering_outputs[i]) {
			*steering += *steering_outputs[i];
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

void Character::SteerToArrive(KinematicSteeringOutput*& steering) {

	KinematicArrive* steering_algorithm = new KinematicArrive(body, closestTurret->getBody());
	steering = steering_algorithm->getSteering();
	delete steering_algorithm;
}


void Character::RenderUI()
{
	SDL_Renderer* renderer = scene->game->getRenderer();
	Matrix4 projectionMatrix = scene->getProjectionMatrix();

	SDL_Rect HPoutline;
	SDL_Rect HPfill;
	Vec3 screenCoords;
	float    w = 40, h = 10;

	// notice use of "body" in the following
	
	w = static_cast<int>(w);
	h = static_cast<int>(h);
	screenCoords = projectionMatrix * body->getPos();
	
	HPoutline.y = static_cast<int>(screenCoords.y - 2.8f * h * scale);
	HPoutline.x = static_cast<int>(screenCoords.x - 1.5f * w);
	HPoutline.w = w;
	HPoutline.h = h;

	HPfill = HPoutline;
	HPfill.w = curHP / maxHP * w;
	// red for fill
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &HPfill);


	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &HPoutline);
}

bool Character::readDesicionTreeFromFile(const char* filename, std::vector<Turret*>& turrets /*= nullptr*/)
{
	if (filename == "HERO") {
		// if player is within range of turret, attack turret
		// otherwise, find path to end node
		Action* trueNode = new Action(ACTION_SET::ARRIVE);
		Action* falseNode = new Action(ACTION_SET::FIND_PATH);
		desicionTree = new TurretInRange(trueNode, falseNode, this, &turrets, &closestTurret);
		return true;
	}
	return false;
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
	square.y = static_cast<int>(screenCoords.y - 0.5 * h);
	square.x = static_cast<int>(screenCoords.x - 0.5 * w);
	square.w = w;
	square.h = h;

	// Convert character orientation from radians to degrees.

	int numFrames = 0;
	int FRAME_SPEED = 50;

	SpriteSheet::QuerySpriteSheet(8, 3, body->getTexture());

	if (body->getVel().x > 0.2f && abs(body->getVel().x) > abs(body->getVel().y)) {
		direction = Direction::RIGHT;
	}
	else if (body->getVel().x < -0.2f && abs(body->getVel().x) > abs(body->getVel().y)) {
		direction = Direction::LEFT;
	}
	else if (body->getVel().y > 0.2f && abs(body->getVel().y) > abs(body->getVel().x)) {
		direction = Direction::FORWARD;
	}
	else if (body->getVel().y < -0.2f && abs(body->getVel().y) > abs(body->getVel().x)) {
		direction = Direction::BACKWARD;
	}
	else {
		direction = Direction::IDLE;
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
	case Direction::IDLE:
	
		sourceRect = SpriteSheet::GetUVTile(4, 2);
		break;
	}
	SpriteSheet::drawPlayer(renderer, body->getTexture(), sourceRect, square, this->scale, true);
}

