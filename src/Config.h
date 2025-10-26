#pragma once

#include <SFML/Graphics.hpp>

/**
 * @file Config.h
 * @brief Game configuration constants
 */

// Maze dimensions
const int MAZE_WIDTH = 21;  
const int MAZE_HEIGHT = 21; 
const int CELL_SIZE = 36; 
const float SPRITE_SCALE_MULTIPLIER = 2.4f; 

// Grid dimensions (includes walls between cells)
const int GRID_WIDTH = MAZE_WIDTH * 2 + 1;    ///< grid width (51)
const int GRID_HEIGHT = MAZE_HEIGHT * 2 + 1;  ///< grid height (51)

// Window dimensions
const int WINDOW_WIDTH = GRID_WIDTH * CELL_SIZE;    
const int WINDOW_HEIGHT = GRID_HEIGHT * CELL_SIZE; 

// Grid cell types
const int CELL_EMPTY = 0;
const int CELL_WALL = 1;

// Player settings
const float PLAYER_MOVE_DELAY = 0.15f;

// Enemy types
enum class EnemyType {
    ASTAR,
    DIJKSTRA,
    BEST
};

// Power-up settings
const sf::Color POWERUP_COLOR = sf::Color(128, 0, 128);  // Purple
const float GHOST_MODE_DURATION = 3.0f;
const float POWERUP_SPAWN_INTERVAL = 8.0f;
const int MAX_POWERUPS = 3;


// Enemy-specific settings
const sf::Color ASTAR_ENEMY_COLOR = sf::Color(255, 0, 0);
const sf::Color DIJKSTRA_ENEMY_COLOR = sf::Color(0, 100, 255);
const sf::Color BEST_ENEMY_COLOR = sf::Color(255, 165, 0);

const float ASTAR_ENEMY_DELAY = 0.15f;
const float DIJKSTRA_ENEMY_DELAY = 0.22f;
const float BEST_ENEMY_DELAY = 0.10f;  // Faster than player (0.15f)

// Best enemy distraction system
const float DISTRACTION_DURATION = 3.0f;     // How long distraction lasts
const float DISTRACTION_COOLDOWN = 8.0f;     // Cooldown between distractions
const float DISTRACTION_CHANCE = 0.15f;      // Chance to get distracted when cooldown is up



// Game colors
const sf::Color PLAYER_COLOR = sf::Color::Blue;
const sf::Color ENEMY_COLOR = sf::Color::Red;
const sf::Color KEY_COLOR = sf::Color::Yellow;
const sf::Color EXIT_COLOR = sf::Color(139, 69, 19);  // Brown
const sf::Color WALL_COLOR = sf::Color(100, 100, 100);  // Gray
const sf::Color PATH_COLOR = sf::Color::Black;