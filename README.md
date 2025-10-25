# Oubliette - Maze Chase 



https://github.com/user-attachments/assets/910f677a-7949-48af-88c2-156afbe7aa35



A maze-based chase game built with C++ and SFML where you must collect a key before escaping through the exit doors. 
To run game (for windows), please download the latest zip from releases. Mac/linux users will need to build from source code.

## How to Play

### Objective
- You are the blue square. Find the yellow key before escaping through the brown exit doors. Red enemies will chase you

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

#### **Enemy AI - A\* Pathfinding with Randomness**
Enemies use A* pathfinding algorithm to chase the player:

```cpp
void Enemy::update() {
    if (shouldMakeRandomMove()) {
        // 10-20% chance to move randomly instead of pathfinding
        moveToRandomAdjacentCell();
    } else {
        path = pathfinder.findPath(currentPos, playerPos);
        followPath();
    }
}
```

#### **Pathfinding Algorithm**
The A* implementation uses Manhattan distance heuristic:

```cpp
std::vector<PathNode> Pathfinder::findPath(start, goal) {
    // Priority queue for open nodes
    // Manhattan distance: |x1-x2| + |y1-y2|
    // G-cost: distance from start
    // H-cost: estimated distance to goal
    // F-cost: G + H (total estimated cost)
}
```

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
