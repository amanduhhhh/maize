#pragma once

#include "Character.h"

// Forward declarations
class Maze;

/**
 * @brief Player character controlled by user input
 *
 * The player appears as a blue rectangle and is controlled via keyboard input.
 * Moves one grid cell at a time with collision detection against walls.
 */
class Player : public Character {
   public:
    /**
     * @brief Constructs a new Player object
     *
     * @param startX Initial X grid position
     * @param startY Initial Y grid position
     */
    Player(int startX, int startY);

    /**
     * @brief Handles player input for grid-based movement
     *
     * @param maze Reference to the maze for collision checking
     *
     * Processes arrow keys and moves player one grid cell at a time.
     * Checks for wall collisions before moving.
     */
    void handleInput(const Maze& maze);

    /**
     * @brief Checks if enough time has passed for movement
     *
     * @return true if player can move
     */
    bool canMove() const;

    /**
     * @brief Activates ghost mode for the player
     *
     * Allows the player to phase through walls for a limited time.
     */
    void activateGhostMode();

    /**
     * @brief Updates ghost mode timer
     *
     * @param deltaTime Time elapsed since last update
     */
    void updateGhostMode(float deltaTime);

    /**
     * @brief Checks if player is currently in ghost mode
     *
     * @return true if ghost mode is active
     */
    bool isGhostMode() const;


   private:
    sf::Clock m_moveTimer;          ///< Timer for controlling movement speed
    static const float MOVE_DELAY;  ///< Delay between moves

    // Ghost mode
    bool m_ghostMode;               ///< Whether ghost mode is active
    float m_ghostModeTimer;         ///< Time remaining in ghost mode
    sf::Color m_originalColor;      ///< Original player color
};
