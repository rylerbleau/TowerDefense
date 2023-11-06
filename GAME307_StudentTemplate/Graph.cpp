#include "Graph.h"

Graph::Graph()
{
}

Graph::~Graph()
{
}

bool Graph::OnCreate(std::vector<Node*> nodes_)
{
	int numNodes = nodes_.size();
	costs.resize(numNodes);

	for (int i = 0; i < numNodes; i++) {


		if (i != nodes_[i]->GetLabel()) {
			std::cerr << "node with label 'i' not in the 'i'th position for " << i << std::endl;
			return false;
		}

		// set up connections
		nodes[i] = nodes_[i];

		costs[i].resize(numNodes);
		for (int j = 0; j < numNodes; j++) {
			// initalizes all connections as 0
			costs[i][j] = 0.0f;

		}
	}

	return true;
}

void Graph::OnDestroy()
{
}

int Graph::NumNodes()
{
	return nodes.size();
}

void Graph::AddWeightedConnection(int fromNode, int toNode, float weight)
{
	costs[fromNode][toNode] = weight;
}

std::vector<int> Graph::Neighbours(int fromNode)
{
	std::vector<int> results = {};
	// loop thru for fromNode
	for (int i = 0; i < NumNodes(); i++) {

		// check if there is connection
		if (costs[fromNode][i] > 0.0f) {
			// return which indices are connected, dont care abt weight rn
			results.push_back(i);
		}
	}
	return results;
}

//  Created by Gail Harris on 2020-Oct-11.
//  Copyright © 2020 Gail Harris. All rights reserved.
//

struct NodeAndPriority
{
public:

	// member variables
	Node* node;
	float priority;

	// constructors
	// We'll use the "universal" constructor when needed.
	// See: https://learn.microsoft.com/en-us/cpp/cpp/initializing-classes-and-structs-without-constructors-cpp?view=msvc-170

	// useful functions
	void print()
	{
		printf("Node: %i, priority: %.2f \n", node->GetLabel(), priority);
	}

	
};

struct ComparePriority
{
	bool operator()(NodeAndPriority const& lhs, NodeAndPriority const& rhs)
	{
		// make it a min queue
		return lhs.priority > rhs.priority;
	}
};

std::vector<int> Graph::Dijkstra(int start, int goal)
{
	float new_cost;
	float priority;
	Node* current = nodes[start];

	std::priority_queue<NodeAndPriority, std::deque<NodeAndPriority>, ComparePriority > frontier;

	frontier.push(NodeAndPriority{ current, 0.0f });
	//track solution path
	std::vector<int> came_from;
	came_from.resize(NumNodes());

	// cost so far
	std::map<int, float> CSF;
	CSF[start] = 0.0f;

	while (!frontier.empty()) {
		current = frontier.top().node;
		frontier.pop();
		if (current->GetLabel() == goal)
			break;
		for (auto& neighbour : Neighbours(current->GetLabel())) {
			new_cost = CSF[current->GetLabel()] + costs[current->GetLabel()][neighbour];
			if (CSF.find(neighbour) == CSF.end() || new_cost < CSF[neighbour]) {
				CSF[neighbour] = new_cost;
				priority = new_cost;
				Node* newNode = new Node{ neighbour };
				frontier.push(NodeAndPriority{ newNode, priority });
				came_from[neighbour] = current->GetLabel();
			}
		}
	}

	std::vector<int> path;

	path.emplace_back(goal);
	for (int i = goal; i != start; ) {	
		path.emplace_back(came_from[i]);
		i = came_from[i];
	}

	std::reverse(path.begin(), path.end());

	return path;
}