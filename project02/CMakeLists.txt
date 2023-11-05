cmake_minimum_required(VERSION 3.22)
project( Project2 )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable( extract_channel extract_channel.cpp )
target_link_libraries( extract_channel ${OpenCV_LIBS} )

add_executable( negative negative.cpp )
target_link_libraries( negative ${OpenCV_LIBS} )

add_executable( mirror mirror.cpp )
target_link_libraries( mirror ${OpenCV_LIBS} )

add_executable( rotate rotate.cpp )
target_link_libraries( rotate ${OpenCV_LIBS} )

add_executable( light light.cpp )
target_link_libraries( light ${OpenCV_LIBS} )
