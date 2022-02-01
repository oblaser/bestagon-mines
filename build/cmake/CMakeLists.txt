#
# author        Oliver Blaser
# date          31.01.2022
# copyright     OLC-3 - Copyright (c) 2022 Oliver Blaser
#

cmake_minimum_required(VERSION 3.13)
project(space-discovery)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(EXE bestagon-mines)

include_directories(../../src/)

set(SOURCES
../../src/application/game.cpp
../../src/application/gui.cpp
../../src/main.cpp
../../src/middleware/util.cpp
../../src/middleware/xpmSprite.cpp
../../src/olcPixelGameEngine.cpp
../../src/resources/sprites.cpp
)

add_executable(${EXE} ${SOURCES})

if(UNIX)
if(NOT APPLE)
target_link_libraries(${EXE} X11 GL pthread png stdc++fs)
endif()
endif()
