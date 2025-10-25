#include "Player.h"

#include <iostream>
#include <SFML/Window/Keyboard.hpp>

#include "Maze.h"

const float Player::MOVE_DELAY = 0.1f;

Player::Player(int startX, int startY)
    : Character(startX, startY, PLAYER_COLOR), m_ghostMode(false), m_ghostModeTimer(0.0f), m_originalColor(PLAYER_COLOR) {
}

void Player::handleInput(const Maze& maze) {
    if (!canMove()) {
        return;
    }

    int oldX = getX();
    int oldY = getY();
    bool moved = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        setPosition(getX(), getY() - 1);
        moved = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        setPosition(getX(), getY() + 1);
        moved = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        setPosition(getX() - 1, getY());
        moved = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        setPosition(getX() + 1, getY());
        moved = true;
    }

    if (moved) {
        if (!m_ghostMode && maze.isValidPosition(getX(), getY()) && maze.isWall(getX(), getY())) {
            setPosition(oldX, oldY);
        } else {
            advanceAnimation();
        }
        m_moveTimer.restart();
    }
}

bool Player::canMove() const {
    return m_moveTimer.getElapsedTime().asSeconds() >= MOVE_DELAY;
}

void Player::activateGhostMode() {
    m_ghostMode = true;
    m_ghostModeTimer = GHOST_MODE_DURATION;
    setColor(POWERUP_COLOR);  
}

void Player::updateGhostMode(float deltaTime) {
    if (m_ghostMode) {
        m_ghostModeTimer -= deltaTime;
        if (m_ghostModeTimer <= 0.0f) {
            m_ghostMode = false;
            m_ghostModeTimer = 0.0f;
            setColor(m_originalColor);  
        }
    }
}

bool Player::isGhostMode() const {
    return m_ghostMode;
}
