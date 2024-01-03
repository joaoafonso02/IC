cmake_minimum_required(VERSION 3.22)
project( Project2 )
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package( OpenCV REQUIRED )
find_package(Python3 COMPONENTS Development NumPy)
# find_package( Sndfile REQUIRED)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_SRC ${CMAKE_BINARY_DIR}/src)

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17 ")

include_directories( ${OpenCV_INCLUDE_DIRS} )
# include_directories( ${Sndfile_INCLUDE_DIRS} )
include_directories(${Python3_INCLUDE_DIRS})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable( image src/image.cpp src/include/Golomb.cpp src/include/BitStream.cpp)
target_link_libraries( image ${OpenCV_LIBS} ${Python3_LIBRARIES})
