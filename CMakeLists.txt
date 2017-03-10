cmake_minimum_required(VERSION 3.1.0)
project(compiler CXX)

find_package(BISON 3.0.4 REQUIRED)
find_package(FLEX REQUIRED)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON) 

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

set(GENERATORS ${CMAKE_SOURCE_DIR}/generators)

set(FLEX_SOURCE  ${CMAKE_BINARY_DIR}/src/scanner.cpp)
add_custom_command(
    OUTPUT ${FLEX_SOURCE}
    COMMAND ${FLEX_EXECUTABLE} -L -o ${FLEX_SOURCE} ${GENERATORS}/scanner.l
    COMMENT "generating scanner files..."
)

set(BISON_TEMP   ${CMAKE_BINARY_DIR}/inc/parser.cpp)
set(BISON_SOURCE ${CMAKE_BINARY_DIR}/src/parser.cpp)
set(BISON_HEADER ${CMAKE_BINARY_DIR}/inc/parser.hh)

set(BISON_HEADERS parser stack location position)
set(TEMP "")
foreach(header ${BISON_HEADERS})
    list(APPEND TEMP inc/${header})
endforeach()
set(BISON_HEADERS ${TEMP})

add_custom_command(
    OUTPUT ${BISON_SOURCE} ${BISON_HEADERS}
    COMMAND ${BISON_EXECUTABLE} -l --output=${BISON_TEMP} --defines=${BISON_HEADER} ${GENERATORS}/parser.yy
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/src
    COMMAND mv ${BISON_TEMP} ${BISON_SOURCE}
    COMMENT "generating parser files..."
)

set_source_files_properties(${BISON_SOURCE} GENERATED)

include_directories(${CMAKE_BINARY_DIR}/inc)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory("src")
add_executable(cpq ${FLEX_SOURCE} ${BISON_SOURCE} ${SRC}) 
set_property(TARGET cpq PROPERTY CXX_STANDARD 14)
set_property(TARGET cpq PROPERTY CXX_STANDARD_REQUIRED ON)

