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
     * @param hasKey Whether the player has collected the key
     *
     * Processes arrow keys and moves player one grid cell at a time.
     * Checks for wall collisions before moving.
     */
    void handleInput(const Maze& maze, bool hasKey);

    /**
     * @brief Checks if enough time has passed for movement
     *
     * @return true if player can move
     */
    bool canMove() const;

    /**
     * @brief Activates ghost mode allowing wall phasing
     */
    void activateGhostMode();

    /**
     * @brief Updates ghost mode timer and deactivates when expired
     *
     * @param deltaTime Time elapsed since last frame
     */
    void updateGhostMode(float deltaTime);

    /**
     * @brief Checks if player is currently in ghost mode
     *
     * @return true if in ghost mode
     */
    bool isGhostMode() const;

    /**
     * @brief Resets ghost mode (used between rounds)
     */
    void resetGhostMode();

   private:
    sf::Clock m_moveTimer;          ///< Timer for controlling movement speed
    static const float MOVE_DELAY;  ///< Delay between moves
    
    // Ghost mode system
    bool m_ghostMode;               ///< Whether player can phase through walls
    float m_ghostModeTimer;         ///< Time remaining in ghost mode
    sf::Color m_originalColor;      ///< Original player color to restore
    
    // Sprite direction
    bool m_facingLeft;              ///< Whether player is facing left (sprite flipped)
};
