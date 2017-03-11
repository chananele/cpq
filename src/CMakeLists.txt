add_subdirectory("instructions")
add_subdirectory("expressions")
add_subdirectory("statements")
add_subdirectory("booleans")

list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/cpq.cpp)
list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/driver.cpp)
list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/symbol.cpp)

set(SOURCES ${SOURCES} PARENT_SCOPE)
