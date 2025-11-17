# Death Riddle 🎮# Death Riddle



A 2D platformer puzzle game where you must solve riddles to escape from the mysterious Game Master.An Interactive CTF-style GUI game where you must escape from the Game Master through solving riddles and making critical decisions.



## 📖 Story## Story



You find yourself trapped in a mysterious realm controlled by the enigmatic Game Master. To escape and regain your freedom, you must navigate through 6 challenging levels filled with enemies, obstacles, and mind-bending riddles. Each enemy guards a riddle - answer correctly to defeat them and progress, but wrong answers will cost you dearly.You wake up in a mysterious realm, bound by the will of an enigmatic Game Master. Your memories are fragmented, and the only way to escape is to prove your worth by solving deadly riddles. Each decision you make affects your fate, and multiple endings await based on your choices, wit, and courage.



Can you outwit the Game Master and escape to freedom?**Can you outsmart Death itself?**



## 🎯 Features## Game Endings



### Gameplay Mechanics1. **True Freedom** - Solve all riddles perfectly and outsmart the Game Master

- **6 Unique Levels**: Progress through increasingly challenging environments2. **The Deal** - Make a pact with the Game Master to become their apprentice

  - Levels 1-5: Classic platformer gameplay with jumping, enemies, and riddles3. **Partial Escape** - Escape but at a great cost (lose health/memories)

  - Level 6: Top-down maze navigation to find the key and escape4. **The Loop** - Fall into the Game Master's trap and restart the cycle

5. **Death's Embrace** - Fail too many riddles and succumb to the darkness

- **Enemy Riddles System**: Encounter animated enemies that challenge you with riddles

  - Pink Monsters and Owlet Monsters patrol each level## Features

  - Answer correctly: Defeat the enemy, gain points (+500), restore health (+25 HP)

  - Answer incorrectly: Take damage (-25 HP) and must try again- Interactive GUI built with Qt6

- Health bar system with consequences

- **Double Jump**: Advanced movement mechanic with 3-second cooldown- Animated player character and Game Master

  - Press jump while in air for extra height- CTF-style riddles and puzzles

  - Essential for reaching difficult platforms in later levels- Multiple branching storylines

- Save/Load game progress

- **Score System**: - Atmospheric effects and animations

  - Collect coins scattered throughout levels

  - Solve riddles for bonus points## Building the Project

  - Track highest score across play sessions

### Prerequisites

- **Lives & Health**:- CMake 3.16 or higher

  - Start with 3 lives and 100 HP- Qt6 (Core, Widgets, Gui, Multimedia)

  - Avoid spikes and enemy contact- C++17 compatible compiler

  - Retry screen when you die

### Build Instructions

### Level Highlights

1. **Level 1 - Tutorial**: Learn basic controls and face your first riddle```bash

2. **Level 2 - First Challenge**: Math riddles and precise jumpsmkdir build

3. **Level 3 - Moving Platforms**: Timing and logic puzzlescd build

4. **Level 4 - Spike Gauntlet**: Binary riddles and dangerous obstaclescmake ..

5. **Level 5 - Final Test**: Complex riddles and double jump mastery requiredcmake --build .

6. **Level 6 - The Labyrinth**: Top-down maze with key collection finale./bin/DeathRiddle

```

## 🎮 Controls

## Game Mechanics

### Platformer Mode (Levels 1-5)

- **Arrow Keys / WASD**: Move left and right- **Health System**: Start with 100 HP. Wrong answers or poor choices reduce health.

- **Space / Up / W**: Jump- **Decision Points**: Choose your path wisely - each decision matters.

  - Press again in air for double jump (3s cooldown)- **Riddles**: CTF-style puzzles that test your logic, coding, and problem-solving skills.

- **ESC**: Pause game- **Time Pressure**: Some riddles have time limits (optional challenge mode).



### Top-Down Mode (Level 6)## Controls

- **Arrow Keys / WASD**: Move in all 4 directions

- **No jumping**: Navigate the maze freely- Mouse: Select dialogue options and interact with UI elements

- Enter: Submit riddle answers

### Game Over / Victory Screens- ESC: Open game menu (Save/Load/Settings)

- **R**: Retry (from Game Over)

- **Space**: Play Again (from Victory)## OOP Design

- **ESC**: Return to main menu

The project follows Object-Oriented Programming principles:

## 🛠️ Technical Details- **Encapsulation**: Private data members with public interfaces

- **Inheritance**: Base classes for widgets and game entities

### Built With- **Polymorphism**: Virtual functions for different riddle types

- **C++17**: Core game logic- **Abstraction**: Clear separation of concerns between game logic and UI

- **Qt6**: Graphics, UI, and event handling

  - Qt6::Core## License

  - Qt6::Widgets

  - Qt6::GuiMIT License - Feel free to modify and learn from this project!

- **CMake**: Build system (3.16+)

### Architecture
```
death-riddle/
├── src/              # Source files (.cpp)
│   ├── main.cpp
│   ├── MainWindow.cpp
│   ├── GameWidget.cpp
│   ├── Player2D.cpp
│   ├── Enemy.cpp
│   ├── Level.cpp
│   ├── AnimatedSprite.cpp
│   └── Riddle.cpp
├── include/          # Header files (.h)
├── assets/           # Game assets
│   └── sprites/      # Character and enemy sprite sheets
└── build/            # Build directory
```

### Key Components
- **GameWidget**: Main game loop (60 FPS), rendering, collision detection
- **Player2D**: Player character with physics, animations, and movement
- **Enemy**: AI-controlled enemies with patrol behavior and death animations
- **Level**: Tile-based level system with enemies, coins, spikes, and goals
- **AnimatedSprite**: Frame-based sprite animation system
- **Riddle**: Question/answer system with hint support

## 🚀 Building & Running

### Prerequisites
- C++17 compatible compiler (GCC, Clang, or MSVC)
- CMake 3.16 or higher
- Qt6 development libraries

### Build Instructions

```bash
# Clone the repository
git clone <repository-url>
cd death-riddle

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build .

# Run the game
./bin/DeathRiddle
```

### Quick Build (Linux/Mac)
```bash
./build.sh
cd build && ./bin/DeathRiddle
```

## 🎨 Game Assets

The game features custom sprite animations:
- **Dude Monster** (Player): Idle, Run, Jump, Hurt, Death animations
- **Pink Monster** (Enemy): Idle, Walk, Hurt, Death animations  
- **Owlet Monster** (Enemy): Idle, Walk, Hurt, Death animations

All sprites are 32×32 pixels with multiple frames per animation.

## 🧩 Riddle Examples

The game features 5 unique riddles ranging from wordplay to logic puzzles:

1. **Keyboard Riddle**: What has keys but no locks?
2. **Cipher Riddle**: Decode ROT13 encryption
3. **Logic Riddle**: Three gates, only one truth-teller
4. **Binary Riddle**: Computer science fundamentals
5. **Classic Riddle**: The more you take, the more you leave behind

## 🏆 Victory Conditions

Complete all 6 levels to achieve victory:
- Defeat all 5 enemy riddles in Levels 1-5
- Navigate the maze and find the key in Level 6
- Reach the final goal with the key to escape

A golden victory screen celebrates your triumph with your final score!

## 🎯 Gameplay Tips

1. **Collect Coins**: They mark safe paths and increase your score
2. **Use Double Jump Wisely**: 3-second cooldown means timing is crucial
3. **Study Riddles Carefully**: Read hints if you're stuck
4. **Avoid Spikes**: Instant 25 HP damage
5. **Level 6 Strategy**: Map out the maze mentally before rushing to the key

## 📝 Development

### Physics System
- **Gravity**: 800 px/s²
- **Friction**: 0.85 multiplier
- **Max Speed**: 200 px/s horizontal, 500 px/s vertical
- **Jump Velocity**: -350 px/s
- **Top-down Speed**: 150 px/s (Level 6)

### Enemy AI
- Patrol behavior: 64 pixels back and forth from spawn
- Death animation: 8 frames before removal
- Riddle triggers once per enemy

### Level Design
- Tile-based system: 32×32 pixel tiles
- Camera follows player with boundaries
- Dynamic collision detection

## 🐛 Known Issues

None currently! The game is fully playable from start to finish.

## 📄 License

This project is open source. Feel free to modify and distribute.

## 🙏 Acknowledgments

- Sprite assets from CraftPix.net
- Built with Qt6 framework
- Inspired by classic puzzle-platformer games

---

**Escape the Game Master. Solve the Riddles. Claim Your Freedom.** 🎮✨
