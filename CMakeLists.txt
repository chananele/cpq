cmake_minimum_required(VERSION 3.1.0)
project(compiler CXX)

find_package(BISON 3.0.4 REQUIRED)
find_package(FLEX REQUIRED)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON) 

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

set(GENERATORS ${CMAKE_SOURCE_DIR}/generators)
set(GENERATED_DIRS ${CMAKE_BINARY_DIR}/src ${CMAKE_BINARY_DIR}/inc)

set(FLEX_SOURCE  ${CMAKE_BINARY_DIR}/src/scanner.cpp)
set(BISON_TEMP   ${CMAKE_BINARY_DIR}/inc/parser.cpp)
set(BISON_SOURCE ${CMAKE_BINARY_DIR}/src/parser.cpp)
set(BISON_HEADER ${CMAKE_BINARY_DIR}/inc/parser.hh)

set(BISON_HEADERS parser stack location position)
set(TEMP "")
foreach(header ${BISON_HEADERS})
    list(APPEND TEMP inc/${header})
endforeach()
set(BISON_HEADERS ${TEMP})

include_directories(${CMAKE_BINARY_DIR}/inc)
include_directories(${CMAKE_SOURCE_DIR}/inc)

add_subdirectory("src")
add_executable(cpq ${SRC} ${FLEX_SOURCE} ${BISON_SOURCE})
set_property(TARGET cpq PROPERTY CXX_STANDARD 14)
set_property(TARGET cpq PROPERTY CXX_STANDARD_REQUIRED ON)

add_custom_command(
    DEPENDS ${GENERATORS}/scanner.l
    OUTPUT  ${FLEX_SOURCE}
    COMMAND mkdir -p ${GENERATED_DIRS}
    COMMAND ${FLEX_EXECUTABLE} -L -o ${FLEX_SOURCE} ${GENERATORS}/scanner.l
    COMMENT "generating scanner files..."
)

add_custom_command(
    DEPENDS ${GENERATORS}/parser.yy
    OUTPUT  ${BISON_SOURCE} ${BISON_HEADERS}
    COMMAND mkdir -p ${GENERATED_DIRS}
    COMMAND ${BISON_EXECUTABLE} -l --output=${BISON_TEMP} --defines=${BISON_HEADER} ${GENERATORS}/parser.yy
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/src
    COMMAND mv ${BISON_TEMP} ${BISON_SOURCE}
    COMMENT "generating parser files..."
)

