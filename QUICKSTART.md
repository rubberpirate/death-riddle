# Death Riddle - Quick Start Guide

## 🎮 Getting Started

### Prerequisites

Before building, ensure you have:

1. **C++ Compiler** (GCC 7+, Clang 5+, or MSVC 2017+)
2. **CMake** 3.16 or higher
3. **Qt6** (Core, Widgets, Gui, Multimedia modules)

### Installing Qt6

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install qt6-base-dev qt6-multimedia-dev cmake build-essential
```

#### Fedora
```bash
sudo dnf install qt6-qtbase-devel qt6-qtmultimedia-devel cmake gcc-c++
```

#### Arch Linux
```bash
sudo pacman -S qt6-base qt6-multimedia cmake gcc
```

#### macOS (with Homebrew)
```bash
brew install qt@6 cmake
```

#### Windows
Download and install Qt6 from: https://www.qt.io/download

## 🔨 Building the Project

### Method 1: Using the Build Script (Linux/macOS)

```bash
cd death-riddle
./build.sh
```

### Method 2: Manual Build

```bash
cd death-riddle
mkdir build
cd build
cmake ..
cmake --build .
```

### Method 3: Release Build

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

## 🚀 Running the Game

After successful build:

```bash
./build/bin/DeathRiddle
```

Or from the build directory:

```bash
cd build/bin
./DeathRiddle
```

## 🎯 How to Play

### Main Menu
- **Start New Game**: Begin your journey
- **About**: Learn about the game
- **Quit**: Exit the application

### During Gameplay

#### Health System
- Start with 100 HP
- Wrong answers damage you
- Hints cost 5 HP
- Death at 0 HP leads to "Death's Embrace" ending

#### Riddle Interface
1. Read the riddle question carefully
2. Type your answer in the text box
3. Click "Submit Answer"
4. Use "Request Hint" if stuck (costs 5 HP)

#### Making Choices
- Read the narrative and Game Master's dialogue
- Click on your preferred choice
- Choices affect the story path and endings

#### Score System
- Earn points for solving riddles
- Higher difficulty = more points
- Track progress in the top-right corner

## 📖 Story and Endings

### The Five Endings

1. **TRUE FREEDOM** ⭐⭐⭐
   - Requirements: Solve all riddles, choose freedom
   - Difficulty: Hardest
   - Reward: Complete victory

2. **THE DEAL** 🤝
   - Requirements: Accept Game Master's offer
   - Difficulty: Moderate
   - Reward: Eternal power (at a cost)

3. **PARTIAL ESCAPE** 🚪
   - Requirements: Escape with compromises
   - Difficulty: Moderate
   - Reward: Freedom with consequences

4. **THE LOOP** 🔄
   - Requirements: Fall into the trap
   - Difficulty: Easy to stumble into
   - Reward: Another chance (restart)

5. **DEATH'S EMBRACE** ☠️
   - Requirements: Health reaches 0
   - Difficulty: Game over
   - Reward: None (but you can restart)

## 🧩 Riddle Hints

### General Tips
1. Read questions carefully
2. Look for patterns
3. Think about common CTF techniques
4. Don't be afraid to use hints

### Riddle Types
- **Text Answer**: Direct questions, lateral thinking
- **Cipher**: Encryption/decryption challenges
- **Logic Puzzle**: Reasoning and deduction
- **Code Challenge**: Programming/binary concepts

## 🎨 Controls

- **Mouse**: Click buttons and choices
- **Keyboard**: Type answers
- **Enter**: Submit riddle answers
- **ESC**: Access menu (Planned feature)

## 💾 Save System

### Auto-Save (Planned)
The game will automatically save your progress.

### Manual Save (Planned)
Access via the Game menu → Save Game

### Load Game (Planned)
Access via the Game menu → Load Game

## 🐛 Troubleshooting

### Build Issues

#### "Qt6 not found"
- Ensure Qt6 is installed
- Set Qt6_DIR environment variable:
  ```bash
  export Qt6_DIR=/path/to/Qt/6.x.x/gcc_64/lib/cmake/Qt6
  ```

#### "CMake version too old"
- Update CMake:
  ```bash
  # Ubuntu/Debian
  sudo apt-get install cmake
  
  # Or download from cmake.org
  ```

#### Linking errors
- Ensure all Qt6 modules are installed
- Check CMakeLists.txt for required components

### Runtime Issues

#### "Cannot find libQt6..."
- Add Qt lib directory to LD_LIBRARY_PATH:
  ```bash
  export LD_LIBRARY_PATH=/path/to/Qt/6.x.x/gcc_64/lib:$LD_LIBRARY_PATH
  ```

#### Black screen on startup
- Check GPU drivers
- Try software rendering:
  ```bash
  QT_QUICK_BACKEND=software ./DeathRiddle
  ```

## 🎓 Learning Resources

### OOP Concepts Demonstrated
1. **Encapsulation**: Private members with public interfaces
2. **Inheritance**: Widget hierarchy, QObject inheritance
3. **Polymorphism**: Virtual functions in Riddle class
4. **Abstraction**: GameState hides complexity
5. **Composition**: GameState contains Player, Story, GameMaster

### Qt Concepts Used
1. **Signal-Slot Mechanism**: Event handling
2. **Property System**: Animations
3. **Custom Widgets**: PlayerWidget, HealthBarWidget
4. **Qt Graphics**: QPainter for custom rendering
5. **Animation Framework**: QPropertyAnimation

## 📚 Project Structure

```
death-riddle/
├── CMakeLists.txt          # Build configuration
├── build.sh                # Build script
├── README.md               # Project overview
├── ARCHITECTURE.md         # Technical documentation
├── QUICKSTART.md          # This file
├── include/               # Header files
│   ├── Player.h
│   ├── GameMaster.h
│   ├── GameState.h
│   ├── Story.h
│   ├── Riddle.h
│   ├── MainWindow.h
│   ├── GameWidget.h
│   ├── HealthBarWidget.h
│   ├── PlayerWidget.h
│   ├── DialogueWidget.h
│   ├── AnimationManager.h
│   └── SaveManager.h
├── src/                   # Implementation files
│   ├── main.cpp
│   ├── Player.cpp
│   ├── GameMaster.cpp
│   ├── GameState.cpp
│   ├── Story.cpp
│   ├── Riddle.cpp
│   ├── MainWindow.cpp
│   ├── GameWidget.cpp
│   ├── HealthBarWidget.cpp
│   ├── PlayerWidget.cpp
│   ├── DialogueWidget.cpp
│   ├── AnimationManager.cpp
│   └── SaveManager.cpp
├── resources/             # Resources
│   ├── resources.qrc
│   └── styles/
│       └── main.qss
└── assets/               # Game assets
```

## 🤝 Contributing

### Adding New Riddles

1. Create riddle in `Story::createRiddles()`
2. Add to story node in `Story::initializeStory()`
3. Set appropriate difficulty and damage

Example:
```cpp
Riddle* myRiddle = new Riddle(this);
myRiddle->setQuestion("Your question here");
myRiddle->setAnswer("correct answer");
myRiddle->setHint("Helpful hint");
myRiddle->setDifficulty(Riddle::Difficulty::MEDIUM);
myRiddle->setDamageOnFailure(15);
myRiddle->setRewardPoints(100);
```

### Adding New Story Nodes

1. Define node in `Story::initializeStory()`
2. Set narrative, dialogue, choices
3. Link to other nodes
4. Optionally attach a riddle

### Customizing Appearance

Edit `resources/styles/main.qss` for global styles or modify widget StyleSheets in code.

## 📞 Support

For issues, questions, or suggestions:
- Check ARCHITECTURE.md for technical details
- Review the source code comments
- Create an issue on the project repository

## 🏆 Achievements (Unofficial)

- **First Blood**: Complete your first riddle
- **Survivor**: Finish with >50 HP
- **Perfect Run**: Complete all riddles without hints
- **Speed Runner**: Finish in under 10 minutes
- **Completionist**: See all 5 endings
- **Researcher**: Use all available hints
- **Negotiator**: Choose "The Deal" ending
- **True Master**: Get "True Freedom" ending

---

**Enjoy your journey through Death Riddle!**

May your wit be sharp and your courage unwavering.

*Can you escape the Game Master?*
