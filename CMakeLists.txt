
cmake_minimum_required(VERSION 2.8)
project(Project_2)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(Project_2 test_laws_5.cpp)
# add_executable(COMPUTERVISION filters.cpp)

target_link_libraries(Project_2 ${OpenCV_LIBS})
