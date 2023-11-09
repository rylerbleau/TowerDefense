#include "Path.h"

Path::Path(): currentNode(0), path(0){}

bool Path::OnCreate(std::vector<Node*> nodes_)
{
	if (nodes_.empty()) {
		std::runtime_error("Not enought nodes in the vector");
		return false;
	}

	path.resize(nodes_.size());
	path = nodes_;
	currentNode = 0;
	return true;
}

void Path::addNode(Node* node_)
{
	path.push_back(node_);
}

void Path::incrementCurrentNode(int step)
{
	if (currentNode != path.size() - 1) {
		currentNode += step;
	}
	else {
		currentNode = path.size() - 1;
	}
}

MATH::Vec3 Path::getCurrentNodePositon()
{
	return path[currentNode]->getPosition();
}
