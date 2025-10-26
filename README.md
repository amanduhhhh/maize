# Oubliette - Maze Chase 
<img width="810" height="810" alt="oubliette" src="https://github.com/user-attachments/assets/2e3b07bb-f7df-44ba-8272-b2c6cb0c93eb" />


https://github.com/user-attachments/assets/69982b72-35eb-4b8f-957a-db2826180b16



A maze-based chase game built with C++ and SFML where you must collect a key before escaping through the exit doors. 
To run game (for windows), please download the latest zip from releases. Mac/linux users will need to build from source code.

## How to Play

### Objective
- Dogmog (the little dog with a blue hat). You must find the key and escape the maze, while being hunted by 3 alien enemies! Eat (non-poisonous) chocolate bars to go ghost mode. 

### Controls
- Move your character (Up, Down, Left, Right)

## Technical Architecture

### Backend Systems

#### **Maze Generation - Depth-First Search (DFS)**
The maze is generated using a recursive backtracking algorithm:
- start with all cells as walls
- begin dfs from center
- recursively carve paths by removing walls (this will ensure every point is reachable from the center)
- add random branching for complexity 

#### **Enemy AI - Different pathfinding algorithms**
The three enemies use their own pathfinding algorithms to catch the player: Red uses A*, orange used Best-first (greedy), and yellow uses dijkstra!
To show the differences in their behaviours, we made yellow the slowest and orange the fastest. However, orange also has a chance of targeting the wrong square, to simulate its inaccuracies. 

### Class Hierarchy

```
Character (conains render and position update functions)
- Player inherits from Character 
- Enemy inherits from Character

Key (rendering and updating key logic)
Maze (DFS algorithm, rendering and generation)
Pathfinder (Enemy A* algorihtm logic)
Game (Handles game loop and state updates)
```

To build the game yourself, please download an appropriate SFML version for your compiler and extract it to the root folder. You can then run CMake. 
