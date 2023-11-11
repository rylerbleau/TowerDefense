#pragma once
#include "Node.h"
#include <vector>

class Path
{
private:
	int currentNode;
	std::vector<Node*> path;
public:
	Path();
	bool OnCreate(std::vector<Node*> nodes_);
	void addNode(Node* node_);
	void incrementCurrentNode(int step);
	MATH::Vec3 getCurrentNodePositon();
	int getPathSize() { return path.size(); }
};

