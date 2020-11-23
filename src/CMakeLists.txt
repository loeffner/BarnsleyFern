cmake_minimum_required (VERSION 3.0)
project(barnsley)

# Prevent compilation in-source
if( ${CMAKE_BINARY_DIR} STREQUAL ${PROJECT_SOURCE_DIR} )
  Message( "" )
  Message( FATAL_ERROR "Source and build directories are the same!")
endif()

# Source Files
set(SRC_FILES main.cpp FernProducer.cpp FernConsumer.cpp)

# Add additional link directories (those of the library):
link_directories(${PROJECT_SOURCE_DIR}/../lib)

# This creates a Makefile to produce the executables
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Add additional include directories (those of the library):
include_directories(${PROJECT_SOURCE_DIR}/../inc)


# barnsley uses threads
target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} png)
target_link_libraries(${PROJECT_NAME} procon)
