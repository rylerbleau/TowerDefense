#include "Path.h"

Path::Path(std::vector<Node*> nodes_)
{
	path.resize(nodes_.size());
	path = nodes_;
	currentNode = 31;
}

void Path::addNode(Node* node_)
{
	path.push_back(node_);
}

void Path::incrementCurrentNode(int step)
{
	currentNode += step;
}

MATH::Vec3 Path::getCurrentNodePositon()
{
	return path[currentNode]->getPosition();
}
