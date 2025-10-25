#include "Game.h"

#include <algorithm>
#include <iostream>

Game::Game()
    : m_window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Oubliette - Maze Chase Game"), m_player(GRID_WIDTH / 2, GRID_HEIGHT / 2), m_pathfinder(), m_key(nullptr), m_hasKey(false), m_currentRound(1), m_gameOver(false), m_roundTransition(false), m_transitionTimer(0.0f), m_roundText(m_font) {
    // Load font for round text
    if (!m_font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        // Fallback if font loading fails
        std::cout << "Warning: Could not load font, using default" << std::endl;
    }

    m_roundText.setFont(m_font);
    m_roundText.setCharacterSize(48);
    m_roundText.setFillColor(sf::Color::White);
    m_roundText.setStyle(sf::Text::Bold);

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
        return;  // Don't update if game is over
    }

    if (m_roundTransition) {
        updateRoundTransition(deltaTime);
        return;
    }

    m_player.handleInput(m_maze);
    m_player.updateGhostMode(deltaTime);

    checkKeyCollection();

    checkPowerUpCollection();

    updatePowerUps(deltaTime);

    if (m_powerupSpawnTimer.getElapsedTime().asSeconds() >= POWERUP_SPAWN_INTERVAL && m_powerups.size() < MAX_POWERUPS) {
        spawnPowerUp();
        m_powerupSpawnTimer.restart();
    }

    // Update all enemies
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

    // Render key if not collected
    if (m_key && !m_hasKey) {
        m_key->render(m_window);
    }

    // Render all power-ups
    for (auto& powerup : m_powerups) {
        powerup.render(m_window);
    }

    // Render all enemies
    for (auto& enemy : m_enemies) {
        enemy.render(m_window);
    }

    m_player.render(m_window);

    // Round transition screen
    if (m_roundTransition) {
        // Black background
        sf::RectangleShape blackOverlay;
        blackOverlay.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        blackOverlay.setFillColor(sf::Color::Black);
        m_window.draw(blackOverlay);

        // Round text
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

    // Add player position to used positions
    usedPositions.push_back({m_player.getX(), m_player.getY()});

    std::vector<EnemyType> enemyTypes = {EnemyType::ASTAR, EnemyType::DIJKSTRA, EnemyType::BEST};

    for (EnemyType type : enemyTypes) {
        // Find spawn location that doesn't overlap with used positions
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> xDist(1, GRID_WIDTH - 2);
        std::uniform_int_distribution<int> yDist(1, GRID_HEIGHT - 2);

        int attempts = 0;
        int enemyX, enemyY;
        bool validSpawn = false;

        while (attempts < 100 && !validSpawn) {
            enemyX = xDist(rng);
            enemyY = yDist(rng);

            // Check if position is valid and not used
            if (!m_maze.isWall(enemyX, enemyY)) {
                // Check distance from player
                int distFromPlayer = std::abs(enemyX - m_player.getX()) + std::abs(enemyY - m_player.getY());

                if (distFromPlayer >= 10) {
                    validSpawn = true;
                    for (const auto& pos : usedPositions) {
                        if (pos.first == enemyX && pos.second == enemyY) {
                            validSpawn = false;
                            break;
                        }
                    }
                }
            }
            attempts++;
        }

        if (validSpawn) {
            m_enemies.emplace_back(m_maze, m_pathfinder, type);
            m_enemies.back().setPosition(enemyX, enemyY);
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

    const float TRANSITION_DURATION = 2.0f;  // 2 seconds

    if (m_transitionTimer >= TRANSITION_DURATION) {
        m_roundTransition = false;
        setupRound();
    }
}

void Game::setupRound() {
    m_maze.regenerate();

    m_player.setPosition(GRID_WIDTH / 2, GRID_HEIGHT / 2);

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
             (keyX == GRID_WIDTH / 2 && keyY == GRID_HEIGHT / 2));  // Avoid center where player spawns

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

