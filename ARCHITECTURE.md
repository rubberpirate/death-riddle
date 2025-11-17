# Death Riddle - Technical Documentation

## Architecture Overview

### Object-Oriented Design

The project follows OOP principles with clear separation of concerns:

```
┌─────────────────────────────────────────────────────┐
│                   MainWindow                         │
│  - Menu system                                       │
│  - Window management                                 │
└──────────────────┬──────────────────────────────────┘
                   │
┌──────────────────▼──────────────────────────────────┐
│                  GameWidget                          │
│  - UI coordination                                   │
│  - Widget management                                 │
└──────────────────┬──────────────────────────────────┘
                   │
┌──────────────────▼──────────────────────────────────┐
│                  GameState                           │
│  - Game logic controller                             │
│  - State machine                                     │
└─────┬────────────┬────────────┬─────────────────────┘
      │            │            │
┌─────▼────┐  ┌───▼─────┐  ┌──▼──────┐
│  Player  │  │GameMaster│ │  Story  │
│          │  │          │  │         │
│ -health  │  │  -mood   │  │ -nodes  │
│ -score   │  │-dialogue │  │-riddles │
└──────────┘  └──────────┘  └─────┬───┘
                                   │
                            ┌──────▼────┐
                            │   Riddle  │
                            │           │
                            │ -question │
                            │ -answer   │
                            └───────────┘
```

## Core Classes

### 1. Player Class
**Responsibility**: Manages player state and attributes

**Key Features**:
- Health management with signals
- Score tracking
- State machine (Idle, Thinking, Damaged, Celebrating, Defeated)
- Statistics tracking (riddles solved, failed attempts)

**OOP Principles**:
- **Encapsulation**: Private data members with public getters/setters
- **Signals/Slots**: Qt's signal-slot mechanism for event handling

### 2. GameMaster Class
**Responsibility**: Represents the antagonist with dynamic behavior

**Key Features**:
- Mood-based reactions
- Dynamic dialogue generation
- Animation state management
- Contextual responses to player actions

**OOP Principles**:
- **Encapsulation**: Mood and dialogue state management
- **Polymorphism**: Different reactions based on game state

### 3. Story Class
**Responsibility**: Manages narrative flow and branching storyline

**Key Features**:
- Graph-based story structure
- 5 unique endings
- Decision tracking
- Riddle integration
- Node-based narrative system

**OOP Principles**:
- **Composition**: Contains Riddle objects
- **Abstraction**: Hides story graph complexity

### 4. Riddle Class
**Responsibility**: Base class for puzzle challenges

**Key Features**:
- Multiple riddle types (text, cipher, logic, code)
- Difficulty levels
- Hint system
- Flexible answer checking

**OOP Principles**:
- **Inheritance**: Can be extended for specialized riddle types
- **Polymorphism**: Virtual checkAnswer() method
- **Encapsulation**: Protected answer storage

### 5. GameState Class
**Responsibility**: Central game controller and state machine

**Key Features**:
- State management (Menu, Playing, Riddle, Game Over, Ending)
- Signal coordination between components
- Game flow control
- Hint management

**OOP Principles**:
- **Composition**: Contains Player, GameMaster, Story
- **Observer Pattern**: Connects signals between objects
- **State Pattern**: Manages game states

## Widget Hierarchy

### GUI Components

#### HealthBarWidget
- Custom painted widget
- Smooth animations using Qt Property Animation
- Color-coded health status
- Gradient effects

#### PlayerWidget
- Animated character representation
- State-based animations
- Custom painting with QPainter
- Multiple animation types (idle, damaged, celebrating)

#### DialogueWidget
- Dynamic choice presentation
- Riddle input interface
- Feedback display
- Hint system

#### GameWidget
- Main game container
- Coordinates all UI elements
- Handles game state visualization
- Ending screen overlay

## Game Flow

```
Main Menu
    ↓
[Start Game]
    ↓
Introduction Node
    ↓
Player Makes Choice
    ↓
[Has Riddle?] → Yes → Riddle Challenge
    ↓                       ↓
    No                [Correct?]
    ↓                  ↓         ↓
Continue Story    Yes (Progress)  No (Damage + Retry)
    ↓                  ↓              ↓
[Is Ending?] ← ─────────         [Dead?]
    ↓                              ↓
   Yes → Show Ending              Yes → Game Over (Death's Embrace)
```

## Story Structure

### Endings

1. **TRUE_FREEDOM** (Node 12)
   - Solve all riddles perfectly
   - Choose freedom over power
   - Requires high skill

2. **THE_DEAL** (Node 13)
   - Accept Game Master's offer
   - Become immortal apprentice
   - Alternative path to survival

3. **PARTIAL_ESCAPE** (Not fully implemented)
   - Escape with consequences
   - Loss of health/memories

4. **THE_LOOP** (Not fully implemented)
   - Fall into trap
   - Cycle restarts

5. **DEATHS_EMBRACE**
   - Health reaches zero
   - Failed too many riddles
   - Game over

### Riddles

1. **Keyboard Riddle** (Easy)
   - Type: Text answer
   - Tests: Lateral thinking
   - Damage: 10 HP

2. **ROT13 Cipher** (Medium)
   - Type: Cipher
   - Tests: Cryptography knowledge
   - Damage: 15 HP

3. **Three Gates Logic** (Hard)
   - Type: Logic puzzle
   - Tests: Logical reasoning
   - Damage: 20 HP

4. **Binary to ASCII** (Medium)
   - Type: Code challenge
   - Tests: Binary/ASCII conversion
   - Damage: 15 HP

5. **Letter E Riddle** (Deadly)
   - Type: Word puzzle
   - Tests: Pattern recognition
   - Damage: 25 HP

## Animation System

### AnimationManager
Provides utility functions for common animations:
- Fade in/out
- Slide transitions
- Scale effects
- Pulse and shake effects

### Property Animations
- Health bar smooth transitions
- Player wobble and scale
- State-based character animations

## Save System

### SaveManager
- JSON-based save format
- Stores: Player stats, story progress, game state
- Auto-save functionality ready
- Multiple save slots support

## Build System

### CMake Configuration
- Qt6 integration
- Automatic MOC, UIC, RCC
- C++17 standard
- Modular source organization

### Dependencies
- Qt6::Core - Core functionality
- Qt6::Widgets - GUI components
- Qt6::Gui - Graphics and painting
- Qt6::Multimedia - Sound effects (ready for future use)

## Future Enhancements

### Planned Features
1. **Sound System**
   - Background music
   - Sound effects for actions
   - Ambient audio

2. **More Riddles**
   - SQL injection puzzles
   - Reverse engineering challenges
   - Network protocol riddles

3. **Enhanced Graphics**
   - Particle effects
   - Advanced shaders
   - Character sprites

4. **Multiplayer**
   - Cooperative mode
   - Competitive riddle solving

5. **Achievement System**
   - Track accomplishments
   - Unlock bonus content

## Testing Strategy

### Unit Testing (To Implement)
- Riddle answer validation
- Player health calculations
- Story node navigation

### Integration Testing
- Full game playthrough
- All ending paths
- Save/load functionality

## Performance Considerations

- Efficient signal-slot connections
- Minimal repaints
- Resource management with smart pointers
- Lazy loading of game assets

## Code Style

- CamelCase for class names
- camelCase for methods
- m_ prefix for member variables
- Clear separation of interface and implementation
- Comprehensive comments for complex logic

---

**Version**: 1.0.0  
**License**: MIT  
**Language**: C++17  
**Framework**: Qt6
