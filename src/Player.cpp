#include "Player.h"

#include <SFML/Window/Keyboard.hpp>

#include "Maze.h"

const float Player::MOVE_DELAY = 0.1f;

Player::Player(int startX, int startY)
    : Character(startX, startY, PLAYER_COLOR) {
}

void Player::handleInput(const Maze& maze) {
    if (!canMove()) {
        return;
    }

    int oldX = getX();
    int oldY = getY();
    bool moved = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        setPosition(getX(), getY() - 1);
        moved = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        setPosition(getX(), getY() + 1);
        moved = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        setPosition(getX() - 1, getY());
        moved = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        setPosition(getX() + 1, getY());
        moved = true;
    }

    if (moved) {
        if (maze.isValidPosition(getX(), getY()) && maze.isWall(getX(), getY())) {
            setPosition(oldX, oldY);
        }
        m_moveTimer.restart();
    }
}

bool Player::canMove() const {
    return m_moveTimer.getElapsedTime().asSeconds() >= MOVE_DELAY;
}
