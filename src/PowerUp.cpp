#include "PowerUp.h"

const float PowerUp::ANIMATION_DELAY = ITEM_ANIMATION_DELAY;

PowerUp::PowerUp(int x, int y) : m_gridX(x), m_gridY(y), m_active(true), m_useSprite(false), m_texture1(nullptr), m_texture2(nullptr), m_useAnimation(false), m_currentFrame(false) {
    m_shape.setRadius(CELL_SIZE / 2.5f);
    m_shape.setFillColor(POWERUP_COLOR);
    m_shape.setOrigin(sf::Vector2f(m_shape.getRadius(), m_shape.getRadius()));

    float pixelX = m_gridX * CELL_SIZE + CELL_SIZE / 2.0f;
    float pixelY = m_gridY * CELL_SIZE + CELL_SIZE / 2.0f;
    m_shape.setPosition(sf::Vector2f(pixelX, pixelY));
}

void PowerUp::setTexture(const sf::Texture& texture) {
    m_sprite = std::make_unique<sf::Sprite>(texture);
    
    sf::Vector2u textureSize = texture.getSize();
    float scaleX = (static_cast<float>(CELL_SIZE) / textureSize.x) * ITEM_SPRITE_SCALE_MULTIPLIER;
    float scaleY = (static_cast<float>(CELL_SIZE) / textureSize.y) * ITEM_SPRITE_SCALE_MULTIPLIER;
    m_sprite->setScale(sf::Vector2f(scaleX, scaleY));
    
    // Center the sprite
    m_sprite->setOrigin(sf::Vector2f(textureSize.x * 0.5f, textureSize.y * 0.5f));
    
    m_useSprite = true;
}

void PowerUp::setAnimationTextures(const sf::Texture& texture1, const sf::Texture& texture2) {
    m_texture1 = &texture1;
    m_texture2 = &texture2;
    m_useAnimation = true;
    m_useSprite = true;
    m_currentFrame = false;
    
    m_sprite = std::make_unique<sf::Sprite>(*m_texture1);
    
    sf::Vector2u textureSize = texture1.getSize();
    float scaleX = (static_cast<float>(CELL_SIZE) / textureSize.x) * ITEM_SPRITE_SCALE_MULTIPLIER;
    float scaleY = (static_cast<float>(CELL_SIZE) / textureSize.y) * ITEM_SPRITE_SCALE_MULTIPLIER;
    m_sprite->setScale(sf::Vector2f(scaleX, scaleY));
    
    m_sprite->setOrigin(sf::Vector2f(textureSize.x * 0.5f, textureSize.y * 0.5f));
}

void PowerUp::update(float deltaTime) {
    if (m_useAnimation && m_animationTimer.getElapsedTime().asSeconds() >= ANIMATION_DELAY) {
        m_currentFrame = !m_currentFrame;
        m_animationTimer.restart();
    }
}

void PowerUp::render(sf::RenderWindow& window) {
    if (m_active) {
        float pixelX = m_gridX * CELL_SIZE + CELL_SIZE / 2.0f;
        float pixelY = m_gridY * CELL_SIZE + CELL_SIZE / 2.0f;
        
        if (m_useSprite && m_sprite) {
            if (m_useAnimation) {
                if (m_currentFrame && m_texture2) {
                    m_sprite->setTexture(*m_texture2);
                } else if (m_texture1) {
                    m_sprite->setTexture(*m_texture1);
                }
            }
            m_sprite->setPosition(sf::Vector2f(pixelX, pixelY));
            window.draw(*m_sprite);
        } else {
            window.draw(m_shape);
        }
    }
}
