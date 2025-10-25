#include "PowerUp.h"

PowerUp::PowerUp(int x, int y) : m_gridX(x), m_gridY(y), m_active(true) {
    m_shape.setRadius(CELL_SIZE / 2.5f);
    m_shape.setFillColor(POWERUP_COLOR);
    m_shape.setOrigin(sf::Vector2f(m_shape.getRadius(), m_shape.getRadius()));

    float pixelX = m_gridX * CELL_SIZE + CELL_SIZE / 2.0f;
    float pixelY = m_gridY * CELL_SIZE + CELL_SIZE / 2.0f;
    m_shape.setPosition(sf::Vector2f(pixelX, pixelY));
}

void PowerUp::render(sf::RenderWindow& window) {
    if (m_active) {
        window.draw(m_shape);
    }
}
