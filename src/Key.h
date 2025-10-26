#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "Config.h"


// Forward declarations
class Maze;

/**
 * @brief Key item that player must collect to unlock exits
 *
 * The key appears as a yellow circle and must be collected before
 * the player can escape through the brown exit tiles.
 */
class Key {
   public:
    /**
     * @brief Constructs a new Key object
     *
     * @param startX Initial X grid position
     * @param startY Initial Y grid position
     */
    Key(int startX, int startY);

    /**
     * @brief Renders the key to the given window
     *
     * @param window Reference to the SFML render window
     */
    void render(sf::RenderWindow& window);

    /**
     * @brief Sets the key texture for sprite rendering
     *
     * @param texture Reference to the texture to use
     */
    void setTexture(const sf::Texture& texture);

    /**
     * @brief Sets up animation with two textures
     *
     * @param texture1 First animation frame
     * @param texture2 Second animation frame
     */
    void setAnimationTextures(const sf::Texture& texture1, const sf::Texture& texture2);

    /**
     * @brief Updates the key animation
     *
     * @param deltaTime Time elapsed since last update
     */
    void update(float deltaTime);

    /**
     * @brief Gets the key's current grid position
     *
     * @return Pair of (x, y) grid coordinates
     */
    std::pair<int, int> getPosition() const { return {m_gridX, m_gridY}; }

    /**
     * @brief Gets the key's X grid position
     *
     * @return X grid coordinate
     */
    int getX() const { return m_gridX; }

    /**
     * @brief Gets the key's Y grid position
     *
     * @return Y grid coordinate
     */
    int getY() const { return m_gridY; }

   private:
    int m_gridX;               ///< Key X position in grid coordinates
    int m_gridY;               ///< Key Y position in grid coordinates
    sf::CircleShape m_circle;  ///< Key visual representation (yellow circle)
    std::unique_ptr<sf::Sprite> m_sprite;  ///< Key sprite representation
    bool m_useSprite;          ///< Whether to use sprite instead of circle
    
    // Animation system
    const sf::Texture* m_texture1;      ///< Pointer to first animation frame
    const sf::Texture* m_texture2;      ///< Pointer to second animation frame
    bool m_useAnimation;                ///< Whether to use animation
    bool m_currentFrame;                ///< Current animation frame (false = frame1, true = frame2)
    sf::Clock m_animationTimer;         ///< Timer for animation
    static const float ANIMATION_DELAY; ///< Delay between animation frames
};
