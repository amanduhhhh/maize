#include "Character.h"

Character::Character(int startX, int startY, sf::Color color)
    : m_gridX(startX), m_gridY(startY) {
    m_shape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    m_shape.setFillColor(color);
    updatePosition();
}

void Character::render(sf::RenderWindow& window) {
    window.draw(m_shape);
}

void Character::setPosition(int x, int y) {
    m_gridX = x;
    m_gridY = y;
    updatePosition();
}

void Character::updatePosition() {
    float pixelX = m_gridX * CELL_SIZE;
    float pixelY = m_gridY * CELL_SIZE;
    m_shape.setPosition(sf::Vector2f(pixelX, pixelY));
}
