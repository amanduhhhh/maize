#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "Config.h"

/**
 * @brief Power-up collectible that grants ghost mode to the player
 *
 * Appears as a purple circle that the player can collect to gain
 * the ability to phase through walls for a limited time.
 */
class PowerUp {
   public:
    /**
     * @brief Constructs a new PowerUp object
     *
     * @param x X grid position
     * @param y Y grid position
     */
    PowerUp(int x, int y);

    /**
     * @brief Renders the power-up to the given window
     *
     * @param window Reference to the SFML render window
     */
    void render(sf::RenderWindow& window);

    /**
     * @brief Gets the power-up's current grid position
     *
     * @return Pair of (x, y) grid coordinates
     */
    std::pair<int, int> getPosition() const { return {m_gridX, m_gridY}; }

    /**
     * @brief Gets the power-up's X grid position
     *
     * @return X grid coordinate
     */
    int getX() const { return m_gridX; }

    /**
     * @brief Gets the power-up's Y grid position
     *
     * @return Y grid coordinate
     */
    int getY() const { return m_gridY; }

    /**
     * @brief Checks if the power-up is still active
     *
     * @return true if power-up is active
     */
    bool isActive() const { return m_active; }

    /**
     * @brief Deactivates the power-up (when collected)
     */
    void deactivate() { m_active = false; }

    /**
     * @brief Sets the power-up texture for sprite rendering
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
     * @brief Updates the power-up animation
     *
     * @param deltaTime Time elapsed since last update
     */
    void update(float deltaTime);

   private:
    int m_gridX;                 ///< Power-up X position in grid coordinates
    int m_gridY;                 ///< Power-up Y position in grid coordinates
    sf::CircleShape m_shape;     ///< Power-up visual representation
    bool m_active;               ///< Whether the power-up is still active
    std::unique_ptr<sf::Sprite> m_sprite;  ///< Power-up sprite representation
    bool m_useSprite;            ///< Whether to use sprite instead of circle
    
    // Animation system
    const sf::Texture* m_texture1;      ///< Pointer to first animation frame
    const sf::Texture* m_texture2;      ///< Pointer to second animation frame
    bool m_useAnimation;                ///< Whether to use animation
    bool m_currentFrame;                ///< Current animation frame (false = frame1, true = frame2)
    sf::Clock m_animationTimer;         ///< Timer for animation
    static const float ANIMATION_DELAY; ///< Delay between animation frames
};
