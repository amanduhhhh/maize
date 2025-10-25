#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "Config.h"

/**
 * @brief Base class for all movable characters in the game
 *
 * Provides common functionality for position management, rendering,
 * and visual representation that is shared between Player and Enemy.
 */
class Character {
   public:
    /**
     * @brief Constructs a new Character object
     *
     * @param startX Initial X grid position
     * @param startY Initial Y grid position
     * @param color Color of the character's visual representation
     */
    Character(int startX, int startY, sf::Color color);

    /**
     * @brief Renders the character to the given window
     *
     * @param window Reference to the SFML render window
     */
    void render(sf::RenderWindow& window);

    /**
     * @brief Gets the character's current grid position
     *
     * @return Pair of (x, y) grid coordinates
     */
    std::pair<int, int> getPosition() const { return {m_gridX, m_gridY}; }

    /**
     * @brief Gets the character's X grid position
     *
     * @return X grid coordinate
     */
    int getX() const { return m_gridX; }

    /**
     * @brief Gets the character's Y grid position
     *
     * @return Y grid coordinate
     */
    int getY() const { return m_gridY; }

    /**
     * @brief Sets the character's grid position
     *
     * @param x New X grid coordinate
     * @param y New Y grid coordinate
     */
    void setPosition(int x, int y);

    /**
     * @brief Sets the character's color
     *
     * @param color New color for the character's visual representation
     */
    void setColor(sf::Color color);

    /**
     * @brief Sets the character's texture (for sprites)
     *
     * @param texture Reference to the texture to use
     */
    void setTexture(const sf::Texture& texture);

    /**
     * @brief Sets the character to use sprite mode
     *
     * @param useSprite Whether to use sprite instead of shape
     */
    void setUseSprite(bool useSprite);

    /**
     * @brief Sets up animation with two textures
     *
     * @param texture1 First animation frame
     * @param texture2 Second animation frame
     */
    void setAnimationTextures(const sf::Texture& texture1, const sf::Texture& texture2);

    /**
     * @brief Advances the animation to the next frame (call on each move)
     */
    void advanceAnimation();

   protected:
    /**
     * @brief Updates the character's visual position based on grid coordinates
     */
    void updatePosition();

    int m_gridX;                 ///< Character X position in grid coordinates
    int m_gridY;                 ///< Character Y position in grid coordinates
    sf::RectangleShape m_shape;  ///< Character visual representation (shape mode)
    std::unique_ptr<sf::Sprite> m_sprite;  ///< Character visual representation (sprite mode)
    bool m_useSprite;            ///< Whether to use sprite instead of shape
    
    // Animation system
    const sf::Texture* m_texture1;      ///< Pointer to first animation frame
    const sf::Texture* m_texture2;      ///< Pointer to second animation frame
    bool m_useAnimation;                ///< Whether to use animation
    bool m_currentFrame;                ///< Current animation frame (false = frame1, true = frame2)
};
