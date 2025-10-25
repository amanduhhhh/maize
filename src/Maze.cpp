#include "Maze.h"

#include <iostream>

Maze::Maze()
    : m_grid(GRID_HEIGHT, std::vector<int>(GRID_WIDTH, CELL_WALL)), m_rng(std::random_device{}()) {
    generateDFS();
}

void Maze::render(sf::RenderWindow& window) {
    sf::RectangleShape cell;
    cell.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));

    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            cell.setPosition(sf::Vector2f(x * CELL_SIZE, y * CELL_SIZE));

            if (m_grid[y][x] == CELL_WALL) {
                cell.setFillColor(WALL_COLOR);
            } else {
                // Check if this is an exit (empty cell on border)
                if (x == 0 || x == GRID_WIDTH - 1 || y == 0 || y == GRID_HEIGHT - 1) {
                    cell.setFillColor(EXIT_COLOR);  // Brown for exits
                } else {
                    cell.setFillColor(PATH_COLOR);
                }
            }

            window.draw(cell);
        }
    }
}

bool Maze::isWall(int gridX, int gridY) const {
    if (!isValidPosition(gridX, gridY))
        return true;  // Treat out-of-bounds as walls

    return m_grid[gridY][gridX] == CELL_WALL;
}

bool Maze::isValidPosition(int gridX, int gridY) const {
    return gridX >= 0 && gridX < GRID_WIDTH &&
           gridY >= 0 && gridY < GRID_HEIGHT;
}

void Maze::generateDFS() {
    // Start with all cells as walls
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            m_grid[y][x] = CELL_WALL;
        }
    }

    // Start DFS from center
    int centerX = GRID_WIDTH / 2;
    int centerY = GRID_HEIGHT / 2;
    dfsRecursive(centerX, centerY);

    // Add branching paths by removing some walls
    addBranchingPaths();

    // Create exits at edges
    createExits();
}

void Maze::dfsRecursive(int x, int y) {
    // make current cell a path
    m_grid[y][x] = CELL_EMPTY;

    // up, down, left, right (2 cells away)
    int directions[4][2] = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}};

    // Shuffle directions
    std::shuffle(std::begin(directions), std::end(directions), m_rng);

    for (int i = 0; i < 4; ++i) {
        int newX = x + directions[i][0];
        int newY = y + directions[i][1];

        // if new position is valid and unvisited
        if (isValidPosition(newX, newY) && m_grid[newY][newX] == CELL_WALL) {
            // Carve path
            int wallX = x + directions[i][0] / 2;
            int wallY = y + directions[i][1] / 2;
            m_grid[wallY][wallX] = CELL_EMPTY;

            dfsRecursive(newX, newY);
            
            // Occasionally create a second path from the same cell for more variety
            std::uniform_int_distribution<int> extraPath(0, 99);
            if (extraPath(m_rng) < 10) {  // 10% chance for extra path
                // Try to create another path in a different direction
                for (int j = i + 1; j < 4; ++j) {
                    int extraX = x + directions[j][0];
                    int extraY = y + directions[j][1];
                    
                    if (isValidPosition(extraX, extraY) && m_grid[extraY][extraX] == CELL_WALL) {
                        int extraWallX = x + directions[j][0] / 2;
                        int extraWallY = y + directions[j][1] / 2;
                        m_grid[extraWallY][extraWallX] = CELL_EMPTY;
                        break;  // Only one extra path per cell
                    }
                }
            }
        }
    }
}

void Maze::addBranchingPaths() {
    std::uniform_int_distribution<int> branchChance(0, 99);
    std::uniform_int_distribution<int> wallX(1, GRID_WIDTH - 2);
    std::uniform_int_distribution<int> wallY(1, GRID_HEIGHT - 2);

    int attempts = 0;
    int maxAttempts = (GRID_WIDTH * GRID_HEIGHT) / 4;  // 25% of all cells

    while (attempts < maxAttempts) {
        int x = wallX(m_rng);
        int y = wallY(m_rng);

        // Only remove walls that are between two paths
        if (m_grid[y][x] == CELL_WALL &&
            isSurroundedByPaths(x, y) &&
            branchChance(m_rng) < 25) {  // 25% chance
            m_grid[y][x] = CELL_EMPTY;
        }

        attempts++;
    }
}

bool Maze::isSurroundedByPaths(int x, int y) const {
    // has at least 2 adjacent path cells
    int pathCount = 0;

    if (x > 0 && m_grid[y][x - 1] == CELL_EMPTY) pathCount++;
    if (x < GRID_WIDTH - 1 && m_grid[y][x + 1] == CELL_EMPTY) pathCount++;
    if (y > 0 && m_grid[y - 1][x] == CELL_EMPTY) pathCount++;
    if (y < GRID_HEIGHT - 1 && m_grid[y + 1][x] == CELL_EMPTY) pathCount++;

    return pathCount >= 2;
}

void Maze::createExits() {
    int exitCount = 2;

    // Possible exit positions (border cells)
    std::vector<std::pair<int, int>> borderPositions;

    // Top and bottom edges
    for (int x = 1; x < GRID_WIDTH - 1; x += 2) {
        borderPositions.push_back({x, 0});
        borderPositions.push_back({x, GRID_HEIGHT - 1});
    }

    // Left and right edges
    for (int y = 1; y < GRID_HEIGHT - 1; y += 2) {
        borderPositions.push_back({0, y});
        borderPositions.push_back({GRID_WIDTH - 1, y});
    }

    // Shuffle and pick random exits
    std::shuffle(borderPositions.begin(), borderPositions.end(), m_rng);

    for (int i = 0; i < exitCount && i < borderPositions.size(); ++i) {
        int exitX = borderPositions[i].first;
        int exitY = borderPositions[i].second;

        m_grid[exitY][exitX] = CELL_EMPTY;
    }
}

void Maze::generateRandomWalls() {
    // Create border walls
    for (int x = 0; x < GRID_WIDTH; ++x) {
        m_grid[0][x] = CELL_WALL;
        m_grid[GRID_HEIGHT - 1][x] = CELL_WALL;
    }

    for (int y = 0; y < GRID_HEIGHT; ++y) {
        m_grid[y][0] = CELL_WALL;
        m_grid[y][GRID_WIDTH - 1] = CELL_WALL;
    }

    // Generate random walls inside (30% chance for each cell to be a wall)
    std::uniform_int_distribution<int> wallChance(0, 9);

    for (int y = 1; y < GRID_HEIGHT - 1; ++y) {
        for (int x = 1; x < GRID_WIDTH - 1; ++x) {
            if (wallChance(m_rng) < 3)  // 30% chance
            {
                m_grid[y][x] = CELL_WALL;
            }
        }
    }

    // Ensure center area is clear for player spawn
    int centerX = GRID_WIDTH / 2;
    int centerY = GRID_HEIGHT / 2;

    for (int y = centerY - 2; y <= centerY + 2; ++y) {
        for (int x = centerX - 2; x <= centerX + 2; ++x) {
            if (isValidPosition(x, y)) {
                m_grid[y][x] = CELL_EMPTY;
            }
        }
    }
}

void Maze::regenerate() {
    // Reset the grid to all walls
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            m_grid[y][x] = CELL_WALL;
        }
    }

    // Generate a new maze
    generateDFS();
}
