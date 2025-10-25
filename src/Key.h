#pragma once

#include <SFML/Graphics.hpp>

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
};
