# Death Riddle - Mario-Style Platformer Upgrade 🎮✨

## Overview
Your Death Riddle game has been upgraded with professional sprite animations and Mario Bros-inspired physics!

## New Features Added

### 🎨 Animated Character Sprites
- **Character**: "Dude Monster" from Craftpix character pack
- **Animations**:
  - **Idle**: 4-frame looping animation (8 FPS)
  - **Run**: 6-frame looping animation (12 FPS) - works for both directions
  - **Jump/Fall**: 8-frame animation (15 FPS)
  - **Hurt**: 4-frame animation (10 FPS)
  - **Death**: 8-frame animation (10 FPS)

### 🎯 Improved Physics (Mario Bros-Inspired)
The physics system now uses smooth acceleration and deceleration like Super Mario Bros:

**Old Physics**:
- Instant speed change (200 px/s)
- Basic gravity (980 px/s²)
- Rigid jump (-400 px/s)

**New Physics**:
- **Acceleration**: 0.24 * 60 = 14.4 px/s² (smooth speed-up)
- **Friction**: 0.94 multiplier (smooth slow-down)
- **Max Speed X**: 10 px/frame (smooth movement)
- **Gravity**: 0.575 * 60 = 34.5 px/s² (floatier jumps)
- **Jump Power**: 1.10 * 60 = 66 px/s (responsive jumping)
- **Max Fall Speed**: 7.5 * 60 = 450 px/s (capped falling)

### 🎬 Animation System Features
- **Sprite Flipping**: Character automatically flips horizontally based on movement direction
- **Frame-based Animation**: Smooth sprite sheet animation with configurable FPS
- **State-based**: Different animations for each player state
- **Looping Control**: Animations can loop or play once
- **Fallback Rendering**: If sprites fail to load, falls back to colored rectangles

## Technical Implementation

### New Classes

#### `AnimatedSprite` (include/AnimatedSprite.h, src/AnimatedSprite.cpp)
```cpp
class AnimatedSprite {
    - loadSpriteSheet(filePath, frameWidth, frameHeight, frameCount)
    - update(deltaTime)  // Advances animation
    - getCurrentFrame()  // Returns current frame rect
    - play(), pause(), reset()
    - setLoop(), setFrameRate()
}
```

### Enhanced Classes

#### `Player2D`
**Added**:
- `QMap<State, AnimatedSprite*> m_animations` - Stores all character animations
- `bool m_facingRight` - Tracks sprite facing direction
- `loadAnimations()` - Loads all sprite sheets from assets
- `updateAnimation(float deltaTime)` - Updates current animation
- `getCurrentSprite()` - Returns active animation
- `isFacingRight()` - Returns facing direction

**Improved**:
- `moveLeft()/moveRight()` - Now uses acceleration instead of instant speed
- `stopHorizontalMovement()` - Applies friction for smooth deceleration
- `update()` - Calls animation update, caps velocities
- State transitions based on velocity magnitude

#### `GameWidget`
**Enhanced**:
- `drawPlayer()` - Renders animated sprites with horizontal flipping
- Maintains fallback rectangle rendering if sprites unavailable

## File Structure
```
death-riddle/
├── assets/
│   └── sprites/                    # NEW: Sprite assets
│       ├── Dude_Monster_Idle_4.png
│       ├── Dude_Monster_Run_6.png
│       ├── Dude_Monster_Jump_8.png
│       ├── Dude_Monster_Hurt_4.png
│       ├── Dude_Monster_Death_8.png
│       └── ...
├── include/
│   ├── AnimatedSprite.h           # NEW
│   └── Player2D.h                 # UPDATED
├── src/
│   ├── AnimatedSprite.cpp         # NEW
│   └── Player2D.cpp               # UPDATED
└── test-subjects/                 # Reference materials
    ├── Super-Mario-Bros/          # Physics reference
    └── craftpix-characters/       # Sprite source

```

## Sprite Specifications
- **Frame Size**: 32x32 pixels (matches Player2D::WIDTH/HEIGHT)
- **Format**: PNG with transparency
- **Layout**: Horizontal sprite sheets (frames laid out left-to-right)
- **Naming Convention**: `Dude_Monster_<AnimationName>_<FrameCount>.png`

## Physics Comparison

| Attribute | Old System | New System | Benefit |
|-----------|------------|------------|---------|
| Movement | Instant | Acceleration-based | Smooth, responsive feel |
| Stopping | Instant | Friction-based | Natural deceleration |
| Gravity | Heavy (980) | Floaty (34.5) | Better jump control |
| Max Speed | None | Capped (10 px/frame) | Consistent gameplay |
| Jump | Fixed | Variable | Mario-like control |

## Controls (Unchanged)
- **Left/Right or A/D**: Move left/right
- **Up/W/Space**: Jump
- **P/Escape**: Pause game

## Gameplay Feel
The new physics create a "Mario-like" feel:
- **Momentum**: You accelerate to max speed and decelerate smoothly
- **Air Control**: You can still control movement while jumping
- **Jump Height**: Floatier gravity allows better platforming precision
- **Visual Feedback**: Animations clearly show player state and direction

## Known Features
- Sprite sheets are loaded from `assets/sprites/` relative path
- Animation frame rates are tuned for smooth 60 FPS gameplay
- Horizontal sprite flipping uses painter transformations
- State changes automatically reset and play appropriate animations
- If sprites fail to load, game shows colored rectangles (graceful degradation)

## Future Enhancement Ideas
1. **Particle Effects**: Use included dust sprites for running/jumping effects
2. **Attack Animation**: Implement the Attack1/Attack2 sprites for combat
3. **More Characters**: Switch between Pink_Monster, Owlet_Monster, Dude_Monster
4. **Sprite-based Tiles**: Replace colored rectangles with pixel art tiles
5. **Background Parallax**: Add layered scrolling backgrounds
6. **Enemy Sprites**: Animate enemies with sprite sheets
7. **Collectible Animations**: Animate coins and power-ups

## Performance
- Sprite rendering uses Qt's optimized `QPainter::drawPixmap()`
- Animations only update active state
- Sprite sheets loaded once at initialization
- No runtime image loading or deallocation
- 60 FPS maintained on modern hardware

## Credits
- **Character Sprites**: Craftpix.net - Free Pixel Art Tiny Hero Sprites
- **Physics Reference**: Super Mario Bros C++ implementation (Gold872/Super-Mario-Bros)
- **Game Design**: Death Riddle - Original CTF adventure game concept

## Testing Checklist
✅ Sprite animations load correctly
✅ Character faces correct direction when moving
✅ Idle animation plays when stationary
✅ Run animation plays when moving
✅ Jump animation plays when in air
✅ Physics feel smooth and responsive
✅ Acceleration and deceleration work properly
✅ Game runs at 60 FPS
✅ Fallback rendering works if sprites missing

## Conclusion
Your platformer now has the polish of a professional game with smooth Mario-inspired physics and beautiful sprite animations! The character is much more expressive and the movement feels significantly better. 🎉
