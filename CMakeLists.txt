cmake_minimum_required(VERSION 3.14)
project(ConsoleCasino LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Code shared by the game and the tests
add_library(casino_core
    src/Card.cpp
    src/Deck.cpp
    src/Hand.cpp
    src/Player.cpp
    src/Input.cpp
    src/Blackjack.cpp
    src/Poker.cpp
    src/Casino.cpp
)
target_include_directories(casino_core PUBLIC include)

# The game
add_executable(casino src/main.cpp)
target_link_libraries(casino PRIVATE casino_core)

# Tests
add_executable(test_hands tests/test_hands.cpp)
target_link_libraries(test_hands PRIVATE casino_core)
