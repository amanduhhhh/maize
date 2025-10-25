#include "Enemy.h"

#include <algorithm>
#include <iostream>
#include <random>

#include "Maze.h"
#include "Pathfinder.h"

const int Enemy::PATH_UPDATE_INTERVAL = 3;  // Recalculate path every 3 moves

Enemy::Enemy(const Maze& maze, Pathfinder& pathfinder, EnemyType type)
    : Character(GRID_WIDTH / 2, GRID_HEIGHT / 2, ENEMY_COLOR), m_maze(maze), m_pathfinder(pathfinder), m_type(type), m_pathIndex(0), m_movesSincePathUpdate(0), m_targetX(0), m_targetY(0), m_isDistracted(false), m_distractionTimer(0.0f), m_distractionCooldown(0.0f) {
    switch (m_type) {
        case EnemyType::ASTAR:
            setColor(ASTAR_ENEMY_COLOR);
            m_moveDelay = ASTAR_ENEMY_DELAY;
            break;
        case EnemyType::DIJKSTRA:
            setColor(DIJKSTRA_ENEMY_COLOR);
            m_moveDelay = DIJKSTRA_ENEMY_DELAY;
            break;
        case EnemyType::BEST:
            setColor(BEST_ENEMY_COLOR);
            m_moveDelay = BEST_ENEMY_DELAY;
            break;
    }

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
    // Update distraction system for Best enemy
    if (m_type == EnemyType::BEST) {
        if (m_isDistracted) {
            m_distractionTimer -= deltaTime;
            if (m_distractionTimer <= 0.0f) {
                m_isDistracted = false;
                m_distractionTimer = 0.0f;
                m_distractionCooldown = DISTRACTION_COOLDOWN;
                std::cout << "Best enemy stopped being distracted, returning to chase player" << std::endl;
            }
        } else {
            m_distractionCooldown -= deltaTime;
        }
    }
        
    if (m_moveTimer.getElapsedTime().asSeconds() < m_moveDelay)
        return;

    // All enemies have a small chance for random moves
    float randomChance = 0.15f;
    
    if (shouldMakeRandomMove(randomChance)) {
        auto randomPos = getRandomAdjacentCell(maze);
        setPosition(randomPos.first, randomPos.second);
        advanceAnimation();
        m_randomMoveCounter++;
    } else {

        // need to recalculate?
        bool needsRecalculation = m_path.empty() || m_pathIndex >= m_path.size() || m_movesSincePathUpdate >= m_pathUpdateInterval;
        
        // Best enemy distraction system
        if (m_type == EnemyType::BEST && !m_isDistracted && m_distractionCooldown <= 0.0f) {
            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_real_distribution<float> distractionChance(0.0f, 1.0f);
            
            if (distractionChance(rng) < DISTRACTION_CHANCE) {
                // Find a valid empty cell to target
                std::uniform_int_distribution<int> randomX(1, GRID_WIDTH - 2);
                std::uniform_int_distribution<int> randomY(1, GRID_HEIGHT - 2);
                
                int attempts = 0;
                do {
                    m_targetX = randomX(rng);
                    m_targetY = randomY(rng);
                    attempts++;
                } while (maze.isWall(m_targetX, m_targetY) && attempts < 50);
                
                if (attempts < 50) {  // Found a valid target
                    m_isDistracted = true;
                    m_distractionTimer = DISTRACTION_DURATION;
                    needsRecalculation = true;
                    std::cout << "Best enemy got distracted! Targeting (" << m_targetX << ", " << m_targetY << ")" << std::endl;
                }
            }
        }
        
        if (needsRecalculation) {
            if (m_type == EnemyType::BEST && !m_isDistracted) {
                m_targetX = playerX;
                m_targetY = playerY;
            } else if (m_type != EnemyType::BEST) {
                m_targetX = playerX;
                m_targetY = playerY;
            }
            // Use different pathfinding based on enemy type
            switch (m_type) {
                case EnemyType::ASTAR:
                    m_path = m_pathfinder.findPath(getX(), getY(), m_targetX, m_targetY, maze);
                    break;
                case EnemyType::DIJKSTRA:
                    m_path = m_pathfinder.findPathDijkstra(getX(), getY(), m_targetX, m_targetY, maze);
                    break;
                case EnemyType::BEST:
                    m_path = m_pathfinder.findPathGreedy(getX(), getY(), m_targetX, m_targetY, maze);
                    break;
            }
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
            advanceAnimation();
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

            if (distanceFromPlayer >= 15) {
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

bool Enemy::shouldMakeRandomMove(float chance) const {
    if (m_randomMoveCounter % 3 == 0) {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(rng) < chance;
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



void Enemy::calculateTarget(int playerX, int playerY) {
    m_targetX = playerX;
    m_targetY = playerY;
}
