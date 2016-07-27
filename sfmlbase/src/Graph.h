#pragma once

#include "constants.h"

#include <vector>

#include "math/Vector2D.h"
#include "Node.h"
#include "Obstacle.h"

const float floodNodeDistance = 30;

class Graph {
public:

	~Graph(){ 
		Clear();
	}
	
	void Clear() {
		//We need to check it's not 0 because we don't want to do 0 minus 1 on an unsigned int
		if (nodeMatrix.empty()) return;
		if (nodeMatrix[0].empty()) return;
		for (uint x = 1; x < nodeMatrix.size() - 1; x++) {
			for (uint y = 1; y < nodeMatrix[0].size() - 1; y++) {
				Node* current = nodeMatrix[x][y];
				if (current == NULL) continue;
				delete current;
			}
		}
	}

	Node* findClosestNode(Vector2D pos) {
		Vector2D posInMatrix = pos / floodNodeDistance;
        int x = Rounded(posInMatrix.x);
        if (x >= (int)nodeMatrix.size()) x = nodeMatrix.size()-1;
		else if (x < 0) x = 0;

        int y = Rounded(posInMatrix.y);
        if (y >= (int)nodeMatrix[x].size()) y = nodeMatrix[x].size()-1;
		else if (y < 0) y = 0;

		//TODO: Do a spiral search til we find a non NULL
		return nodeMatrix[x][y]; //Note we are returning null sometimes,

	}

	void Flood(bool alsoFloodInDiagonal = true) {
		
		sf::Clock clock;

		Vector2D matrixSize = Vector2D(Input::GetWindowSize()) / floodNodeDistance;
		nodeMatrix = std::vector< std::vector<Node*> >((int)ceil(matrixSize.x), std::vector<Node*>((int)ceil(matrixSize.y),NULL));

		std::set<Obstacle*> obstacles = Obstacle::getAll();

		//Create nodes
		for (Uint32 x = 0; x < nodeMatrix.size(); x++) {
			for (Uint32 y = 0; y < nodeMatrix[0].size(); y++) {

				Vector2D pos(x*floodNodeDistance + floodNodeDistance/4, y*floodNodeDistance + floodNodeDistance/4);
				bool thereIsAnObstacleHere = false;

				for (std::set<Obstacle*>::iterator it = obstacles.begin(); it != obstacles.end(); it++) {

					Obstacle* obst = *it;
					Vector2D obstPos = obst->Pos();
					float distance = obst->BRadius();

					if (obstPos.DistanceSq(pos) < distance*distance) {
						//There is an obstacle in this position
						thereIsAnObstacleHere = true;
						break;
					}

				}

				if (!thereIsAnObstacleHere) {
					nodeMatrix[x][y] = new Node(sf::Vector2i(x,y),pos);
				}

			}
		}

		
		//Conect nodes with edges
		for (uint x = 1; x < nodeMatrix.size() - 1; x++) {
			for (uint y = 1; y < nodeMatrix[0].size() - 1; y++) {
				
				Node* current = nodeMatrix[x][y];
				if (current == NULL) continue;

				Node* right = nodeMatrix[x+1][y];
				Node* left = nodeMatrix[x-1][y];
				Node* up = nodeMatrix[x][y+1];
				Node* down = nodeMatrix[x][y-1];
				
				if (right) current->AddEdge(right);
				if (left) current->AddEdge(left);
				if (up) current->AddEdge(up);
				if (down) current->AddEdge(down);

				if (alsoFloodInDiagonal) {
					Node* upright = nodeMatrix[x+1][y+1];
					Node* downright = nodeMatrix[x+1][y-1];
					Node* upleft = nodeMatrix[x-1][y+1];
					Node* downleft = nodeMatrix[x-1][y-1];

					if (upright) current->AddEdge(upright);
					if (downright) current->AddEdge(downright);
					if (upleft) current->AddEdge(upleft);
					if (downleft) current->AddEdge(downleft);
				}

			}
		}

		//std::cout << matrixPrint(nodeMatrix) << std::endl;

		std::cout << "Flood time: " << clock.getElapsedTime().asMilliseconds() << "ms" << std::endl;
	}

	void Draw(sf::RenderTarget& rt) {
		sf::CircleShape cs;

		const float circleSize = 2;
		cs.setRadius(circleSize);
		cs.setOrigin(circleSize,circleSize);
		cs.setFillColor(sf::Color::Green);

		for (uint x = 0; x < nodeMatrix.size(); x++) {
			for (uint y = 0; y < nodeMatrix[0].size(); y++) {
				
				Node* current = nodeMatrix[x][y];
				if (current == NULL) continue;

				cs.setPosition(current->posInWorld);
				rt.draw(cs);

				for (uint i = 0; i < current->edges.size(); i++) {
					Vector2D a = current->posInWorld;
					Vector2D b = current->edges[i].node->posInWorld;

					sf::Vertex line[2] = {
						sf::Vertex(a,sf::Color::Green),
						sf::Vertex(b,sf::Color::Green)
					};
					rt.draw(line, 2, sf::Lines);
				}

			}
		}

	}


private:
	std::vector< std::vector<Node*> > nodeMatrix; //a cell is null when there is an obstacle in that position
	
};
