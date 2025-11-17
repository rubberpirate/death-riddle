# Death Riddle - Platformer Transformation Complete! 🎮

## What Changed
Your Death Riddle game has been successfully transformed from a visual novel/dialogue-based adventure game into a **2D platformer** with physics-based movement and tile-based levels!

## New Features

### 🎮 Gameplay
- **Arrow Key Movement**: Use Left/Right arrow keys (or A/D) to move horizontally
- **Jump**: Press Up arrow, W, or Space to jump
- **Physics**: Realistic gravity (980 px/s²), jumping, and collisions
- **5 Progressive Levels**: 
  1. The Awakening (Tutorial)
  2. Cipher Challenge
  3. Logic Leap
  4. Binary Bridge
  5. Final Test

### 🏗️ Game Elements
- **Solid Platforms**: Gray blocks you can walk on and jump from
- **Coins**: Collect golden coins for +100 points each
- **Spikes**: Dangerous red hazards that deal 25 damage
- **Riddle Triggers**: Purple tiles with "?" that pause the game and show a riddle
- **Checkpoints**: Blue tiles that save your spawn point
- **Goal**: Green tile with "★" that completes the level

### 🧩 Riddles (Preserved from Original)
All 5 riddles are still in the game but now triggered by stepping on special tiles:
1. "What has keys but no locks..." (Answer: keyboard)
2. ROT13 cipher challenge (Answer: THIS IS NOT FREE)
3. Three gates logic puzzle (Answer: gold)
4. Binary to ASCII (Answer: FREE)
5. Letter riddle (Answer: e)

### 📊 HUD Display
- **Health Bar**: Shows current HP (starts at 100)
- **Score**: Tracks your points
- **Lives**: Shows remaining lives (starts at 3)
- **Level Name**: Displays current level

## How to Play

1. **Start the Game**: Run `./build/bin/DeathRiddle` from the project root
2. **Click "Start New Game"** on the main menu
3. **Move with Arrow Keys**: 
   - Left/Right (or A/D) to move
   - Up/W/Space to jump
4. **Collect coins** and **avoid spikes**
5. **Solve riddles** when you hit purple triggers
6. **Reach the green goal** to complete each level

## Controls
- **Movement**: Arrow Keys or WASD
- **Jump**: Up Arrow, W, or Space
- **Pause**: P or Escape

## Scoring System
- Collect Coin: +100 points
- Solve Riddle Correctly: +500 points, +25 HP
- Wrong Riddle Answer: -25 HP
- Death: Lose 1 life, respawn at checkpoint/spawn point

## Technical Details

### New Classes Created
- **Player2D**: Physics-based player with velocity, gravity, states (IDLE, RUNNING, JUMPING, FALLING, DEAD)
- **Level**: Tile-based level system with 30x20 grid of 32px tiles
- **Tile**: Represents individual tiles with type and properties

### Architecture
- 60 FPS game loop with delta time
- Pixel-perfect collision detection
- Camera system that follows the player
- Pixelated rendering style (no antialiasing)

### Files Modified
- `include/GameWidget.h` - Complete rewrite for platformer
- `src/GameWidget.cpp` - Complete rewrite with game loop, physics, rendering
- `CMakeLists.txt` - Updated to include new source files

### Files Backed Up
- Original GameWidget.cpp saved as `src/GameWidget.cpp.backup`

## Building the Project
```bash
cd /home/rubberpirate/death-riddle
cd build
cmake ..
cmake --build .
./bin/DeathRiddle
```

## Next Steps (Optional Enhancements)
- Add sprite sheets for animated player
- Add sound effects for jumps, coins, damage
- Add background music
- Create more levels
- Add power-ups (double jump, speed boost)
- Add enemies that patrol platforms
- Add moving platforms
- Add particle effects for coins and death

## Enjoy Your New Platformer! 🎉
The game is now fully playable with physics-based movement, challenging platforming sections, and CTF-style riddles integrated into the gameplay!
