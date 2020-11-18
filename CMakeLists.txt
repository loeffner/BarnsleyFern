cmake_minimum_required (VERSION 3.0)
project(HA1)

# Prevent compilation in-source
if( ${CMAKE_BINARY_DIR} STREQUAL ${PROJECT_SOURCE_DIR} )
  Message( "" )
  Message( FATAL_ERROR "Source and build directories are the same!")
endif()

# Create executable as usual
set(SRC_FILES main.cpp Worker.cpp Producer.cpp Consumer.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})

# HA1 uses threads
target_link_libraries(${PROJECT_NAME} pthread)