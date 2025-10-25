#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Character.h"
#include "Config.h"

// Forward declarations
class Maze;
class Pathfinder;

/**
 * @brief Enemy AI entity that chases the player using A* pathfinding
 *
 * The enemy appears as a red rectangle and uses pathfinding to pursue
 * the player through the maze. Moves slightly slower than the player.
 */
class Enemy : public Character {
   public:
    /**
     * @brief Constructs a new Enemy object
     *
     * @param maze Reference to the maze for collision checking
     * @param pathfinder Reference to the pathfinder for AI
     * @param type Type of enemy (determines color, speed, and pathfinding)
     *
     * Spawns the enemy at a random empty location away from the center.
     */
    Enemy(const Maze& maze, Pathfinder& pathfinder, EnemyType type);

    /**
     * @brief Updates the enemy's position and AI
     *
     * @param deltaTime Time elapsed since last update
     * @param playerX Player's current X position
     * @param playerY Player's current Y position
     * @param maze Reference to the maze for pathfinding
     *
     * Handles movement timing, path following, and path recalculation.
     */
    void update(float deltaTime, int playerX, int playerY, const Maze& maze);

    /**
     * @brief Checks if the enemy has caught the player
     *
     * @param playerX Player's X position
     * @param playerY Player's Y position
     * @return true if enemy and player are in the same cell
     */
    bool hasCaughtPlayer(int playerX, int playerY) const;

   private:
    /**
     * @brief Finds a random empty spawn location
     *
     * @param maze Reference to the maze for collision checking
     * @param playerX Player's X position (to avoid spawning too close)
     * @param playerY Player's Y position (to avoid spawning too close)
     * @return Pair of (x, y) spawn coordinates
     */
    std::pair<int, int> findRandomSpawnLocation(const Maze& maze, int playerX, int playerY);

    /**
     * @brief Checks if enemy should make a random move instead of pathfinding
     *
     * @param chance Probability of making a random move (0.0 to 1.0)
     * @return true if enemy should move randomly
     */
    bool shouldMakeRandomMove(float chance) const;

    /**
     * @brief Gets a random valid adjacent cell
     *
     * @param maze Reference to the maze for collision checking
     * @return Pair of (x, y) coordinates of random adjacent cell, or current position if none found
     */
    std::pair<int, int> getRandomAdjacentCell(const Maze& maze) const;



    /**
     * @brief Calculates target position based on Pac-Man style behavior
     *
     * @param playerX Player's X position
     * @param playerY Player's Y position
     */
    void calculateTarget(int playerX, int playerY);

    const Maze& m_maze;        ///< Reference to the maze
    Pathfinder& m_pathfinder;  ///< Reference to the pathfinder
    EnemyType m_type;          ///< Type of enemy (determines behavior)

    // Movement and AI
    sf::Clock m_moveTimer;                    ///< Timer for controlling movement speed
    float m_moveDelay;                        ///< Delay between moves (type-specific)
    std::vector<std::pair<int, int>> m_path;  ///< Current path to player
    int m_pathIndex;                          ///< Current position in path
    int m_movesSincePathUpdate;               ///< Counter for path recalculation
    int m_pathUpdateInterval;                 ///< Individual path update interval (2-5)
    float m_randomMoveChance;                 ///< Chance to make random move (0.15 = 15%)
    int m_randomMoveCounter;                  ///< Counter for random moves
    static const int PATH_UPDATE_INTERVAL;    ///< Recalculate path every N moves


    // Target tracking
    int m_targetX, m_targetY;                 ///< Current target position
    
    // Distraction system for Best enemy
    bool m_isDistracted;                      ///< Whether enemy is currently distracted
    float m_distractionTimer;                 ///< Time remaining in distracted state
    float m_distractionCooldown;              ///< Cooldown before next distraction
};
