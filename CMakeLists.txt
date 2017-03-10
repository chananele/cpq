cmake_minimum_required(VERSION 3.1.0)
project(compiler CXX)

find_package(BISON 3.0.4 REQUIRED)
find_package(FLEX REQUIRED)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON) 

set(GENERATORS ${CMAKE_SOURCE_DIR}/generators)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/src)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/inc)

set(FLEX_SOURCE  ${CMAKE_BINARY_DIR}/src/scanner.cpp)
set(BISON_TEMP   ${CMAKE_BINARY_DIR}/inc/parser.cpp)
set(BISON_SOURCE ${CMAKE_BINARY_DIR}/src/parser.cpp)
set(BISON_HEADER ${CMAKE_BINARY_DIR}/inc/parser.hh)

set(BISON_HEADERS "")
list(APPEND BISON_HEADERS inc/location.hh)
list(APPEND BISON_HEADERS inc/position.hh)
list(APPEND BISON_HEADERS inc/parser.hh)
list(APPEND BISON_HEADERS inc/stack.hh)

add_custom_command(
    DEPENDS ${GENERATORS}/scanner.l
    OUTPUT  ${FLEX_SOURCE}
    COMMAND ${FLEX_EXECUTABLE} -L -o ${FLEX_SOURCE} ${GENERATORS}/scanner.l
    COMMENT "generating scanner files..."
)

add_custom_command(
    DEPENDS ${GENERATORS}/parser.yy
    OUTPUT  ${BISON_SOURCE} ${BISON_HEADERS}
    COMMAND ${BISON_EXECUTABLE} -l --output=${BISON_TEMP} --defines=${BISON_HEADER} ${GENERATORS}/parser.yy
    COMMAND ${CMAKE_COMMAND} -E copy ${BISON_TEMP} ${BISON_SOURCE}
    COMMAND ${CMAKE_COMMAND} -E remove ${BISON_TEMP}
    COMMENT "generating parser files..."
)

include_directories(${CMAKE_BINARY_DIR}/inc)
include_directories(${CMAKE_SOURCE_DIR}/inc)

set(SOURCES ${FLEX_SOURCE} ${BISON_SOURCE})
add_subdirectory("src")

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

if(MSVC)
    # Flex and Bison do not generate warning free code.
    # Disabling warnings is a bit overkill. I will have to fix this in future.
    add_definitions(/W0)
endif()

add_executable(cpq ${SOURCES})

set_property(TARGET cpq PROPERTY CXX_STANDARD 14)
set_property(TARGET cpq PROPERTY CXX_STANDARD_REQUIRED ON)
