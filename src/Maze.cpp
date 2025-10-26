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
            if (m_grid[y][x] == CELL_WALL) {
                continue;
            }
            
            bool isEdge = (x == 0 || x == GRID_WIDTH - 1 || y == 0 || y == GRID_HEIGHT - 1);
            
            if (isEdge) {
                const float doorThickness = CELL_SIZE / 2.0f;
                const float doorLength = CELL_SIZE * 1.5f;
                sf::RectangleShape door;
                door.setFillColor(EXIT_COLOR);
                
                float centerX = x * CELL_SIZE + CELL_SIZE / 2.0f;
                float centerY = y * CELL_SIZE + CELL_SIZE / 2.0f;
                
                if (x == 0) {
                    door.setSize(sf::Vector2f(doorThickness, doorLength));
                    door.setPosition(sf::Vector2f(x * CELL_SIZE, centerY - doorLength / 2.0f));
                } else if (x == GRID_WIDTH - 1) {
                    door.setSize(sf::Vector2f(doorThickness, doorLength));
                    door.setPosition(sf::Vector2f(x * CELL_SIZE + CELL_SIZE - doorThickness, centerY - doorLength / 2.0f));
                } else if (y == 0) {
                    door.setSize(sf::Vector2f(doorLength, doorThickness));
                    door.setPosition(sf::Vector2f(centerX - doorLength / 2.0f, y * CELL_SIZE));
                } else if (y == GRID_HEIGHT - 1) {
                    door.setSize(sf::Vector2f(doorLength, doorThickness));
                    door.setPosition(sf::Vector2f(centerX - doorLength / 2.0f, y * CELL_SIZE + CELL_SIZE - doorThickness));
                }
                
                window.draw(door);
            } else {
                cell.setPosition(sf::Vector2f(x * CELL_SIZE, y * CELL_SIZE));
                cell.setFillColor(PATH_COLOR);
                window.draw(cell);
            }
        }
    }

    drawWallLines(window);
}

void Maze::drawWallLines(sf::RenderWindow& window) {
    const float lineThickness = 4.0f;
    sf::RectangleShape line;
    line.setFillColor(WALL_COLOR);

    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            if (m_grid[y][x] == CELL_WALL) {
                float centerX = x * CELL_SIZE + CELL_SIZE / 2.0f;
                float centerY = y * CELL_SIZE + CELL_SIZE / 2.0f;

                bool hasWallAbove = (y > 0 && m_grid[y - 1][x] == CELL_WALL);
                bool hasWallRight = (x < GRID_WIDTH - 1 && m_grid[y][x + 1] == CELL_WALL);
                bool hasWallBelow = (y < GRID_HEIGHT - 1 && m_grid[y + 1][x] == CELL_WALL);
                bool hasWallLeft = (x > 0 && m_grid[y][x - 1] == CELL_WALL);

                if (!hasWallAbove && !hasWallRight && !hasWallBelow && !hasWallLeft) {
                    line.setSize(sf::Vector2f(CELL_SIZE * 0.8f, lineThickness));
                    line.setPosition(sf::Vector2f(centerX - CELL_SIZE * 0.4f, centerY - lineThickness / 2.0f));
                    window.draw(line);
                } else {
                    
                    if (hasWallAbove) {
                        line.setSize(sf::Vector2f(lineThickness, CELL_SIZE / 2.0f));
                        line.setPosition(sf::Vector2f(centerX - lineThickness / 2.0f, centerY - CELL_SIZE / 2.0f));
                        window.draw(line);
                    }

                    if (hasWallRight) {
                        line.setSize(sf::Vector2f(CELL_SIZE / 2.0f, lineThickness));
                        line.setPosition(sf::Vector2f(centerX, centerY - lineThickness / 2.0f));
                        window.draw(line);
                    }

                    // Wall below
                    if (hasWallBelow) {
                        line.setSize(sf::Vector2f(lineThickness, CELL_SIZE / 2.0f));
                        line.setPosition(sf::Vector2f(centerX - lineThickness / 2.0f, centerY));
                        window.draw(line);
                    }

                    if (hasWallLeft) {
                        line.setSize(sf::Vector2f(CELL_SIZE / 2.0f, lineThickness));
                        line.setPosition(sf::Vector2f(centerX - CELL_SIZE / 2.0f, centerY - lineThickness / 2.0f));
                        window.draw(line);
                    }
                }
            }
        }
    }
}

bool Maze::isWall(int gridX, int gridY) const {
    if (!isValidPosition(gridX, gridY))
        return true;  

    return m_grid[gridY][gridX] == CELL_WALL;
}

bool Maze::isValidPosition(int gridX, int gridY) const {
    return gridX >= 0 && gridX < GRID_WIDTH &&
           gridY >= 0 && gridY < GRID_HEIGHT;
}

void Maze::generateDFS() {
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            m_grid[y][x] = CELL_WALL;
        }
    }

    int centerX = GRID_WIDTH / 2;
    int centerY = GRID_HEIGHT / 2;
    dfsRecursive(centerX, centerY);

    addBranchingPaths();

    createExits();
}

void Maze::dfsRecursive(int x, int y) {
    // make current cell a path
    m_grid[y][x] = CELL_EMPTY;

    // up, down, left, right (2 cells away)
    int directions[4][2] = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}};

    std::shuffle(std::begin(directions), std::end(directions), m_rng);

    for (int i = 0; i < 4; ++i) {
        int newX = x + directions[i][0];
        int newY = y + directions[i][1];

        // if new position is valid and unvisited
        if (isValidPosition(newX, newY) && m_grid[newY][newX] == CELL_WALL) {
            int wallX = x + directions[i][0] / 2;
            int wallY = y + directions[i][1] / 2;
            m_grid[wallY][wallX] = CELL_EMPTY;

            dfsRecursive(newX, newY);
            
            std::uniform_int_distribution<int> extraPath(0, 99);
            if (extraPath(m_rng) < 10) {  
                for (int j = i + 1; j < 4; ++j) {
                    int extraX = x + directions[j][0];
                    int extraY = y + directions[j][1];
                    
                    if (isValidPosition(extraX, extraY) && m_grid[extraY][extraX] == CELL_WALL) {
                        int extraWallX = x + directions[j][0] / 2;
                        int extraWallY = y + directions[j][1] / 2;
                        m_grid[extraWallY][extraWallX] = CELL_EMPTY;
                        break;  
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

        if (m_grid[y][x] == CELL_WALL &&
            isSurroundedByPaths(x, y) &&
            !isCornerPiece(x, y) &&
            branchChance(m_rng) < 25) {  // 25% chance
            m_grid[y][x] = CELL_EMPTY;
        }

        attempts++;
    }
}

bool Maze::isSurroundedByPaths(int x, int y) const {
    int pathCount = 0;

    if (x > 0 && m_grid[y][x - 1] == CELL_EMPTY) pathCount++;
    if (x < GRID_WIDTH - 1 && m_grid[y][x + 1] == CELL_EMPTY) pathCount++;
    if (y > 0 && m_grid[y - 1][x] == CELL_EMPTY) pathCount++;
    if (y < GRID_HEIGHT - 1 && m_grid[y + 1][x] == CELL_EMPTY) pathCount++;

    return pathCount >= 2;
}

bool Maze::isCornerPiece(int x, int y) const {
    
    bool hasWallAbove = (y > 0 && m_grid[y - 1][x] == CELL_WALL);
    bool hasWallRight = (x < GRID_WIDTH - 1 && m_grid[y][x + 1] == CELL_WALL);
    bool hasWallBelow = (y < GRID_HEIGHT - 1 && m_grid[y + 1][x] == CELL_WALL);
    bool hasWallLeft = (x > 0 && m_grid[y][x - 1] == CELL_WALL);
    
    int wallCount = 0;
    if (hasWallAbove) wallCount++;
    if (hasWallRight) wallCount++;
    if (hasWallBelow) wallCount++;
    if (hasWallLeft) wallCount++;
    
    if (wallCount == 2) {
        return (hasWallAbove && hasWallRight) ||  
               (hasWallRight && hasWallBelow) ||
               (hasWallBelow && hasWallLeft) ||  
               (hasWallLeft && hasWallAbove);    
    }
    
    return false;
}

void Maze::createExits() {
    int exitCount = 2;

    std::vector<std::pair<int, int>> borderPositions;

    for (int x = 1; x < GRID_WIDTH - 1; x += 2) {
        borderPositions.push_back({x, 0});
        borderPositions.push_back({x, GRID_HEIGHT - 1});
    }

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
    for (int x = 0; x < GRID_WIDTH; ++x) {
        m_grid[0][x] = CELL_WALL;
        m_grid[GRID_HEIGHT - 1][x] = CELL_WALL;
    }

    for (int y = 0; y < GRID_HEIGHT; ++y) {
        m_grid[y][0] = CELL_WALL;
        m_grid[y][GRID_WIDTH - 1] = CELL_WALL;
    }

    std::uniform_int_distribution<int> wallChance(0, 9);

    for (int y = 1; y < GRID_HEIGHT - 1; ++y) {
        for (int x = 1; x < GRID_WIDTH - 1; ++x) {
            if (wallChance(m_rng) < 3)  // 30% chance
            {
                m_grid[y][x] = CELL_WALL;
            }
        }
    }

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
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            m_grid[y][x] = CELL_WALL;
        }
    }

    generateDFS();
}
