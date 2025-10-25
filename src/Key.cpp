#include "Key.h"

Key::Key(int startX, int startY)
    : m_gridX(startX), m_gridY(startY) {
    m_circle.setRadius(CELL_SIZE / 2.0f);
    m_circle.setFillColor(KEY_COLOR);
    m_circle.setOrigin(CELL_SIZE / 2.0f, CELL_SIZE / 2.0f);
}

void Key::render(sf::RenderWindow& window) {
    // Update circle position to match grid position
    float pixelX = m_gridX * CELL_SIZE + CELL_SIZE / 2.0f;
    float pixelY = m_gridY * CELL_SIZE + CELL_SIZE / 2.0f;
    m_circle.setPosition(pixelX, pixelY);

    window.draw(m_circle);
}
