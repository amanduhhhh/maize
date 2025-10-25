#include "Character.h"

Character::Character(int startX, int startY, sf::Color color)
    : m_gridX(startX), m_gridY(startY), m_useSprite(false), m_texture1(nullptr), m_texture2(nullptr), m_useAnimation(false), m_currentFrame(false) {
    m_shape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    m_shape.setFillColor(color);
    // m_sprite will be initialized when textures are set
    updatePosition();
}

void Character::render(sf::RenderWindow& window) {
    if (m_useSprite && m_sprite) {
        // Update sprite texture to current animation frame
        if (m_useAnimation) {
            if (m_currentFrame && m_texture2) {
                m_sprite->setTexture(*m_texture2);
            } else if (m_texture1) {
                m_sprite->setTexture(*m_texture1);
            }
        }
        window.draw(*m_sprite);
    } else {
        window.draw(m_shape);
    }
}

void Character::setPosition(int x, int y) {
    m_gridX = x;
    m_gridY = y;
    updatePosition();
}

void Character::setColor(sf::Color color) {
    m_shape.setFillColor(color);
}

void Character::setTexture(const sf::Texture& texture) {
    // In SFML 3, sprite requires a texture in constructor
    m_sprite = std::make_unique<sf::Sprite>(texture);
    // Scale sprite larger than cell size for more visual presence
    sf::Vector2u textureSize = texture.getSize();
    float scaleX = (static_cast<float>(CELL_SIZE) / textureSize.x) * SPRITE_SCALE_MULTIPLIER;
    float scaleY = (static_cast<float>(CELL_SIZE) / textureSize.y) * SPRITE_SCALE_MULTIPLIER;
    m_sprite->setScale(sf::Vector2f(scaleX, scaleY));
    
    // Center the sprite in its cell since it's now bigger
    m_sprite->setOrigin(sf::Vector2f(textureSize.x * 0.5f, textureSize.y * 0.5f));
}

void Character::setUseSprite(bool useSprite) {
    m_useSprite = useSprite;
}

void Character::setAnimationTextures(const sf::Texture& texture1, const sf::Texture& texture2) {
    m_texture1 = &texture1;
    m_texture2 = &texture2;
    m_useAnimation = true;
    m_useSprite = true;
    m_currentFrame = false;
    
    // Create sprite with first texture (SFML 3 requires texture in constructor)
    m_sprite = std::make_unique<sf::Sprite>(*m_texture1);
    
    // Scale sprite larger than cell size for more visual presence
    sf::Vector2u textureSize = texture1.getSize();
    float scaleX = (static_cast<float>(CELL_SIZE) / textureSize.x) * SPRITE_SCALE_MULTIPLIER;
    float scaleY = (static_cast<float>(CELL_SIZE) / textureSize.y) * SPRITE_SCALE_MULTIPLIER;
    m_sprite->setScale(sf::Vector2f(scaleX, scaleY));
    
    // Center the sprite in its cell since it's now bigger
    m_sprite->setOrigin(sf::Vector2f(textureSize.x * 0.5f, textureSize.y * 0.5f));
}

void Character::advanceAnimation() {
    if (m_useAnimation) {
        m_currentFrame = !m_currentFrame;
    }
}

void Character::updatePosition() {
    float pixelX = m_gridX * CELL_SIZE;
    float pixelY = m_gridY * CELL_SIZE;
    m_shape.setPosition(sf::Vector2f(pixelX, pixelY));
    if (m_sprite) {
        // Center sprite in cell (since we set origin to center)
        m_sprite->setPosition(sf::Vector2f(pixelX + CELL_SIZE / 2.0f, pixelY + CELL_SIZE / 2.0f));
    }
}
