#include "Enemy.h"

#include <algorithm>
#include <random>

#include "Maze.h"
#include "Pathfinder.h"

const float Enemy::MOVE_DELAY = 0.15f;
const int Enemy::PATH_UPDATE_INTERVAL = 3;  // Recalculate path every 3 moves

Enemy::Enemy(const Maze& maze, Pathfinder& pathfinder)
    : Character(GRID_WIDTH / 2, GRID_HEIGHT / 2, ENEMY_COLOR), m_maze(maze), m_pathfinder(pathfinder), m_pathIndex(0), m_movesSincePathUpdate(0) {
    // Initialize random move behavior with some variation
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> intervalDist(2, 5);
    std::uniform_real_distribution<float> chanceDist(0.1f, 0.2f);

    m_pathUpdateInterval = intervalDist(rng);
    m_randomMoveChance = chanceDist(rng);
    m_randomMoveCounter = 0;

    auto spawnPos = findRandomSpawnLocation(maze, GRID_WIDTH / 2, GRID_HEIGHT / 2);
    setPosition(spawnPos.first, spawnPos.second);
}

void Enemy::update(float deltaTime, int playerX, int playerY, const Maze& maze) {
    if (m_moveTimer.getElapsedTime().asSeconds() < MOVE_DELAY)
        return;

    // Check if we should make a random move instead of pathfinding
    if (shouldMakeRandomMove()) {
        auto randomPos = getRandomAdjacentCell(maze);
        setPosition(randomPos.first, randomPos.second);
        m_randomMoveCounter++;
    } else {
        // need to recalculate?
        if (m_path.empty() || m_pathIndex >= m_path.size() || m_movesSincePathUpdate >= m_pathUpdateInterval) {
            m_path = m_pathfinder.findPath(getX(), getY(), playerX, playerY, maze);
            m_pathIndex = 0;
            m_movesSincePathUpdate = 0;

            // If no path found, don't move
            if (m_path.empty()) {
                m_moveTimer.restart();
                return;
            }
        }

        if (m_pathIndex < m_path.size()) {
            setPosition(m_path[m_pathIndex].first, m_path[m_pathIndex].second);
            m_pathIndex++;
            m_movesSincePathUpdate++;
        }
    }

    m_moveTimer.restart();
}

bool Enemy::hasCaughtPlayer(int playerX, int playerY) const {
    return getX() == playerX && getY() == playerY;
}

std::pair<int, int> Enemy::findRandomSpawnLocation(const Maze& maze, int playerX, int playerY) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> xDist(1, GRID_WIDTH - 2);
    std::uniform_int_distribution<int> yDist(1, GRID_HEIGHT - 2);

    int attempts = 0;
    const int maxAttempts = 100;

    while (attempts < maxAttempts) {
        int x = xDist(rng);
        int y = yDist(rng);
        if (maze.isValidPosition(x, y) && !maze.isWall(x, y)) {
            int distanceFromPlayer = std::abs(x - playerX) + std::abs(y - playerY);

            // Spawn at least 10 cells away from player
            if (distanceFromPlayer >= 10) {
                return {x, y};
            }
        }

        attempts++;
    }

    // Fallback: spawn at any empty cell (excluding center)
    for (int y = 1; y < GRID_HEIGHT - 1; ++y) {
        for (int x = 1; x < GRID_WIDTH - 1; ++x) {
            if (maze.isValidPosition(x, y) && !maze.isWall(x, y)) {
                // Avoid spawning at center where player starts
                if (x != GRID_WIDTH / 2 || y != GRID_HEIGHT / 2) {
                    return {x, y};
                }
            }
        }
    }

    // Last resort: spawn at center
    return {GRID_WIDTH / 2, GRID_HEIGHT / 2};
}

bool Enemy::shouldMakeRandomMove() const {
    // Every few moves, check if we should move randomly
    if (m_randomMoveCounter % 3 == 0) {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(rng) < m_randomMoveChance;
    }
    return false;
}

std::pair<int, int> Enemy::getRandomAdjacentCell(const Maze& maze) const {
    std::vector<std::pair<int, int>> validMoves;
    int currentX = getX();
    int currentY = getY();

    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    for (int i = 0; i < 4; ++i) {
        int newX = currentX + directions[i][0];
        int newY = currentY + directions[i][1];

        if (maze.isValidPosition(newX, newY) && !maze.isWall(newX, newY)) {
            validMoves.push_back({newX, newY});
        }
    }

    if (validMoves.empty()) {
        return {currentX, currentY}; 
    }

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, validMoves.size() - 1);
    return validMoves[dist(rng)];
}
