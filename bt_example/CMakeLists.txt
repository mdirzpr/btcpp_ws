cmake_minimum_required(VERSION 3.22)
project(bt_example)

# BehaviorTree.CPP requires C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(behaviortree_cpp REQUIRED)

include_directories(include)

add_executable(autonomy_node_sim
  src/autonomy_node_sim.cpp
  src/action_nodes.cpp
)

target_link_libraries(autonomy_node_sim
  BT::behaviortree_cpp
)

install(TARGETS autonomy_node_sim DESTINATION bin)
install(DIRECTORY bt_structures/ DESTINATION share/bt_example/bt_structures)
install(FILES config/executor.yaml DESTINATION share/bt_example/config)
