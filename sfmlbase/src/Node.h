#pragma once

#include "constants.h"

#include <vector>

#include "math/Vector2D.h"

struct Node {

	struct Edge {
		Edge(float _distance, Node* _node) : node(_node), distance(_distance) { };
		Node* node;
		int distance;
	};

	Node(sf::Vector2i _posInMatrix, sf::Vector2f _posInWorld) : 
		posInMatrix(_posInMatrix),
		posInWorld(_posInWorld)
	{

	}

	void AddEdge(Node* node) {
		float distance = Vector2D(posInWorld).Distance(node->posInWorld);
		edges.push_back(Edge(distance,node));
	}

	sf::Vector2i posInMatrix;
	sf::Vector2f posInWorld;
	std::vector<Edge> edges;

};

