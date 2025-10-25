#include "Pathfinder.h"

#include <iostream>

#include "Maze.h"


std::vector<std::pair<int, int>> Pathfinder::findPath(int startX, int startY, int goalX, int goalY, const Maze& maze) {
    // Check if start and goal are valid
    if (!maze.isValidPosition(startX, startY) || !maze.isValidPosition(goalX, goalY)) {
        return {};  // Invalid positions
    }

    // Check if start and goal are the same
    if (startX == goalX && startY == goalY) {
        return {{startX, startY}};
    }

    // Priority queue for open set (min-heap based on f-score)
    std::priority_queue<Node*, std::vector<Node*>, NodeComparator> openSet;

    // Hash set for closed set (visited nodes)
    std::unordered_set<int> closedSet;

    // Hash map for open set lookup (for updating existing nodes)
    std::unordered_map<int, Node*> openMap;

    Node* startNode = new Node(startX, startY, 0, manhattanDistance(startX, startY, goalX, goalY));
    openSet.push(startNode);
    openMap[startX * GRID_WIDTH + startY] = startNode;

    while (!openSet.empty()) {
        // Get node with lowest f-score
        Node* current = openSet.top();
        openSet.pop();

        // Move from open to closed set
        int currentKey = current->x * GRID_WIDTH + current->y;
        openMap.erase(currentKey);
        closedSet.insert(currentKey);

        // Check if we reached the goal
        if (current->x == goalX && current->y == goalY) {
            std::vector<std::pair<int, int>> path = reconstructPath(current);

            // Clean up memory
            for (auto& pair : openMap) {
                delete pair.second;
            }
            // Note: current and other closed nodes will be cleaned up by reconstructPath

            return path;
        }

        // Explore neighbors
        std::vector<std::pair<int, int>> neighbors = getNeighbors(*current, maze);

        for (const auto& neighbor : neighbors) {
            int neighborX = neighbor.first;
            int neighborY = neighbor.second;
            int neighborKey = neighborX * GRID_WIDTH + neighborY;

            // Skip if already in closed set
            if (closedSet.find(neighborKey) != closedSet.end()) {
                continue;
            }

            int tentativeG = current->g + 1;  // Each step costs 1

            // Check if this path to neighbor is better
            auto openIt = openMap.find(neighborKey);
            if (openIt != openMap.end()) {
                // Neighbor already in open set
                Node* existingNode = openIt->second;
                if (tentativeG < existingNode->g) {
                    // better path to this neighbor
                    existingNode->g = tentativeG;
                    existingNode->f = existingNode->g + existingNode->h;
                    existingNode->parent = current;
                }
            } else {
                // New node, add to open set
                Node* neighborNode = new Node(neighborX, neighborY, tentativeG,
                                              manhattanDistance(neighborX, neighborY, goalX, goalY), current);
                openSet.push(neighborNode);
                openMap[neighborKey] = neighborNode;
            }
        }
    }

    // Clean up remaining memory
    for (auto& pair : openMap) {
        delete pair.second;
    }

    return {};
}

int Pathfinder::manhattanDistance(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

std::vector<std::pair<int, int>> Pathfinder::getNeighbors(const Node& node, const Maze& maze) {
    std::vector<std::pair<int, int>> neighbors;

    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    for (int i = 0; i < 4; ++i) {
        int newX = node.x + directions[i][0];
        int newY = node.y + directions[i][1];

        // Check if position is valid and not a wall
        if (maze.isValidPosition(newX, newY) && !maze.isWall(newX, newY)) {
            neighbors.push_back({newX, newY});
        }
    }

    return neighbors;
}

std::vector<std::pair<int, int>> Pathfinder::reconstructPath(Node* goalNode) {
    std::vector<std::pair<int, int>> path;
    Node* current = goalNode;

    while (current != nullptr) {
        path.push_back({current->x, current->y});
        current = current->parent;
    }

    // Reverse to get path from start to goal
    std::reverse(path.begin(), path.end());

    // Clean up memory
    current = goalNode;
    while (current != nullptr) {
        Node* parent = current->parent;
        delete current;
        current = parent;
    }

    return path;
}
