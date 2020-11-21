cmake_minimum_required (VERSION 3.0)
project(HA1)

# Prevent compilation in-source
if( ${CMAKE_BINARY_DIR} STREQUAL ${PROJECT_SOURCE_DIR} )
  Message( "" )
  Message( FATAL_ERROR "Source and build directories are the same!")
endif()

# Create executable as usual
set(SRC_FILES main.cpp FernProducer.cpp FernProducer.cpp)

# This creates a Makefile to produce the executables
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Add additional include directories (those of the library):
include_directories("../inc")

# Add additional link directories (those of the library):
link_directories("../lib")

# HA1 uses threads
target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} png)