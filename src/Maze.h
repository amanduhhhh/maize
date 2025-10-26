#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include <vector>

#include "Config.h"

/**
 * @brief Maze class for generating and rendering the game maze
 *
 * Currently generates random walls for collision testing.
 * Future: Will implement DFS algorithm for proper maze generation.
 */
class Maze {
   public:
    /**
     * @brief Constructs a new Maze object
     *
     * Initializes the grid and generates random walls for testing.
     */
    Maze();

    /**
     * @brief Renders the maze to the given window
     *
     * @param window Reference to the SFML render window
     *
     * Draws walls as gray rectangles and paths as black rectangles.
     */
    void render(sf::RenderWindow& window);

    /**
     * @brief Checks if a grid position is a wall
     *
     * @param gridX X coordinate in grid space
     * @param gridY Y coordinate in grid space
     * @return true if the position is a wall, false if it's a path
     */
    bool isWall(int gridX, int gridY) const;

    /**
     * @brief Checks if a grid position is valid (within bounds)
     *
     * @param gridX X coordinate in grid space
     * @param gridY Y coordinate in grid space
     * @return true if the position is within the grid bounds
     */
    bool isValidPosition(int gridX, int gridY) const;

    /**
     * @brief Gets the grid width
     *
     * @return Width of the grid in cells
     */
    int getGridWidth() const { return GRID_WIDTH; }

    /**
     * @brief Gets the grid height
     *
     * @return Height of the grid in cells
     */
    int getGridHeight() const { return GRID_HEIGHT; }

    /**
     * @brief Regenerates the maze with a new layout
     *
     * Creates a completely new maze layout for the next round.
     */
    void regenerate();

   private:
    /**
     * @brief Draws wall lines using the new line-based rendering system
     *
     * @param window Reference to the SFML render window
     *
     * Draws thin lines from the center of each wall cell to adjacent walls.
     */
    void drawWallLines(sf::RenderWindow& window);
    /**
     * @brief Generates a maze using Depth-First Search algorithm
     *
     * Creates a perfect maze starting from the center with guaranteed
     * paths to all exits. Uses recursive backtracking.
     */
    void generateDFS();

    /**
     * @brief Recursive DFS helper function
     *
     * @param x Current X position in grid
     * @param y Current Y position in grid
     *
     * Recursively carves paths from the current position.
     */
    void dfsRecursive(int x, int y);

    /**
     * @brief Adds branching paths by removing some walls
     *
     * Creates loops and multiple routes by strategically removing walls
     * that are between existing paths.
     */
    void addBranchingPaths();

    /**
     * @brief Checks if a wall cell is surrounded by paths
     *
     * @param x X coordinate of wall cell
     * @param y Y coordinate of wall cell
     * @return true if wall has at least 2 adjacent path cells
     */
    bool isSurroundedByPaths(int x, int y) const;

    /**
     * @brief Checks if a wall cell forms a corner piece
     *
     * @param x X coordinate of wall cell
     * @param y Y coordinate of wall cell
     * @return true if wall forms a corner with exactly 2 adjacent walls
     */
    bool isCornerPiece(int x, int y) const;

    /**
     * @brief Creates exit points at the edges of the maze
     *
     * Opens 2-4 exits by removing border walls.
     */
    void createExits();

    /**
     * @brief Adds random open areas to create more interconnected paths
     *
     * Creates larger open spaces and additional connections to reduce dead ends.
     */
    void addRandomOpenAreas();

    /**
     * @brief Ensures all areas of the maze are well-connected
     *
     * Creates corridors between major open areas to prevent isolated sections.
     */
    void ensureConnectivity();

    /**
     * @brief Generates random walls for collision testing
     *
     * Creates a random pattern of walls and paths.
     * This is temporary - will be replaced with proper maze generation.
     */
    void generateRandomWalls();

    std::vector<std::vector<int>> m_grid;  ///< 2D grid: 0=path, 1=wall
    std::mt19937 m_rng;                    ///< Random number generator
};
