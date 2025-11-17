cmake_minimum_required(VERSION 3.16)
project(DeathRiddle VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Multimedia)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/Player2D.cpp
    src/Enemy.cpp
    src/Level.cpp
    src/Riddle.cpp
    src/AnimatedSprite.cpp
    src/MainWindow.cpp
    src/GameWidget.cpp
    # Old visual novel files (commented out)
    # src/Player.cpp
    # src/GameMaster.cpp
    # src/GameState.cpp
    # src/Story.cpp
    # src/HealthBarWidget.cpp
    # src/DialogueWidget.cpp
    # src/PlayerWidget.cpp
    # src/AnimationManager.cpp
    # src/SaveManager.cpp
)

# Header files
set(HEADERS
    include/Player2D.h
    include/Enemy.h
    include/Level.h
    include/Riddle.h
    include/AnimatedSprite.h
    include/MainWindow.h
    include/GameWidget.h
    # Old visual novel headers (commented out)
    # include/Player.h
    # include/GameMaster.h
    # include/GameState.h
    # include/Story.h
    # include/HealthBarWidget.h
    # include/DialogueWidget.h
    # include/PlayerWidget.h
    # include/AnimationManager.h
    # include/SaveManager.h
)

# Resource files
set(RESOURCES
    resources/resources.qrc
)

# Create executable
add_executable(${PROJECT_NAME} 
    ${SOURCES} 
    ${HEADERS}
    ${RESOURCES}
)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} 
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Multimedia
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
