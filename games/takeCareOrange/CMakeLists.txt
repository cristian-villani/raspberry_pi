cmake_minimum_required(VERSION 3.5)
project(takeCareOrange)

# C++17 Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Finde Qt5 Widgets
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# Füge die ausführbare Datei hinzu
add_executable(takeCareOrange main.cpp)

# Linke Qt5 Widgets
target_link_libraries(takeCareOrange Qt5::Widgets)

# Meta-Object Compiler (MOC) wird automatisch von CMake aufgerufen
set_target_properties(takeCareOrange PROPERTIES AUTOMOC ON)
