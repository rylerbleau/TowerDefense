# TowerDefense

This project is a tower defense game with a focus on AI algorithms such as steering behaviors, decision-making logic, and pathfinding algorithms, all coming together to intelligent enemy behavior.

## Description
In TowerCarnage, players defend their towers against AI-controlled enemies. These enemies use a mix of algorithms to move around the game world. The player must carefully observe the enemy AI patterns and strategically place turrets to protect the towers. This game aims to demonstrate how different AI techniques can be incorporated into a game to create a fun and dynamic experience.

## Key AI Features
- **Steering Behaviors:** The AI enemies in TowerCarnage are using kinematic behaviors such as Separation, Arrive, and Flee, enabling them to navigate toward targets which may be turrets or towers.
- **Decision Making:** At the core of AI's intelligence is a behavior tree-based system. This allows the AI to make decisions based on their surrounding environment.
- **Pathfinding:** Implemented Dijkstra's algorithm to ensure that the AI can target the end goal towers. This enables the AI to find the shortest routes towards the player's defenses, posing a continuous strategic challenge.

## Getting Started
### Prerequisites
- Visual Studio 2019 or newer

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/rylerbleau/TowerDefense.git
   ```
2. Open the solution file in Visual Studio.
3. Set the build configuration to either 32-bit Release or Debug mode.
4. Build and run the project.
   
   All necessary libraries are pre-built and included in the repository.

## Usage
- **Game Controls:**
  - **Left Mouse Button:** Use this to place characters. Note that characters can only be placed along the path tiles.
  - **Right Mouse Button:** This button is used to place turrets. Turrets can only be spawned on grass tiles.
  - **Middle Mouse Button:** Use this to change the end node along the path.

- **Customizing Maps:**
  - For those interested in customizing the game maps, navigate to the `assets/levels` folder. Here, you can alter or create a new layout of the maps.
  - <details>
<summary>## Tile Agenda for Characters (Click to Expand/Collapse)</summary>

- 'P': Path Tile
- 'G': Grass Tile
- 'F': Flower Tile
- 'T': Green Tree on Grass Tile
- 'O': Orange Tree on Grass Tile
- 'R': Right Grass Tile
- 'L': Left Grass Tile
- 'B': Bottom Grass Tile
- 'N': Top Grass Tile
- '1': Bottom Left Grass Corner Tile
- '2': Top Right Grass Corner Tile
- 'C': Building with Door Tile
- 'S': Rock Tile
- 'W': Well on Rock Tile
- 'J': Stock on Rock Tile
- 'V': Board on Rock Tile

</details>

