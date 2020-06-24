#include "AStar.h"

/*
AStar::AStar(Map* level)
{
	// Generate graph
	//std::cout << "\nGENERATING GRAPH\n";
	
	int mapX = level->getMapLengthX();
	int mapZ = level->getMapLengthZ();

	//int **map level->getWallPositions
	glm::ivec2 mapSize = level->getMapSize();

	bool finished = false;
	for (int x = 0; x < mapX; x++) {
		for (int z = 0; z < mapZ; z++) {
			if (map[z][x] == 0) {
				// Add a node
				new Node(glm::ivec2(x, z), mapSize, map, this);
				finished = true;
			}
		}
		if (finished) {
			break;
		}
	}

	for (int z = 0; z < mapSize.y; z++) {
		for (int x = 0; x < mapX; x++) {
			if (getNodeAtPosition(glm::ivec2(x, z)) != nullptr) {
				//std::cout << 0;
			}
			else {
				//std::cout << 1;
			}
		}
		//std::cout << "\t\t";
		for (int x = 0; x < mapX; x++) {
			//std::cout << map[z][x];
		}
		//std::cout << '\n';
	}
	//std::cout << "\n------------\n\n";
}

std::vector<glm::ivec2> AStar::getPath(glm::ivec2 startPosition, glm::ivec2 goalPosition)
{
	std::cout << "\nA*\n-------------------\n";
	std::vector<glm::ivec2> path;

	Node *start = getNodeAtPosition(startPosition);
	std::cout << "\nSTART POSITION: " << startPosition.x << ", " << startPosition.y << " / ";
	//std::cout << start->getPosition().x << ", " << start->getPosition().z << '\n';
	Node *goal = getNodeAtPosition(goalPosition);
	std::cout << "\nGOAL POSITION: " << goalPosition.x << ", " << goalPosition.y << " / ";
	//std::cout << goal->getPosition().x << ", " << goal->getPosition().z << '\n';

	// could not find start and or goal nodes
	if (start == nullptr || goal == nullptr) {
		path.push_back(startPosition);
		return path;
	}
	else {
	}

	std::set<Node *> closedSet;			// nodes already evaluated
	std::set<Node *> openSet;			// nodes that have been discovered, but not evaluated
	std::map<Node *, Node *> cameFrom;	// node in key came from node in value
	std::map<Node *, float> gScore;		// cost of getting from start to a node
	std::map<Node *, float> fScore;		// estimated cost of getting from start to goal through a node

	openSet.insert(start);
	for (Node *node : nodes) {
		gScore[node] = 99999;
	}
	gScore[start] = 0;
	for (Node *node : nodes) {
		fScore[node] = 99999;
	}
	fScore[start] = 99999;
	
	fScore[start] =
		abs((goalPosition - start->getPosition()).x) +
		abs((goalPosition - start->getPosition()).z);

	//std::cout << "\nConstructing path\n";
	while (openSet.size() > 0) {
		// get node in openSet with lowest fScore
		Node *current = nullptr;
		float lowestFScore = 99999;
		for (Node * node : openSet) {
			if (fScore[node] < lowestFScore) {
				lowestFScore = fScore[node];
				current = node;
			}
		}
		//std::cout << "\nCurrent node: ";
		//std::cout << current->getPosition().x << ", " << current->getPosition().z << '\n';
		// reached goal! :)
		if (current == goal) {
			// reconstruct path
			std::cout << "\n\nReconstructed path:\n";
			path.push_back(current->getPosition());
		    std::cout << current->getPosition().x << ", " << current->getPosition().z << '\n';
			while (cameFrom.find(current) != cameFrom.end()) {
				current = cameFrom[current];
				path.push_back(current->getPosition());
				std::cout << current->getPosition().x << ", " << current->getPosition().z << '\n';
			}
			std::cout << "\n-------------------\n\n";
			return path;
		}

		// current visited
		openSet.erase(current);
		closedSet.insert(current);

		for (Node *neighbour : current->getNeighbours()) {
			//std::cout << "\n\tNeighbour: "; std::cout << neighbour->getPosition().x << "; " << neighbour->getPosition().z << '\n';
			if (closedSet.find(neighbour) != closedSet.end()) {
				//std::cout << "\t\tAlready visited\n";
				// neighbour already visited
				continue;
			}

			// the gscore if we are to go to the neighbour from this node
			float tentativeGScore = gScore[current] + 1;

			if (openSet.find(neighbour) == openSet.end()) {
				//std::cout << "\t\tNot in open set\n";
				// first time looking at neighbour
				openSet.insert(neighbour);
			}
			else if (gScore[neighbour] < tentativeGScore) {
				//std::cout << "\t\tBetter path to this neighbour\n";
				// neighbour already had a better gscore
				continue;
			}
			//std::cout << "\t\tBest path to this neighbour\n";
			// found the best path for this neighbour so far
			cameFrom[neighbour] = current;
			gScore[neighbour] = tentativeGScore;
			glm::vec2 distance = goalPosition - neighbour->getPosition();
			//std::cout << "\t\tDistance from neighbour to goal: " << distance.x << ", " << distance.z << '\n';
			fScore[neighbour] = gScore[neighbour] + abs(distance.x) + abs(distance.y);
			//std::cout << "\t\tfScore: " << fScore[neighbour] << '\n';
		}
	}


	return { startPosition };
}

void AStar::addNode(Node * node)
{
	nodes.push_back(node);
}

Node * AStar::getNodeAtPosition(glm::ivec2 position)
{
	for (Node *node : nodes) {
		if (node->getPosition() == position) {
			return node;
		}
	}
	return nullptr;
}
*/