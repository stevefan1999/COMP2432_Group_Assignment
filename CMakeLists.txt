cmake_minimum_required (VERSION 3.8)

project ("COMP2432_Group_Assignment")

file(GLOB PROJECT_SOURCES
  ${PROJECT_SOURCE_DIR}/src/*.c
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable (${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/include
)
