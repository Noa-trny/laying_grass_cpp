cmake_minimum_required(VERSION 3.16)
project(LayingGrass LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_DIR src)
set(INCLUDE_DIR include)

file(GLOB SRC_FILES CONFIGURE_DEPENDS "${SOURCE_DIR}/*.cpp")
set(MAIN_SOURCE "${SOURCE_DIR}/main.cpp")

set(COMMON_SOURCES ${SRC_FILES})
list(REMOVE_ITEM COMMON_SOURCES "${CMAKE_SOURCE_DIR}/${MAIN_SOURCE}")

add_executable(LayingGrass ${COMMON_SOURCES} ${MAIN_SOURCE})
target_include_directories(LayingGrass PUBLIC ${INCLUDE_DIR})

