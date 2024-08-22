# CMakeLists.txt
cmake_minimum_required(VERSION 3.5)
project(pick_place)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH "/usr/local" ${CMAKE_PREFIX_PATH})

find_package(behaviortree_cpp REQUIRED)

add_executable(pickPlace main.cpp)
target_link_libraries(pickPlace BT::behaviortree_cpp)