#include "Path.h"

Path::Path(std::vector<Node*> nodes_)
{
	path.resize(nodes_.size());
	path = nodes_;
	currentNode = 0;
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
