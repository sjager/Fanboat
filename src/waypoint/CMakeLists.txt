cmake_minimum_required(VERSION 2.8.3)
project(waypoint)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  sensor_msgs
  message_generation
)

add_message_files(
   FILES
   fanboatInfo.msg
)

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  sensor_msgs
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES waypoint
#  CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(waypoint_node src/waypoint.cpp)
add_dependencies(waypoint_node waypoint_generate_messages_cpp)
target_link_libraries(waypoint_node ${catkin_LIBRARIES})

add_executable(search_state_node src/search.cpp)
add_dependencies(search_state_node waypoint_generate_messages_cpp)
target_link_libraries(search_state_node ${catkin_LIBRARIES})

add_executable(pursue_state_node src/pursue.cpp)
add_dependencies(pursue_state_node waypoint_generate_messages_cpp)
target_link_libraries(pursue_state_node ${catkin_LIBRARIES})

add_executable(avoid_state_node src/avoid.cpp)
add_dependencies(avoid_state_node waypoint_generate_messages_cpp)
target_link_libraries(avoid_state_node ${catkin_LIBRARIES})

