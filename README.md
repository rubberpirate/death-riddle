# Death Riddle

An Interactive CTF-style GUI game where you must escape from the Game Master through solving riddles and making critical decisions.

## Story

You wake up in a mysterious realm, bound by the will of an enigmatic Game Master. Your memories are fragmented, and the only way to escape is to prove your worth by solving deadly riddles. Each decision you make affects your fate, and multiple endings await based on your choices, wit, and courage.

**Can you outsmart Death itself?**

## Game Endings

1. **True Freedom** - Solve all riddles perfectly and outsmart the Game Master
2. **The Deal** - Make a pact with the Game Master to become their apprentice
3. **Partial Escape** - Escape but at a great cost (lose health/memories)
4. **The Loop** - Fall into the Game Master's trap and restart the cycle
5. **Death's Embrace** - Fail too many riddles and succumb to the darkness

## Features

- Interactive GUI built with Qt6
- Health bar system with consequences
- Animated player character and Game Master
- CTF-style riddles and puzzles
- Multiple branching storylines
- Save/Load game progress
- Atmospheric effects and animations

## Building the Project

### Prerequisites
- CMake 3.16 or higher
- Qt6 (Core, Widgets, Gui, Multimedia)
- C++17 compatible compiler

### Build Instructions

```bash
mkdir build
cd build
cmake ..
cmake --build .
./bin/DeathRiddle
```

## Game Mechanics

- **Health System**: Start with 100 HP. Wrong answers or poor choices reduce health.
- **Decision Points**: Choose your path wisely - each decision matters.
- **Riddles**: CTF-style puzzles that test your logic, coding, and problem-solving skills.
- **Time Pressure**: Some riddles have time limits (optional challenge mode).

## Controls

- Mouse: Select dialogue options and interact with UI elements
- Enter: Submit riddle answers
- ESC: Open game menu (Save/Load/Settings)

## OOP Design

The project follows Object-Oriented Programming principles:
- **Encapsulation**: Private data members with public interfaces
- **Inheritance**: Base classes for widgets and game entities
- **Polymorphism**: Virtual functions for different riddle types
- **Abstraction**: Clear separation of concerns between game logic and UI

## License

MIT License - Feel free to modify and learn from this project!
