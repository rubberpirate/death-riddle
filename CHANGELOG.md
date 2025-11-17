# Death Riddle - Recent Updates

## Latest Changes (v1.2)

### Enemy Death Animation Fix
- **Fixed enemy freeze bug**: Enemies now properly play death animation and disappear
  - Added explicit animation reset and play in `die()` function
  - Death animation plays for full 8 frames before enemy is removed
  - Added debug output to track death animation lifecycle
  - Enemy updates continue while dead (for animation) but stops movement

### Level 6: The Labyrinth (NEW!)
- **Top-down maze level**: Navigate a complex maze to find the key
  - **Maze Structure**: Multiple corridors with walls creating challenging paths
  - **Key Mechanic**: Golden key placed in center of maze (worth +1000 points)
  - **Goal Requirement**: Must collect key before reaching goal
  - **Obstacles**: Spikes placed strategically throughout maze
  - **Collectibles**: 5 coins hidden along the paths
  
- **New TileType**: Added `KEY` tile type for collectible keys
- **Key System**: 
  - Rendered as golden key sprite
  - Collected with collision detection
  - Required to unlock Level 6 goal
  - Shows message when trying to reach goal without key

### Previous Updates

## Enemy System Improvements
- **Fixed riddle retrigger bug**: Added `m_riddleTriggered` boolean flag to Enemy class
  - Prevents riddle popup spam when answering incorrectly
  - Riddle triggers only once per enemy on first collision
  
- **Added enemies to all 6 levels**: Previously only Level 1 had enemies
  - **Level 1**: Pink Monster at (21, 11) - Basic riddle (riddle 0)
  - **Level 2**: Owlet Monster at (18, 14) - Math riddle (riddle 1)
  - **Level 3**: Pink Monster at (21, 11) - Logic riddle (riddle 2)
  - **Level 4**: Owlet Monster at (25, 12) - Binary riddle (riddle 3)
  - **Level 5**: Pink Monster at (19, 12) - Final riddle (riddle 4)
  - **Level 6**: Top-down maze (no enemies, key collection required)
  
- **Removed old RIDDLE_TRIGGER tiles**: Replaced static purple blocks with animated enemy sprites

## Double Jump Feature
- **New mechanic**: Press jump while in the air to perform a double jump
  - Same velocity as normal jump (-350 px/s)
  - Available immediately after landing
  
- **Cooldown system**: 3-second cooldown after using double jump
  - Prevents spam and adds strategic element
  - Restores when player lands on ground
  
- **Implementation details**:
  - Added `m_hasDoubleJump` flag (resets on landing)
  - Added `m_doubleJumpCooldown` timer (counts down in update loop)
  - Modified `jump()` function to check for air jumps
  - Modified `setOnGround()` to restore double jump on landing

## Technical Changes

### Modified Files:
1. **include/Enemy.h**: Added riddle trigger tracking
2. **src/Enemy.cpp**: Fixed death animation, initialize riddle flag
3. **src/GameWidget.cpp**: Check riddle trigger, handle KEY collection, debug output
4. **src/Level.cpp**: Added Enemy instances to all levels + Level 6 maze
5. **include/Player2D.h**: Added double jump variables
6. **src/Player2D.cpp**: Implemented double jump mechanics
7. **include/Level.h**: Added KEY tile type and createLevel6()

### Fixes Applied:
- ✅ Riddle popup spam on wrong answers (added trigger flag)
- ✅ Missing enemies in Levels 2-5 (added Enemy instances)
- ✅ Level 5 accessibility (double jump helps reach platforms)
- ✅ Enemy death animation freeze (explicit animation reset/play)
- ✅ Level 6 maze with key collection mechanic

## Game Structure:
- **6 Total Levels**
  - Levels 1-5: Platformer with enemy riddles
  - Level 6: Top-down maze with key collection
  
## How to Play:
- **Arrow Keys / WASD**: Move left/right
- **Space / Up / W**: Jump (press again in air for double jump)
- **R**: Retry after death
- **ESC**: Return to menu

## Tips:
- Double jump has 3-second cooldown after use
- Enemies patrol 64 pixels from spawn point
- Correct riddle answers kill the enemy (+500 points, +25 HP)
- Wrong answers damage the player (-25 HP)
- Level 6 requires finding the key before reaching the goal
- Look for coins to increase your score!

## Testing Notes:
- Watch console output for "Triggering enemy death animation" and "Removing dead enemy" messages
- Level 6 key spawn: (15, 9) in grid coordinates
- Level 6 goal spawn: (28, 2) - requires key to complete
