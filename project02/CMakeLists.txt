cmake_minimum_required(VERSION 3.22)
project( Project2 )
find_package( OpenCV REQUIRED )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_SRC ${CMAKE_BINARY_DIR}/src)

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17")

include_directories( ${OpenCV_INCLUDE_DIRS} )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_executable( extract_channel src/extract_channel.cpp )
target_link_libraries( extract_channel ${OpenCV_LIBS} )

add_executable( negative src/negative.cpp )
target_link_libraries( negative ${OpenCV_LIBS} )

add_executable( mirror src/mirror.cpp )
target_link_libraries( mirror ${OpenCV_LIBS} )

add_executable( rotate src/rotate.cpp )
target_link_libraries( rotate ${OpenCV_LIBS} )

add_executable( light src/light.cpp )
target_link_libraries( light ${OpenCV_LIBS} )

add_executable( test_BitStream src/test_BitStream.cpp src/include/BitStream.cpp )

add_executable( test_Golomb src/test_Golomb.cpp src/include/Golomb.cpp src/include/BitStream.cpp )
