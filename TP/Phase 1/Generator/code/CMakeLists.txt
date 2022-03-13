cmake_minimum_required(VERSION 3.5)

# Project Name
project(Generator)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(${PROJECT_NAME} generator.cpp primitivas.cpp)
set_property(DIRECTORY ${Generator_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Generator)