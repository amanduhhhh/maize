#pragma once

#include <algorithm>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Config.h"


/**
 * @brief A* pathfinding algorithm for grid-based mazes
 *
 * Finds the shortest path between two points using the A* algorithm
 * with Manhattan distance heuristic. Optimized for grid-based movement.
 */
class Pathfinder {
   public:
    /**
     * @brief Represents a node in the pathfinding grid
     */
    struct Node {
        int x, y;      ///< Grid coordinates
        int g;         ///< Cost from start to this node
        int h;         ///< Heuristic cost to goal
        int f;         ///< Total cost (g + h)
        Node* parent;  ///< Parent node for path reconstruction

        Node(int x, int y, int g, int h, Node* parent = nullptr)
            : x(x), y(y), g(g), h(h), f(g + h), parent(parent) {}
    };

    /**
     * @brief Finds the shortest path from start to goal
     *
     * @param startX Starting X coordinate
     * @param startY Starting Y coordinate
     * @param goalX Goal X coordinate
     * @param goalY Goal Y coordinate
     * @param maze Reference to the maze for collision checking
     * @return Vector of grid positions representing the path (empty if no path found)
     */
    std::vector<std::pair<int, int>> findPath(int startX, int startY, int goalX, int goalY, const class Maze& maze);

    /**
     * @brief Calculates Manhattan distance between two points
     *
     * @param x1 First point X coordinate
     * @param y1 First point Y coordinate
     * @param x2 Second point X coordinate
     * @param y2 Second point Y coordinate
     * @return Manhattan distance
     */
    static int manhattanDistance(int x1, int y1, int x2, int y2);

   private:
    /**
     * @brief Gets valid neighbors of a node
     *
     * @param node Current node
     * @param maze Reference to the maze for collision checking
     * @return Vector of valid neighbor positions
     */
    std::vector<std::pair<int, int>> getNeighbors(const Node& node, const class Maze& maze);

    /**
     * @brief Reconstructs the path from goal to start
     *
     * @param goalNode The goal node
     * @return Vector of grid positions representing the path
     */
    std::vector<std::pair<int, int>> reconstructPath(Node* goalNode);

    /**
     * @brief Custom comparator for priority queue (min-heap)
     */
    struct NodeComparator {
        bool operator()(const Node* a, const Node* b) const {
            return a->f > b->f;  // Lower f-score has higher priority
        }
    };
};
