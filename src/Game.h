#pragma once

#include <SFML/Graphics.hpp>

#include "Config.h"
#include "Enemy.h"
#include "Key.h"
#include "Maze.h"
#include "Pathfinder.h"
#include "Player.h"
#include "PowerUp.h"

/**
 * @brief Main game class that manages the game loop, window, and player
 *
 * This class handles the core game functionality including window management,
 * event processing, game updates, and rendering. It serves as the central
 * coordinator for all game systems.
 */
class Game {
   public:
    /**
     * @brief Constructs a new Game object
     *
     * Initializes the SFML window, sets up the player rectangle,
     * and configures initial game parameters.
     */
    Game();

    /**
     * @brief Runs the main game loop
     *
     * This is the primary game loop that handles events, updates game state,
     * and renders the current frame. Continues until the window is closed.
     */
    void run();

   private:
    /**
     * @brief Processes all pending SFML events
     *
     * Handles window events such as closing, resizing, and input events.
     * Called once per frame before updating game state.
     */
    void processEvents();

    /**
     * @brief Updates game state based on elapsed time
     *
     * @param deltaTime Time elapsed since last update in seconds
     *
     * Updates all game objects, handles input, and advances game logic.
     * Uses deltaTime for frame-rate independent movement.
     */
    void update(float deltaTime);

    /**
     * @brief Checks if player has reached an exit
     *
     * @return true if player is at an exit position
     */
    bool checkWinCondition() const;

    /**
     * @brief Checks if enemy has caught the player
     *
     * @return true if enemy and player are in the same cell
     */
    bool checkGameOverCondition() const;

    /**
     * @brief Renders the current frame to the window
     *
     * Clears the window, draws all game objects, and displays the frame.
     * Called once per frame after updating game state.
     */
    void render();

    /**
     * @brief Starts a new round with regenerated maze and more enemies
     */
    void startNewRound();

    /**
     * @brief Checks if player has collected the key
     */
    void checkKeyCollection();

    /**
     * @brief Spawns enemies for the current round
     * 
     * @param roundNumber Current round number (1-5)
     */
    void spawnEnemiesForRound(int roundNumber);
    
    /**
     * @brief Starts the round transition screen
     */
    void startRoundTransition();
    
    /**
     * @brief Updates the round transition
     * 
     * @param deltaTime Time elapsed since last update
     */
    void updateRoundTransition(float deltaTime);
    
    /**
     * @brief Sets up the actual round (maze, key, enemies)
     */
    void setupRound();

    /**
     * @brief Spawns a new power-up at a random location
     */
    void spawnPowerUp();

    /**
     * @brief Checks for power-up collection by the player
     */
    void checkPowerUpCollection();

    /**
     * @brief Updates all power-ups and removes inactive ones
     *
     * @param deltaTime Time elapsed since last update
     */
    void updatePowerUps(float deltaTime);


    sf::RenderWindow m_window;
    Player m_player;
    Maze m_maze;

    // Enemy AI
    Pathfinder m_pathfinder;
    std::vector<Enemy> m_enemies;
    Key* m_key;
    bool m_hasKey;
    int m_currentRound;
    bool m_gameOver;
    
    // Power-ups
    std::vector<PowerUp> m_powerups;
    sf::Clock m_powerupSpawnTimer;
    
    // Round transition
    bool m_roundTransition;
    float m_transitionTimer;
    sf::Font m_font;
    sf::Text m_roundText;
};
