#pragma once

#include <SFML/Graphics.hpp>

/**
 * @file Config.h
 * @brief Game configuration constants
 */

// Maze dimensions
const int MAZE_WIDTH = 25;
const int MAZE_HEIGHT = 25;
const int CELL_SIZE = 12;

// Grid dimensions (includes walls between cells)
const int GRID_WIDTH = MAZE_WIDTH * 2 + 1;    ///< grid width (51)
const int GRID_HEIGHT = MAZE_HEIGHT * 2 + 1;  ///< grid height (51)

// Window dimensions
const int WINDOW_WIDTH = GRID_WIDTH * CELL_SIZE;    ///< (816px)
const int WINDOW_HEIGHT = GRID_HEIGHT * CELL_SIZE;  ///< (816px)

// Grid cell types
const int CELL_EMPTY = 0;
const int CELL_WALL = 1;

// Player settings
const float PLAYER_MOVE_DELAY = 0.15f;

// Game colors
const sf::Color PLAYER_COLOR = sf::Color::Blue;
const sf::Color ENEMY_COLOR = sf::Color::Red;
const sf::Color KEY_COLOR = sf::Color::Yellow;
const sf::Color EXIT_COLOR = sf::Color(139, 69, 19);  // Brown
const sf::Color WALL_COLOR = sf::Color(100, 100, 100);  // Gray
const sf::Color PATH_COLOR = sf::Color::Black;