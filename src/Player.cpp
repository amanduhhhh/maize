#include "Player.h"

#include <iostream>
#include <SFML/Window/Keyboard.hpp>

#include "Maze.h"

const float Player::MOVE_DELAY = 0.1f;

Player::Player(int startX, int startY)
    : Character(startX, startY, PLAYER_COLOR), m_ghostMode(false), m_ghostModeTimer(0.0f), m_originalColor(PLAYER_COLOR), m_facingLeft(false) {
}

void Player::handleInput(const Maze& maze, bool hasKey) {
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
        if (!m_facingLeft) {
            m_facingLeft = true;
            setFlipped(true);
        }
        moved = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        setPosition(getX() + 1, getY());
        if (m_facingLeft) {
            m_facingLeft = false;
            setFlipped(false);
        }
        moved = true;
    }

    if (moved) {
        bool isValidPos = maze.isValidPosition(getX(), getY());
        bool isWall = maze.isWall(getX(), getY());
        
        if (!isValidPos) {
            if (hasKey) {
                advanceAnimation();
            } else {
                setPosition(oldX, oldY);
            }
        } else {
            bool isBoundaryWall = isWall && 
                                  (getX() == 0 || getX() == maze.getGridWidth() - 1 || 
                                   getY() == 0 || getY() == maze.getGridHeight() - 1);
            
            if (isWall && (!m_ghostMode || isBoundaryWall)) {
                setPosition(oldX, oldY);
            } else {
                advanceAnimation();
            }
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

void Player::resetGhostMode() {
    m_ghostMode = false;
    m_ghostModeTimer = 0.0f;
    setColor(m_originalColor);
}
