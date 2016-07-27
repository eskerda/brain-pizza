#pragma once

#include "constants.h"

#include <vector>

#include "math/Vector2D.h"
#include "Graph.h"
#include "Node.h"

const float distanceToTargetNextNode = 10;


//TODO: Detect case where there is no solution

//MILLORA: Pathfinder en a un step per frame
//MILLORA: Pathfinder en una funcio "idleUpdate" o "idleLogic" que s'executi en lloc de fer waits

struct AStar {

	AStar() {
		finishedPath = true;
	}
	
	~AStar() {
		//Do nothing
	}

	void Compute(Graph* graph, Vector2D origin, Vector2D destination) {
		nodes.clear();
		currentNodeIndex = 0;
		finishedPath = false;

		Node* firstNode = graph->findClosestNode(origin);
		if (firstNode == NULL) {
			//FIXME: We are on obstacle!
			finishedPath = true;
			return;
		}
		Node* lastNode = graph->findClosestNode(destination);
		if (lastNode == NULL) {
			//Click on an obstacle, do nothing
			finishedPath = true;
			return;
		}
		
		sf::Clock clock;

		DoSearch(firstNode, lastNode);

		std::cout << "A* time: " << clock.getElapsedTime().asMilliseconds() << "ms" << std::endl;
		
		if (nodes.empty()) finishedPath = true;
	}

	void Draw(sf::RenderTarget& rt) {
		std::vector<sf::Vertex> va;
		for (uint i = 0; i < nodes.size(); i++) {
			va.push_back(nodes[i].ToVertex(sf::Color::Blue));
		}
		rt.draw(&va[0],va.size(),sf::LinesStrip);
	}

	bool IsFinished() {
		return finishedPath;
	}

	Vector2D GetNextPos(Vector2D currentPos) {
		
		assert(!IsFinished());

		Vector2D currentNodePos = nodes[currentNodeIndex];
		if (currentPos.Distance(currentNodePos) < distanceToTargetNextNode) {
			currentNodeIndex++;
			if (currentNodeIndex >= nodes.size()) {
				finishedPath = true; //Path ended, calling this function again will make it crash
			}
		}
		return currentNodePos;
	}

private:

	float Heuristic(Node* a, Node* b) {
		return Vector2D(a->posInWorld).Distance(b->posInWorld);
	}

	void FoundSolution(std::map<Node*, Node*>& came_from, Node* start, Node* goal) {
		std::vector<Node*> inversepath;
		
		inversepath.push_back(goal);
		while (goal != start) {
			goal = came_from[goal];
			inversepath.push_back(goal);
		}
		
		//Reverse loop and initialize the nodes array with the final positions
		int last = inversepath.size()-1;
		for (int i = last; i >= 0; i--) {
			nodes.push_back(inversepath[i]->posInWorld);
		}

	}
	
	void DoSearch(Node* start, Node* goal) {
		//Based on http://en.wikipedia.org/wiki/A*_search_algorithm

		std::set<Node*> openset;
		openset.insert(start);
		std::set<Node*> closedset;

		std::map<Node*, Node*> came_from;

		std::map<Node*,float> g_scores;
		std::map<Node*,float> f_scores;
		
		g_scores[start] = 0;
		f_scores[start] = g_scores[start] + Heuristic(start,goal);
 
		while(!openset.empty()) {

			//Current: the node in openset having the lowest f_score
			std::set<Node*>::iterator current = openset.begin();
			for(std::set<Node*>::iterator it = current; it != openset.end(); it++){
				if (f_scores[*it] < f_scores[*current]) current = it;
			}
			
			Node* currentNode = *current;
			if (currentNode == goal) {
				FoundSolution(came_from, start, goal);
				return;
			}

			openset.erase(current);
			closedset.insert(currentNode);

			for (uint i = 0; i < currentNode->edges.size(); i++) {
				Node::Edge& e = currentNode->edges[i];
				if (closedset.find(e.node) != closedset.end()) continue; //Already closed

				float new_g_score = g_scores[currentNode] + e.distance;
 
				bool wasopen = (openset.find(e.node) != openset.end());
				if (!wasopen || new_g_score <= g_scores[e.node]) { //Not yet open or open but with higher score
					came_from[e.node] = currentNode;
					g_scores[e.node] = new_g_score;
					f_scores[e.node] = g_scores[e.node] + Heuristic(e.node, goal);
					if (!wasopen) openset.insert(e.node);
				}

			}

		}

	}


	bool finishedPath;
	uint currentNodeIndex;
	std::vector<Vector2D> nodes;

};

