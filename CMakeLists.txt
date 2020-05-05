cmake_minimum_required (VERSION 3.8)
# cmake_policy(SET CMP0098 NEW) # use new flex policy
cmake_policy(SET CMP0088 NEW) # use new bison policy

project ("COMP2432_Group_Assignment" C)

set(CMAKE_C_STANDARD 99)

find_package(BISON)
find_package(FLEX)

file(GLOB PROJECT_SOURCES
  ${PROJECT_SOURCE_DIR}/src/*.c
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable (${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC "-static")

set(PARSER_LOCATION "${PROJECT_SOURCE_DIR}/src/parser")

bison_target(input_parse ${PARSER_LOCATION}/input.y ${CMAKE_CURRENT_BINARY_DIR}/input.parse.c)
flex_target(input_lex ${PARSER_LOCATION}/input.l ${CMAKE_CURRENT_SOURCE_DIR}/input.lex.c)
add_flex_bison_dependency(input_lex input_parse)
add_library(input_parser ${BISON_input_parse_OUTPUTS} ${FLEX_input_lex_OUTPUTS})
target_include_directories(
  input_parser
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE input_parser
)