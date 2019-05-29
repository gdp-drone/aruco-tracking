cmake_minimum_required(VERSION 3.13)
project(aruco_tracking)

find_package(OpenCV 4 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)

add_executable(aruco_tracker main.cpp Tracker.cpp Tracker.h)
target_link_libraries(aruco_tracker ${OpenCV_LIBS})

add_executable(kalman_tracker kalman_tracker.cpp)
target_link_libraries(kalman_tracker ${OpenCV_LIBS})