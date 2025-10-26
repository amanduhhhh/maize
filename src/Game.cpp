#include "Game.h"

#include <algorithm>
#include <iostream>

Game::Game()
    : m_window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Oubliette - Maze Chase Game"), m_player(GRID_WIDTH / 2, GRID_HEIGHT / 2), m_pathfinder(), m_key(nullptr), m_hasKey(false), m_currentRound(1), m_gameOver(false), m_roundTransition(false), m_transitionTimer(0.0f), m_roundText(m_font) {
    if (!m_font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cout << "Warning: Could not load font, using default" << std::endl;
    }

    m_roundText.setFont(m_font);
    m_roundText.setCharacterSize(48);
    m_roundText.setFillColor(sf::Color::White);
    m_roundText.setStyle(sf::Text::Bold);

    // Load textures
    if (!loadTextures()) {
        std::cout << "Warning: Could not load animation textures, using colored shapes" << std::endl;
    } else {
        m_player.setAnimationTextures(m_playerTexture1, m_playerTexture2);
    }

    startNewRound();
}

void Game::run() {
    sf::Clock clock;

    // Main game loop
    while (m_window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    while (auto event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        }
    }
}

void Game::update(float deltaTime) {
    if (m_gameOver) {
        return;
    }

    if (m_roundTransition) {
        updateRoundTransition(deltaTime);
        return;
    }

    m_player.handleInput(m_maze, m_hasKey);
    m_player.updateGhostMode(deltaTime);

    checkKeyCollection();

    checkPowerUpCollection();

    updatePowerUps(deltaTime);

    if (m_powerupSpawnTimer.getElapsedTime().asSeconds() >= POWERUP_SPAWN_INTERVAL && m_powerups.size() < MAX_POWERUPS) {
        spawnPowerUp();
        m_powerupSpawnTimer.restart();
    }

    for (auto& enemy : m_enemies) {
        enemy.update(deltaTime, m_player.getX(), m_player.getY(), m_maze);
    }

    if (checkWinCondition()) {
        std::cout << "Congratulations! You escaped the maze! Starting round " << (m_currentRound + 1) << "..." << std::endl;
        m_currentRound++;
        startNewRound();
    }

    if (checkGameOverCondition()) {
        m_gameOver = true;
        std::cout << "Game Over! The enemy caught you!" << std::endl;
        m_window.close();
    }
}

void Game::render() {
    m_window.clear(sf::Color::Black);

    m_maze.render(m_window);

    if (m_key && !m_hasKey) {
        m_key->render(m_window);
    }

    for (auto& powerup : m_powerups) {
        powerup.render(m_window);
    }

    for (auto& enemy : m_enemies) {
        enemy.render(m_window);
    }

    m_player.render(m_window);

    // Round transition screen
    if (m_roundTransition) {
        sf::RectangleShape blackOverlay;
        blackOverlay.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        blackOverlay.setFillColor(sf::Color::Black);
        m_window.draw(blackOverlay);

        m_window.draw(m_roundText);
    }

    if (m_gameOver) {
        // Simple text rendering could be added here
        // For now, we'll just keep the window open
    }

    m_window.display();
}

bool Game::checkWinCondition() const {
    if (!m_hasKey) return false;
    return m_player.getX() < 0 || m_player.getX() >= GRID_WIDTH ||
           m_player.getY() < 0 || m_player.getY() >= GRID_HEIGHT;
}

bool Game::checkGameOverCondition() const {
    for (const auto& enemy : m_enemies) {
        if (enemy.hasCaughtPlayer(m_player.getX(), m_player.getY())) {
            return true;
        }
    }
    return false;
}

void Game::startNewRound() {
    startRoundTransition();
}

void Game::checkKeyCollection() {
    if (m_hasKey || !m_key) return;

    if (m_player.getX() == m_key->getX() && m_player.getY() == m_key->getY()) {
        m_hasKey = true;
        std::cout << "Key collected! You can now escape through the brown exits!" << std::endl;
    }
}

void Game::spawnEnemiesForRound(int roundNumber) {
    std::vector<std::pair<int, int>> usedPositions;
    std::vector<std::pair<int, int>> validSpawnPositions;

    usedPositions.push_back({m_player.getX(), m_player.getY()});

    for (int y = 1; y < GRID_HEIGHT - 1; ++y) {
        for (int x = 1; x < GRID_WIDTH - 1; ++x) {
            if (!m_maze.isWall(x, y)) {
                int distFromPlayer = std::abs(x - m_player.getX()) + std::abs(y - m_player.getY());
                if (distFromPlayer >= 10) {
                    int distFromLeftEdge = x;
                    int distFromRightEdge = GRID_WIDTH - 1 - x;
                    int distFromTopEdge = y;
                    int distFromBottomEdge = GRID_HEIGHT - 1 - y;

                    int minDistToEdge = std::min({distFromLeftEdge, distFromRightEdge,
                                                  distFromTopEdge, distFromBottomEdge});

                    if (minDistToEdge <= 3) {
                        validSpawnPositions.push_back({x, y});
                    }
                }
            }
        }
    }

    std::vector<EnemyType> enemyTypes = {EnemyType::ASTAR, EnemyType::DIJKSTRA, EnemyType::BEST};

    for (EnemyType type : enemyTypes) {
        std::random_device rd;
        std::mt19937 rng(rd());

        int attempts = 0;
        int enemyX, enemyY;
        bool validSpawn = false;

        std::shuffle(validSpawnPositions.begin(), validSpawnPositions.end(), rng);

        for (const auto& pos : validSpawnPositions) {
            bool positionUsed = false;
            for (const auto& usedPos : usedPositions) {
                if (pos.first == usedPos.first && pos.second == usedPos.second) {
                    positionUsed = true;
                    break;
                }
            }

            if (!positionUsed) {
                enemyX = pos.first;
                enemyY = pos.second;
                validSpawn = true;
                break;
            }
        }

        if (validSpawn) {
            m_enemies.emplace_back(m_maze, m_pathfinder, type);
            m_enemies.back().setPosition(enemyX, enemyY);
            m_enemies.back().updateSpeedForRound(m_currentRound);

            switch (type) {
                case EnemyType::ASTAR:
                    m_enemies.back().setAnimationTextures(m_astarTexture1, m_astarTexture2);
                    break;
                case EnemyType::DIJKSTRA:
                    m_enemies.back().setAnimationTextures(m_dijkstraTexture1, m_dijkstraTexture2);
                    break;
                case EnemyType::BEST:
                    m_enemies.back().setAnimationTextures(m_bestTexture1, m_bestTexture2);
                    break;
            }

            usedPositions.push_back({enemyX, enemyY});
        }
    }
}

void Game::startRoundTransition() {
    m_roundTransition = true;
    m_transitionTimer = 0.0f;

    m_roundText.setString(std::to_string(m_currentRound));

    sf::FloatRect textRect = m_roundText.getLocalBounds();
    m_roundText.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f,
                                       textRect.position.y + textRect.size.y / 2.0f));
    m_roundText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));
}

void Game::updateRoundTransition(float deltaTime) {
    m_transitionTimer += deltaTime;

    const float TRANSITION_DURATION = 2.0f;

    if (m_transitionTimer >= TRANSITION_DURATION) {
        m_roundTransition = false;
        setupRound();
    }
}

void Game::setupRound() {
    m_maze.regenerate();

    m_player.setPosition(GRID_WIDTH / 2, GRID_HEIGHT / 2);
    m_player.resetGhostMode();

    m_hasKey = false;

    if (m_key) {
        delete m_key;
        m_key = nullptr;
    }

    // Spawn new key at random location
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> xDist(1, GRID_WIDTH - 2);
    std::uniform_int_distribution<int> yDist(1, GRID_HEIGHT - 2);

    int keyX, keyY;
    do {
        keyX = xDist(rng);
        keyY = yDist(rng);
    } while (m_maze.isWall(keyX, keyY) ||
             (keyX == GRID_WIDTH / 2 && keyY == GRID_HEIGHT / 2));

    m_key = new Key(keyX, keyY);

    m_enemies.clear();
    m_powerups.clear();
    spawnEnemiesForRound(m_currentRound);
}

void Game::spawnPowerUp() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> xDist(1, GRID_WIDTH - 2);
    std::uniform_int_distribution<int> yDist(1, GRID_HEIGHT - 2);

    int powerupX, powerupY;
    int attempts = 0;
    bool validSpawn = false;

    while (attempts < 100 && !validSpawn) {
        powerupX = xDist(rng);
        powerupY = yDist(rng);

        if (!m_maze.isWall(powerupX, powerupY)) {
            int distFromPlayer = std::abs(powerupX - m_player.getX()) + std::abs(powerupY - m_player.getY());
            if (distFromPlayer >= 5) {
                if (!m_key || powerupX != m_key->getX() || powerupY != m_key->getY()) {
                    bool occupiedByEnemy = false;
                    for (const auto& enemy : m_enemies) {
                        if (enemy.getX() == powerupX && enemy.getY() == powerupY) {
                            occupiedByEnemy = true;
                            break;
                        }
                    }
                    if (!occupiedByEnemy) {
                        validSpawn = true;
                    }
                }
            }
        }
        attempts++;
    }

    if (validSpawn) {
        m_powerups.emplace_back(powerupX, powerupY);
    }
}

void Game::checkPowerUpCollection() {
    for (auto it = m_powerups.begin(); it != m_powerups.end();) {
        if (it->isActive() && m_player.getX() == it->getX() && m_player.getY() == it->getY()) {
            m_player.activateGhostMode();
            it->deactivate();
            std::cout << "Ghost Mode activated! You can phase through walls!" << std::endl;
            it = m_powerups.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::updatePowerUps(float deltaTime) {
    m_powerups.erase(
        std::remove_if(m_powerups.begin(), m_powerups.end(),
                       [](const PowerUp& powerup) { return !powerup.isActive(); }),
        m_powerups.end());
}

bool Game::loadTextures() {
    bool player1Loaded = m_playerTexture1.loadFromFile("src/assets/player1.png");
    bool player2Loaded = m_playerTexture2.loadFromFile("src/assets/player2.png");

    bool astar1Loaded = m_astarTexture1.loadFromFile("src/assets/astar1.png");
    bool astar2Loaded = m_astarTexture2.loadFromFile("src/assets/astar2.png");
    bool dijkstra1Loaded = m_dijkstraTexture1.loadFromFile("src/assets/dijkstra1.png");
    bool dijkstra2Loaded = m_dijkstraTexture2.loadFromFile("src/assets/dijkstra2.png");
    bool best1Loaded = m_bestTexture1.loadFromFile("src/assets/best1.png");
    bool best2Loaded = m_bestTexture2.loadFromFile("src/assets/best2.png");

    return player1Loaded && player2Loaded && astar1Loaded && astar2Loaded &&
           dijkstra1Loaded && dijkstra2Loaded && best1Loaded && best2Loaded;
}
