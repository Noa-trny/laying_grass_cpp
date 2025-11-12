cmake_minimum_required(VERSION 3.10)
project(LayingGrass)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_DIR src)
set(INCLUDE_DIR include)

file(GLOB_RECURSE ALL_CPP "${SOURCE_DIR}/*.cpp")
# S'assurer que TileParser.cpp et GameUtils.cpp sont inclus (peut être ajouté après création des fichiers)
list(APPEND ALL_CPP "${SOURCE_DIR}/TileParser.cpp")
list(APPEND ALL_CPP "${SOURCE_DIR}/GameUtils.cpp")
file(GLOB_RECURSE HEADERS "${INCLUDE_DIR}/*.hpp")

set(MAIN_APP ${SOURCE_DIR}/main.cpp)

set(SOURCES_COMMON ${ALL_CPP})
list(FILTER SOURCES_COMMON EXCLUDE REGEX "(^|/|\\\\)main(_client|_server)?\\.cpp$")
list(FILTER SOURCES_COMMON EXCLUDE REGEX "(^|/|\\\\)(Network|Client|Server|Protocol)\\.cpp$")

# Définir la cible avec le point d'entrée main
set(SOURCES_APP ${SOURCES_COMMON} ${MAIN_APP} ${HEADERS})

add_executable(${PROJECT_NAME} ${SOURCES_APP})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})
